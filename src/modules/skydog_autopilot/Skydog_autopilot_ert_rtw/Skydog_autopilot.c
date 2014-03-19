/*
 * File: Skydog_autopilot.c
 *
 * Code generated for Simulink model 'Skydog_autopilot'.
 *
 * Model version                  : 1.215
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Mon Mar 10 17:46:49 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Skydog_autopilot.h"
#include "Skydog_autopilot_private.h"

/* Exported block parameters */
real32_T Alti_control_I = 0.3F;        /* Variable: Alti_control_I
                                        * Referenced by: '<S4>/Gain1'
                                        */
real32_T Alti_control_P = 0.6F;        /* Variable: Alti_control_P
                                        * Referenced by: '<S4>/Gain'
                                        */
real32_T Pitch_control_I = 8.0F;       /* Variable: Pitch_control_I
                                        * Referenced by: '<S6>/Gain1'
                                        */
real32_T Pitch_control_P = 8.5F;       /* Variable: Pitch_control_P
                                        * Referenced by: '<S6>/Gain'
                                        */
real32_T Roll_control_I = 5.0F;        /* Variable: Roll_control_I
                                        * Referenced by: '<S10>/Gain1'
                                        */
real32_T Roll_control_P = 8.0F;        /* Variable: Roll_control_P
                                        * Referenced by: '<S10>/Gain'
                                        */
real32_T Roll_rate_control_P = 3.0F;   /* Variable: Roll_rate_control_P
                                        * Referenced by: '<S9>/Gain'
                                        */
real32_T Roll_yaw_FF = 1.0F;           /* Variable: Roll_yaw_FF
                                        * Referenced by: '<S2>/Gain'
                                        */
real32_T Speed_control_I = 0.0F;       /* Variable: Speed_control_I
                                        * Referenced by: '<S5>/Gain1'
                                        */
real32_T Speed_control_P = 1.0F;       /* Variable: Speed_control_P
                                        * Referenced by: '<S5>/Gain'
                                        */
real32_T Yaw_rate_control_I = 0.0F;    /* Variable: Yaw_rate_control_I
                                        * Referenced by: '<S11>/Gain1'
                                        */
real32_T Yaw_rate_control_P = 0.01F;   /* Variable: Yaw_rate_control_P
                                        * Referenced by: '<S11>/Gain'
                                        */

/* Exported data definition */

/* Definition for custom storage class: ExportToFile */
real32_T Aileron_w;
real32_T Altitude_r;
real32_T Altitude_w;
real32_T Elevator_w;
real32_T Groundspeed_r;
real32_T Groundspeed_w;
int16_T Mode_w;
real32_T Pitch_acc_r;
real32_T Pitch_r;
real32_T Pitch_speed_r;
real32_T RC_aileron_r;
real32_T RC_elevator_r;
real32_T RC_flaps_r;
real32_T RC_rudder_r;
real32_T RC_throttle_r;
real32_T Roll_acc_r;
real32_T Roll_r;
real32_T Roll_speed_r;
real32_T Roll_w;
real32_T Rudder_w;
real32_T Throttle_w;
real32_T Yaw_acc_r;
real32_T Yaw_r;
real32_T Yaw_speed_r;
real32_T debug1;
real32_T debug2;

/* Block states (auto storage) */
D_Work_Skydog_autopilot Skydog_autopilot_DWork;

/* Real-time model */
RT_MODEL_Skydog_autopilot Skydog_autopilot_M_;
RT_MODEL_Skydog_autopilot *const Skydog_autopilot_M = &Skydog_autopilot_M_;

