/**
 * @file skydog_path_planning.c
 * Application for path planning and control from simulink (generated code).
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */
 
#include <nuttx/config.h>
#include <stdio.h>
#include <errno.h>
#include <nuttx/sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <systemlib/err.h>
#include <systemlib/systemlib.h>
#include <unistd.h>
#include <drivers/drv_hrt.h>
#include <inttypes.h>

#include <uORB/uORB.h>
#include <uORB/topics/sensor_combined.h>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/vehicle_gps_position.h>
#include <uORB/topics/manual_control_setpoint.h>
#include <uORB/topics/vehicle_control_mode.h>
#include <uORB/topics/airspeed.h>
#include <uORB/topics/vehicle_local_position.h>
#include <uORB/topics/home_position.h>
#include <uORB/topics/vehicle_status.h>
#include <uORB/topics/skydog_autopilot_setpoint.h>
#include <uORB/topics/skydog_waypoints.h>
#include <uORB/topics/debug_key_value.h>
#include <systemlib/systemlib.h>

#include <mavlink/mavlink_log.h>
#include <skydog_path_planning/skydog_path_planning_params.h>

// simulink model includes
#include <skydog_path_planning/Skydog_path_planning_ert_rtw/SkydogSignals.h>
#include <skydog_path_planning/Skydog_path_planning_ert_rtw/skydog_path_planning.h>
#include <skydog_path_planning/Skydog_path_planning_ert_rtw/rtwtypes.h>

 
static bool thread_should_exit = false;		/**< thread exit flag */
static bool thread_running = false;		/**< thread status flag */
static int daemon_task;				/**< Handle of thread */

static int mavlink_fd = -1;
static unsigned int running_frequency = 20; // [Hz]


/**
 * thread management function.
 */
__EXPORT int skydog_path_planning_main(int argc, char *argv[]);
/**
 * Mainloop of this app.
 */
int skydog_path_planning_thread_main(int argc, char *argv[]);

/**
 * Print the correct usage.
 */
static void usage(const char *reason);

usage(const char *reason)
{
	if (reason)
		warnx("%s\n", reason);
	errx(1, "usage: skydog_path_planning {start|stop|status} [-f <step frequency>]\n"
			"\t-r\tStep frequency in Hz, 20Hz default\n");
}


int skydog_path_planning_main(int argc, char *argv[])
{
	if (argc < 1)
			usage("missing command");

		if (!strcmp(argv[1], "start")) {

			if (thread_running) {
				warnx("skydog_path_planning daemon already running\n");
				/* this is not an error */
				exit(0);
			}

			thread_should_exit = false;
			daemon_task = task_spawn_cmd("skydog_path_planning",
						 SCHED_DEFAULT,
						 SCHED_PRIORITY_DEFAULT,
						 4048,
						 skydog_path_planning_thread_main,
						 (const char **)argv);
			exit(0);
		}

		if (!strcmp(argv[1], "stop")) {
			thread_should_exit = true;
			printf("skydog_path_planning stopped\n");
			exit(0);
		}

		if (!strcmp(argv[1], "status")) {
			if (thread_running) {
				warnx("\trunning\n");
			} else {
				warnx("\tnot started\n");
			}
			exit(0);
		}

		usage("unrecognized command");
		exit(1);

}

