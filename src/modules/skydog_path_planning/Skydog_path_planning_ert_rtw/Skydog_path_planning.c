/*
 * File: Skydog_path_planning.c
 *
 * Code generated for Simulink model 'Skydog_path_planning'.
 *
 * Model version                  : 1.281
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Wed Mar 26 23:42:29 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Skydog_path_planning.h"
#include "Skydog_path_planning_private.h"

/* Exported block parameters */
real_T L_want = 10.0;                  /* Variable: L_want
                                        * Referenced by: '<S2>/L_dist_UP'
                                        */
real_T R_want = 25.0;                  /* Variable: R_want
                                        * Referenced by: '<S2>/R_radius_circle_path'
                                        */
real_T Trash_want = 10.0;              /* Variable: Trash_want
                                        * Referenced by: '<S2>/T_trash_dist'
                                        */

/* Exported data definition */

/* Definition for custom storage class: ExportToFile */
real32_T Act_wps_index;
real32_T Altitude2_r;
real32_T Altitude2_w;
boolean_T Error;
real32_T Groundspeed2_r[3];
real32_T Home_position[4];
int16_T Mode2_w;
real32_T P[3];
real32_T Roll2_w;
real32_T Speed_w;
real32_T U[3];
real32_T Waypoints_w[60];
real32_T X_earth_r;
real32_T Y_earth_r;
real32_T d2;
real32_T eta;

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
  real_T rtb_eta;
  real_T rtb_L1a;
  real_T rtb_eta_VE;
  real_T rtb_eta_LE;
  real_T rtb_phi_out[2];
  real_T rtb_d2_dist;
  real_T wp_llh[60];
  real_T i;
  real_T j;
  real_T k;
  real_T d2_dist;
  int32_T classic;
  real_T alt_m;
  real_T dist;
  real_T phi_w;
  real_T Nc;
  real_T rtb_Sum;
  real_T rtb_x_ned;
  real_T rtb_y_ned;
  real_T rtb_DataTypeConversion9[60];
  real_T rtb_wp_ned[60];
  int32_T i_0;
  int32_T rtb_Sum_0;
  real_T dist_one_idx;
  real_T d2_idx;
  real_T d2_idx_0;
  real_T d2_idx_1;
  real_T d2_one_idx;
  real_T d2_one_idx_0;
  real_T phi_out_idx;
  real_T phi_out_idx_0;

  /* MATLAB Function: '<S2>/recalculate_GPS_meters_U' incorporates:
   *  DataTypeConversion: '<S2>/Data Type Conversion10'
   *  DataTypeConversion: '<S2>/Data Type Conversion6'
   *  DataTypeConversion: '<S2>/Data Type Conversion7'
   *  Inport: '<Root>/Home_position'
   *  Inport: '<Root>/X_earth_r'
   *  Inport: '<Root>/Y_earth_r'
   */
  /* MATLAB Function 'Skydog_path_planning/recalculate_GPS_meters_U': '<S9>:1' */
  /* init: */
  /* convert latitude and longitude from degrees to meters */
  /* Earth radius in N 0° */
  /* earth latitude */
  /* obvod zeme v miste pohybu (latitude N 49) */
  /* obvod prepocten na delku jednoho stupne GPS  */
  /* '<S9>:1:12' */
  rtb_x_ned = ((real_T)X_earth_r - (real_T)Home_position[1]) *
    73032.420396526082;

  /* '<S9>:1:13' */
  rtb_y_ned = ((real_T)Y_earth_r - (real_T)Home_position[2]) *
    73032.420396526082;

  /* MATLAB Function 'Skydog_path_planning/recalc_GPS_meters': '<S7>:1' */
  /* init: */
  /* '<S7>:1:6' */
  for (i_0 = 0; i_0 < 60; i_0++) {
    /* MATLAB Function: '<S2>/recalc_GPS_meters' incorporates:
     *  DataTypeConversion: '<S2>/Data Type Conversion9'
     *  Inport: '<Root>/Waypoints_w'
     */
    wp_llh[i_0] = Waypoints_w[i_0];
    rtb_wp_ned[i_0] = 0.0;

    /* DataTypeConversion: '<S2>/Data Type Conversion9' incorporates:
     *  Inport: '<Root>/Waypoints_w'
     */
    rtb_DataTypeConversion9[i_0] = Waypoints_w[i_0];
  }

  /* MATLAB Function: '<S2>/recalc_GPS_meters' incorporates:
   *  DataTypeConversion: '<S2>/Data Type Conversion10'
   *  Inport: '<Root>/Home_position'
   */
  /* '<S7>:1:8' */
  i = 0.0;

  /* '<S7>:1:9' */
  j = 0.0;

  /* index of checking waypoint */
  /* control active wps in list: */
  while (i == 0.0) {
    /* '<S7>:1:11' */
    /* '<S7>:1:12' */
    j++;
    if (j >= 15.0) {
      /* '<S7>:1:14' */
      /* '<S7>:1:15' */
      i = 1.0;
    }

    if (rtb_DataTypeConversion9[(int32_T)j - 1] == 0.0) {
      /* '<S7>:1:18' */
      if (rtb_DataTypeConversion9[(int32_T)j + 29] == 0.0) {
        /* '<S7>:1:19' */
        /* '<S7>:1:20' */
        i = 1.0;
      } else {
        /* '<S7>:1:22' */
        i = 0.0;
      }
    }
  }

  /* START input home position on first place in wp list: */
  /* '<S7>:1:28' */
  for (classic = 0; classic <= (int32_T)-((-1.0 - j) + 2.0) - 1; classic++) {
    /* '<S7>:1:28' */
    i = j + -(real_T)classic;

    /* '<S7>:1:29' */
    i_0 = (int32_T)i;
    rtb_Sum_0 = (int32_T)(i - 1.0);
    dist_one_idx = wp_llh[rtb_Sum_0 + 14];
    dist = wp_llh[rtb_Sum_0 + 29];
    d2_idx_0 = wp_llh[rtb_Sum_0 + 44];
    wp_llh[i_0 - 1] = wp_llh[rtb_Sum_0 - 1];
    wp_llh[i_0 + 14] = dist_one_idx;
    wp_llh[i_0 + 29] = dist;
    wp_llh[i_0 + 44] = d2_idx_0;

    /* '<S7>:1:28' */
  }

  /* '<S7>:1:32' */
  wp_llh[0] = Home_position[0];
  wp_llh[15] = Home_position[1];
  wp_llh[30] = Home_position[2];

  /* '<S7>:1:33' */
  wp_llh[45] = wp_llh[46];

  /* END_ input home pos on first place in wp list */
  /* START____convert latitude and longitude from degrees to meters */
  /* Earth radius in N 0° */
  /* earth latitude */
  /* obvod zeme v miste pohybu (latitude N 49) */
  /* obvod prepocten na delku jednoho stupne GPS */
  /*  deg2met =  7.3032e+004; %1degree GPS = 73032 m na 49°N (Brno) */
  /* recalculate from GPS to meters diference from home position */
  /* '<S7>:1:45' */
  for (classic = 0; classic <= (int32_T)j - 1; classic++) {
    /* '<S7>:1:45' */
    /* '<S7>:1:46' */
    rtb_wp_ned[(int32_T)(1.0 + (real_T)classic) - 1] = wp_llh[(int32_T)(1.0 +
      (real_T)classic) - 1];

    /* '<S7>:1:47' */
    rtb_wp_ned[(int32_T)(1.0 + (real_T)classic) + 14] = (wp_llh[(int32_T)(1.0 +
      (real_T)classic) + 14] - (real_T)Home_position[1]) * 73032.420396526082;

    /* x_earth of wp from home pos X */
    /* '<S7>:1:48' */
    rtb_wp_ned[(int32_T)(1.0 + (real_T)classic) + 29] = (wp_llh[(int32_T)(1.0 +
      (real_T)classic) + 29] - (real_T)Home_position[2]) * 73032.420396526082;

    /* y_earth of wp from home pos Y */
    /* '<S7>:1:49' */
    rtb_wp_ned[(int32_T)(1.0 + (real_T)classic) + 44] = wp_llh[(int32_T)(1.0 +
      (real_T)classic) + 44];

    /* '<S7>:1:45' */
  }

  /* Sum: '<S2>/Sum' incorporates:
   *  Constant: '<S2>/k_init'
   *  Memory: '<S2>/Memory1'
   */
  /* '<S7>:1:52' */
  /* number of active waypoints (others are zeros) */
  rtb_Sum = 1.0 + Skydog_path_planning_DWork.Memory1_PreviousInput;

  /* MATLAB Function: '<S2>/P_wps_nfz' incorporates:
   *  Constant: '<S2>/L_dist_UP'
   *  Constant: '<S2>/R_radius_circle_path'
   *  Constant: '<S2>/T_trash_dist'
   *  DataTypeConversion: '<S2>/Data Type Conversion10'
   *  DataTypeConversion: '<S2>/Data Type Conversion5'
   *  Inport: '<Root>/Altitude2_r'
   *  Inport: '<Root>/Error'
   *  Inport: '<Root>/Home_position'
   *  MATLAB Function: '<S2>/recalc_GPS_meters'
   *  SignalConversion: '<S3>/TmpSignal ConversionAt SFunction Inport1'
   */
  memcpy((void *)&wp_llh[0], (void *)&rtb_wp_ned[0], 60U * sizeof(real_T));

  /* MATLAB Function 'Skydog_path_planning/P_wps_nfz': '<S3>:1' */
  /* START_____init values */
  /* '<S3>:1:5' */
  /* number of active waypoints in wp strukt */
  /* '<S3>:1:7' */
  k = rtb_Sum - 1.0;

  /*  d1 = zeros(1,3); %init vector btw U and last wp */
  /* '<S3>:1:10' */
  /* init vector btw U and next wp */
  /* START_____recalc home position */
  /* '<S3>:1:13' */
  /* '<S3>:1:15' */
  /*  home_m(2) = 0; */
  /*  home_m(3) = 0; */
  /* '<S3>:1:18' */
  /* END_____ recalc home position */
  /* END_____init values */
  /* START_____control end trajektory and errors */
  /* '<S3>:1:23' */
  classic = 1;

  /* '<S3>:1:24' */
  i_0 = 0;
  if (Error == TRUE) {
    /* '<S3>:1:25' */
    /* '<S3>:1:26' */
    rtb_Sum_0 = (int32_T)(rtb_Sum + 1.0);
    wp_llh[rtb_Sum_0 - 1] = Home_position[0];
    wp_llh[rtb_Sum_0 + 14] = 0.0;
    wp_llh[rtb_Sum_0 + 29] = 0.0;
    wp_llh[rtb_Sum_0 + 44] = rtb_wp_ned[(int32_T)rtb_Sum + 44];

    /* '<S3>:1:27' */
    classic = 0;

    /* '<S3>:1:28' */
    i_0 = 1;

    /* pridat algoritmus krouzeni kolem home pos!! (stejny alg jako pro */
    /* reseni max_phi) */
  }

  /* pri doleteni na konec trajektory -> "go home" */
  if (rtb_Sum >= (j + 1.0) - 1.0) {
    /* '<S3>:1:34' */
    /* '<S3>:1:35' */
    rtb_Sum_0 = (int32_T)(rtb_Sum + 1.0);
    wp_llh[rtb_Sum_0 - 1] = Home_position[0];
    wp_llh[rtb_Sum_0 + 14] = 0.0;
    wp_llh[rtb_Sum_0 + 29] = 0.0;
    wp_llh[rtb_Sum_0 + 44] = rtb_wp_ned[(int32_T)rtb_Sum + 44];

    /* '<S3>:1:36' */
    classic = 0;

    /* '<S3>:1:37' */
    i_0 = 1;
  }

  /* END____ control end trajektory and errors */
  /* START________diference between wps: */
  /* '<S3>:1:43' */
  alt_m = wp_llh[(int32_T)(rtb_Sum + 1.0) - 1] - wp_llh[(int32_T)rtb_Sum - 1];

  /* compute altitude distance (meters) */
  /* '<S3>:1:44' */
  i = wp_llh[(int32_T)(rtb_Sum + 1.0) + 14] - wp_llh[(int32_T)rtb_Sum + 14];

  /* compute longitudal distance (meters) */
  /* '<S3>:1:45' */
  j = wp_llh[(int32_T)(rtb_Sum + 1.0) + 29] - wp_llh[(int32_T)rtb_Sum + 29];

  /* compute lateral distance (meters) */
  /* '<S3>:1:47' */
  dist = sqrt(rt_powd_snf(i, 2.0) + rt_powd_snf(j, 2.0));

  /* compute full distance (meters) */
  /* '<S3>:1:48' */
  dist_one_idx = i / dist;

  /* normalized dist vector(0-1) */
  /* START____vector (distance and angle) btw U and next wp: */
  /* '<S3>:1:51' */
  d2_idx = wp_llh[(int32_T)(rtb_Sum + 1.0) - 1] - (real_T)Altitude2_r;

  /* alt dist (m) */
  /* '<S3>:1:52' */
  d2_idx_0 = wp_llh[(int32_T)(rtb_Sum + 1.0) + 14] - rtb_x_ned;

  /* x dist(m) */
  /* '<S3>:1:53' */
  d2_idx_1 = wp_llh[(int32_T)(rtb_Sum + 1.0) + 29] - rtb_y_ned;

  /* y dist (m) */
  /* '<S3>:1:55' */
  d2_dist = sqrt(rt_powd_snf(d2_idx_0, 2.0) + rt_powd_snf(d2_idx_1, 2.0));

  /* '<S3>:1:56' */
  d2_one_idx = d2_idx_0 / d2_dist;
  d2_one_idx_0 = d2_idx_1 / d2_dist;

  /* difference btw. last wp and U [m] */
  /*  d1(1) = U(1) - wp(k,1); %alt dist (m) */
  /*  d1(2) = U(2) - wp(k,2);%x dist (m) */
  /*  d1(3) = U(3) - wp(k,3);%y dist (m)  */
  /*   */
  /*  d1_dist = sqrt(d1(2)^2+d1(3)^2);   */
  /* START_____control max pitch angle: */
  /* params: */
  /* radius of wanted circle (m) - in non classic planning */
  /* init values: */
  /* '<S3>:1:70' */
  phi_w = d2_idx / d2_dist;

  /* actual wanted pitch angle btw U and wp(k+1) */
  /* '<S3>:1:71' */
  /* max pitch angle */
  /* '<S3>:1:72' */
  Nc = 0.0;

  /* if phi_w greater then max pitch angle =>  */
  if (alt_m / dist > 0.3490658503988659) {
    /* '<S3>:1:74' */
    /* '<S3>:1:75' */
    classic = 0;
  }

  /*  if phi_w lower then phi_min (-phi_max) */
  if (alt_m / dist < -0.3490658503988659) {
    /* '<S3>:1:79' */
    /* '<S3>:1:80' */
    classic = 0;
  }

  while (fabs(phi_w) > 0.3490658503988659) {
    /* '<S3>:1:83' */
    /* '<S3>:1:84' */
    Nc++;

    /* '<S3>:1:85' */
    phi_w = d2_idx / (Nc * 2.0 * 3.1415926535897931 * R_want + d2_dist);

    /* '<S3>:1:86' */
    classic = 0;
  }

  /* END_________diference between wps */
  /* START______ P compute: */
  /* params: */
  /* wanted diference btw U and P (m) */
  /* tolerante distance btw U and wp (consist for switching) */
  /* init values: */
  /* center of circle path (init) */
  /* '<S3>:1:98' */
  /* vector in XY plane (from C to U) */
  /* '<S3>:1:101' */
  i = 0.0;

  /* init altitude distence btw U and wp(k+1) */
  /* '<S3>:1:104' */
  phi_out_idx = 0.0;
  phi_out_idx_0 = 0.0;
  if (classic == 1) {
    /* '<S3>:1:107' */
    /* P - classic */
    /* '<S3>:1:109' */
    d2_idx = wp_llh[(int32_T)(rtb_Sum + 1.0) - 1];

    /* '<S3>:1:110' */
    d2_idx_0 = d2_one_idx * L_want + rtb_x_ned;

    /* '<S3>:1:111' */
    dist_one_idx = d2_one_idx_0 * L_want + rtb_y_ned;
  } else {
    /* classic = 0 -> prodlouzit trasu spiralou (reseni max pitch angle a krouzeni kolem home pos) */
    /* '<S3>:1:115' */
    /* '<S3>:1:116' */
    alt_m = asin(L_want / (2.0 * R_want)) * 2.0;

    /* angle around C from U to P (phi_CUP) */
    /* '<S3>:1:118' */
    rtb_Sum_0 = (int32_T)(rtb_Sum + 1.0);
    d2_idx = wp_llh[rtb_Sum_0 - 1] + R_want * 0.0;
    d2_idx_0 = -(j / dist) * R_want + wp_llh[rtb_Sum_0 + 14];
    d2_idx_1 = wp_llh[rtb_Sum_0 + 29] + R_want * dist_one_idx;

    /* '<S3>:1:120' */
    j = sqrt(rt_powd_snf(d2_idx_0 - rtb_x_ned, 2.0) + rt_powd_snf(d2_idx_1 -
              rtb_y_ned, 2.0));

    /* '<S3>:1:122' */
    i = rtb_x_ned - d2_idx_0;
    if (i != 0.0) {
      i /= j;
    }

    dist = i;

    /* in xE dirr */
    /* '<S3>:1:123' */
    i = rtb_y_ned - d2_idx_1;
    if (i != 0.0) {
      i /= j;
    }

    /* in yE dirr */
    /* uhel mezi CU a CW = (dist_one(2) , -dist_one(1)): */
    /* angle - CU,xE: */
    /* '<S3>:1:127' */
    j = atan(i / dist);

    /* angle - CW,xE: */
    /*          phi_CWE = atan(-dist_one(1)/dist_one(2)); */
    /* correction: */
    /* phi_CUE: */
    if ((i < 0.0) && (dist < 0.0)) {
      /* '<S3>:1:133' */
      /* '<S3>:1:134' */
      j += -3.1415926535897931;
    } else {
      if ((i > 0.0) && (dist < 0.0)) {
        /* '<S3>:1:135' */
        /* '<S3>:1:136' */
        j += 3.1415926535897931;
      }
    }

    /* '<S3>:1:140' */
    phi_out_idx = j + alt_m;

    /* switch to circle planning and from circle planning: */
    /* '<S3>:1:143' */
    i = fabs(wp_llh[(int32_T)(rtb_Sum + 1.0) - 1] - (real_T)Altitude2_r);

    /* '<S3>:1:144' */
    /* Nc - number of loops (from condition phi_w) */
    /* '<S3>:1:146' */
    phi_out_idx_0 = i;
    if (i_0 == 0) {
      /* '<S3>:1:150' */
      if (i < (Nc + 0.5) * 2.0 * 3.1415926535897931 * R_want * fabs(phi_w)) {
        /* '<S3>:1:151' */
        if (d2_dist < 2.0 * R_want + Trash_want) {
          /* '<S3>:1:152' */
          /* circle planning with altitude increasing */
          /* '<S3>:1:154' */
          /* '<S3>:1:155' */
          d2_idx_0 += cos(j + alt_m) * R_want;

          /* '<S3>:1:156' */
          dist_one_idx = sin(j + alt_m) * R_want + d2_idx_1;
        } else {
          /* '<S3>:1:159' */
          /* '<S3>:1:160' */
          d2_idx_0 = d2_one_idx * L_want + rtb_x_ned;

          /* '<S3>:1:161' */
          dist_one_idx = d2_one_idx_0 * L_want + rtb_y_ned;
        }
      } else {
        /* line planning with altitude increasing    */
        /* '<S3>:1:167' */
        /*  + phi_w * (d1_dist+L); */
        /* '<S3>:1:168' */
        d2_idx_0 = d2_one_idx * L_want + rtb_x_ned;

        /* '<S3>:1:169' */
        dist_one_idx = d2_one_idx_0 * L_want + rtb_y_ned;
      }
    } else {
      /* go_home = 1 */
      if (d2_dist < 3.0 * R_want) {
        /* '<S3>:1:173' */
        /* '<S3>:1:174' */
        /* '<S3>:1:175' */
        d2_idx_0 += cos(j + alt_m) * R_want;

        /* '<S3>:1:176' */
        dist_one_idx = sin(j + alt_m) * R_want + d2_idx_1;
      } else {
        /* '<S3>:1:178' */
        /* '<S3>:1:179' */
        d2_idx_0 = d2_one_idx * L_want + rtb_x_ned;

        /* '<S3>:1:180' */
        dist_one_idx = d2_one_idx_0 * L_want + rtb_y_ned;
      }
    }
  }

  /* START_______compute switching consistance: */
  /* switch z polohy wp(k+1) a U - dle podminek v "big_picture_autopilot" !!! */
  if (classic == 1) {
    /* '<S3>:1:187' */
    if (d2_dist < Trash_want) {
      /* '<S3>:1:188' */
      /* '<S3>:1:189' */
      k = rtb_Sum;
    }
  } else {
    if ((i < 5.0) && (d2_dist < Trash_want / 2.0) && (i_0 == 0)) {
      /* '<S3>:1:192' */
      /* '<S3>:1:193' */
      /* '<S3>:1:194' */
      /* '<S3>:1:195' */
      k = rtb_Sum;
    }
  }

  /* '<S3>:1:201' */
  /* do logovaci appky   */
  rtb_phi_out[0] = phi_out_idx;
  rtb_phi_out[1] = phi_out_idx_0;
  rtb_d2_dist = d2_dist;

  /* MATLAB Function: '<S2>/lambda_eta' incorporates:
   *  DataTypeConversion: '<S2>/Data Type Conversion8'
   *  Inport: '<Root>/Groundspeed2_r'
   *  MATLAB Function: '<S2>/P_wps_nfz'
   *  SignalConversion: '<S6>/TmpSignal ConversionAt SFunction Inport1'
   */
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
  i = d2_idx_0 - rtb_x_ned;

  /* '<S6>:1:23' */
  j = dist_one_idx - rtb_y_ned;

  /* '<S6>:1:24' */
  /* uhel lambda - uhel mezi zadana trajektorie a odchylka uav od ni: */
  /*  lambda_cos = (d1^2+wd^2-d2^2)/(2*d1*wd); */
  /* uhel eta - uhel mezi L1a a vektorem rychlosti Gsp: */
  /*  Gsp_di = sqrt(Gsp(1)^2+Gsp(2)^2); */
  /*  cos_eta = ((L1E*Gsp(1)+L1N*Gsp(2))/(L1a*Gsp_di)); */
  /* % vypocet uhlu eta vzhledem k ose x-lat "East direction": */
  /* '<S6>:1:34' */
  alt_m = atan((real_T)Groundspeed2_r[1] / (real_T)Groundspeed2_r[0]);

  /* '<S6>:1:35' */
  dist = atan(j / i);

  /* osetreni max a min hodnot:  */
  if ((j < 0.0) && (i < 0.0)) {
    /* '<S6>:1:39' */
    /* '<S6>:1:40' */
    dist += -3.1415926535897931;
  } else {
    if ((j > 0.0) && (i < 0.0)) {
      /* '<S6>:1:41' */
      /* '<S6>:1:42' */
      dist += 3.1415926535897931;
    }
  }

  if ((Groundspeed2_r[0] < 0.0F) && (Groundspeed2_r[1] < 0.0F)) {
    /* '<S6>:1:45' */
    /* '<S6>:1:46' */
    alt_m += -3.1415926535897931;
  } else {
    if ((Groundspeed2_r[0] < 0.0F) && (Groundspeed2_r[1] > 0.0F)) {
      /* '<S6>:1:47' */
      /* '<S6>:1:48' */
      alt_m += 3.1415926535897931;
    }
  }

  /* vysledny uhel eta: */
  /* '<S6>:1:52' */
  rtb_eta = dist - alt_m;
  rtb_L1a = sqrt(rt_powd_snf(i, 2.0) + rt_powd_snf(j, 2.0));
  rtb_eta_VE = alt_m;
  rtb_eta_LE = dist;

  /* End of MATLAB Function: '<S2>/lambda_eta' */

  /* MATLAB Function: '<S2>/acc_lat_roll_angle' incorporates:
   *  DataTypeConversion: '<S2>/Data Type Conversion8'
   *  Inport: '<Root>/Groundspeed2_r'
   */
  /* MATLAB Function 'Skydog_path_planning/acc_lat_roll_angle': '<S4>:1' */
  /* NE speed vect: */
  /* '<S4>:1:5' */
  /*  laterar acceleration: - zvolena delka L1 */
  /* '<S4>:1:8' */
  /* '<S4>:1:10' */
  i = (rt_powd_snf((real_T)Groundspeed2_r[0], 2.0) + rt_powd_snf((real_T)
        Groundspeed2_r[1], 2.0)) * 2.0 * sin(rtb_eta) / 10.0 / 9.81;

  /* '<S4>:1:12' */
  /* rad - maximum roll angle */
  if (i >= 0.69813170079773179) {
    /* '<S4>:1:13' */
    /* '<S4>:1:14' */
    i = 0.69813170079773179;
  } else if (i <= -0.69813170079773179) {
    /* '<S4>:1:15' */
    /* '<S4>:1:16' */
    i = -0.69813170079773179;
  } else {
    /* '<S4>:1:18' */
  }

  /* End of MATLAB Function: '<S2>/acc_lat_roll_angle' */

  /* DataTypeConversion: '<S2>/Data Type Conversion4' */
  Roll2_w = (real32_T)i;

  /* MATLAB Function: '<S2>/altitude_wanted' incorporates:
   *  MATLAB Function: '<S2>/P_wps_nfz'
   */
  /* MATLAB Function 'Skydog_path_planning/altitude_wanted': '<S5>:1' */
  /* '<S5>:1:4' */
  i = d2_idx;
  if (d2_idx >= 2000.0) {
    /* '<S5>:1:6' */
    /* '<S5>:1:7' */
    i = 2000.0;
  }

  /* End of MATLAB Function: '<S2>/altitude_wanted' */

  /* DataTypeConversion: '<S2>/Data Type Conversion1' */
  Altitude2_w = (real32_T)i;

  /* DataTypeConversion: '<S2>/Data Type Conversion2' */
  /* MATLAB Function 'Skydog_path_planning/speed_wanted': '<S10>:1' */
  /*  speed_w = wp_act(1,4); */
  /* '<S10>:1:5' */
  Speed_w = 11.0F;

  /* DataTypeConversion: '<S2>/Data Type Conversion3' incorporates:
   *  MATLAB Function: '<S2>/P_wps_nfz'
   */
  P[0] = (real32_T)d2_idx;
  P[1] = (real32_T)d2_idx_0;
  P[2] = (real32_T)dist_one_idx;

  /* DataTypeConversion: '<S2>/Data Type Conversion15' */
  Act_wps_index = (real32_T)rtb_Sum;

  /* DataTypeConversion: '<S2>/Data Type Conversion14' incorporates:
   *  DataTypeConversion: '<S2>/Data Type Conversion5'
   *  Inport: '<Root>/Altitude2_r'
   */
  U[0] = Altitude2_r;
  U[1] = (real32_T)rtb_x_ned;
  U[2] = (real32_T)rtb_y_ned;

  /* DataTypeConversion: '<S2>/Data Type Conversion11' */
  d2 = (real32_T)rtb_d2_dist;

  /* DataTypeConversion: '<S2>/Data Type Conversion12' */
  eta = (real32_T)rtb_eta;

  /* MATLAB Function: '<S2>/recalc_GPS_meters1' incorporates:
   *  Inport: '<Root>/Mode2_w'
   */
  /* MATLAB Function 'Skydog_path_planning/recalc_GPS_meters1': '<S8>:1' */
  if (Mode2_w == 2) {
    /* Update for Memory: '<S2>/Memory1' incorporates:
     *  MATLAB Function: '<S2>/P_wps_nfz'
     */
    /* '<S8>:1:4' */
    /* '<S8>:1:5' */
    Skydog_path_planning_DWork.Memory1_PreviousInput = k;
  } else {
    /* Update for Memory: '<S2>/Memory1' */
    /* '<S8>:1:7' */
    Skydog_path_planning_DWork.Memory1_PreviousInput = 0.0;
  }

  /* End of MATLAB Function: '<S2>/recalc_GPS_meters1' */
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
  Act_wps_index = 0.0F;
  U[0] = 0.0F;
  U[1] = 0.0F;
  U[2] = 0.0F;
  d2 = 0.0F;
  eta = 0.0F;

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

  Home_position[0] = 0.0F;
  Home_position[1] = 0.0F;
  Home_position[2] = 0.0F;
  Home_position[3] = 0.0F;
  Error = FALSE;
  Mode2_w = 0;
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
