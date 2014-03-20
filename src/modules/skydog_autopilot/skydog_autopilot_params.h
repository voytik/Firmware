 /**
 * @file skydog_autopilot_params.h
 * Parameters for skydog_autopilot
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */

#include <systemlib/param/param.h>

struct skydog_autopilot_params {
	float Alti_control_I;
	float Alti_control_P;
	float Pitch_control_I;
	float Pitch_control_P;
	float Pitch_rate_control_P;
	float Roll_control_I;
	float Roll_control_P;
	float Roll_rate_control;
	float Roll_yaw_FF;
	float Speed_control_I;
	float Speed_control_P;
	float Yaw_rate_control_I;
	float Yaw_rate_control_P;

};

struct skydog_autopilot_param_handles {
	param_t Alti_control_I;
	param_t Alti_control_P;
	param_t Pitch_control_I;
	param_t Pitch_control_P;
	param_t Pitch_rate_control_P;
	param_t Roll_control_I;
	param_t Roll_control_P;
	param_t Roll_rate_control;
	param_t Roll_yaw_FF;
	param_t Speed_control_I;
	param_t Speed_control_P;
	param_t Yaw_rate_control_I;
	param_t Yaw_rate_control_P;
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
