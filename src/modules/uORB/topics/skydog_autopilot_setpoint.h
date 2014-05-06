/**
 * @file skydog_autopilot_sepoint.h
 * Topic for Skydog autopilot implementation
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */

#ifndef SKYDOG_AUTOPILOT_SETPOINT_H_
#define SKYDOG_AUTOPILOT_SETPOINT_H_

#include <stdint.h>
#include <stdbool.h>
#include "../uORB.h"

/* control sets with pre-defined applications */
#define ORB_ID_SKYDOG_AUTOPILOT_SETPOINT	ORB_ID(skydog_autopilot_setpoint)

// skydog_autopilot_setpoint structure
struct skydog_autopilot_setpoint_s
{
	float Roll_w;		// wanted roll [rad]
	float Altitude_w;	// wanted altitude [m]
	float Groundspeed_w;	// wanted speed [m/s]
	bool Valid;			// other values valid?
	float Current_waypoint;	//index of current wp (for logging)
	float P[3];	//tracking point (for logging)
	float U[3]; //local position point (for logging)
	float eta; //current eta (for logging)
	float d2;	//current distance d2 (for logging)
	short Autopilot_mode; 	//current control mode (for logging)

};

/* register this as object request broker structure */
ORB_DECLARE(skydog_autopilot_setpoint);

#endif /* SKYDOG_AUTOPILOT_SETPOINT_H_ */
