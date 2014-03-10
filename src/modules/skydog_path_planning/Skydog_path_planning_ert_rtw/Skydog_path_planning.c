/*
 * File: Skydog_path_planning.c
 *
 * Code generated for Simulink model 'Skydog_path_planning'.
 *
 * Model version                  : 1.214
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Mon Mar 10 12:21:42 2014
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
real32_T Altitude2_r;
real32_T Altitude2_w;
boolean_T Error;
real32_T Groundspeed2_r[3];
int16_T Mode2_w;
real32_T Nfz_w[24];
real32_T P[3];
real32_T Roll2_w;
real32_T Speed_w;
real32_T Time;
uint8_T Waypoints_count;
real32_T Waypoints_w[60];
real32_T X_earth_r;
real32_T Y_earth_r;

/* Block states (auto storage) */
D_Work_Skydog_path_planning Skydog_path_planning_DWork;

/* Real-time model */
RT_MODEL_Skydog_path_planning Skydog_path_planning_M_;
RT_MODEL_Skydog_path_planning *const Skydog_path_planning_M =
  &Skydog_path_planning_M_;
real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T tmp;
  real_T tmp_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = (rtNaN);
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/* Model step function */
void Skydog_path_planning_step(void)
{
  /* local block i/o variables */
  real_T rtb_DataTypeConversion9[60];
  real_T rtb_eta;
  real_T rtb_L1a;
  real_T rtb_acc_lat;
  real_T rtb_t01;
  uint16_T rtb_Gain6;
  real_T xE;
  real_T yE;
  real_T dist;
  real_T rtb_Sum;
  real_T rtb_Sum1;
  real_T rtb_k1;
  int32_T i;
  real_T rtb_P_idx;
  real_T dp_idx;
  real_T dp_idx_0;

  /* Sum: '<S2>/Sum' incorporates:
   *  Constant: '<S2>/k_init'
   *  Memory: '<S2>/Memory1'
   */
  rtb_Sum = Skydog_path_planning_P.k_init_Value +
    Skydog_path_planning_DWork.Memory1_PreviousInput;

  /* Sum: '<S2>/Sum1' incorporates:
   *  Constant: '<S2>/t0_init'
   *  Memory: '<S2>/Memory'
   */
  rtb_Sum1 = Skydog_path_planning_P.t0_init_Value +
    Skydog_path_planning_DWork.Memory_PreviousInput;

  /* MATLAB Function: '<S2>/P_wps_nfz' incorporates:
   *  Constant: '<S2>/Constant'
   *  DataTypeConversion: '<S2>/Data Type Conversion13'
   *  Inport: '<Root>/Time'
   */
  /* MATLAB Function 'Skydog_path_planning/P_wps_nfz': '<S3>:1' */
  /* START_____init values */
  /* '<S3>:1:5' */
  /* '<S3>:1:6' */
  /* END_____init values */
  /* START________diference between wps: */
  /* '<S3>:1:14' */
  /* compute altitude distance */
  /* '<S3>:1:15' */
  xE = Skydog_path_planning_P.Constant_Value[(int32_T)(rtb_Sum + 1.0) + 14] -
    Skydog_path_planning_P.Constant_Value[(int32_T)rtb_Sum + 14];

  /* compute longitudal distance */
  /* '<S3>:1:16' */
  yE = Skydog_path_planning_P.Constant_Value[(int32_T)(rtb_Sum + 1.0) + 29] -
    Skydog_path_planning_P.Constant_Value[(int32_T)rtb_Sum + 29];

  /* compute lateral distance */
  /* '<S3>:1:18' */
  dist = sqrt(rt_powd_snf(xE, 2.0) + rt_powd_snf(yE, 2.0));

  /* compute full distance (in NE plane) */
  /* path direction : */
  /* '<S3>:1:21' */
  /* '<S3>:1:22' */
  /* END_________diference between wps */
  /* difference btw. last wp and P (based on time counter) [m] */
  /* '<S3>:1:26' */
  /* '<S3>:1:27' */
  dp_idx = ((real_T)Time - rtb_Sum1) * Skydog_path_planning_P.Constant_Value
    [(int32_T)rtb_Sum + 44] * (xE / dist);

  /* actual difference btw. P and wp(k) in longitude direction */
  /* '<S3>:1:28' */
  dp_idx_0 = ((real_T)Time - rtb_Sum1) * Skydog_path_planning_P.Constant_Value
    [(int32_T)rtb_Sum + 44] * (yE / dist);

  /* actual difference btw. P and wp(k) in latitude direction */
  /* '<S3>:1:29' */
  xE = sqrt(rt_powd_snf(dp_idx, 2.0) + rt_powd_snf(dp_idx_0, 2.0));

  /* actual difference btw. P and wp(k) - real values */
  /* clasic planning */
  /* '<S3>:1:32' */
  rtb_P_idx = Skydog_path_planning_P.Constant_Value[(int32_T)rtb_Sum + 14] +
    dp_idx;

  /* '<S3>:1:33' */
  dp_idx = Skydog_path_planning_P.Constant_Value[(int32_T)rtb_Sum + 29] +
    dp_idx_0;

  /* distance btw last pased waypoint and P-track point: */
  /*      distP = sqrt((P(3)-wp(k,3))^2+(P(2)-wp(k,2))^2); */
  /* distance btw nex waypoint and P-track point: */
  /* '<S3>:1:39' */
  /* P-altitude: */
  /*      delta_alt = alt*distP/dist; */
  /* '<S3>:1:43' */
  /* '<S3>:1:45' */
  dp_idx_0 = (Skydog_path_planning_P.Constant_Value[(int32_T)(rtb_Sum + 1.0) - 1]
              - Skydog_path_planning_P.Constant_Value[(int32_T)rtb_Sum - 1]) *
    xE / dist + Skydog_path_planning_P.Constant_Value[(int32_T)rtb_Sum - 1];

  /* bod "P" do logovaci appky */
  /* compute switching consistance: */
  /* pripsat podminku switche na kruhovou trajektorii kolem wp (neco ve smyslu */
  /* toho co už jsem tvoøil !!! (pri urcite vzdalenosti od bodu zacne tocit + */
  /* upravit "switch waypoints condition 01" */
  /*  switch waypoints condition 01: */
  if (dist - xE <= 0.0) {
    /* '<S3>:1:55' */
    /* distance between next wp and P */
    if (rtb_Sum < 14.0) {
      /* '<S3>:1:56' */
      /* '<S3>:1:57' */
      rtb_k1 = rtb_Sum;

      /* index of next wp */
      /* '<S3>:1:58' */
      rtb_t01 = Time;

      /* lock time when wp was changed */
    } else {
      /* '<S3>:1:60' */
      rtb_k1 = rtb_Sum - 1.0;

      /* '<S3>:1:61' */
      rtb_t01 = rtb_Sum1;
    }
  } else {
    /* '<S3>:1:64' */
    rtb_k1 = rtb_Sum - 1.0;

    /* '<S3>:1:65' */
    rtb_t01 = rtb_Sum1;
  }

  /* MATLAB Function: '<S2>/lambda_eta' incorporates:
   *  DataTypeConversion: '<S2>/Data Type Conversion6'
   *  DataTypeConversion: '<S2>/Data Type Conversion7'
   *  DataTypeConversion: '<S2>/Data Type Conversion8'
   *  Inport: '<Root>/Groundspeed_r'
   *  Inport: '<Root>/X_earth_r'
   *  Inport: '<Root>/Y_earth_r'
   */
  /* '<S3>:1:68' */
  /* do logovaci appky */
  /* MATLAB Function 'Skydog_path_planning/lambda_eta': '<S6>:1' */
  /*  wp1 = wp(1,1:4); */
  /*  wp2 = wp(2,1:4); */
  /* vektor wp1,wp2: */
  /*  wp_12N = wp2(3)-wp1(3); */
  /*  wp_12E = wp2(2)-wp1(2); */
  /*  wd = sqrt(wp_12N^2+wp_12E^2); */
  /*  vektor wp1,U: */
  /*  wp_1uE = U(2) - wp1(2); */
  /*  wp_1uN = U(3) - wp1(3); */
  /*  d1 = sqrt(wp_1uE^2+wp_1uN^2); */
  /* vektor U,wp2: */
  /*  wp_u2E = wp2(2)-U(2); */
  /*  wp_u2N = wp2(3)-U(3) ; */
  /*  d2 = sqrt(wp_u2E^2+wp_u2N^2); */
  /* vektor U,P: */
  /* '<S6>:1:22' */
  xE = rtb_P_idx - (real_T)X_earth_r;

  /* '<S6>:1:23' */
  yE = dp_idx - (real_T)Y_earth_r;

  /* '<S6>:1:24' */
  /* uhel lambda - uhel mezi zadana trajektorie a odchylka uav od ni: */
  /*  lambda_cos = (d1^2+wd^2-d2^2)/(2*d1*wd); */
  /* uhel eta - uhel mezi L1a a vektorem rychlosti Gsp: */
  /*  Gsp_di = sqrt(Gsp(1)^2+Gsp(2)^2); */
  /*  cos_eta = ((L1E*Gsp(1)+L1N*Gsp(2))/(L1a*Gsp_di)); */
  /* % vypocet uhlu eta vzhledem k ose x-lat "East direction": */
  /* '<S6>:1:34' */
  dist = atan((real_T)Groundspeed2_r[1] / (real_T)Groundspeed2_r[0]);

  /* '<S6>:1:35' */
  rtb_Sum1 = atan(yE / xE);

  /* osetreni max a min hodnot:  */
  if ((yE < 0.0) && (xE < 0.0)) {
    /* '<S6>:1:39' */
    /* '<S6>:1:40' */
    rtb_Sum1 += -3.1415926535897931;
  } else {
    if ((yE > 0.0) && (xE < 0.0)) {
      /* '<S6>:1:41' */
      /* '<S6>:1:42' */
      rtb_Sum1 += 3.1415926535897931;
    }
  }

  if ((Groundspeed2_r[0] < 0.0F) && (Groundspeed2_r[1] < 0.0F)) {
    /* '<S6>:1:45' */
    /* '<S6>:1:46' */
    dist += -3.1415926535897931;
  } else {
    if ((Groundspeed2_r[0] < 0.0F) && (Groundspeed2_r[1] > 0.0F)) {
      /* '<S6>:1:47' */
      /* '<S6>:1:48' */
      dist += 3.1415926535897931;
    }
  }

  /* vysledny uhel eta: */
  /* '<S6>:1:52' */
  rtb_eta = rtb_Sum1 - dist;

  /* % switch waypoints condition 02: */
  rtb_L1a = sqrt(rt_powd_snf(xE, 2.0) + rt_powd_snf(yE, 2.0));

  /* End of MATLAB Function: '<S2>/lambda_eta' */

  /* MATLAB Function: '<S2>/acc_lat_roll_angle' incorporates:
   *  DataTypeConversion: '<S2>/Data Type Conversion8'
   *  Inport: '<Root>/Groundspeed_r'
   */
  /* MATLAB Function 'Skydog_path_planning/acc_lat_roll_angle': '<S4>:1' */
  /* NE speed vect: */
  /* '<S4>:1:5' */
  /*  laterar acceleration: - zvolena delka L1 */
  /* '<S4>:1:8' */
  xE = (rt_powd_snf((real_T)Groundspeed2_r[0], 2.0) + rt_powd_snf((real_T)
         Groundspeed2_r[1], 2.0)) * 2.0 * sin(rtb_eta) / 35.0;

  /* '<S4>:1:10' */
  yE = xE / 9.81;

  /* '<S4>:1:12' */
  /* rad - maximum roll angle */
  if (yE >= 0.69813170079773179) {
    /* '<S4>:1:13' */
    /* '<S4>:1:14' */
    yE = 0.69813170079773179;
  } else if (yE <= -0.69813170079773179) {
    /* '<S4>:1:15' */
    /* '<S4>:1:16' */
    yE = -0.69813170079773179;
  } else {
    /* '<S4>:1:18' */
  }

  rtb_acc_lat = xE;

  /* End of MATLAB Function: '<S2>/acc_lat_roll_angle' */

  /* DataTypeConversion: '<S2>/Data Type Conversion4' */
  Roll2_w = (real32_T)yE;

  /* MATLAB Function: '<S2>/altitude_wanted' */
  /* MATLAB Function 'Skydog_path_planning/altitude_wanted': '<S5>:1' */
  /* '<S5>:1:4' */
  xE = dp_idx_0;
  if (dp_idx_0 >= 2000.0) {
    /* '<S5>:1:6' */
    /* '<S5>:1:7' */
    xE = 2000.0;
  }

  /* End of MATLAB Function: '<S2>/altitude_wanted' */

  /* DataTypeConversion: '<S2>/Data Type Conversion1' */
  /* alt_w = 50; */
  Altitude2_w = (real32_T)xE;

  /* DataTypeConversion: '<S2>/Data Type Conversion2' incorporates:
   *  Constant: '<S2>/Constant'
   *  MATLAB Function: '<S2>/P_wps_nfz'
   *  MATLAB Function: '<S2>/speed_wanted'
   */
  /* MATLAB Function 'Skydog_path_planning/speed_wanted': '<S7>:1' */
  /* '<S7>:1:4' */
  /* speed_w = 11; */
  Speed_w = (real32_T)Skydog_path_planning_P.Constant_Value[(int32_T)rtb_Sum +
    44];

  /* DataTypeConversion: '<S2>/Data Type Conversion3' */
  P[0] = (real32_T)dp_idx_0;
  P[1] = (real32_T)rtb_P_idx;
  P[2] = (real32_T)dp_idx;

  /* DataTypeConversion: '<S2>/Data Type Conversion9' incorporates:
   *  Inport: '<Root>/Waypoints_w'
   */
  for (i = 0; i < 60; i++) {
    rtb_DataTypeConversion9[i] = Waypoints_w[i];
  }

  /* End of DataTypeConversion: '<S2>/Data Type Conversion9' */

  /* Gain: '<S2>/Gain6' incorporates:
   *  Inport: '<Root>/Waypoints_count'
   */
  rtb_Gain6 = (uint16_T)((uint32_T)Skydog_path_planning_P.Gain6_Gain * (uint32_T)
    Waypoints_count);

  /* Update for Memory: '<S2>/Memory1' */
  Skydog_path_planning_DWork.Memory1_PreviousInput = rtb_k1;

  /* Update for Memory: '<S2>/Memory' */
  Skydog_path_planning_DWork.Memory_PreviousInput = rtb_t01;
}

