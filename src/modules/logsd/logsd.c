/**
 * @file logsd.c
 * Application for PX4 logging into csv file.
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
#include <uORB/topics/actuator_outputs.h>
#include <uORB/topics/skydog_autopilot_setpoint.h>

#include <systemlib/systemlib.h>
#include <mavlink/mavlink_log.h>

//#define __STDC_FORMAT_MACROS
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
// logging constants
static size_t buff_size = 700; // [bytes]
static int logging_frequency = 100; // [Hz]
static int flush_in_seconds = 5;
/*
static useconds_t sleep_delay = 0;
static useconds_t sleep_delay_wanted = 0;
static int delay = 500;
*/
static bool debug = true;
static int debug_file;
static FILE *file;

static bool file_exist(const char *filename);
static int create_logfolder(void);
static int open_logfile(void);
static int open_debugfile(void);
static int write_debugfile(char *message);
static void logsd_usage(const char *reason);

static void
logsd_usage(const char *reason)
{
        if (reason)
                fprintf(stderr, "%s\n", reason);

        errx(1, "usage: logsd {start|stop|status} [-f <log rate>] [-b <buffer size>] [-s <flush every x second>] [debug]\n"
             "\t-r\tLog rate in Hz, %d Hz default\n"
             "\t-b\tLog buffer size in bytes, default is %u\n"
        	 "\t-s\tFlush to SD card every defined second, default is %d\n"
             "\tdebug\tEnable debugging (not default)\n", logging_frequency, buff_size, flush_in_seconds);
}

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
			daemon_task = task_spawn_cmd("logsd",
						 SCHED_DEFAULT,
						 SCHED_PRIORITY_DEFAULT-10,
						 4096,
						 logsd_thread_main,
						 (const char **)argv);
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

