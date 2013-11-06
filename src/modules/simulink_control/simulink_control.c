/**
 * @file simulink_control.c
 * Application for low-level attitude control from simulink (generated code).
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
#include <uORB/topics/manual_control_setpoint.h>
#include <uORB/topics/airspeed.h>
#include <uORB/topics/actuator_controls.h>

#include <systemlib/systemlib.h>

// simulink model includes
#include <simulink_control/test_ert_rtw/globalSignals.h>
#include <simulink_control/test_ert_rtw/test.h>            /* Model's header file */
#include <simulink_control/test_ert_rtw/rtwtypes.h>

 
static bool thread_should_exit = false;		/**< daemon exit flag */
static bool thread_running = false;		/**< daemon status flag */
static int daemon_task;				/**< Handle of daemon task / thread */

/**
 * daemon management function.
 */
__EXPORT int simulink_control_main(int argc, char *argv[]);
/**
 * Mainloop of daemon.
 */
int simulink_control_thread_main(int argc, char *argv[]);

/**
 * Print the correct usage.
 */
static void usage(const char *reason);

usage(const char *reason)
{
	if (reason)
		warnx("%s\n", reason);
	errx(1, "usage: daemon {start|stop|status} [-p <additional params>]\n\n");
}

/**
 * The daemon app only briefly exists to start
 * the background job. The stack size assigned in the
 * Makefile does only apply to this management task.
 *
 * The actual stack size should be set in the call
 * to task_create().
 */
int simulink_control_main(int argc, char *argv[])
{
	if (argc < 1)
			usage("missing command");

		if (!strcmp(argv[1], "start")) {

			if (thread_running) {
				warnx("simulink_control daemon already running\n");
				/* this is not an error */
				exit(0);
			}

			thread_should_exit = false;
			daemon_task = task_spawn_cmd("daemon",
						 SCHED_DEFAULT,
						 SCHED_PRIORITY_MAX - 20,
						 2048,
						 simulink_control_thread_main,
						 (argv) ? (const char **)&argv[2] : (const char **)NULL);
			exit(0);
		}

		if (!strcmp(argv[1], "stop")) {
			thread_should_exit = true;
			printf("simulink_control stopped\n");
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
int simulink_control_thread_main(int argc, char *argv[])
{
	thread_running = true;

	printf("simulink_control started\n");

	// refresh rate in ms
	int rate = 100;
	int error_counter = 0;

		/* subscribe to sensor_combined topic */
			int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
			/* limit the interval*/
			orb_set_interval(sensor_sub_fd, rate);

		/* subscribe to gps topic */
			int gps_sub_fd = orb_subscribe(ORB_ID(vehicle_gps_position));
			orb_set_interval(gps_sub_fd, rate);

		/* subscribe to vehicel attitude topic */
			int attitude_sub_fd = orb_subscribe(ORB_ID(vehicle_attitude));
			orb_set_interval(attitude_sub_fd, rate);


		/* subscribe to airspeed channels topic */
			int airspeed_sub_fd = orb_subscribe(ORB_ID(airspeed));
			orb_set_interval(airspeed_sub_fd, rate);

			/* one could wait for multiple topics with this technique, just using one here */
			struct pollfd fds[] = {
				{ .fd = sensor_sub_fd,   .events = POLLIN },
				{ .fd = gps_sub_fd,   .events = POLLIN },
				{ .fd = attitude_sub_fd,   .events = POLLIN },
				{ .fd = airspeed_sub_fd,   .events = POLLIN },
			};

			//buffs to hold data
			struct sensor_combined_s sensors_raw;
			struct vehicle_gps_position_s gps_raw;
			struct vehicle_attitude_s attitude_raw;
			struct airspeed_s airspeed_raw;
			// output struct
			struct actuator_controls_s actuators;

			//set all buffers to 0
			memset(&sensors_raw, 0, sizeof(sensors_raw));
			memset(&gps_raw, 0, sizeof(gps_raw));
			memset(&attitude_raw, 0, sizeof(attitude_raw));
			memset(&airspeed_raw, 0, sizeof(airspeed_raw));
			memset(&actuators, 0, sizeof(actuators));

		    /* publish actuator controls with zero values */
		    for (unsigned i = 0; i < NUM_ACTUATOR_CONTROLS; i++) {
		             actuators.control[i] = 0.0f;
		     }

		     //Advertise that this controller will publish actuator
		     orb_advert_t actuator_pub = orb_advertise(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, &actuators);

		     // initialize simulink model
		     test_initialize();


			while (!thread_should_exit){
						/* wait for sensor update of 4 file descriptor for 1000 ms (1 second) */
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
								/* copy attitude raw data into local buffer */
								orb_copy(ORB_ID(vehicle_attitude), attitude_sub_fd, &attitude_raw);
								/* copy rc raw data into local buffer */
								orb_copy(ORB_ID(airspeed), airspeed_sub_fd, &airspeed_raw);

								// here would be the call to simulink function
								inVar = attitude_raw.roll;
								test_step();
								printf("%4.4f, %4.4f\n", inVar, outVar);


								/* sanity check and publish actuator outputs */
								if (isfinite(actuators.control[0]) &&
									isfinite(actuators.control[1]) &&
									isfinite(actuators.control[2]) &&
									isfinite(actuators.control[3])) {
								orb_publish(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, actuator_pub, &actuators);
									}


								//printf("[simulink_control] Loop processed \n");
							}
					}
			}

			printf("[simulink_control] exiting, stopping all motors.\n");
				thread_running = false;

				/* kill all outputs */
				for (unsigned i = 0; i < NUM_ACTUATOR_CONTROLS; i++)
					actuators.control[i] = 0.0f;

				orb_publish(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, actuator_pub, &actuators);


				fflush(stdout);
				exit(0);

				return 0;
}
