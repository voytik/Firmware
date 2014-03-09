/**
 * @file skydog_autopilot.c
 * Application for plane control from simulink (generated code).
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

#include <uORB/uORB.h>
#include <uORB/topics/sensor_combined.h>
#include <uORB/topics/vehicle_gps_position.h>
#include <uORB/topics/vehicle_attitude.h>
#include <uORB/topics/vehicle_control_mode.h>
#include <uORB/topics/manual_control_setpoint.h>
#include <uORB/topics/airspeed.h>
#include <uORB/topics/actuator_controls.h>
#include <uORB/topics/skydog_autopilot_setpoint.h>
#include <uORB/topics/debug_key_value.h>

#include <systemlib/systemlib.h>
#include <mavlink/mavlink_log.h>

// simulink model includes
#include <Skydog_autopilot/Skydog_autopilot_ert_rtw/SkydogSignals.h>
#include <skydog_autopilot/Skydog_autopilot_ert_rtw/Skydog_autopilot.h>
#include <skydog_autopilot/Skydog_autopilot_ert_rtw/rtwtypes.h>


 
static bool thread_should_exit = false;		/**< daemon exit flag */
static bool thread_running = false;		/**< daemon status flag */
static int daemon_task;				/**< Handle of daemon task / thread */

static int mavlink_fd = -1;
static unsigned int running_frequency = 100; // [Hz]
static bool debug = true;

/**
 * daemon management function.
 */
__EXPORT int skydog_autopilot_main(int argc, char *argv[]);
/**
 * Mainloop of daemon.
 */
int skydog_autopilot_thread_main(int argc, char *argv[]);

/**
 * Print the correct usage.
 */
static void usage(const char *reason);

