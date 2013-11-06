/**
 * @file logsd.c
 * Aapplication for PX4 logging into csv file.
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */
 
#include <nuttx/config.h>
#include <nuttx/sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
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

#include <systemlib/systemlib.h>
#include <mavlink/mavlink_log.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>


static bool thread_should_exit = false;		/**< daemon exit flag */
static bool thread_running = false;		/**< daemon status flag */
static int daemon_task;				/**< Handle of daemon task / thread */

/**
 * daemon management function.
 */
__EXPORT int logsd_main(int argc, char *argv[]);
/**
 * Mainloop of daemon.
 */
int logsd_thread_main(int argc, char *argv[]);

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

static const int MAX_NO_LOGFOLDER = 999;	/**< Maximum number of log folders */
static const int MAX_NO_LOGFILE = 999;		/**< Maximum number of log files */
static const char *mountpoint = "/fs/microsd";
static char folder_path[64];

static bool file_exist(const char *filename);
static int create_logfolder(void);
static int open_logfile(void);

/**
 * The daemon app only briefly exists to start
 * the background job. The stack size assigned in the
 * Makefile does only apply to this management task.
 *
 * The actual stack size should be set in the call
 * to task_create().
 */
int logsd_main(int argc, char *argv[])
{
	if (argc < 1)
			usage("missing command");

		if (!strcmp(argv[1], "start")) {

			if (thread_running) {
				warnx("logsd daemon already running\n");
				/* this is not an error */
				exit(0);
			}

			thread_should_exit = false;
			daemon_task = task_spawn_cmd("daemon",
						 SCHED_DEFAULT,
						 SCHED_PRIORITY_DEFAULT,
						 8192,
						 logsd_thread_main,
						 (argv) ? (const char **)&argv[2] : (const char **)NULL);
			exit(0);
		}

		if (!strcmp(argv[1], "stop")) {
			thread_should_exit = true;
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
int logsd_thread_main(int argc, char *argv[])
{
	thread_running = true;

	printf("logsd started\n");

	// refresh rate in ms
	int rate = 10;

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

	/* subscribe to rc channels topic */
		int rc_sub_fd = orb_subscribe(ORB_ID(manual_control_setpoint));
		orb_set_interval(rc_sub_fd, rate);

	/* subscribe to airspeed channels topic */
		int airspeed_sub_fd = orb_subscribe(ORB_ID(airspeed));
		orb_set_interval(airspeed_sub_fd, rate);

		/* one could wait for multiple topics with this technique, just using one here */
		struct pollfd fds[] = {
			{ .fd = sensor_sub_fd,   .events = POLLIN },
			{ .fd = gps_sub_fd,   .events = POLLIN },
			{ .fd = attitude_sub_fd,   .events = POLLIN },
			{ .fd = rc_sub_fd,   .events = POLLIN },
			{ .fd = airspeed_sub_fd,   .events = POLLIN },
		};

		int error_counter = 0;
		int n = 0;
		int i = 0;
		int m = 0;
		char buff_all[250];

		//buffs to hold data
		struct sensor_combined_s sensors_raw;
		struct vehicle_gps_position_s gps_raw;
		struct vehicle_attitude_s attitude_raw;
		struct manual_control_setpoint_s rc_raw;
		struct airspeed_s airspeed_raw;

		//set all buffers to 0
		memset(&sensors_raw, 0, sizeof(sensors_raw));
		memset(&gps_raw, 0, sizeof(gps_raw));
		memset(&attitude_raw, 0, sizeof(attitude_raw));
		memset(&rc_raw, 0, sizeof(rc_raw));
		memset(&airspeed_raw, 0, sizeof(airspeed_raw));

		/* create file to log into */
		printf("[logsd] start logging\n");
		create_logfolder();
		int log_file = open_logfile();

		//write header
		m = sprintf(buff_all, "Roll,Pitch,Yaw,Rollspeed,Pitchspeed,Yawspeed,Rollacc,Pitchacc,Yawacc,Elevator,Rudder,Throttle,Ailerons,Flaps,Latitude,Longitude,Altitude,Airspeed,GPSspeed,Acc_1,Acc_2,Acc_3,Gyr_1,Gyr_2,Gyr_3,Mag_1,Mag_2,Mag_3\n");
		n = write(log_file, buff_all, m);
		//printf("header size %d\n", n);


		while (!thread_should_exit){
			/* wait for sensor update of 4 file descriptor for 1000 ms (1 second) */
			int poll_ret = poll(fds, 1, 1000);

			/* handle the poll result */
			if (poll_ret == 0) {
				/* this means none of our providers is giving us data */
				printf("[logsd] Got no data within a second\n");
			} else if (poll_ret < 0) {
				/* this is seriously bad - should be an emergency */
				if (error_counter < 10 || error_counter % 50 == 0) {
					/* use a counter to prevent flooding (and slowing us down) */
					printf("[logsd] ERROR return value from poll(): %d\n"
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
					orb_copy(ORB_ID(manual_control_setpoint), rc_sub_fd, &rc_raw);
					/* copy rc raw data into local buffer */
					orb_copy(ORB_ID(airspeed), airspeed_sub_fd, &airspeed_raw);


					/* ---- logging starts here ---- */

						// write to already allocated buffer
						n = sprintf(buff_all, "%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%d,%d,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f\n",
							attitude_raw.roll,
							attitude_raw.pitch,
							attitude_raw.yaw,
							attitude_raw.rollspeed,
							attitude_raw.pitchspeed,
							attitude_raw.yawspeed,
							attitude_raw.rollacc,
							attitude_raw.pitchacc,
							attitude_raw.yawacc,
							rc_raw.pitch,
							rc_raw.yaw,
							rc_raw.throttle,
							rc_raw.roll,
							rc_raw.aux1,
							gps_raw.lat,
							gps_raw.lon,
							sensors_raw.baro_alt_meter,
							airspeed_raw.true_airspeed_m_s,
							gps_raw.vel_d_m_s,
							sensors_raw.accelerometer_m_s2[0],
							sensors_raw.accelerometer_m_s2[1],
							sensors_raw.accelerometer_m_s2[2],
							sensors_raw.gyro_rad_s[0],
							sensors_raw.gyro_rad_s[1],
							sensors_raw.gyro_rad_s[2],
							sensors_raw.magnetometer_ga[0],
							sensors_raw.magnetometer_ga[1],
							sensors_raw.magnetometer_ga[2]);
							//sensors_raw.timestamp);

						//printf("data written to buffer %d\n", n);
						//printf("%s", buff_all);

					m = write(log_file, buff_all, n);
					//check if write succesful
					if (m == -1)
					{
						printf("[logsd] write error: %s, exiting\n", strerror(errno));
						thread_should_exit = true;
						fsync(log_file);
					}
					i++;

					// flush data to file every second
					if (i%100==0)
					{
						fsync(log_file);
						//printf("%s", read_ptr);


						printf("%s", buff_all);
						printf("written to file %d\n", m);

						/*
						printf("timestamp: %PRIu64\n",
								sensors_raw.timestamp);
						*/
					}
				}
			}
		}

		fsync(log_file);
		close(log_file);
		printf("[logsd] stop logging\n");

		thread_running = false;

		return 0;
	}
int create_logfolder()
{
	/* make folder on sdcard */
	uint16_t folder_number = 1; // start with folder sess001
	int mkdir_ret;

	/* look for the next folder that does not exist */
	while (folder_number <= MAX_NO_LOGFOLDER) {
		/* set up folder path: e.g. /fs/microsd/sess001 */
		sprintf(folder_path, "%s/sess%03u", mountpoint, folder_number);
		mkdir_ret = mkdir(folder_path, S_IRWXU | S_IRWXG | S_IRWXO);
		/* the result is -1 if the folder exists */

		if (mkdir_ret == 0) {
			/* folder does not exist, success */
			break;

		} else if (mkdir_ret == -1) {
			/* folder exists already */
			folder_number++;
			continue;

		} else {
			warn("failed creating new folder");
			return -1;
		}
	}

	if (folder_number >= MAX_NO_LOGFOLDER) {
		/* we should not end up here, either we have more than MAX_NO_LOGFOLDER on the SD card, or another problem */
		warnx("all %d possible folders exist already.", MAX_NO_LOGFOLDER);
		return -1;
	}

	return 0;
}

int open_logfile()
{
	/* make folder on sdcard */
	uint16_t file_number = 1; // start with file log001

	/* string to hold the path to the log */
	char path_buf[64] = "";

	int fd = 0;

	/* look for the next file that does not exist */
	while (file_number <= MAX_NO_LOGFILE) {
		/* set up file path: e.g. /fs/microsd/sess001/log001.bin */
		sprintf(path_buf, "%s/log%03u.txt", folder_path, file_number);

		if (file_exist(path_buf)) {
			file_number++;
			continue;
		}

		fd = open(path_buf, O_CREAT | O_WRONLY | O_DSYNC);

		if (fd == 0) {
			warn("opening %s failed", path_buf);
		}

		warnx("logging to: %s.", path_buf);
		//mavlink_log_info(mavlink_fd, "[sdlog2] log: %s", path_buf);

		return fd;
	}

	if (file_number > MAX_NO_LOGFILE) {
		/* we should not end up here, either we have more than MAX_NO_LOGFILE on the SD card, or another problem */
		warnx("all %d possible files exist already.", MAX_NO_LOGFILE);
		return -1;
	}

	return 0;
}

bool file_exist(const char *filename)
{
	struct stat buffer;
	return stat(filename, &buffer) == 0;
}
