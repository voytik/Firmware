/*
 * File: Skydog_path_planning.c
 *
 * Code generated for Simulink model 'Skydog_path_planning'.
 *
<<<<<<< HEAD
 * Model version                  : 1.148
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Mon Dec 02 22:04:28 2013
=======
 * Model version                  : 1.130
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Tue Nov 26 21:43:50 2013
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Skydog_path_planning.h"
#include "Skydog_path_planning_private.h"

/* Exported data definition */

/* Definition for custom storage class: ExportToFile */
<<<<<<< HEAD
real32_T Altitude2_r;
real32_T Altitude_w;
real32_T Groundspeed2_r;
real32_T Groundspeed_w;
int32_T Lattitude_r;
int32_T Longitude_r;
int32_T Mode2_w;
=======
real32_T Airspeed_w;
real32_T Altitude_r2;
real32_T Altitude_w;
real32_T Groundspeed_r;
int32_T Lattitude_r;
int32_T Longitude_r;
int32_T Mode_w;
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
real_T Non_fly_zone_w;
real32_T Roll_w;
real32_T Waypoints_w;

/* Real-time model */
RT_MODEL_Skydog_path_planning Skydog_path_planning_M_;
RT_MODEL_Skydog_path_planning *const Skydog_path_planning_M =
  &Skydog_path_planning_M_;

/* Model step function */
void Skydog_path_planning_step(void)
{
<<<<<<< HEAD
  /* Gain: '<S1>/Gain3' */
  Roll_w = Skydog_path_planning_P.Gain3_Gain * 0.0F;
=======
  /* Gain: '<S1>/Gain' */
  Roll_w = Skydog_path_planning_P.Gain_Gain * 0.0F;
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1

  /* Gain: '<S1>/Gain1' */
  Altitude_w = Skydog_path_planning_P.Gain1_Gain * 0.0F;

  /* Gain: '<S1>/Gain2' */
<<<<<<< HEAD
  Groundspeed_w = Skydog_path_planning_P.Gain2_Gain * 0.0F;
=======
  Airspeed_w = Skydog_path_planning_P.Gain2_Gain * 0.0F;
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
}

/* Model initialize function */
void Skydog_path_planning_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Skydog_path_planning_M, (NULL));

  /* block I/O */

  /* custom signals */
  Roll_w = 0.0F;
  Altitude_w = 0.0F;
<<<<<<< HEAD
  Groundspeed_w = 0.0F;

  /* external inputs */
  Altitude2_r = 0.0F;
  Lattitude_r = 0;
  Longitude_r = 0;
  Groundspeed2_r = 0.0F;
  Waypoints_w = 0.0F;
  Non_fly_zone_w = 0.0;
  Mode2_w = 0;
=======
  Airspeed_w = 0.0F;

  /* external inputs */
  Altitude_r2 = 0.0F;
  Lattitude_r = 0;
  Longitude_r = 0;
  Groundspeed_r = 0.0F;
  Waypoints_w = 0.0F;
  Non_fly_zone_w = 0.0;
  Mode_w = 0;
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
}

/* Model terminate function */
void Skydog_path_planning_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