/* Model step function */
void Skydog_autopilot_step(void)
{
  /* local block i/o variables */
  real32_T rtb_Sum5;
  real32_T rtb_Sum6;
  real32_T rtb_Sum5_m;
  real32_T rtb_Sum3;
  real32_T rtb_Sum2;
  real32_T rtb_Sum1_i;
  real32_T rtb_Saturation4;
  real32_T rtb_Saturation5;
  real32_T rtb_Saturation1_p;
  real32_T rtb_Saturation1_e;
  real32_T rtb_Saturation1_n;
  real32_T rtb_Saturation6;
  real32_T rtb_Saturation1_l;
  real32_T rtb_Switch3;
  real32_T rtb_Sum1;

  /* Sum: '<S2>/Sum5' incorporates:
   *  Inport: '<Root>/Altitude_r'
   *  Inport: '<Root>/Altitude_w'
   */
  rtb_Sum5 = Altitude_w - Altitude_r;

  /* DiscreteIntegrator: '<S4>/Discrete-Time Integrator' */
  rtb_Saturation1_p = Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE;

  /* Sum: '<S4>/Sum3' incorporates:
   *  Gain: '<S4>/Gain'
   */
  rtb_Saturation1_p += Alti_control_P * rtb_Sum5;

  /* Saturate: '<S4>/Saturation1' */
  rtb_Saturation1_p = rtb_Saturation1_p >= 0.34906584F ? 0.34906584F :
    rtb_Saturation1_p <= (-0.34906584F) ? (-0.34906584F) : rtb_Saturation1_p;

  /* Switch: '<S7>/Switch1' incorporates:
   *  Inport: '<Root>/Mode_w'
   *  Inport: '<Root>/RC_elevator_r'
   */
  if (Mode_w > 1) {
    rtb_Saturation4 = rtb_Saturation1_p;
  } else {
    rtb_Saturation4 = RC_elevator_r;
  }

  /* End of Switch: '<S7>/Switch1' */

  /* Saturate: '<S3>/Saturation4' */
  rtb_Saturation4 = rtb_Saturation4 >= 0.34906584F ? 0.34906584F :
    rtb_Saturation4 <= (-0.34906584F) ? (-0.34906584F) : rtb_Saturation4;

  /* Sum: '<S3>/Sum6' incorporates:
   *  Inport: '<Root>/Pitch_r'
   */
  rtb_Sum6 = rtb_Saturation4 - Pitch_r;

  /* DiscreteIntegrator: '<S6>/Discrete-Time Integrator' */
  rtb_Saturation1_e = Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b;

  /* Sum: '<S6>/Sum3' incorporates:
   *  Gain: '<S6>/Gain'
   */
  rtb_Saturation1_e += Pitch_control_P * rtb_Sum6;

  /* Saturate: '<S6>/Saturation1' */
  rtb_Saturation1_e = rtb_Saturation1_e >= 3.0F ? 3.0F : rtb_Saturation1_e <=
    (-3.0F) ? (-3.0F) : rtb_Saturation1_e;

  /* Sum: '<S3>/Sum5' incorporates:
   *  Inport: '<Root>/Pitch_speed_r'
   */
  rtb_Sum5_m = rtb_Saturation1_e - Pitch_speed_r;

  /* Gain: '<S8>/Gain' */
  rtb_Switch3 = (-3.0F) * rtb_Sum5_m;

  /* Saturate: '<S8>/Saturation1' */
  rtb_Saturation1_l = rtb_Switch3 >= 0.261799395F ? 0.261799395F : rtb_Switch3 <=
    (-0.261799395F) ? (-0.261799395F) : rtb_Switch3;

  /* Saturate: '<S3>/Saturation2' */
  Elevator_w = rtb_Saturation1_l >= 0.261799395F ? 0.261799395F :
    rtb_Saturation1_l <= (-0.261799395F) ? (-0.261799395F) : rtb_Saturation1_l;

  /* Switch: '<S7>/Switch' incorporates:
   *  Inport: '<Root>/Mode_w'
   *  Inport: '<Root>/RC_aileron_r'
   *  Inport: '<Root>/Roll_w'
   */
  if (Mode_w > 1) {
    rtb_Saturation5 = Roll_w;
  } else {
    rtb_Saturation5 = RC_aileron_r;
  }

  /* End of Switch: '<S7>/Switch' */

  /* Saturate: '<S3>/Saturation5' */
  rtb_Saturation5 = rtb_Saturation5 >= 0.34906584F ? 0.34906584F :
    rtb_Saturation5 <= (-0.34906584F) ? (-0.34906584F) : rtb_Saturation5;

  /* Sum: '<S3>/Sum3' incorporates:
   *  Inport: '<Root>/Roll_r'
   */
  rtb_Sum3 = rtb_Saturation5 - Roll_r;

  /* DiscreteIntegrator: '<S10>/Discrete-Time Integrator' */
  rtb_Saturation1_n = Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f;

  /* Sum: '<S10>/Sum3' incorporates:
   *  Gain: '<S10>/Gain'
   */
  rtb_Saturation1_n += Roll_control_P * rtb_Sum3;

  /* Saturate: '<S10>/Saturation1' */
  rtb_Saturation1_n = rtb_Saturation1_n >= 3.0F ? 3.0F : rtb_Saturation1_n <=
    (-3.0F) ? (-3.0F) : rtb_Saturation1_n;

  /* Sum: '<S3>/Sum2' incorporates:
   *  Inport: '<Root>/Roll_speed_r'
   */
  rtb_Sum2 = rtb_Saturation1_n - Roll_speed_r;

  /* Gain: '<S9>/Gain' */
  rtb_Saturation6 = Roll_rate_control_P * rtb_Sum2;

  /* Saturate: '<S9>/Saturation1' */
  rtb_Saturation6 = rtb_Saturation6 >= 0.366519153F ? 0.366519153F :
    rtb_Saturation6 <= (-0.366519153F) ? (-0.366519153F) : rtb_Saturation6;

  /* Saturate: '<S3>/Saturation' */
  Aileron_w = rtb_Saturation6 >= 0.366519153F ? 0.366519153F : rtb_Saturation6 <=
    (-0.366519153F) ? (-0.366519153F) : rtb_Saturation6;

  /* Switch: '<S7>/Switch2' incorporates:
   *  Gain: '<S2>/Gain'
   *  Inport: '<Root>/Mode_w'
   *  Inport: '<Root>/RC_rudder_r'
   *  Inport: '<Root>/Roll_w'
   */
  if (Mode_w > 1) {
    rtb_Saturation6 = Roll_yaw_FF * Roll_w;
  } else {
    rtb_Saturation6 = RC_rudder_r;
  }

  /* End of Switch: '<S7>/Switch2' */

  /* Saturate: '<S3>/Saturation6' */
  rtb_Saturation6 = rtb_Saturation6 >= 1.0F ? 1.0F : rtb_Saturation6 <= (-1.0F) ?
    (-1.0F) : rtb_Saturation6;

  /* Sum: '<S3>/Sum1' incorporates:
   *  Inport: '<Root>/Yaw_speed_r'
   */
  rtb_Sum1_i = rtb_Saturation6 - Yaw_speed_r;

  /* Sum: '<S11>/Sum1' incorporates:
   *  DiscreteIntegrator: '<S11>/Discrete-Time Integrator'
   *  Gain: '<S11>/Gain'
   */
  rtb_Switch3 = Yaw_rate_control_P * rtb_Sum1_i +
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr;

  /* Saturate: '<S11>/Saturation1' */
  rtb_Switch3 = rtb_Switch3 >= 0.244346097F ? 0.244346097F : rtb_Switch3 <=
    (-0.244346097F) ? (-0.244346097F) : rtb_Switch3;

  /* Saturate: '<S3>/Saturation1' */
  Rudder_w = rtb_Switch3 >= 0.244346097F ? 0.244346097F : rtb_Switch3 <=
    (-0.244346097F) ? (-0.244346097F) : rtb_Switch3;

  /* Sum: '<S2>/Sum1' incorporates:
   *  Inport: '<Root>/Groundspeed_r'
   *  Inport: '<Root>/Groundspeed_w'
   */
  rtb_Sum1 = Groundspeed_w - Groundspeed_r;

  /* Switch: '<S7>/Switch3' incorporates:
   *  Inport: '<Root>/Mode_w'
   *  Inport: '<Root>/RC_throttle_r'
   */
  if (Mode_w > 1) {
    /* Sum: '<S5>/Sum3' incorporates:
     *  DiscreteIntegrator: '<S5>/Discrete-Time Integrator'
     *  Gain: '<S5>/Gain'
     */
    rtb_Switch3 = Speed_control_P * rtb_Sum1 +
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d;

    /* Saturate: '<S5>/Saturation1' */
    rtb_Switch3 = rtb_Switch3 >= 1.0F ? 1.0F : rtb_Switch3 <= 0.0F ? 0.0F :
      rtb_Switch3;
  } else {
    rtb_Switch3 = RC_throttle_r;
  }

  /* End of Switch: '<S7>/Switch3' */

  /* Saturate: '<S3>/Saturation3' */
  Throttle_w = rtb_Switch3 >= 1.0F ? 1.0F : rtb_Switch3 <= 0.0F ? 0.0F :
    rtb_Switch3;

  /* Gain: '<S1>/Gain' */
  debug1 = 1.0F * rtb_Saturation1_e;

  /* Gain: '<S1>/Gain1' */
  debug2 = 1.0F * rtb_Saturation1_l;

  /* Gain: '<S4>/Gain1' */
  rtb_Switch3 = Alti_control_I * rtb_Sum5;

  /* Gain: '<S5>/Gain1' */
  rtb_Saturation1_l = Speed_control_I * rtb_Sum1;

  /* Update for DiscreteIntegrator: '<S4>/Discrete-Time Integrator' */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE = 0.01F * rtb_Switch3 +
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE >= 0.52359879F) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE = 0.52359879F;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE <= (-0.52359879F))
    {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE = -0.52359879F;
    }
  }

  /* End of Update for DiscreteIntegrator: '<S4>/Discrete-Time Integrator' */

  /* Update for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S6>/Gain1'
   */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b = Pitch_control_I *
    rtb_Sum6 * 0.01F + Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b >= 4.5F) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b = 4.5F;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b <= (-4.5F)) {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b = -4.5F;
    }
  }

  /* End of Update for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' */

  /* Update for DiscreteIntegrator: '<S10>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S10>/Gain1'
   */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f = Roll_control_I *
    rtb_Sum3 * 0.01F + Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f >= 4.5F) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f = 4.5F;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f <= (-4.5F)) {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f = -4.5F;
    }
  }

  /* End of Update for DiscreteIntegrator: '<S10>/Discrete-Time Integrator' */

  /* Update for DiscreteIntegrator: '<S11>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S11>/Gain1'
   */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr = Yaw_rate_control_I *
    rtb_Sum1_i * 0.01F + Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr >= 0.366519153F) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr = 0.366519153F;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr <= (-0.366519153F))
    {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr = -0.366519153F;
    }
  }

  /* End of Update for DiscreteIntegrator: '<S11>/Discrete-Time Integrator' */

  /* Update for DiscreteIntegrator: '<S5>/Discrete-Time Integrator' */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d = 0.01F *
    rtb_Saturation1_l + Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d >= 0.52359879F) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d = 0.52359879F;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d <= (-0.52359879F))
    {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d = -0.52359879F;
    }
  }

  /* End of Update for DiscreteIntegrator: '<S5>/Discrete-Time Integrator' */
}

