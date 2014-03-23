/****************************************************************************
 *
 *   Copyright (C) 2013 Anton Babushkin. All rights reserved.
 *   Author: 	Anton Babushkin	<rk3dov@gmail.com>
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

/*
 * @file position_estimator_inav_params.h
 *
 * Parameters for Position Estimator
 */

#include <systemlib/param/param.h>

struct skydog_autopilot_params {
	float alt_I;
	/*
	float w_alt_acc;
	float w_alt_sonar;
	float w_pos_gps_p;
	float w_pos_gps_v;
	float w_pos_acc;
	float w_pos_flow;
	float w_acc_bias;
	float flow_k;
	float sonar_filt;
	float sonar_err;
	float land_t;
	float land_disp;
	float land_thr;
	*/
};

struct skydog_autopilot_param_handles {
	param_t alt_I;
	/*
	param_t w_alt_acc;
	param_t w_alt_sonar;
	param_t w_pos_gps_p;
	param_t w_pos_gps_v;
	param_t w_pos_acc;
	param_t w_pos_flow;
	param_t w_acc_bias;
	param_t flow_k;
	param_t sonar_filt;
	param_t sonar_err;
	param_t land_t;
	param_t land_disp;
	param_t land_thr;
	*/
};

/**
 * Initialize all parameter handles and values
 *
 */
int parameters_init(struct skydog_autopilot_param_handles *h);

/**
 * Update all parameters
 *
 */
int parameters_update(const struct skydog_autopilot_param_handles *h, struct skydog_autopilot_params *p);
