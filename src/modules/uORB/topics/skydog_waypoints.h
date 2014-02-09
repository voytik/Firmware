/**
 * @file skydog_waypoins.h
 * Topic for Skydog waypoints transition from mavlink to simulink code
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */

#ifndef SKYDOG_WAYPOINTS_H_
#define SKYDOG_WAYPOINTS_H_

#include <stdint.h>
#include <stdbool.h>
#include "../uORB.h"

#define WAYPOINTS_MAX   15

/* control sets with pre-defined applications */
#define ORB_ID_SKYDOG_WAYPOINTS_H_	ORB_ID(skydog_waypoints)

// skydog_autopilot_setpoint structure
struct skydog_waypoints_s
{
	struct {
	float longitude;		//  long [EWdegrees*e7]
	float latitude;	// lat [NSdegrees*e7]
	float altitude;		// alt [m]
	float speed;		//  speed [m/s]
	} waypoints[WAYPOINTS_MAX];

	uint8_t wpm_count;     // number of valid waypoints

};

/* register this as object request broker structure */
ORB_DECLARE(skydog_waypoints);

#endif /* SKYDOG_WAYPOINTS_H_ */
