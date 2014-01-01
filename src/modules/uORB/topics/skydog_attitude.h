/**
 * @file skydog_attitude.h
 * Topic for Skydog attitude
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */

#ifndef SKYDOG_ATTITUDE_H_
#define SKYDOG_ATTITUDE_H_

#include <stdint.h>
#include <stdbool.h>
#include "../uORB.h"

/* control sets with pre-defined applications */
#define ORB_ID_SKYDOG_ATTITUDE_H_	ORB_ID(skydog_attitude)

// skydog_autopilot_setpoint structure
struct skydog_attitude_s
{
	float Roll;		//  roll [rad]
	float Pitch;	// pitch [rad]
	float Yaw;		// yaw [rad]
	float Rollspeed;		//  rollspeed [rad/s]
	float Pitchspeed;	// pitchspeed [rad/s]
	float Yawspeed;		// yawspeed [rad/s]
	bool Valid;			// other values valid?
};

/* register this as object request broker structure */
ORB_DECLARE(skydog_attitude);

#endif /* SKYDOG_ATTITUDE_H_ */
