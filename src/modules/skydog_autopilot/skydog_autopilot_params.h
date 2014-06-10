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
 * @file skydog_autopilot_params.h
 *
 * Parameters for skydog_autopilot
 */

#include <systemlib/param/param.h>

struct skydog_autopilot_params {
	float Alt_P;
	float Alt_I;
	float Pitch_P;
	float Pitch_I;
	float Pitch_rate_control_P;
	float Roll_control_I;
	float Roll_control_P;
	float Roll_rate_control_P;
	float Roll_yaw_FF;
	float Speed_control_I;
	float Speed_control_P;
	float Yaw_rate_control_I;
	float Yaw_rate_control_P;
	float Pitch_trim;

};

struct skydog_autopilot_param_handles {
	param_t Alt_P;
	param_t Alt_I;
	param_t Pitch_P;
	param_t Pitch_I;
	param_t Pitch_rate_control_P;
	param_t Roll_control_I;
	param_t Roll_control_P;
	param_t Roll_rate_control_P;
	param_t Roll_yaw_FF;
	param_t Speed_control_I;
	param_t Speed_control_P;
	param_t Yaw_rate_control_I;
	param_t Yaw_rate_control_P;
	param_t Pitch_trim;

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