/* Model initialize function */
void Skydog_path_planning_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize error status */
  rtmSetErrorStatus(Skydog_path_planning_M, (NULL));

  /* block I/O */

  /* custom signals */
  Roll2_w = 0.0F;
  Altitude2_w = 0.0F;
  Speed_w = 0.0F;
  P[0] = 0.0F;
  P[1] = 0.0F;
  P[2] = 0.0F;

  /* states (dwork) */
  (void) memset((void *)&Skydog_path_planning_DWork, 0,
                sizeof(D_Work_Skydog_path_planning));

  /* external inputs */
  Altitude2_r = 0.0F;
  X_earth_r = 0.0F;
  Y_earth_r = 0.0F;
  Groundspeed2_r[0] = 0.0F;
  Groundspeed2_r[1] = 0.0F;
  Groundspeed2_r[2] = 0.0F;

  {
    int_T i;
    for (i = 0; i < 60; i++) {
      Waypoints_w[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 24; i++) {
      Nfz_w[i] = 0.0F;
    }
  }

  Mode2_w = 0;
  Time = 0.0F;
  Waypoints_count = 0U;
  Error = FALSE;

  /* InitializeConditions for Memory: '<S2>/Memory1' */
  Skydog_path_planning_DWork.Memory1_PreviousInput =
    Skydog_path_planning_P.Memory1_X0;

  /* InitializeConditions for Memory: '<S2>/Memory' */
  Skydog_path_planning_DWork.Memory_PreviousInput =
    Skydog_path_planning_P.Memory_X0;
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
