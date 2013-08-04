/****************************************************************************
 *
 *   Copyright (C) 2012 PX4 Development Team. All rights reserved.
 *   Author: @author Example User <mail@example.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
 
/**
 * @file sdlog.c
 * Minimal application example for PX4 autopilot.
 */
 
#include <nuttx/config.h>
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
#include <unistd.h>
#include <drivers/drv_hrt.h>

#include <uORB/uORB.h>
#include <uORB/topics/sensor_combined.h>

#include <systemlib/systemlib.h>
#include <mavlink/mavlink_log.h>

static const int MAX_NO_LOGFOLDER = 999;	/**< Maximum number of log folders */
static const int MAX_NO_LOGFILE = 999;		/**< Maximum number of log files */
static const char *mountpoint = "/fs/microsd";
static char folder_path[64];
 
__EXPORT int logsd_main(int argc, char *argv[]);

static bool file_exist(const char *filename);
static int create_logfolder(void);
static int open_logfile(void);
 
int logsd_main(int argc, char *argv[])
{
	printf("logsd started\n");


	/* subscribe to sensor_combined topic */
		int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
		/* limit the interval to 100Hz*/
		orb_set_interval(sensor_sub_fd, 100);

		/* one could wait for multiple topics with this technique, just using one here */
		struct pollfd fds[] = {
			{ .fd = sensor_sub_fd,   .events = POLLIN },
			/* there could be more file descriptors here, in the form like:
			 * { .fd = other_sub_fd,   .events = POLLIN },
			 */
		};

		int error_counter = 0;
		int n = 8;
		char read_ptr[28];
		int i = 0;

		/* create file to log into */
		printf("[logsd] start logging\n");
		create_logfolder();
		int log_file = open_logfile();


		while (i<5000) {
			/* wait for sensor update of 1 file descriptor for 1000 ms (1 second) */
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
					/* obtained data for the first file descriptor */
					struct sensor_combined_s raw;
					/* copy sensors raw data into local buffer */
					orb_copy(ORB_ID(sensor_combined), sensor_sub_fd, &raw);
					/*
					 *
					 printf("[logsd] Accelerometer:\t%8.4f\t%8.4f\t%8.4f\n",
						(double)raw.accelerometer_m_s2[0],
						(double)raw.accelerometer_m_s2[1],
						(double)raw.accelerometer_m_s2[2]);
						*/

					/* ---- logging starts here ---- */

					/*
					snprintf(read_ptr, 28, "%8.4f,%8.4f,%8.4f\n",
							raw.accelerometer_m_s2[0],
							raw.accelerometer_m_s2[1],
							raw.accelerometer_m_s2[2]);
					 */

					n = write(log_file, read_ptr, 28);


					//printf("%s", read_ptr);

					i++;

				}
				/* there could be more file descriptors here, in the form like:
				 * if (fds[1..n].revents & POLLIN) {}
				 */
			}
		}

		fsync(log_file);
		close(log_file);
		printf("[logsd] stop logging\n");

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
