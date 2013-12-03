/*
 * File: Skydog_autopilot.c
 *
 * Code generated for Simulink model 'Skydog_autopilot'.
 *
<<<<<<< HEAD
 * Model version                  : 1.148
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Mon Dec 02 22:04:15 2013
=======
 * Model version                  : 1.121
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Sun Nov 24 22:56:20 2013
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Skydog_autopilot.h"
#include "Skydog_autopilot_private.h"

/* Exported data definition */

/* Definition for custom storage class: ExportToFile */
<<<<<<< HEAD
real32_T Aileron_w;
real32_T Altitude_r;
real32_T Altitude_w;
real32_T Elevator_w;
real32_T Flaps_w;
real32_T Groundspeed_r;
real32_T Groundspeed_w;
int16_T Mode_w;
real32_T Pitch_acc_r;
=======
real32_T Airspeed_r;
real32_T Airspeed_w;
real32_T Altitude_r;
real32_T Altitude_w;
real32_T Pitch_acc_y;
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
real32_T Pitch_r;
real32_T Pitch_speed_r;
real32_T RC_aileron_r;
real32_T RC_elevator_r;
<<<<<<< HEAD
real32_T RC_flaps_r;
real32_T RC_rudder_r;
real32_T RC_throttle_r;
real32_T Roll_acc_r;
real32_T Roll_r;
real32_T Roll_speed_r;
real32_T Roll_w;
real32_T Rudder_w;
real32_T Throttle_w;
=======
real32_T RC_flaps_y;
real32_T RC_rudder_y;
real32_T RC_throttle_y;
real32_T Roll_r;
real32_T Roll_speed_r;
real32_T Roll_w;
int16_T Stab_mode_w;
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
real32_T Yaw_acc_r;
real32_T Yaw_r;
real32_T Yaw_speed_r;

<<<<<<< HEAD
=======
/* External inputs (root inport signals with auto storage) */
ExternalInputs_Skydog_autopilot Skydog_autopilot_U;

>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
/* Real-time model */
RT_MODEL_Skydog_autopilot Skydog_autopilot_M_;
RT_MODEL_Skydog_autopilot *const Skydog_autopilot_M = &Skydog_autopilot_M_;

/* Model step function */
void Skydog_autopilot_step(void)
{
<<<<<<< HEAD
  /* Constant: '<S3>/Constant' */
  Elevator_w = Skydog_autopilot_P.Constant_Value;

  /* Constant: '<S3>/Constant1' */
  Aileron_w = Skydog_autopilot_P.Constant1_Value;

  /* Constant: '<S3>/Constant2' */
  Rudder_w = Skydog_autopilot_P.Constant2_Value;

  /* Constant: '<S3>/Constant3' */
  Throttle_w = Skydog_autopilot_P.Constant3_Value;

  /* Constant: '<S3>/Constant4' */
  Flaps_w = Skydog_autopilot_P.Constant4_Value;
=======
  /* (no output/update code required) */
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
}

/* Model initialize function */
void Skydog_autopilot_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Skydog_autopilot_M, (NULL));

<<<<<<< HEAD
  /* block I/O */

  /* custom signals */
  Elevator_w = 0.0F;
  Aileron_w = 0.0F;
  Rudder_w = 0.0F;
  Throttle_w = 0.0F;
  Flaps_w = 0.0F;

  /* external inputs */
  Roll_w = 0.0F;
  Altitude_w = 0.0F;
  Groundspeed_w = 0.0F;
  Roll_r = 0.0F;
  Altitude_r = 0.0F;
  Groundspeed_r = 0.0F;
=======
  /* external inputs */
  (void) memset((void *)&Skydog_autopilot_U, 0,
                sizeof(ExternalInputs_Skydog_autopilot));
  Roll_w = 0.0F;
  Altitude_w = 0.0F;
  Airspeed_w = 0.0F;
  Roll_r = 0.0F;
  Altitude_r = 0.0F;
  Airspeed_r = 0.0F;
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
  Pitch_r = 0.0F;
  Yaw_r = 0.0F;
  Roll_speed_r = 0.0F;
  Pitch_speed_r = 0.0F;
  Yaw_speed_r = 0.0F;
<<<<<<< HEAD
  Roll_acc_r = 0.0F;
  Pitch_acc_r = 0.0F;
  Yaw_acc_r = 0.0F;
  RC_aileron_r = 0.0F;
  RC_elevator_r = 0.0F;
  RC_rudder_r = 0.0F;
  RC_throttle_r = 0.0F;
  RC_flaps_r = 0.0F;
  Mode_w = 0;
=======
  Pitch_acc_y = 0.0F;
  Yaw_acc_r = 0.0F;
  RC_aileron_r = 0.0F;
  RC_elevator_r = 0.0F;
  RC_rudder_y = 0.0F;
  RC_throttle_y = 0.0F;
  RC_flaps_y = 0.0F;
  Stab_mode_w = 0;
>>>>>>> 8587d3062e901e9cd7c49fdf92e2582568eabde1
}

/* Model terminate function */
void Skydog_autopilot_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
