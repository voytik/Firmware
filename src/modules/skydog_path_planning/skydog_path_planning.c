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

// simulink model includes
#include <skydog_path_planning/Skydog_path_planning_ert_rtw/SkydogSignals.h>
#include <skydog_path_planning/Skydog_path_planning_ert_rtw/skydog_path_planning.h>
#include <skydog_path_planning/Skydog_path_planning_ert_rtw/rtwtypes.h>

 
static bool thread_should_exit = false;		/**< daemon exit flag */
static bool thread_running = false;		/**< daemon status flag */
static int daemon_task;				/**< Handle of daemon task / thread */

static int mavlink_fd = -1;
static unsigned int running_frequency = 20; // [Hz]
static bool debug = true;


/**
 * daemon management function.
 */
__EXPORT int skydog_path_planning_main(int argc, char *argv[]);
/**
 * Mainloop of daemon.
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
	errx(1, "usage: skydog_path_planning {start|stop|status} [-f <step frequency>] [debug]\n"
			"\t-r\tStep frequency in Hz, 20Hz default\n"
			"\tdebug\tEnable debugging (not default)\n");
}

/**
 * The daemon app only briefly exists to start
 * the background job. The stack size assigned in the
 * Makefile does only apply to this management task.
 *
 * The actual stack size should be set in the call
 * to task_create().
 */
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

	while ((ch = getopt(argc, argv, "f:debug")) != EOF) {
		switch (ch) {
		case 'f': {		running_frequency = strtoul(optarg, NULL, 10);
						if (running_frequency < 1)
						{ running_frequency = 1;
						  warnx("Minimal running frequency is 1Hz, setting this");
						}
				}break;

		case 'debug':
				debug = true;
				break;

		default:
				usage("unrecognized flag");
				errx(1, "exiting.");
			}
		}


	mavlink_fd = open(MAVLINK_LOG_DEVICE, 0);
	if (mavlink_fd < 0) {
	        warnx("failed to open MAVLink log stream, start mavlink app first.");
	}

	// refresh rate in ms
	int rate = 1000/running_frequency;
	int error_counter = 0;
	float current_wp = 0.0f;
	int j = 0;

		/* subscribe to sensor_combined topic */
			int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
			/* limit the interval*/
			orb_set_interval(sensor_sub_fd, rate);

		/* subscribe to gps topic */
			int gps_sub_fd = orb_subscribe(ORB_ID(vehicle_gps_position));

		/* subscribe to airspeed channels topic */
			int airspeed_sub_fd = orb_subscribe(ORB_ID(airspeed));

		/* subscribe to local position topic */
			int position_sub_fd = orb_subscribe(ORB_ID(vehicle_local_position));

		/* subscribe to home position topic */
			int home_sub_fd = orb_subscribe(ORB_ID(home_position));

		/* subscribe to vehicle status topic */
			int status_sub_fd = orb_subscribe(ORB_ID(vehicle_status));

		/* subscribe to waypoints topic */
			int skydog_sub_fd = orb_subscribe(ORB_ID(skydog_waypoints));

			/* one could wait for multiple topics with this technique, just using one here */
			struct pollfd fds[] = {
				{ .fd = sensor_sub_fd,   .events = POLLIN },
			};

			//buffs to hold data
			struct sensor_combined_s sensors_raw;
			struct vehicle_gps_position_s gps_raw;
			struct airspeed_s airspeed_raw;
			struct vehicle_local_position_s position;
			struct home_position_s home;
			struct vehicle_status_s status;
			struct skydog_waypoints_s waypoints;

			// output struct
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

		     mavlink_log_info(mavlink_fd, "[skydog_path_planning] initialized");

			while (!thread_should_exit){
						/* wait for sensor update of first file descriptor for 1000 ms (1 second) */
						int poll_ret = poll(fds, 1, 1000);

						/* handle the poll result */
						if (poll_ret == 0) {
							/* this means none of our providers is giving us data */
							printf("[simulink_control] Got no data within a second\n");
						} else if (poll_ret < 0) {
							/* this is seriously bad - should be an emergency */
							if (error_counter < 10 || error_counter % 50 == 0) {
								/* use a counter to prevent flooding (and slowing us down) */
								printf("[simulink_control] ERROR return value from poll(): %d\n"
									, poll_ret);
							}
							error_counter++;
						} else {

							if (fds[0].revents & POLLIN) {

								/* copy sensors raw data into local buffer */
								orb_copy(ORB_ID(sensor_combined), sensor_sub_fd, &sensors_raw);
								/* copy gps raw data into local buffer */
								orb_copy(ORB_ID(vehicle_gps_position), gps_sub_fd, &gps_raw);
								/* copy airspeed raw data into local buffer */
								orb_copy(ORB_ID(airspeed), airspeed_sub_fd, &airspeed_raw);
								/* copy position data into local buffer */
								orb_copy(ORB_ID(vehicle_local_position), position_sub_fd, &position);
								/* copy position data into local buffer */
								orb_copy(ORB_ID(home_position), home_sub_fd, &home);
								/* copy status data into local buffer */
								orb_copy(ORB_ID(vehicle_status), status_sub_fd, &status);
								/* copy waypoints data into local buffer */
								orb_copy(ORB_ID(skydog_waypoints), skydog_sub_fd, &waypoints);

								// get current state
								if (status.main_state == MAIN_STATE_MANUAL) {
									Mode2_w = 0;
									skydog.Valid = false;
								}
								if(status.main_state == MAIN_STATE_SEATBELT ||status.main_state == MAIN_STATE_EASY){
									Mode2_w = 1;
									skydog.Valid = false;
								}
								if (status.main_state == MAIN_STATE_AUTO) {
									Mode2_w = 2;
									skydog.Valid = true;
								}


								//fill in data
								Altitude2_r = sensors_raw.baro_alt_meter;
								X_earth_r = gps_raw.lon/10000000.0f;
								Y_earth_r = gps_raw.lat/10000000.0f;
								Groundspeed2_r[0] = -gps_raw.vel_e_m_s;
								Groundspeed2_r[1] = -gps_raw.vel_n_m_s;
								Groundspeed2_r[2] = gps_raw.vel_d_m_s;
								Home_position[0] = home.alt/1000.0f;
								Home_position[1] = home.lon/10000000.0f;
								Home_position[2] = home.lat/10000000.0f;
								Home_position[3] = 11;

								//update parameters
								L_want = 20.0;
								/*
								R_want = 25.0;
								Trash_want = 10.0;
								*/

								// if rc signal/mavlink lost or low battery set error flag to true (and go HOME)
								if (status.rc_signal_lost || status.offboard_control_signal_lost || status.battery_warning == VEHICLE_BATTERY_WARNING_LOW)
								{
									Error = 1;
								}else{
									Error = 0;
								}
								Error = 0;

								//waypoint transfer from skydog_waypoints topic
								for (uint8_t i = 0; i < waypoints.wpm_count; i++)
								{
									Waypoints_w[i] = waypoints.waypoints[i].altitude;
									Waypoints_w[i+15] = waypoints.waypoints[i].longitude;
									Waypoints_w[i+30] = waypoints.waypoints[i].latitude;
									Waypoints_w[i+45] = 11; //waypoints.waypoints[i].speed;
								}
								//set other waypoints to zero
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
								skydog.Roll_w = 0.5f * Roll2_w;
								skydog.Altitude_w = Altitude2_w;
								skydog.Groundspeed_w = Speed_w;
								skydog.Valid = true;

								//copy debug for logging data
								skydog.Current_waypoint = Act_wps_index-1.0f;
								skydog.P[0] = P[0];
								skydog.P[1] = P[1];
								skydog.P[2] = P[2];
								skydog.U[0] = U[0];
								skydog.U[1] = U[1];
								skydog.U[2] = U[2];
								skydog.eta = eta;
								skydog.d2 = d2;

								/* publish values to skydog_autopilot_setpoint topic*/
								orb_publish(ORB_ID(skydog_autopilot_setpoint), skydog_pub, &skydog);

								// copy debug values for QGC
								debug_qgc.value3 = P[1];
								debug_qgc.value4 = P[2];
								debug_qgc.value5 = Roll2_w;
								debug_qgc.value6 = Altitude2_w;

								// publish values to debug topic
								orb_publish(ORB_ID(debug_key_value), debug_pub, &debug_qgc);

								//send actual waypoint to QGC
								if (current_wp != Act_wps_index){
									mavlink_log_critical(mavlink_fd, "#audio: skydog switched to waypoint %1.0f", Act_wps_index-1.0f);
									current_wp = Act_wps_index;
								}
								if (Act_wps_index > waypoints.wpm_count && waypoints.wpm_count != 0){
									mavlink_log_critical(mavlink_fd, "#audio: skydog flying home");
								}

								//send debug values directly to QGC debug console
								if (j>80){
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
									j = 0;
								}
								j++;

							}
					}
			}

			printf("[skydog_path_planning] exiting, stopping all motors.\n");
				thread_running = false;

				/* set Valid flag to false and publish */
				skydog.Valid = false;
				orb_publish(ORB_ID_SKYDOG_AUTOPILOT_SETPOINT, skydog_pub, &skydog);

				exit(0);
				return 0;
}
