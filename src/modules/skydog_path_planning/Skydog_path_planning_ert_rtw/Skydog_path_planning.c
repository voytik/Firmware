/*
 * File: Skydog_path_planning.c
 *
 * Code generated for Simulink model 'Skydog_path_planning'.
 *
 * Model version                  : 1.257
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Mon Mar 24 21:36:42 2014
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
                                        * Referenced by: '<S3>/Constant7'
                                        */
real_T R_want = 25.0;                  /* Variable: R_want
                                        * Referenced by: '<S3>/Constant9'
                                        */
real_T Trash_want = 10.0;              /* Variable: Trash_want
                                        * Referenced by: '<S3>/Constant10'
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
real32_T wp_actual[8];

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
  real_T rtb_phi_out[2];
  real_T rtb_d2_dist;
  real_T j;
  real_T k;
  real_T d2_dist;
  int32_T go_home;
  real_T yE_m;
  real_T dist;
  real_T phi_w;
  real_T Nc;
  real_T gama;
  real_T rtb_Sum;
  real_T rtb_x_ned;
  real_T rtb_y_ned;
  real_T rtb_DataTypeConversion9[60];
  real_T rtb_wp_ned[60];
  int32_T i;
  int32_T rtb_Sum_0;
  real_T d2_idx;
  real_T d2_idx_0;
  real_T d2_idx_1;
  real_T d2_one_idx;
  real_T d2_one_idx_0;
  real_T CU_idx;
  real_T CU_idx_0;
  real_T phi_out_idx;
  real_T phi_out_idx_0;

  /* MATLAB Function: '<S3>/recalculate_GPS_meters_U' incorporates:
   *  DataTypeConversion: '<S3>/Data Type Conversion10'
   *  DataTypeConversion: '<S3>/Data Type Conversion6'
   *  DataTypeConversion: '<S3>/Data Type Conversion7'
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

  /* MATLAB Function 'Skydog_path_planning/recalc_GPS_meters': '<S8>:1' */
  /* init: */
  /* '<S8>:1:6' */
  for (i = 0; i < 60; i++) {
    /* DataTypeConversion: '<S3>/Data Type Conversion9' incorporates:
     *  Inport: '<Root>/Waypoints_w'
     */
    rtb_DataTypeConversion9[i] = Waypoints_w[i];

    /* MATLAB Function: '<S3>/recalc_GPS_meters' */
    rtb_wp_ned[i] = 0.0;
  }

  /* MATLAB Function: '<S3>/recalc_GPS_meters' incorporates:
   *  DataTypeConversion: '<S3>/Data Type Conversion10'
   *  Inport: '<Root>/Home_position'
   */
  /* '<S8>:1:8' */
  i = 0;

  /* '<S8>:1:9' */
  j = 0.0;

  /* index of checking waypoint */
  /* control active wps in list: */
  while (i == 0) {
    /* '<S8>:1:11' */
    /* '<S8>:1:12' */
    j++;
    if (j >= 15.0) {
      /* '<S8>:1:14' */
      /* '<S8>:1:15' */
      i = 1;
    }

    if (rtb_DataTypeConversion9[(int32_T)j - 1] == 0.0) {
      /* '<S8>:1:18' */
      if (rtb_DataTypeConversion9[(int32_T)j + 29] == 0.0) {
        /* '<S8>:1:19' */
        /* '<S8>:1:20' */
        i = 1;
      } else {
        /* '<S8>:1:22' */
        i = 0;
      }
    }
  }

  /* convert latitude and longitude from degrees to meters */
  /* Earth radius in N 0° */
  /* wp_llh(1,1); %earth latitude of operating area */
  /* obvod zeme v miste pohybu (latitude N 49) */
  /* obvod prepocten na delku jednoho stupne GPS */
  /*  deg2met =  7.3032e+004; % 1 degree GPS = 73032 m na 49°N (Brno+-) */
  /* '<S8>:1:35' */
  for (i = 0; i <= (int32_T)(j - 1.0) - 1; i++) {
    /* '<S8>:1:35' */
    /* '<S8>:1:36' */
    rtb_wp_ned[(int32_T)(1.0 + (real_T)i) - 1] = rtb_DataTypeConversion9
      [(int32_T)(1.0 + (real_T)i) - 1];

    /* '<S8>:1:37' */
    rtb_wp_ned[(int32_T)(1.0 + (real_T)i) + 14] = (rtb_DataTypeConversion9
      [(int32_T)(1.0 + (real_T)i) + 14] - (real_T)Home_position[1]) *
      73032.420396526082;

    /* '<S8>:1:38' */
    rtb_wp_ned[(int32_T)(1.0 + (real_T)i) + 29] = (rtb_DataTypeConversion9
      [(int32_T)(1.0 + (real_T)i) + 29] - (real_T)Home_position[2]) *
      73032.420396526082;

    /* '<S8>:1:39' */
    rtb_wp_ned[(int32_T)(1.0 + (real_T)i) + 44] = rtb_DataTypeConversion9
      [(int32_T)(1.0 + (real_T)i) + 44];

    /* '<S8>:1:35' */
  }

  /* Sum: '<S3>/Sum' incorporates:
   *  Constant: '<S3>/k_init'
   *  Memory: '<S3>/Memory1'
   */
  /* '<S8>:1:42' */
  /* number of active waypoints (others are zeros) */
  rtb_Sum = 1.0 + Skydog_path_planning_DWork.Memory1_PreviousInput;

  /* MATLAB Function: '<S3>/P_wps_nfz' incorporates:
   *  Constant: '<S3>/Constant10'
   *  Constant: '<S3>/Constant7'
   *  Constant: '<S3>/Constant9'
   *  DataTypeConversion: '<S3>/Data Type Conversion10'
   *  DataTypeConversion: '<S3>/Data Type Conversion5'
   *  Inport: '<Root>/Altitude2_r'
   *  Inport: '<Root>/Error'
   *  Inport: '<Root>/Home_position'
   *  MATLAB Function: '<S3>/recalc_GPS_meters'
   *  SignalConversion: '<S4>/TmpSignal ConversionAt SFunction Inport1'
   */
  memcpy((void *)&rtb_DataTypeConversion9[0], (void *)&rtb_wp_ned[0], 60U *
         sizeof(real_T));

  /* MATLAB Function 'Skydog_path_planning/P_wps_nfz': '<S4>:1' */
  /* START_____init values */
  /* '<S4>:1:5' */
  /* number of active waypoints in wp strukt */
  /* '<S4>:1:7' */
  k = rtb_Sum - 1.0;

  /* init vector btw U and last wp */
  /* '<S4>:1:10' */
  /* init vector btw U and next wp */
  /* START_____recalc home position */
  /* '<S4>:1:13' */
  /* '<S4>:1:15' */
  /*  home_m(2) = 0; */
  /*  home_m(3) = 0; */
  /* '<S4>:1:18' */
  /* END_____ recalc home position */
  /* END_____init values */
  /* START_____control end trajektory and errors */
  /* '<S4>:1:23' */
  i = 1;

  /* '<S4>:1:24' */
  go_home = 0;
  if (Error == TRUE) {
    /* '<S4>:1:25' */
    /* '<S4>:1:26' */
    rtb_Sum_0 = (int32_T)(rtb_Sum + 1.0);
    rtb_DataTypeConversion9[rtb_Sum_0 - 1] = Home_position[0];
    rtb_DataTypeConversion9[rtb_Sum_0 + 14] = 0.0;
    rtb_DataTypeConversion9[rtb_Sum_0 + 29] = 0.0;
    rtb_DataTypeConversion9[rtb_Sum_0 + 44] = Home_position[3];

    /* '<S4>:1:27' */
    i = 0;

    /* '<S4>:1:28' */
    go_home = 1;

    /* pridat algoritmus krouzeni kolem home pos!! (stejny alg jako pro */
    /* reseni max_phi) */
  }

  /* pri doleteni na konec trajektory -> "go home" */
  if (rtb_Sum >= j - 1.0) {
    /* '<S4>:1:34' */
    /* '<S4>:1:35' */
    rtb_Sum_0 = (int32_T)(rtb_Sum + 1.0);
    rtb_DataTypeConversion9[rtb_Sum_0 - 1] = Home_position[0];
    rtb_DataTypeConversion9[rtb_Sum_0 + 14] = 0.0;
    rtb_DataTypeConversion9[rtb_Sum_0 + 29] = 0.0;
    rtb_DataTypeConversion9[rtb_Sum_0 + 44] = Home_position[3];

    /* '<S4>:1:36' */
    i = 0;

    /* '<S4>:1:37' */
    go_home = 1;
  }

  /* END____ control end trajektory and errors */
  /* START________diference between wps: */
  /* '<S4>:1:43' */
  /* compute altitude distance (meters) */
  /* '<S4>:1:44' */
  j = rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) + 14] -
    rtb_DataTypeConversion9[(int32_T)rtb_Sum + 14];

  /* compute longitudal distance (deg) */
  /* '<S4>:1:45' */
  yE_m = rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) + 29] -
    rtb_DataTypeConversion9[(int32_T)rtb_Sum + 29];

  /* compute lateral distance (deg) */
  /* '<S4>:1:47' */
  dist = sqrt(rt_powd_snf(j, 2.0) + rt_powd_snf(yE_m, 2.0));

  /* compute full distance (meters) */
  /* '<S4>:1:48' */
  /* normalized dist vector(0-1) */
  /* START____vector (distance and angle) btw U and next wp: */
  /* '<S4>:1:51' */
  d2_idx = rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) - 1] - (real_T)
    Altitude2_r;

  /* alt dist (m) */
  /* '<S4>:1:52' */
  d2_idx_0 = rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) + 14] - rtb_x_ned;

  /* x dist(m) */
  /* '<S4>:1:53' */
  d2_idx_1 = rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) + 29] - rtb_y_ned;

  /* y dist (m) */
  /* '<S4>:1:55' */
  d2_dist = sqrt(rt_powd_snf(d2_idx_0, 2.0) + rt_powd_snf(d2_idx_1, 2.0));

  /* '<S4>:1:56' */
  d2_one_idx = d2_idx_0 / d2_dist;
  d2_one_idx_0 = d2_idx_1 / d2_dist;

  /* difference btw. last wp and U [m] */
  /* alt dist (m) */
  /* x dist (m) */
  /* y dist (m)  */
  /* START_____control max pitch angle: */
  /* params: */
  /* radius of wanted circle (m) - in non classic planning */
  /* init values: */
  /* '<S4>:1:70' */
  phi_w = d2_idx / d2_dist;

  /* actual wanted pitch angle btw U and wp(k+1) */
  /* '<S4>:1:71' */
  /* max pitch angle */
  /* '<S4>:1:72' */
  Nc = 0.0;

  /* if phi_w greater then max pitch angle =>  */
  if ((rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) - 1] -
       rtb_DataTypeConversion9[(int32_T)rtb_Sum - 1]) / dist >
      0.3490658503988659) {
    /* '<S4>:1:74' */
    /* '<S4>:1:75' */
    i = 0;
  }

  while (phi_w > 0.3490658503988659) {
    /* '<S4>:1:78' */
    /* '<S4>:1:79' */
    Nc++;

    /* '<S4>:1:80' */
    phi_w = d2_idx / (Nc * 2.0 * 3.1415926535897931 * R_want + d2_dist);

    /* '<S4>:1:81' */
    i = 0;
  }

  /*  if phi_w lower then phi_min (-phi_max) */
  /* END_________diference between wps */
  /* START______ P compute: */
  /* params: */
  /* wanted diference btw U and P (m) */
  /* tolerante distance btw U and wp (consist for switching) */
  /* init values: */
  /* center of circle path (init) */
  /* '<S4>:1:95' */
  /* vector in XY plane (from C to U) */
  /*  phi_CUW = 0; */
  /*  phi_CWE = 0; */
  /* '<S4>:1:101' */
  phi_out_idx = 0.0;
  phi_out_idx_0 = 0.0;
  if (i == 1) {
    /* '<S4>:1:104' */
    /* P - classic */
    /* '<S4>:1:106' */
    d2_idx = rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) - 1];

    /* '<S4>:1:107' */
    CU_idx = d2_one_idx * L_want + rtb_x_ned;

    /* '<S4>:1:108' */
    CU_idx_0 = d2_one_idx_0 * L_want + rtb_y_ned;
  } else {
    /* classic = 0 -> prodlouzit trasu spiralou (reseni max pitch angle a krouzeni kolem home pos) */
    /* '<S4>:1:112' */
    /* '<S4>:1:113' */
    gama = asin(L_want / (2.0 * R_want)) * 2.0;

    /* angle around C from U to P (phi_CUP) */
    /* '<S4>:1:115' */
    rtb_Sum_0 = (int32_T)(rtb_Sum + 1.0);
    d2_idx = rtb_DataTypeConversion9[rtb_Sum_0 - 1] + R_want * 0.0;
    d2_idx_0 = -(yE_m / dist) * R_want + rtb_DataTypeConversion9[rtb_Sum_0 + 14];
    d2_idx_1 = j / dist * R_want + rtb_DataTypeConversion9[rtb_Sum_0 + 29];

    /* '<S4>:1:117' */
    yE_m = sqrt(rt_powd_snf(d2_idx_0 - rtb_x_ned, 2.0) + rt_powd_snf(d2_idx_1 -
      rtb_y_ned, 2.0));

    /* '<S4>:1:119' */
    j = rtb_x_ned - d2_idx_0;
    if (j != 0.0) {
      j /= yE_m;
    }

    CU_idx = j;

    /* in xE dirr */
    /* '<S4>:1:120' */
    j = rtb_y_ned - d2_idx_1;
    if (j != 0.0) {
      j /= yE_m;
    }

    CU_idx_0 = j;

    /* in yE dirr */
    /* uhel mezi CU a CW = (dist_one(2) , -dist_one(1)): */
    /* angle - CU,xE: */
    /* '<S4>:1:124' */
    j = atan(j / CU_idx);

    /* angle - CW,xE: */
    /*          phi_CWE = atan(-dist_one(1)/dist_one(2)); */
    /* correction: */
    /* phi_CUE: */
    if ((CU_idx_0 < 0.0) && (CU_idx < 0.0)) {
      /* '<S4>:1:130' */
      /* '<S4>:1:131' */
      j += -3.1415926535897931;
    } else {
      if ((CU_idx_0 > 0.0) && (CU_idx < 0.0)) {
        /* '<S4>:1:132' */
        /* '<S4>:1:133' */
        j += 3.1415926535897931;
      }
    }

    /* '<S4>:1:137' */
    phi_out_idx = j + gama;

    /* switch to circle planning and from circle planning: */
    /* '<S4>:1:140' */
    yE_m = rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) - 1] - (real_T)
      Altitude2_r;

    /* '<S4>:1:141' */
    /* Nc - number of loops (from condition phi_w) */
    /* '<S4>:1:143' */
    phi_out_idx_0 = yE_m;
    if (yE_m < (Nc + 0.5) * 2.0 * 3.1415926535897931 * R_want * phi_w) {
      /* '<S4>:1:145' */
      if (d2_dist < 2.0 * R_want + Trash_want) {
        /* '<S4>:1:146' */
        /* circle planning with altitude increasing */
        /* '<S4>:1:148' */
        /* '<S4>:1:149' */
        CU_idx = cos(j + gama) * R_want + d2_idx_0;

        /* '<S4>:1:150' */
        CU_idx_0 = sin(j + gama) * R_want + d2_idx_1;
      } else {
        /* '<S4>:1:152' */
        /* '<S4>:1:153' */
        CU_idx = d2_one_idx * L_want + rtb_x_ned;

        /* '<S4>:1:154' */
        CU_idx_0 = d2_one_idx_0 * L_want + rtb_y_ned;
      }
    } else {
      /* line planning with altitude increasing    */
      /* '<S4>:1:158' */
      d2_idx = rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) - 1];

      /*  + phi_w * (d1_dist+L); */
      /* '<S4>:1:159' */
      CU_idx = d2_one_idx * L_want + rtb_x_ned;

      /* '<S4>:1:160' */
      CU_idx_0 = d2_one_idx_0 * L_want + rtb_y_ned;
    }

    /* circle planning in altitude level (around home possition) */
    /*          if go_home = 1 */
    /*              P(1,1) = C(1);  */
    /*              P(1,2) = C(2) + R*cos(phi_CUE+gama); */
    /*              P(1,3) = C(3) + R*sin(phi_CUE+gama); */
    /*          end */
  }

  /* START_______compute switching consistance: */
  /* switch z polohy wp(k+1) a U - dle podminek v "big_picture_autopilot" !!! */
  if (i == 1) {
    /* '<S4>:1:174' */
    if (d2_dist < Trash_want) {
      /* '<S4>:1:175' */
      /* '<S4>:1:176' */
      k = rtb_Sum;
    }
  } else {
    if ((rtb_DataTypeConversion9[(int32_T)(rtb_Sum + 1.0) - 1] - (real_T)
         Altitude2_r < 0.0) && (go_home == 0)) {
      /* '<S4>:1:179' */
      /* '<S4>:1:180' */
      /* '<S4>:1:181' */
      k = rtb_Sum;
    }
  }

  /* '<S4>:1:186' */
  /* do logovaci appky   */
  rtb_phi_out[0] = phi_out_idx;
  rtb_phi_out[1] = phi_out_idx_0;
  rtb_d2_dist = d2_dist;

  /* MATLAB Function: '<S3>/lambda_eta' incorporates:
   *  DataTypeConversion: '<S3>/Data Type Conversion8'
   *  Inport: '<Root>/Groundspeed2_r'
   *  MATLAB Function: '<S3>/P_wps_nfz'
   *  SignalConversion: '<S7>/TmpSignal ConversionAt SFunction Inport1'
   */
  /* MATLAB Function 'Skydog_path_planning/lambda_eta': '<S7>:1' */
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
  /* '<S7>:1:22' */
  j = CU_idx - rtb_x_ned;

  /* '<S7>:1:23' */
  yE_m = CU_idx_0 - rtb_y_ned;

  /* '<S7>:1:24' */
  /* uhel lambda - uhel mezi zadana trajektorie a odchylka uav od ni: */
  /*  lambda_cos = (d1^2+wd^2-d2^2)/(2*d1*wd); */
  /* uhel eta - uhel mezi L1a a vektorem rychlosti Gsp: */
  /*  Gsp_di = sqrt(Gsp(1)^2+Gsp(2)^2); */
  /*  cos_eta = ((L1E*Gsp(1)+L1N*Gsp(2))/(L1a*Gsp_di)); */
  /* % vypocet uhlu eta vzhledem k ose x-lat "East direction": */
  /* '<S7>:1:34' */
  dist = atan((real_T)Groundspeed2_r[1] / (real_T)Groundspeed2_r[0]);

  /* '<S7>:1:35' */
  gama = atan(yE_m / j);

  /* osetreni max a min hodnot:  */
  if ((yE_m < 0.0) && (j < 0.0)) {
    /* '<S7>:1:39' */
    /* '<S7>:1:40' */
    gama += -3.1415926535897931;
  } else {
    if ((yE_m > 0.0) && (j < 0.0)) {
      /* '<S7>:1:41' */
      /* '<S7>:1:42' */
      gama += 3.1415926535897931;
    }
  }

  if ((Groundspeed2_r[0] < 0.0F) && (Groundspeed2_r[1] < 0.0F)) {
    /* '<S7>:1:45' */
    /* '<S7>:1:46' */
    dist += -3.1415926535897931;
  } else {
    if ((Groundspeed2_r[0] < 0.0F) && (Groundspeed2_r[1] > 0.0F)) {
      /* '<S7>:1:47' */
      /* '<S7>:1:48' */
      dist += 3.1415926535897931;
    }
  }

  /* vysledny uhel eta: */
  /* '<S7>:1:52' */
  rtb_eta = gama - dist;
  rtb_L1a = sqrt(rt_powd_snf(j, 2.0) + rt_powd_snf(yE_m, 2.0));

  /* End of MATLAB Function: '<S3>/lambda_eta' */

  /* MATLAB Function: '<S3>/acc_lat_roll_angle' incorporates:
   *  DataTypeConversion: '<S3>/Data Type Conversion8'
   *  Inport: '<Root>/Groundspeed2_r'
   */
  /* MATLAB Function 'Skydog_path_planning/acc_lat_roll_angle': '<S5>:1' */
  /* NE speed vect: */
  /* '<S5>:1:5' */
  /*  laterar acceleration: - zvolena delka L1 */
  /* '<S5>:1:8' */
  /* '<S5>:1:10' */
  j = (rt_powd_snf((real_T)Groundspeed2_r[0], 2.0) + rt_powd_snf((real_T)
        Groundspeed2_r[1], 2.0)) * 2.0 * sin(rtb_eta) / 30.0 / 9.81;

  /* '<S5>:1:12' */
  /* rad - maximum roll angle */
  if (j >= 0.69813170079773179) {
    /* '<S5>:1:13' */
    /* '<S5>:1:14' */
    j = 0.69813170079773179;
  } else if (j <= -0.69813170079773179) {
    /* '<S5>:1:15' */
    /* '<S5>:1:16' */
    j = -0.69813170079773179;
  } else {
    /* '<S5>:1:18' */
  }

  /* End of MATLAB Function: '<S3>/acc_lat_roll_angle' */

  /* DataTypeConversion: '<S3>/Data Type Conversion4' */
  Roll2_w = (real32_T)j;

  /* MATLAB Function: '<S3>/altitude_wanted' incorporates:
   *  MATLAB Function: '<S3>/P_wps_nfz'
   */
  /* MATLAB Function 'Skydog_path_planning/altitude_wanted': '<S6>:1' */
  /* '<S6>:1:4' */
  j = d2_idx;
  if (d2_idx >= 2000.0) {
    /* '<S6>:1:6' */
    /* '<S6>:1:7' */
    j = 2000.0;
  }

  /* End of MATLAB Function: '<S3>/altitude_wanted' */

  /* DataTypeConversion: '<S3>/Data Type Conversion1' */
  Altitude2_w = (real32_T)j;

  /* DataTypeConversion: '<S3>/Data Type Conversion2' */
  /* MATLAB Function 'Skydog_path_planning/speed_wanted': '<S10>:1' */
  /*  speed_w = wp_act(1,4); */
  /* '<S10>:1:5' */
  Speed_w = 11.0F;

  /* DataTypeConversion: '<S3>/Data Type Conversion3' incorporates:
   *  MATLAB Function: '<S3>/P_wps_nfz'
   */
  P[0] = (real32_T)d2_idx;
  P[1] = (real32_T)CU_idx;
  P[2] = (real32_T)CU_idx_0;

  /* DataTypeConversion: '<S3>/Data Type Conversion15' */
  Act_wps_index = (real32_T)rtb_Sum;

  /* DataTypeConversion: '<S3>/Data Type Conversion14' incorporates:
   *  DataTypeConversion: '<S3>/Data Type Conversion5'
   *  Inport: '<Root>/Altitude2_r'
   */
  U[0] = Altitude2_r;
  U[1] = (real32_T)rtb_x_ned;
  U[2] = (real32_T)rtb_y_ned;

  /* DataTypeConversion: '<S3>/Data Type Conversion11' incorporates:
   *  MATLAB Function: '<S3>/P_wps_nfz'
   */
  for (i = 0; i < 4; i++) {
    wp_actual[(i << 1)] = (real32_T)rtb_DataTypeConversion9[(15 * i + (int32_T)
      rtb_Sum) - 1];
    wp_actual[1 + (i << 1)] = (real32_T)rtb_DataTypeConversion9[((int32_T)
      (rtb_Sum + 1.0) + 15 * i) - 1];
  }

  /* End of DataTypeConversion: '<S3>/Data Type Conversion11' */

  /* Update for Memory: '<S3>/Memory1' incorporates:
   *  MATLAB Function: '<S3>/P_wps_nfz'
   */
  Skydog_path_planning_DWork.Memory1_PreviousInput = k;
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

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      wp_actual[i] = 0.0F;
    }
  }

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