usage(const char *reason)
{
	if (reason)
		warnx("%s\n", reason);
	errx(1, "usage: skydog_autopilot {start|stop|status} [-f <step frequency>] [debug]\n"
			"\t-f\tStep frequency in Hz, 100Hz default\n"
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
int skydog_autopilot_main(int argc, char *argv[])
{
	if (argc < 1)
			usage("missing command");

		if (!strcmp(argv[1], "start")) {

			if (thread_running) {
				warnx("skydog_autopilot daemon already running\n");
				/* this is not an error */
				exit(0);
			}

			thread_should_exit = false;
			daemon_task = task_spawn_cmd("skydog_autopilot",
						 SCHED_DEFAULT,
						 SCHED_PRIORITY_MAX - 20,
						 2048,
						 skydog_autopilot_thread_main,
						 (const char **)argv);
			exit(0);
		}

		if (!strcmp(argv[1], "stop")) {
			thread_should_exit = true;
			printf("skydog_autopilot stopped\n");
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
int skydog_autopilot_thread_main(int argc, char *argv[])
{
	thread_running = true;
	printf("skydog_autopilot started\n");

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


	/* initialize mavlink */
	mavlink_fd = open(MAVLINK_LOG_DEVICE, 0);
	if (mavlink_fd < 0) {
			warnx("failed to open MAVLink log stream, start mavlink app first.");
	}

	// refresh rate in ms
	int rate = 1000/running_frequency;
	int error_counter = 0;
	uint8_t autopilot_mode = 0;
	uint8_t current_autopilot_mode = 0;
	int j = 0;

		/* subscribe to sensor_combined topic */
			int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
			/* limit the interval*/
			orb_set_interval(sensor_sub_fd, rate);

		/* subscribe to gps topic */
			int gps_sub_fd = orb_subscribe(ORB_ID(vehicle_gps_position));
			//orb_set_interval(gps_sub_fd, rate);

		/* subscribe to vehicle attitude topic */
			int attitude_sub_fd = orb_subscribe(ORB_ID(vehicle_attitude));
			//orb_set_interval(attitude_sub_fd, rate);

		/* subscribe to vehicle mode topic */
			int control_mode_sub_fd = orb_subscribe(ORB_ID(vehicle_control_mode));
			//orb_set_interval(control_mode_sub_fd, rate);

		/* subscribe to airspeed channels topic */
			int airspeed_sub_fd = orb_subscribe(ORB_ID(airspeed));
			//orb_set_interval(airspeed_sub_fd, rate);

		/* subscribe to rc channels topic */
			int rc_sub_fd = orb_subscribe(ORB_ID(manual_control_setpoint));
			//orb_set_interval(rc_sub_fd, rate);

		/* subscribe to skydog_autopilot_setpoint  topic */
			int skydog_sub_fd = orb_subscribe(ORB_ID(skydog_autopilot_setpoint));
			//orb_set_interval(skydog_sub_fd, rate);


			/* one could wait for multiple topics with this technique */
			struct pollfd fds[] = {
				{ .fd = sensor_sub_fd,   .events = POLLIN},
			};

			//buffs to hold data
			struct sensor_combined_s sensors_raw;
			struct vehicle_gps_position_s gps_raw;
			struct vehicle_attitude_s attitude_raw;
			struct vehicle_control_mode_s control_mode;
			struct airspeed_s airspeed_raw;
			struct manual_control_setpoint_s rc_raw;
			struct skydog_autopilot_setpoint_s skydog;
			// output struct
			struct actuator_controls_s actuators;
			struct debug_key_value_s debug_qgc;

			//set all buffers to 0
			memset(&sensors_raw, 0, sizeof(sensors_raw));
			memset(&gps_raw, 0, sizeof(gps_raw));
			memset(&attitude_raw, 0, sizeof(attitude_raw));
			memset(&control_mode, 0, sizeof(control_mode));
			memset(&airspeed_raw, 0, sizeof(airspeed_raw));
			memset(&rc_raw, 0, sizeof(rc_raw));
			memset(&actuators, 0, sizeof(actuators));
			memset(&skydog, 0, sizeof(skydog));
			memset(&debug_qgc, 0, sizeof(debug_qgc));

		    /* publish actuator controls with zero values */
		    for (unsigned i = 0; i < NUM_ACTUATOR_CONTROLS; i++) {
		             actuators.control[i] = 0.0f;
		     }
		     //Advertise that this controller will publish actuator and make first publication
		     orb_advert_t actuator_pub = orb_advertise(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, &actuators);

		     //fill in name for debug value
		     snprintf(debug_qgc.key, 10, "debug1");
		     snprintf(debug_qgc.key2, 10, "debug2");

		     //Advertise that this controller will publish actuator and make first publication
		     orb_advert_t debug_pub = orb_advertise(ORB_ID(debug_key_value), &debug_qgc);

		     // initialize simulink model
		     Skydog_autopilot_initialize();

		     // notify user through QGC that the autopilot is initialized
		     mavlink_log_info(mavlink_fd, "[skydog_autopilot] initialized");


			while (!thread_should_exit){
						/* wait for sensor update of file descriptor for 1000 ms (1 second) */
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
								/* copy vehicle mode data into local buffer */
								orb_copy(ORB_ID(vehicle_control_mode), control_mode_sub_fd, &control_mode);

								// is MODE MANUAL selected
								if (control_mode.flag_control_manual_enabled && control_mode.flag_control_attitude_enabled == false && control_mode.flag_control_auto_enabled == false) {

									autopilot_mode = 0;

									/* copy rc raw data into local buffer */
									orb_copy(ORB_ID(manual_control_setpoint), rc_sub_fd, &rc_raw);

									// MODE MANUAL selected
									printf("[skydog_autopilot] MODE MANUAL selected\n");

									// just copy RC input directly to output
									actuators.control[0] = rc_raw.roll;
									actuators.control[1] = rc_raw.pitch;
									actuators.control[2] = rc_raw.yaw;
									actuators.control[3] = rc_raw.throttle;
									actuators.control[4] = rc_raw.aux1;

								}else{

									/* copy sensors raw data into local buffer */
									orb_copy(ORB_ID(sensor_combined), sensor_sub_fd, &sensors_raw);
									/* copy gps raw data into local buffer */
									orb_copy(ORB_ID(vehicle_gps_position), gps_sub_fd, &gps_raw);
									/* copy attitude raw data into local buffer */
									orb_copy(ORB_ID(vehicle_attitude), attitude_sub_fd, &attitude_raw);
									/* copy rc airspeed data into local buffer */
									orb_copy(ORB_ID(airspeed), airspeed_sub_fd, &airspeed_raw);
									/* copy rc raw data into local buffer */
									orb_copy(ORB_ID(manual_control_setpoint), rc_sub_fd, &rc_raw);
									/* copy skydog data into local buffer */
									orb_copy(ORB_ID(skydog_autopilot_setpoint), skydog_sub_fd, &skydog);

									//fill in inputs for simulink code
									Roll_w = skydog.Roll_w;
									Altitude_w = skydog.Altitude_w;
									Groundspeed_w = skydog.Groundspeed_w;
									Altitude_r = sensors_raw.baro_alt_meter;
									Roll_r = attitude_raw.roll;
									Groundspeed_r = gps_raw.vel_m_s;
									Pitch_r = attitude_raw.pitch;
									Yaw_r = attitude_raw.yaw;
									Roll_speed_r = attitude_raw.rollspeed;
									Pitch_speed_r = attitude_raw.pitchspeed;
									Yaw_speed_r = attitude_raw.yawspeed;
									Roll_acc_r = attitude_raw.rollacc;
									Pitch_acc_r = attitude_raw.pitchacc;
									Yaw_acc_r = attitude_raw.yawacc;
	/*								RC_aileron_r = rc_raw.roll;
									RC_elevator_r = rc_raw.pitch;
									RC_rudder_r = rc_raw.yaw;
									RC_throttle_r = rc_raw.throttle;
									RC_flaps_r = rc_raw.aux1;
*/
									RC_aileron_r = 0;
									RC_elevator_r = 0;
									RC_rudder_r = 0;
									RC_throttle_r = 0.5;

									if (control_mode.flag_control_attitude_enabled && control_mode.flag_control_auto_enabled) {
										if (skydog.Valid)
										{
											// is MODE AUTOPILOT selected
											printf("[skydog_autopilot] MODE AUTOPILOT selected\n");
											Mode_w = 2;
											autopilot_mode = 2;
										}else{
											// stay in stabilization until skydog_path_planning starts giving data
											Mode_w = 1;
											autopilot_mode = 1;
										}
									}else{
										// is MODE STABILIZATION selected
										printf("[skydog_autopilot] MODE STABILIZATION selected\n");
										Mode_w = 1;
										autopilot_mode = 1;

									}

									//run Simulink code
									Skydog_autopilot_step();

									// copy output in radians and normalize to [-1,1]
									actuators.control[0] = 0.2f*(Aileron_w * 2.65f);
									actuators.control[1] = -0.4f*(Elevator_w * 3.7f);
									actuators.control[2] = Rudder_w * 4.09f;
									actuators.control[3] = Throttle_w;
									actuators.control[4] = 0;//Flaps_w;

									//copy debug value to debug topic
									debug_qgc.value = debug1;
									debug_qgc.value2 = debug2;

									// publish values to debug topic
									orb_publish(ORB_ID(debug_key_value), debug_pub, &debug_qgc);

									if (j>200){
										printf("[skydog_autopilot] ailerons:%4.4f, elevator:%4.4f, rudder:%4.4f, throttle:%4.4f\n",actuators.control[0], actuators.control[1], actuators.control[2], Throttle_w);
										printf("[skydog_autopilot] db1:%4.4f, db2:%4.4f\n", debug1, debug2);

										//mavlink_log_info(mavlink_fd, "[skydog_autopilot] db1:%4.4f, db2:%4.4f", debug1, debug2);
										j = 0;
									}
									j++;


							} //control based on current mode finished

								// sanity check and publish actuator outputs
								if (isfinite(actuators.control[0]) &&
									isfinite(actuators.control[1]) &&
									isfinite(actuators.control[2]) &&
									isfinite(actuators.control[3]) &&
									isfinite(actuators.control[4])) {
									orb_publish(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, actuator_pub, &actuators);
								}else{
									printf("[skydog_autopilot] actuator values not finite\n");
								}

								// check which mode selected and send this once to ground station
								if (autopilot_mode != current_autopilot_mode){

									if (autopilot_mode == 0){
										mavlink_log_info(mavlink_fd, "[skydog_autopilot] sleeping, manual mode");
										mavlink_log_info(mavlink_fd, "[skydog_autopilot] man:%d,att:%d,auto:%d",control_mode.flag_control_manual_enabled,control_mode.flag_control_attitude_enabled, control_mode.flag_control_auto_enabled);
									}
									if (autopilot_mode == 1){
										mavlink_log_info(mavlink_fd, "[skydog_autopilot] running, stabilization mode");
										mavlink_log_info(mavlink_fd, "[skydog_autopilot] man:%d,att:%d,auto:%d",control_mode.flag_control_manual_enabled,control_mode.flag_control_attitude_enabled, control_mode.flag_control_auto_enabled);
									}
									if (autopilot_mode == 2){
										mavlink_log_info(mavlink_fd, "[skydog_autopilot] running, autopilot mode");
										mavlink_log_info(mavlink_fd, "[skydog_autopilot] man:%d,att:%d,auto:%d",control_mode.flag_control_manual_enabled,control_mode.flag_control_attitude_enabled, control_mode.flag_control_auto_enabled);
									}
									// update flag with current mode
									current_autopilot_mode = autopilot_mode;
								}

						}
					}
			}

			printf("[skydog_autopilot] exiting, stopping all motors.\n");
				thread_running = false;

				/* kill all outputs */
				for (unsigned i = 0; i < NUM_ACTUATOR_CONTROLS; i++)
					actuators.control[i] = 0.0f;

				orb_publish(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, actuator_pub, &actuators);


				fflush(stdout);
				exit(0);

				return 0;
}
