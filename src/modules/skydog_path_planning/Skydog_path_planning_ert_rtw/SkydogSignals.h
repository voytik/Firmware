/*
 * File: SkydogSignals.h
 *
 * Code generated for Simulink model 'Skydog_path_planning'.
 *
 * Model version                  : 1.318
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Tue Jun 10 20:16:37 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_SkydogSignals_h_
#define RTW_HEADER_SkydogSignals_h_
#include "rtwtypes.h"
#include "Skydog_path_planning_types.h"

/* Exported data declaration */

/* Declaration for custom storage class: ExportToFile */
extern real32_T Act_wps_index;
extern real32_T Altitude2_r;
extern real32_T Altitude2_w;
extern boolean_T Error;
extern real32_T Groundspeed2_r[3];
extern real32_T Home_position[4];
extern int16_T Mode2_w;
extern real32_T P[3];
extern real32_T Roll2_w;
extern real32_T Speed_w;
extern real32_T U[3];
extern real32_T Waypoints_w[60];
extern real32_T X_earth_r;
extern real32_T Y_earth_r;
extern real32_T d2;
extern real32_T eta;

#endif                                 /* RTW_HEADER_SkydogSignals_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
