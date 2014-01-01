/**
 * @file skydog_attitude.c
 * Application for attitude control from simulink (generated code).
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
#include <uORB/topics/skydog_attitude.h>

#include <systemlib/systemlib.h>

// simulink model includes
/*
#include <skydog_attitude/Skydog_autopilot_ert_rtw/SkydogSignals.h>
#include <skydog_attitude/Skydog_autopilot_ert_rtw/Skydog_autopilot.h>
#include <skydog_attitude/Skydog_autopilot_ert_rtw/rtwtypes.h>
*/
 
static bool thread_should_exit = false;		/**< daemon exit flag */
static bool thread_running = false;		/**< daemon status flag */
static int daemon_task;				/**< Handle of daemon task / thread */

/**
 * daemon management function.
 */
__EXPORT int skydog_attitude_main(int argc, char *argv[]);
/**
 * Mainloop of daemon.
 */
int skydog_attitude_thread_main(int argc, char *argv[]);

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
int skydog_attitude_main(int argc, char *argv[])
{
	if (argc < 1)
			usage("missing command");

		if (!strcmp(argv[1], "start")) {

			if (thread_running) {
				warnx("skydog_attitude daemon already running\n");
				/* this is not an error */
				exit(0);
			}

			thread_should_exit = false;
			daemon_task = task_spawn_cmd("skydog_attitude",
						 SCHED_DEFAULT,
						 SCHED_PRIORITY_MAX - 20,
						 2048,
						 skydog_attitude_thread_main,
						 (argv) ? (const char **)&argv[2] : (const char **)NULL);
			exit(0);
		}

		if (!strcmp(argv[1], "stop")) {
			thread_should_exit = true;
			printf("skydog_attitude stopped\n");
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
int skydog_attitude_thread_main(int argc, char *argv[])
{
	thread_running = true;

	printf("skydog_attitude started\n");

	// refresh rate in ms
	int rate = 10;
	int error_counter = 0;

		/* subscribe to sensor_combined topic */
			int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
			/* limit the interval*/
			orb_set_interval(sensor_sub_fd, rate);

			/* one could wait for multiple topics with this technique */
			struct pollfd fds[] = {
				{ .fd = sensor_sub_fd,   .events = POLLIN }
			};

			//buffs to hold sensors data
			struct sensor_combined_s sensors_raw;

			// output struct
			struct skydog_attitude_s attitude;

			//set all buffers to 0
			memset(&sensors_raw, 0, sizeof(sensors_raw));
			memset(&attitude, 0, sizeof(attitude));


		    //Advertise that this controller will publish actuator and make first publication
		    orb_advert_t attitude_pub = orb_advertise(ORB_ID_SKYDOG_ATTITUDE_H_, &attitude);

		     // initialize simulink model
		     //skydog_attitude_initialize();


			while (!thread_should_exit){
				/* wait for sensor update of file descriptor for 1000 ms (1 second) */
				int poll_ret = poll(fds, 1, 1000);

				/* handle the poll result */
				if (poll_ret == 0) {
					/* this means none of our providers is giving us data */
					printf("[skydog_attitude] Got no data within a second\n");
				} else if (poll_ret < 0) {
					/* this is seriously bad - should be an emergency */
					if (error_counter < 10 || error_counter % 50 == 0) {
						/* use a counter to prevent flooding (and slowing us down) */
						printf("[skydog_attitude] ERROR return value from poll(): %d\n"
							, poll_ret);
					}
					error_counter++;
				} else {

					if (fds[0].revents & POLLIN) {
						/* copy sensors raw data into local buffer */
						orb_copy(ORB_ID(sensor_combined), sensor_sub_fd, &sensors_raw);

							//fill in inputs for simulink code


							//run Simulink code
							//Skydog_autopilot_step();

							// copy output
							attitude.Roll = 0;
							attitude.Pitch = 0;
							attitude.Yaw = 0;
							attitude.Rollspeed = 0;
							attitude.Pitchspeed = 0;
							attitude.Yawspeed = 0;
							attitude.Valid = true;

						}

						/* publish attitude */

						orb_publish(ORB_ID_SKYDOG_ATTITUDE_H_, attitude_pub, &attitude);
						}

					}


			printf("[skydog_attitude] exiting\n");

			thread_running = false;

			// set all attitude to zero and flag valid to false
			attitude.Roll = 0;
			attitude.Pitch = 0;
			attitude.Yaw = 0;
			attitude.Rollspeed = 0;
			attitude.Pitchspeed = 0;
			attitude.Yawspeed = 0;
			attitude.Valid = false;

			// publish
			orb_publish(ORB_ID_SKYDOG_ATTITUDE_H_, attitude_pub, &attitude);


			fflush(stdout);
			exit(0);

			return 0;
}