/* Model initialize function */
void Skydog_autopilot_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Skydog_autopilot_M, (NULL));

  /* block I/O */

  /* custom signals */
  Elevator_w = 0.0F;
  Aileron_w = 0.0F;
  Rudder_w = 0.0F;
  Throttle_w = 0.0F;
  debug1 = 0.0F;
  debug2 = 0.0F;

  /* states (dwork) */
  (void) memset((void *)&Skydog_autopilot_DWork, 0,
                sizeof(D_Work_Skydog_autopilot));

  /* external inputs */
  Roll_w = 0.0F;
  Altitude_w = 0.0F;
  Groundspeed_w = 0.0F;
  Roll_r = 0.0F;
  Altitude_r = 0.0F;
  Groundspeed_r = 0.0F;
  Pitch_r = 0.0F;
  Yaw_r = 0.0F;
  Roll_speed_r = 0.0F;
  Pitch_speed_r = 0.0F;
  Yaw_speed_r = 0.0F;
  Roll_acc_r = 0.0F;
  Pitch_acc_r = 0.0F;
  Yaw_acc_r = 0.0F;
  RC_aileron_r = 0.0F;
  RC_elevator_r = 0.0F;
  RC_rudder_r = 0.0F;
  RC_throttle_r = 0.0F;
  RC_flaps_r = 0.0F;
  Mode_w = 0;
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