// main log function
int skydog_path_planning_thread_main(int argc, char *argv[])
{
	thread_running = true;
	printf("skydog_path_planning started\n");

	/* work around some stupidity in task_create's argv handling */
	argc -= 2;
	argv += 2;
	int ch;

	while ((ch = getopt(argc, argv, "f")) != EOF) {
		switch (ch) {
		case 'f': {		running_frequency = strtoul(optarg, NULL, 10);
						if (running_frequency < 1)
						{ running_frequency = 1;
						  warnx("Minimal running frequency is 1Hz, setting this");
						}
				}break;

		default:
				usage("unrecognized flag");
				errx(1, "exiting.");
			}
		}


	mavlink_fd = open(MAVLINK_LOG_DEVICE, 0);
	if (mavlink_fd < 0) {
	        warnx("failed to open MAVLink log stream, start mavlink app first.");
	}

	// initialize local variables
	int rate = 1000/running_frequency;  //refresh rate in ms
	int error_counter = 0;		// error counter for data poll
	float current_wp = 1.0f;	// current waypoint index
	float home_wp = 0.0f;	// index of home position is always 0
	int loops_processed = 0;	// counter of loops processed (used for debug outputs)
	bool param_updated;		// were parameters updated?
	bool home_set = false;	// is home set?
	bool param_notified = false;

	// subscribe to sensor_combined topic
	int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
	// limit the interval based on selcted rate
	orb_set_interval(sensor_sub_fd, rate);

	// subscribe to parameters topic
	int param_sub_fd = orb_subscribe(ORB_ID(parameter_update));

	// subscribe to gps topic
	int gps_sub_fd = orb_subscribe(ORB_ID(vehicle_gps_position));

	// subscribe to airspeed channels topic
	int airspeed_sub_fd = orb_subscribe(ORB_ID(airspeed));

	// subscribe to local position topic
	int position_sub_fd = orb_subscribe(ORB_ID(vehicle_local_position));

	// subscribe to home position topic
	int home_sub_fd = orb_subscribe(ORB_ID(home_position));

	// subscribe to vehicle status topic
	int status_sub_fd = orb_subscribe(ORB_ID(vehicle_status));

	// subscribe to waypoints topic
	int skydog_sub_fd = orb_subscribe(ORB_ID(skydog_waypoints));

	// struct for data poll
	struct pollfd fds[] = {
		{ .fd = sensor_sub_fd,   .events = POLLIN },
	};

	//buffs to hold data
	struct sensor_combined_s sensors_raw;
	struct parameter_update_s param_update;
	struct vehicle_gps_position_s gps_raw;
	struct airspeed_s airspeed_raw;
	struct vehicle_local_position_s position;
	struct home_position_s home;
	struct vehicle_status_s status;
	struct skydog_waypoints_s waypoints;

	// output structs
	struct skydog_autopilot_setpoint_s skydog;
	struct debug_key_value_s debug_qgc;

	//set all buffers to 0
	memset(&sensors_raw, 0, sizeof(sensors_raw));
	memset(&gps_raw, 0, sizeof(gps_raw));
	memset(&airspeed_raw, 0, sizeof(airspeed_raw));
	memset(&position, 0, sizeof(position));
	memset(&home, 0, sizeof(home));
	memset(&status, 0, sizeof(status));
	memset(&skydog, 0, sizeof(skydog));
	memset(&waypoints, 0, sizeof(waypoints));
	memset(&debug_qgc, 0, sizeof(debug_qgc));

	 // advertise that this controller will publish skydog_topic
	 orb_advert_t skydog_pub = orb_advertise(ORB_ID(skydog_autopilot_setpoint), &skydog);

	 //fill in names for debug value
	 snprintf(debug_qgc.key3, 10, "P_long");
	 snprintf(debug_qgc.key4, 10, "P_lat");
	 snprintf(debug_qgc.key5, 10, "Roll2_w");
	 snprintf(debug_qgc.key6, 10, "Altitude2_w");

	 //Advertise that this controller will publish debug values and make first publication
	 orb_advert_t debug_pub = orb_advertise(ORB_ID(debug_key_value), &debug_qgc);

	 // initialize simulink model
	 Skydog_path_planning_initialize();

	 //parameters initialize
	 struct skydog_path_planning_params params;
	 struct skydog_path_planning_param_handles param_handles;
	 // initialize parameter handles
	 parameters_init(&param_handles);
	 //update topic
	 orb_copy(ORB_ID(parameter_update), param_sub_fd, &param_update); // read from param topic to clear updated flag
	 // first parameters update
	 parameters_update(&param_handles, &params);
	 // update simulink model parameters
	 L_want = params.L;
	 R_want = params.R;
	 Trash_want = params.Trash;
	 Alt_want = params.Alt_Trash;

	 // notify user through QGC that the autopilot is initialized
	 mavlink_log_info(mavlink_fd, "[skydog_path_planning] initialized");

	 // start of main loop------------------------------------------------------------------
	 while (!thread_should_exit){
		// wait for sensor update for 1000 ms
		int poll_ret = poll(fds, 1, 1000);

		// handle the poll result
		if (poll_ret == 0) {
			// none of our providers is giving us data
			printf("[simulink_control] Got no data within a second\n");
		} else if (poll_ret < 0) {
			// this is seriously bad - should be an emergency
			if (error_counter < 10 || error_counter % 50 == 0) {
				// use a counter to prevent flooding (and slowing us down)
				printf("[simulink_control] ERROR return value from poll(): %d\n"
					, poll_ret);
			}
			error_counter++;
		} else {

			if (fds[0].revents & POLLIN) {

				//check if parameters updated
				orb_check(param_sub_fd, &param_updated);
				//if updated, update local copies
				if(param_updated)
				{
					//clear update flag
					orb_copy(ORB_ID(parameter_update), param_sub_fd, &param_update);
					//update parameters
					parameters_update(&param_handles, &params);
					// update simulink model parameters
					L_want = params.L;
					R_want = params.R;
					Trash_want = params.Trash;
					Alt_want = params.Alt_Trash;

					//notify user
					if(!param_notified)
					{
						mavlink_log_critical(mavlink_fd, "#audio: skydog path parameters updated");
						param_notified = true;
					}
				}

				// copy sensors raw data into local copy
				orb_copy(ORB_ID(sensor_combined), sensor_sub_fd, &sensors_raw);
				// copy gps raw data into local copy
				orb_copy(ORB_ID(vehicle_gps_position), gps_sub_fd, &gps_raw);
				// copy airspeed raw data into local copy
				orb_copy(ORB_ID(airspeed), airspeed_sub_fd, &airspeed_raw);
				// copy position data into local copy
				orb_copy(ORB_ID(vehicle_local_position), position_sub_fd, &position);
				// copy position data into local copy
				orb_copy(ORB_ID(home_position), home_sub_fd, &home);
				// copy status data into local copy
				orb_copy(ORB_ID(vehicle_status), status_sub_fd, &status);
				// copy waypoints data into local copy
				orb_copy(ORB_ID(skydog_waypoints), skydog_sub_fd, &waypoints);

				// get current state
				if (status.main_state == MAIN_STATE_MANUAL) {
					Mode2_w = 0;
					skydog.Valid = false;
				}
				if(status.main_state == MAIN_STATE_ALTCTL ||status.main_state == MAIN_STATE_POSCTL){
					Mode2_w = 1;
					skydog.Valid = false;
				}
				if (status.main_state == MAIN_STATE_AUTO_MISSION) {
					Mode2_w = 2;
					skydog.Valid = true;
				}

				//check if home position is set and send to QGC once
				if(home.valid && !home_set){
					//mavlink_log_critical(mavlink_fd, "#audio: skydog home set");
					home_set = true;
				}

				//fill in data for simulink code
				//Altitude2_r = sensors_raw.baro_alt_meter;
				Altitude2_r = gps_raw.alt/1000.0f;
				X_earth_r = gps_raw.lon/10000000.0f;
				Y_earth_r = gps_raw.lat/10000000.0f;
				Groundspeed2_r[0] = -gps_raw.vel_e_m_s;
				Groundspeed2_r[1] = -gps_raw.vel_n_m_s;
				Groundspeed2_r[2] = gps_raw.vel_d_m_s;
				Home_position[0] = (home.alt/1000.0f) + params.Home_Alt_Offset;
				Home_position[1] = home.lon/10000000.0f;
				Home_position[2] = home.lat/10000000.0f;
				Home_position[3] = params.WP_Speed;

				// if rc signal/mavlink lost or low battery set error flag to true (and go HOME)
				if (status.rc_signal_lost || status.offboard_control_signal_lost || status.battery_warning == VEHICLE_BATTERY_WARNING_LOW)
				{
					Error = 1;
				}else{
					Error = 0;
				}
				Error = 0;
				//turn error off if in HIL
				if (status.hil_state == HIL_STATE_ON)
				{
					Error = 0;
				}

				//waypoint transfer from skydog_waypoints topic
				for (uint8_t i = 0; i < waypoints.wpm_count; i++)
				{
					Waypoints_w[i] = waypoints.waypoints[i].altitude;
					Waypoints_w[i+15] = waypoints.waypoints[i].longitude;
					Waypoints_w[i+30] = waypoints.waypoints[i].latitude;
					Waypoints_w[i+45] = params.WP_Speed; //11; //waypoints.waypoints[i].speed;
				}
				//set empty waypoints to zero
				for (uint8_t i = waypoints.wpm_count; i < 15; i++)
				{
					Waypoints_w[i] = 0;
					Waypoints_w[i+15] = 0;
					Waypoints_w[i+30] = 0;
					Waypoints_w[i+45] = 0;
				}

				//run Simulink code
				Skydog_path_planning_step();

				// copy output to skydog topic
				skydog.Roll_w = Roll2_w;
				skydog.Altitude_w = Altitude2_w;
				skydog.Groundspeed_w = Speed_w;
				skydog.Valid = true;
				skydog.Autopilot_mode = Mode2_w;

				//copy debug data for logging
				skydog.Current_waypoint = Act_wps_index-1.0f;
				skydog.P[0] = P[0];
				skydog.P[1] = P[1];
				skydog.P[2] = P[2];
				skydog.U[0] = U[0];
				skydog.U[1] = U[1];
				skydog.U[2] = U[2];
				skydog.eta = eta;
				skydog.d2 = d2;

				// publish values to skydog_autopilot_setpoint topic
				orb_publish(ORB_ID(skydog_autopilot_setpoint), skydog_pub, &skydog);

				// copy debug values for QGC
				debug_qgc.value3 = P[1];
				debug_qgc.value4 = P[2];
				debug_qgc.value5 = Roll2_w;
				debug_qgc.value6 = Altitude2_w;

				// publish values to debug topic
				orb_publish(ORB_ID(debug_key_value), debug_pub, &debug_qgc);

				// if changed, send current waypoint to QGC
				if (current_wp != Act_wps_index && Act_wps_index <= waypoints.wpm_count){
					mavlink_log_critical(mavlink_fd, "#audio: skydog switched to waypoint %1.0f", Act_wps_index-1.0f);
					current_wp = Act_wps_index;
				}
				// send flying home notification if necessary
				if (Act_wps_index > waypoints.wpm_count && waypoints.wpm_count != 0 && home_wp != Act_wps_index){
					mavlink_log_critical(mavlink_fd, "#audio: skydog flying home");
					home_wp = Act_wps_index;
				}

				//send debug values directly to QGC debug console every x seconds
				if (loops_processed>80){
					//mavlink_log_info(mavlink_fd, "[skydog_path] Lat:%4.4f, Lon:%4.4f, Alt:%4.4f", Y_earth_r, X_earth_r, Altitude2_r);
					//mavlink_log_info(mavlink_fd, "[home] Lat:%4.4f, Lon:%4.4f, Alt:%4.4f", Home_position[2], Home_position[1], Home_position[0]);
					//mavlink_log_info(mavlink_fd, "[wp1] Lat:%4.4f, Lon:%4.4f, Alt:%4.4f",Waypoints_w[30],Waypoints_w[15],Waypoints_w[0]);
					//mavlink_log_info(mavlink_fd, "[wp2] Lat:%4.4f, Lon:%4.4f, Alt:%4.4f",Waypoints_w[2+4],Waypoints_w[1+4],Waypoints_w[0+4]);
					//mavlink_log_info(mavlink_fd, "[wp_act] error:%d,rc::%d,mav:%d,batt:%d",Error,status.rc_signal_lost,status.offboard_control_signal_lost,status.battery_warning);
					//mavlink_log_info(mavlink_fd, "[pos_act] lat:%4.4f,lon:%4.4f,alt:%4.4f",Y_earth_r,X_earth_r,Altitude2_r);
					//mavlink_log_info(mavlink_fd, "w1:%4.4f,w2:%4.4f,w3:%4.4f,w4:%4.4f", wp_actual[0],wp_actual[2],wp_actual[4],wp_actual[6]);
					//mavlink_log_info(mavlink_fd, "w5:%4.4f,w6:%4.4f,w7:%4.4f,w8:%4.4f", wp_actual[1],wp_actual[3],wp_actual[5],wp_actual[7]);
					//mavlink_log_info(mavlink_fd, "skydog flying to waypointp %1.0f",Act_wps_index);
					//mavlink_log_info(mavlink_fd, "U1:%4.4f,U2:%4.4f,U3:%4.4f", U[0],U[1],U[2]);
					//mavlink_log_info(mavlink_fd, "P1:%4.4f,P2:%4.4f,P3:%4.4f", P[0],P[1],P[2]);
					//mavlink_log_info(mavlink_fd, "eta: %4.4f", eta);
					//mavlink_log_info(mavlink_fd, "[gspeed] gs1:%4.4f, gs2%4.4f, gs3:%4.4f",Groundspeed2_r[0],Groundspeed2_r[1],Groundspeed2_r[2]);
					loops_processed = 0;
					param_notified = false;
				}
				loops_processed++;

			}	//end of if data poll
		}	// end of if getting data
	}	//end of main loop

	printf("[skydog_path_planning] exiting, stopping all motors.\n");
	thread_running = false;

	// set Valid flag to false and publish
	skydog.Valid = false;
	orb_publish(ORB_ID_SKYDOG_AUTOPILOT_SETPOINT, skydog_pub, &skydog);

	exit(0);
	return 0;
}
