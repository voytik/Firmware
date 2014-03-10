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
	float current_time = 0;

	bool manual_enabled = false;

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

		/* subscribe to vehicle mode topic */
			int control_mode_sub_fd = orb_subscribe(ORB_ID(vehicle_control_mode));

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
			struct vehicle_control_mode_s control_mode;
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
			memset(&control_mode, 0, sizeof(control_mode));
			memset(&waypoints, 0, sizeof(waypoints));
			memset(&debug_qgc, 0, sizeof(debug_qgc));

		     // advertise that this controller will publish skydog_topic
		     orb_advert_t skydog_pub = orb_advertise(ORB_ID(skydog_autopilot_setpoint), &skydog);

		     //fill in name for debug value
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
								/* copy vehicle mode data into local buffer */
								orb_copy(ORB_ID(vehicle_control_mode), control_mode_sub_fd, &control_mode);

								// testing only!
								control_mode.flag_control_auto_enabled = true;

								// check if auto mode selected, if not don't run simulink code
								if (control_mode.flag_control_auto_enabled){

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


									//fill in arguments
									Altitude2_r = sensors_raw.baro_alt_meter;
									X_earth_r = position.x;
									Y_earth_r = position.y;
									Groundspeed2_r[0] =position.vx;
									Groundspeed2_r[1] =position.vy;
									Groundspeed2_r[2] =position.vz;
									//Nfz_w[0] = 0.0f; // TODO no fly zone implementation
									Mode2_w = 2; // TODO mode
									Time = current_time;
									Waypoints_count = waypoints.wpm_count;

									// if rc signal/mavlink lost,low battery go HOME
									if (status.rc_signal_lost || status.offboard_control_signal_lost || status.battery_warning == VEHICLE_BATTERY_WARNING_LOW)
									{
										Error = true;
									}else{
										Error = false;
									}

									//waypoint transfer from skydog_waypoints topic
									for (uint8_t i = 0; i < waypoints.wpm_count; i++)
									{
										Waypoints_w[i*4] = waypoints.waypoints[i].longitude;
										Waypoints_w[i*4+1] = waypoints.waypoints[i].latitude;
										Waypoints_w[i*4+2] = waypoints.waypoints[i].altitude;
										Waypoints_w[i*4+3] = waypoints.waypoints[i].speed;
									}


									//run Simulink code
									Skydog_path_planning_step();

		/*							// copy output to skydog topic
									skydog.Roll_w = Roll_w;
									skydog.Altitude_w = Altitude_w;
									skydog.Groundspeed_w = Groundspeed_w;
									skydog.Valid = true;
		*/
									// display wanted values for debug purposes
									//printf("[skydog_path_planning] Roll_w:%4.4f, Altitude_w:%4.4.f, Speed_w:%4.4f\n", Roll2_w,Altitude2_w,Speed_w);



									// copy debug values for QGC
									debug_qgc.value3 = P[1];
									debug_qgc.value4 = P[2];
									debug_qgc.value5 = Roll2_w;
									debug_qgc.value6 = Altitude2_w;

									// publish values to debug topic
									orb_publish(ORB_ID(debug_key_value), debug_pub, &debug_qgc);

									// skydog autopilot testing
									skydog.Roll_w = 0;
									skydog.Altitude_w = 1000;
									skydog.Groundspeed_w = 15;
									skydog.Valid = true;

									/* publish values to skydog_autopilot_setpoint topic*/
									orb_publish(ORB_ID(skydog_autopilot_setpoint), skydog_pub, &skydog);

									//update time
									current_time = current_time + (1.0f/running_frequency);
								}

								// check which mode selected and send this once to ground station
								if (control_mode.flag_control_auto_enabled!=manual_enabled){

									if (!control_mode.flag_control_auto_enabled){
										mavlink_log_info(mavlink_fd, "[skydog_path_planning] sleeping, manual mode");
									}else{
										mavlink_log_info(mavlink_fd, "[skydog_path_planning] running, auto mode");
									}
									// update flag with current mode
									manual_enabled = control_mode.flag_control_auto_enabled;
								}
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
