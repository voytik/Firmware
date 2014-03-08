/*
 * File: Skydog_autopilot.c
 *
 * Code generated for Simulink model 'Skydog_autopilot'.
 *
 * Model version                  : 1.189
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Sat Mar 08 17:38:46 2014
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
  rtb_Saturation1_p += Skydog_autopilot_P.Gain_Gain_l * rtb_Sum5;

  /* Saturate: '<S4>/Saturation1' */
  rtb_Saturation1_p = rtb_Saturation1_p >=
    Skydog_autopilot_P.Saturation1_UpperSat_j ?
    Skydog_autopilot_P.Saturation1_UpperSat_j : rtb_Saturation1_p <=
    Skydog_autopilot_P.Saturation1_LowerSat_i ?
    Skydog_autopilot_P.Saturation1_LowerSat_i : rtb_Saturation1_p;

  /* Switch: '<S7>/Switch1' incorporates:
   *  Inport: '<Root>/Mode_w'
   *  Inport: '<Root>/RC_elevator_r'
   */
  if (Mode_w > Skydog_autopilot_P.Switch1_Threshold) {
    rtb_Saturation4 = rtb_Saturation1_p;
  } else {
    rtb_Saturation4 = RC_elevator_r;
  }

  /* End of Switch: '<S7>/Switch1' */

  /* Saturate: '<S3>/Saturation4' */
  rtb_Saturation4 = rtb_Saturation4 >= Skydog_autopilot_P.Saturation4_UpperSat ?
    Skydog_autopilot_P.Saturation4_UpperSat : rtb_Saturation4 <=
    Skydog_autopilot_P.Saturation4_LowerSat ?
    Skydog_autopilot_P.Saturation4_LowerSat : rtb_Saturation4;

  /* Sum: '<S3>/Sum6' incorporates:
   *  Inport: '<Root>/Pitch_r'
   */
  rtb_Sum6 = rtb_Saturation4 - Pitch_r;

  /* DiscreteIntegrator: '<S6>/Discrete-Time Integrator' */
  rtb_Saturation1_e = Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b;

  /* Sum: '<S6>/Sum3' incorporates:
   *  Gain: '<S6>/Gain'
   */
  rtb_Saturation1_e += Skydog_autopilot_P.Gain_Gain_b * rtb_Sum6;

  /* Saturate: '<S6>/Saturation1' */
  rtb_Saturation1_e = rtb_Saturation1_e >=
    Skydog_autopilot_P.Saturation1_UpperSat_k ?
    Skydog_autopilot_P.Saturation1_UpperSat_k : rtb_Saturation1_e <=
    Skydog_autopilot_P.Saturation1_LowerSat_c ?
    Skydog_autopilot_P.Saturation1_LowerSat_c : rtb_Saturation1_e;

  /* Sum: '<S3>/Sum5' incorporates:
   *  Inport: '<Root>/Pitch_speed_r'
   */
  rtb_Sum5_m = rtb_Saturation1_e - Pitch_speed_r;

  /* Gain: '<S8>/Gain' */
  rtb_Switch3 = Skydog_autopilot_P.Gain_Gain_p * rtb_Sum5_m;

  /* Saturate: '<S8>/Saturation1' */
  rtb_Saturation1_l = rtb_Switch3 >= Skydog_autopilot_P.Saturation1_UpperSat_i ?
    Skydog_autopilot_P.Saturation1_UpperSat_i : rtb_Switch3 <=
    Skydog_autopilot_P.Saturation1_LowerSat_g ?
    Skydog_autopilot_P.Saturation1_LowerSat_g : rtb_Switch3;

  /* Saturate: '<S3>/Saturation2' */
  Elevator_w = rtb_Saturation1_l >= Skydog_autopilot_P.Saturation2_UpperSat ?
    Skydog_autopilot_P.Saturation2_UpperSat : rtb_Saturation1_l <=
    Skydog_autopilot_P.Saturation2_LowerSat ?
    Skydog_autopilot_P.Saturation2_LowerSat : rtb_Saturation1_l;

  /* Switch: '<S7>/Switch' incorporates:
   *  Inport: '<Root>/Mode_w'
   *  Inport: '<Root>/RC_aileron_r'
   *  Inport: '<Root>/Roll_w'
   */
  if (Mode_w > Skydog_autopilot_P.Switch_Threshold) {
    rtb_Saturation5 = Roll_w;
  } else {
    rtb_Saturation5 = RC_aileron_r;
  }

  /* End of Switch: '<S7>/Switch' */

  /* Saturate: '<S3>/Saturation5' */
  rtb_Saturation5 = rtb_Saturation5 >= Skydog_autopilot_P.Saturation5_UpperSat ?
    Skydog_autopilot_P.Saturation5_UpperSat : rtb_Saturation5 <=
    Skydog_autopilot_P.Saturation5_LowerSat ?
    Skydog_autopilot_P.Saturation5_LowerSat : rtb_Saturation5;

  /* Sum: '<S3>/Sum3' incorporates:
   *  Inport: '<Root>/Roll_r'
   */
  rtb_Sum3 = rtb_Saturation5 - Roll_r;

  /* DiscreteIntegrator: '<S10>/Discrete-Time Integrator' */
  rtb_Saturation1_n = Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f;

  /* Sum: '<S10>/Sum3' incorporates:
   *  Gain: '<S10>/Gain'
   */
  rtb_Saturation1_n += Skydog_autopilot_P.Gain_Gain_m * rtb_Sum3;

  /* Saturate: '<S10>/Saturation1' */
  rtb_Saturation1_n = rtb_Saturation1_n >=
    Skydog_autopilot_P.Saturation1_UpperSat_c ?
    Skydog_autopilot_P.Saturation1_UpperSat_c : rtb_Saturation1_n <=
    Skydog_autopilot_P.Saturation1_LowerSat_m ?
    Skydog_autopilot_P.Saturation1_LowerSat_m : rtb_Saturation1_n;

  /* Sum: '<S3>/Sum2' incorporates:
   *  Inport: '<Root>/Roll_speed_r'
   */
  rtb_Sum2 = rtb_Saturation1_n - Roll_speed_r;

  /* Gain: '<S9>/Gain' */
  rtb_Saturation6 = Skydog_autopilot_P.Gain_Gain_e * rtb_Sum2;

  /* Saturate: '<S9>/Saturation1' */
  rtb_Saturation6 = rtb_Saturation6 >= Skydog_autopilot_P.Saturation1_UpperSat_b
    ? Skydog_autopilot_P.Saturation1_UpperSat_b : rtb_Saturation6 <=
    Skydog_autopilot_P.Saturation1_LowerSat_ij ?
    Skydog_autopilot_P.Saturation1_LowerSat_ij : rtb_Saturation6;

  /* Saturate: '<S3>/Saturation' */
  Aileron_w = rtb_Saturation6 >= Skydog_autopilot_P.Saturation_UpperSat ?
    Skydog_autopilot_P.Saturation_UpperSat : rtb_Saturation6 <=
    Skydog_autopilot_P.Saturation_LowerSat ?
    Skydog_autopilot_P.Saturation_LowerSat : rtb_Saturation6;

  /* Switch: '<S7>/Switch2' incorporates:
   *  Gain: '<S2>/Gain'
   *  Inport: '<Root>/Mode_w'
   *  Inport: '<Root>/RC_rudder_r'
   *  Inport: '<Root>/Roll_w'
   */
  if (Mode_w > Skydog_autopilot_P.Switch2_Threshold) {
    rtb_Saturation6 = Skydog_autopilot_P.Gain_Gain_k * Roll_w;
  } else {
    rtb_Saturation6 = RC_rudder_r;
  }

  /* End of Switch: '<S7>/Switch2' */

  /* Saturate: '<S3>/Saturation6' */
  rtb_Saturation6 = rtb_Saturation6 >= Skydog_autopilot_P.Saturation6_UpperSat ?
    Skydog_autopilot_P.Saturation6_UpperSat : rtb_Saturation6 <=
    Skydog_autopilot_P.Saturation6_LowerSat ?
    Skydog_autopilot_P.Saturation6_LowerSat : rtb_Saturation6;

  /* Sum: '<S3>/Sum1' incorporates:
   *  Inport: '<Root>/Yaw_speed_r'
   */
  rtb_Sum1_i = rtb_Saturation6 - Yaw_speed_r;

  /* Sum: '<S11>/Sum1' incorporates:
   *  DiscreteIntegrator: '<S11>/Discrete-Time Integrator'
   *  Gain: '<S11>/Gain'
   */
  rtb_Switch3 = Skydog_autopilot_P.Gain_Gain_ba * rtb_Sum1_i +
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr;

  /* Saturate: '<S11>/Saturation1' */
  rtb_Switch3 = rtb_Switch3 >= Skydog_autopilot_P.Saturation1_UpperSat_jh ?
    Skydog_autopilot_P.Saturation1_UpperSat_jh : rtb_Switch3 <=
    Skydog_autopilot_P.Saturation1_LowerSat_cj ?
    Skydog_autopilot_P.Saturation1_LowerSat_cj : rtb_Switch3;

  /* Saturate: '<S3>/Saturation1' */
  Rudder_w = rtb_Switch3 >= Skydog_autopilot_P.Saturation1_UpperSat_e ?
    Skydog_autopilot_P.Saturation1_UpperSat_e : rtb_Switch3 <=
    Skydog_autopilot_P.Saturation1_LowerSat_ga ?
    Skydog_autopilot_P.Saturation1_LowerSat_ga : rtb_Switch3;

  /* Sum: '<S2>/Sum1' incorporates:
   *  Inport: '<Root>/Groundspeed_r'
   *  Inport: '<Root>/Groundspeed_w'
   */
  rtb_Sum1 = Groundspeed_w - Groundspeed_r;

  /* Switch: '<S7>/Switch3' incorporates:
   *  Inport: '<Root>/Mode_w'
   *  Inport: '<Root>/RC_throttle_r'
   */
  if (Mode_w > Skydog_autopilot_P.Switch3_Threshold) {
    /* Sum: '<S5>/Sum3' incorporates:
     *  DiscreteIntegrator: '<S5>/Discrete-Time Integrator'
     *  Gain: '<S5>/Gain'
     */
    rtb_Switch3 = Skydog_autopilot_P.Gain_Gain * rtb_Sum1 +
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d;

    /* Saturate: '<S5>/Saturation1' */
    rtb_Switch3 = rtb_Switch3 >= Skydog_autopilot_P.Saturation1_UpperSat ?
      Skydog_autopilot_P.Saturation1_UpperSat : rtb_Switch3 <=
      Skydog_autopilot_P.Saturation1_LowerSat ?
      Skydog_autopilot_P.Saturation1_LowerSat : rtb_Switch3;
  } else {
    rtb_Switch3 = RC_throttle_r;
  }

  /* End of Switch: '<S7>/Switch3' */

  /* Saturate: '<S3>/Saturation3' */
  Throttle_w = rtb_Switch3 >= Skydog_autopilot_P.Saturation3_UpperSat ?
    Skydog_autopilot_P.Saturation3_UpperSat : rtb_Switch3 <=
    Skydog_autopilot_P.Saturation3_LowerSat ?
    Skydog_autopilot_P.Saturation3_LowerSat : rtb_Switch3;

  /* Gain: '<S1>/Gain' */
  debug1 = Skydog_autopilot_P.Gain_Gain_p1 * rtb_Saturation1_e;

  /* Gain: '<S1>/Gain1' */
  debug2 = Skydog_autopilot_P.Gain1_Gain * rtb_Saturation1_l;

  /* Gain: '<S4>/Gain1' */
  rtb_Switch3 = Skydog_autopilot_P.Gain1_Gain_n * rtb_Sum5;

  /* Gain: '<S5>/Gain1' */
  rtb_Saturation1_l = Skydog_autopilot_P.Gain1_Gain_j * rtb_Sum1;

  /* Update for DiscreteIntegrator: '<S4>/Discrete-Time Integrator' */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE =
    Skydog_autopilot_P.DiscreteTimeIntegrator_gainval * rtb_Switch3 +
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE >=
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperSat) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE =
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperSat;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE <=
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerSat) {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE =
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerSat;
    }
  }

  /* End of Update for DiscreteIntegrator: '<S4>/Discrete-Time Integrator' */

  /* Update for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S6>/Gain1'
   */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b =
    Skydog_autopilot_P.Gain1_Gain_k * rtb_Sum6 *
    Skydog_autopilot_P.DiscreteTimeIntegrator_gainva_d +
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b >=
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperS_o) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b =
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperS_o;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b <=
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerS_a) {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b =
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerS_a;
    }
  }

  /* End of Update for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' */

  /* Update for DiscreteIntegrator: '<S10>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S10>/Gain1'
   */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f =
    Skydog_autopilot_P.Gain1_Gain_e * rtb_Sum3 *
    Skydog_autopilot_P.DiscreteTimeIntegrator_gainva_g +
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f >=
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperS_k) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f =
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperS_k;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f <=
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerS_c) {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f =
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerS_c;
    }
  }

  /* End of Update for DiscreteIntegrator: '<S10>/Discrete-Time Integrator' */

  /* Update for DiscreteIntegrator: '<S11>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S11>/Gain1'
   */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr =
    Skydog_autopilot_P.Gain1_Gain_m * rtb_Sum1_i *
    Skydog_autopilot_P.DiscreteTimeIntegrator_gainva_f +
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr >=
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperS_a) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr =
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperS_a;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr <=
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerS_l) {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr =
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerS_l;
    }
  }

  /* End of Update for DiscreteIntegrator: '<S11>/Discrete-Time Integrator' */

  /* Update for DiscreteIntegrator: '<S5>/Discrete-Time Integrator' */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d =
    Skydog_autopilot_P.DiscreteTimeIntegrator_gainva_h * rtb_Saturation1_l +
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d;
  if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d >=
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperS_b) {
    Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d =
      Skydog_autopilot_P.DiscreteTimeIntegrator_UpperS_b;
  } else {
    if (Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d <=
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerS_m) {
      Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d =
        Skydog_autopilot_P.DiscreteTimeIntegrator_LowerS_m;
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

  /* InitializeConditions for DiscreteIntegrator: '<S4>/Discrete-Time Integrator' */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE =
    Skydog_autopilot_P.DiscreteTimeIntegrator_IC;

  /* InitializeConditions for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_b =
    Skydog_autopilot_P.DiscreteTimeIntegrator_IC_h;

  /* InitializeConditions for DiscreteIntegrator: '<S10>/Discrete-Time Integrator' */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_f =
    Skydog_autopilot_P.DiscreteTimeIntegrator_IC_i;

  /* InitializeConditions for DiscreteIntegrator: '<S11>/Discrete-Time Integrator' */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTAT_fr =
    Skydog_autopilot_P.DiscreteTimeIntegrator_IC_k;

  /* InitializeConditions for DiscreteIntegrator: '<S5>/Discrete-Time Integrator' */
  Skydog_autopilot_DWork.DiscreteTimeIntegrator_DSTATE_d =
    Skydog_autopilot_P.DiscreteTimeIntegrator_IC_m;
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