// main logging function
int logsd_thread_main(int argc, char *argv[])
{
	thread_running = true;

	printf("logsd started\n");

	/* work around some stupidity in task_create's argv handling */
	argc -= 2;
	argv += 2;
	int ch;

	while ((ch = getopt(argc, argv, "f:b:s:debug")) != EOF) {
			switch (ch) {
			case 'f': {
							logging_frequency = strtoul(optarg, NULL, 10);
							if (logging_frequency < 1)
							{ logging_frequency = 1;
							  warnx("Minimal logging frequency is 1Hz, setting this");
							}
					}
					break;

			case 'b': {
						//delay	= strtoul(optarg, NULL, 10);
						size_t buff_size_s = strtoul(optarg, NULL, 10);
							if (buff_size_s > buff_size) {
								buff_size = buff_size_s;
							}else{
								warnx("Minimal buffer size is %d bytes, setting this", buff_size);
							}
					}
					break;

			case 's': {
						flush_in_seconds = strtoul(optarg, NULL, 10);
							if (flush_in_seconds < 1) {
								flush_in_seconds = 1;
								warnx("Minimal flushing is every second, setting this");
							}
						}
						break;

			case 'debug':
					debug = true;
					break;

			default:
					logsd_usage("unrecognized flag");
					errx(1, "exiting.");
			}
	}

	// refresh rate in ms
	unsigned rate = 1000/logging_frequency;
	//sleep_delay = (1000000 / logging_frequency)-delay;

	/* subscribe to sensor_combined topic */
		int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
		/* limit the interval*/
		orb_set_interval(sensor_sub_fd, rate);

	/* subscribe to gps topic */
		int gps_sub_fd = orb_subscribe(ORB_ID(vehicle_gps_position));
		bool gps_updated;

	/* subscribe to vehicle attitude topic */
		int attitude_sub_fd = orb_subscribe(ORB_ID(vehicle_attitude));
		bool attitude_updated;

	/* subscribe to rc channels topic */
		int rc_sub_fd = orb_subscribe(ORB_ID(manual_control_setpoint));
		bool rc_updated;

	/* subscribe to airspeed channels topic */
		int airspeed_sub_fd = orb_subscribe(ORB_ID(airspeed));
		bool airspeed_updated;

	/* subscribe to actuator outputs channels topic */
		int actuators_sub_fd = orb_subscribe(ORB_ID(actuator_outputs_0));
		bool actuators_updated;

	/* subscribe to skydog_attitude channels topic */
		int skydog_autopilot_setpoint_sub_fd = orb_subscribe(ORB_ID(skydog_autopilot_setpoint));
		bool skydog_updated;

		/* one could wait for multiple topics with this technique, just using one here */
		struct pollfd fds[] = {
			{ .fd = sensor_sub_fd,   .events = POLLIN},
		/*	{ .fd = gps_sub_fd,   .events = POLLIN},
			{  .fd = attitude_sub_fd,   .events = POLLIN},
			{  .fd = rc_sub_fd,   .events = POLLIN},
			{  .fd = airspeed_sub_fd,   .events = POLLIN},
			{  .fd = actuators_sub_fd,   .events = POLLIN},
			{  .fd = skydog_attitude_sub_fd,   .events = POLLIN} */
		};

		int error_counter = 0;
		size_t n = 0;
		int i = 0;
		ssize_t m = 0;
		bool rc_flag = false;

		// buff for log string
		char buff_all[buff_size];

		//buffs to hold data
		struct sensor_combined_s sensors_raw;
		struct vehicle_gps_position_s gps_raw;
		struct vehicle_attitude_s attitude_raw;
		struct manual_control_setpoint_s rc_raw;
		struct airspeed_s airspeed_raw;
		struct actuator_outputs_s actuator_outputs_raw;
		struct skydog_autopilot_setpoint_s skydog_autopilot_setpoint_raw;

		//set all buffers to 0
		memset(&sensors_raw, 0, sizeof(sensors_raw));
		memset(&gps_raw, 0, sizeof(gps_raw));
		memset(&attitude_raw, 0, sizeof(attitude_raw));
		memset(&rc_raw, 0, sizeof(rc_raw));
		memset(&airspeed_raw, 0, sizeof(airspeed_raw));
		memset(&actuator_outputs_raw, 0, sizeof(actuator_outputs_raw));
		memset(&skydog_autopilot_setpoint_raw, 0, sizeof(skydog_autopilot_setpoint_raw));
		memset(&buff_all, 0, sizeof(buff_all));


		/* create file to log into */
		printf("[logsd] start logging\n");
		create_logfolder();
		int log_file = open_logfile();
		if (debug)
		{
			//open debug file

			char file_path[64];
			sprintf(file_path, "%s/logsd_debug.txt", folder_path);
			file = fopen(file_path,"a+"); /* apend file (add text to a file or create a file if it does not exist.*/
			//debug_file = open_debugfile();
		}

		//write header
		n = snprintf(buff_all, buff_size,"%% Time[ms],Flag[t/f],Roll[rad],Pitch[rad],Yaw[rad],Rollspeed[rad/s],Pitchspeed[rad/s],Yawspeed[rad/s],"
				"Rollacc[rad/s2],Pitchacc[rad/s2],Yawacc[rad/s2],RC_Elevator[],RC_Rudder[],RC_Throttle[],RC_Ailerons[],RC_Flaps[],"
				"Elevator[],Rudder[],Throttle[],Ailerons[],Flaps[],Latitude[NSdegrees*e7],Longitude[EWdegrees*e7],GPSaltitude[m*e3],"
				"Altitude[m],Airspeed[m/s],DiffPressure[pa],GPSspeed[m/s],Mode[],AP_roll[rad],AP_speed[m/s],AP_altitude[m],AP_valid[t/f], AP_P[1],AP_P[2],AP_P[3],AP_U[1],AP_U[2],AP_U[3],AP_eta[rad],AP_dist[m]\n"
				"%% Logging frequency %d Hz\n",logging_frequency);

		//check if buffer large enough
		if (n>=buff_size)
		{
			// if not, write only buffer bytes
			m = write(log_file, buff_all, buff_size);
			if (debug)
			{	fprintf(file, "[logsd] Writing header, which is larger then buffer: %d \n",n);
				printf("[logsd] Writing header, which is larger then buffer: %d bytes\n",n);
			}
		}else
		{
			// if yes write actual size of string bytes
			m = write(log_file, buff_all, n);
			printf("%s", buff_all);
			printf("[logsd] Writing header: %zu bytes\n",n);
		}

		uint64_t timestamp_start = hrt_absolute_time();
		uint64_t timestamp = 0;

		do {
			/* wait for sensor update of file descriptor for 1000 ms (1 second) */
			int poll_ret = poll(fds, 1, 1000);

			/* handle the poll result */
			if (poll_ret == 0) {
				/* this means none of our providers is giving us data */
				if (debug)
				{fprintf(file, "%s","[logsd] Got no data within a second\n");
				 printf("[logsd] Got no data within a second\n");
				}
			} else if (poll_ret < 0) {
				/* this is seriously bad - should be an emergency */
				if (error_counter < 10 || error_counter % 50 == 0) {
					/* use a counter to prevent flooding (and slowing us down) */
					if (debug)
					{fprintf(file, "[logsd] ERROR return value from poll(): %d\n", poll_ret);
					 printf("[logsd] ERROR return value from poll(): %d\n", poll_ret);}
				}
				error_counter++;
			} else {

				if (fds[0].revents & POLLIN) {
					/* copy sensors raw data into local buffer */
					orb_copy(ORB_ID(sensor_combined), sensor_sub_fd, &sensors_raw);

					/* copy gps raw data into local buffer */
					orb_check(gps_sub_fd, &gps_updated);
					if (gps_updated)
					{
						orb_copy(ORB_ID(vehicle_gps_position), gps_sub_fd, &gps_raw);
					}

					/* copy attitude raw data into local buffer */
					orb_check(attitude_sub_fd, &attitude_updated);
					if (attitude_updated)
					{
						orb_copy(ORB_ID(vehicle_attitude), attitude_sub_fd, &attitude_raw);
					}

					/* copy rc raw data into local buffer */
					orb_check(rc_sub_fd, &rc_updated);
					if (rc_updated)
					{
						orb_copy(ORB_ID(manual_control_setpoint), rc_sub_fd, &rc_raw);
					}

					/* copy airspeed data into local buffer */
					orb_check(airspeed_sub_fd, &airspeed_updated);
					if (airspeed_updated)
					{
						orb_copy(ORB_ID(airspeed), airspeed_sub_fd, &airspeed_raw);
					}

					/* copy actuators data into local buffer */
					orb_check(actuators_sub_fd, &actuators_updated);
					if (actuators_updated)
					{
						orb_copy(ORB_ID(actuator_outputs_0), actuators_sub_fd, &actuator_outputs_raw);
					}

				    /* copy skydog attitude data into local buffer */
					orb_check(skydog_autopilot_setpoint_sub_fd, &skydog_updated);
					if (skydog_updated)
					{
						orb_copy(ORB_ID(skydog_autopilot_setpoint), skydog_autopilot_setpoint_sub_fd, &skydog_autopilot_setpoint_raw);
					}

					/* get timestamp in ms */
					timestamp = (hrt_absolute_time() - timestamp_start)/1000;

					/* create rc flag */
					if (rc_raw.aux3 > 0.0f)
					{
						rc_flag = true;
					}else{
						rc_flag = false;
					}

					/* ---- logging starts here ---- */

					// write to already allocated buffer
					n = snprintf(buff_all, buff_size,"%llu,%d,"	// timestamp,rc_flag,
							"%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,"  //euler angles + derivations
							"%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,"  // rc channels
							"%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,"  // output to servos
							"%d,%d,%d,"			// gps data
							"%4.4f,%4.4f,%4.4f,%4.4f,"  //altitude, airspeed
							"%d,"  // skydog autopilot mode
							"%4.4f,%4.4f,%4.4f,%d," 	// skydog path planning wanted values
							"%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f\n", 	// skydog path planning debug values
						timestamp,
						rc_flag,
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
						actuator_outputs_raw.output[1],		//elevator
						actuator_outputs_raw.output[2],		//rudder
						actuator_outputs_raw.output[3],		//throttle
						actuator_outputs_raw.output[0],		//ailerons
						actuator_outputs_raw.output[4],		//flaps
						gps_raw.lat,
						gps_raw.lon,
						gps_raw.alt,
						sensors_raw.baro_alt_meter,
						airspeed_raw.true_airspeed_m_s,
						sensors_raw.differential_pressure_pa,
						gps_raw.vel_m_s,
						skydog_autopilot_setpoint_raw.Autopilot_mode,
						skydog_autopilot_setpoint_raw.Roll_w,
						skydog_autopilot_setpoint_raw.Groundspeed_w,
						skydog_autopilot_setpoint_raw.Altitude_w,
						skydog_autopilot_setpoint_raw.Valid,
						skydog_autopilot_setpoint_raw.P[0],
						skydog_autopilot_setpoint_raw.P[1],
						skydog_autopilot_setpoint_raw.P[2],
						skydog_autopilot_setpoint_raw.U[0],
						skydog_autopilot_setpoint_raw.U[1],
						skydog_autopilot_setpoint_raw.U[2],
						skydog_autopilot_setpoint_raw.eta,
						skydog_autopilot_setpoint_raw.d2);


					// check if buffer not overloaded
					if (n>=buff_size)
					{
						if(debug)
						{fprintf(file, "[logsd] buffer overloaded, tried to write %d bytes, excessing data cropped\n", n);
						 printf("[logsd] buffer overloaded, tried to write %d bytes, excessing data cropped\n", n);
						}
						m = write(log_file, buff_all, buff_size);
					}else{
						m = write(log_file, buff_all, n);
						//m = fwrite(buff_all, sizeof(char), n, file);
						//m = fprintf(file, buff_all);
					}
					//check if write succesful
					if (m == -1)
					{
						if (debug)
						{fprintf(file, "[logsd] write error: %s, exiting\n", strerror(errno));
						 printf("[logsd] write error: %s, exiting\n", strerror(errno));}
						thread_should_exit = true;
						fsync(log_file);
					}
					// check if written all data from buffer
					if( m != n)
					{
						printf("[logsd] written: %d bytes from %d bytes in buffer\n", m, n);
						fprintf(file, "[logsd] written: %d bytes from %d bytes in buffer\n", m, n);
					}
					i++;

					// flush data to file every x second
					if (i%(flush_in_seconds*logging_frequency)==0)
					{
						fsync(log_file);
						fsync(file);
						i = 0;

						if (debug)
						{printf("%s", buff_all);
						 printf("written to file: %zu bytes\n", n);

						 //fprintf(file, "%s", buff_all);
						 //fprintf(file, "written to file: %d bytes\n", m);
						}

					}

				}
			}

		} while (!thread_should_exit);

		fsync(log_file);
		close(log_file);
		if (debug)
		{
			fclose(file);
		}
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
		/* set up file path: e.g. /fs/microsd/sess001/log001.csv */
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
