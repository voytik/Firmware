/*
 * File: Skydog_path_planning.h
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

#ifndef RTW_HEADER_Skydog_path_planning_h_
#define RTW_HEADER_Skydog_path_planning_h_
#ifndef Skydog_path_planning_COMMON_INCLUDES_
# define Skydog_path_planning_COMMON_INCLUDES_
#include <math.h>
#include <stddef.h>
#include <string.h>
#include "rtwtypes.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"
#include "rtGetNaN.h"
#endif                                 /* Skydog_path_planning_COMMON_INCLUDES_ */

#include "Skydog_path_planning_types.h"

/* Includes for objects with custom storage classes. */
#include "SkydogSignals.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T Memory1_PreviousInput;        /* '<S3>/Memory1' */
  struct {
    void *LoggedData;
  } Scope_PWORK;                       /* '<S3>/Scope' */

  struct {
    void *LoggedData;
  } ToWorkspace1_PWORK;                /* '<S3>/To Workspace1' */

  struct {
    void *LoggedData;
  } ToWorkspace2_PWORK;                /* '<S3>/To Workspace2' */

  struct {
    void *LoggedData;
  } ToWorkspace4_PWORK;                /* '<S3>/To Workspace4' */

  struct {
    void *LoggedData;
  } ToWorkspace6_PWORK;                /* '<S3>/To Workspace6' */
} D_Work_Skydog_path_planning;

/* Real-time Model Data Structure */
struct RT_MODEL_Skydog_path_planning {
  const char_T * volatile errorStatus;
};

/* Block states (auto storage) */
extern D_Work_Skydog_path_planning Skydog_path_planning_DWork;

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern real_T L_want;                  /* Variable: L_want
                                        * Referenced by: '<S3>/Constant7'
                                        */
extern real_T R_want;                  /* Variable: R_want
                                        * Referenced by: '<S3>/Constant9'
                                        */
extern real_T Trash_want;              /* Variable: Trash_want
                                        * Referenced by: '<S3>/Constant10'
                                        */

/* Model entry point functions */
extern void Skydog_path_planning_initialize(void);
extern void Skydog_path_planning_step(void);
extern void Skydog_path_planning_terminate(void);

/* Real-time Model object */
extern struct RT_MODEL_Skydog_path_planning *const Skydog_path_planning_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('SkyDog_Autopilot_discrete2/Skydog_path_planning')    - opens subsystem SkyDog_Autopilot_discrete2/Skydog_path_planning
 * hilite_system('SkyDog_Autopilot_discrete2/Skydog_path_planning/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SkyDog_Autopilot_discrete2'
 * '<S3>'   : 'SkyDog_Autopilot_discrete2/Skydog_path_planning'
 * '<S4>'   : 'SkyDog_Autopilot_discrete2/Skydog_path_planning/P_wps_nfz'
 * '<S5>'   : 'SkyDog_Autopilot_discrete2/Skydog_path_planning/acc_lat_roll_angle'
 * '<S6>'   : 'SkyDog_Autopilot_discrete2/Skydog_path_planning/altitude_wanted'
 * '<S7>'   : 'SkyDog_Autopilot_discrete2/Skydog_path_planning/lambda_eta'
 * '<S8>'   : 'SkyDog_Autopilot_discrete2/Skydog_path_planning/recalc_GPS_meters'
 * '<S9>'   : 'SkyDog_Autopilot_discrete2/Skydog_path_planning/recalculate_GPS_meters_U'
 * '<S10>'  : 'SkyDog_Autopilot_discrete2/Skydog_path_planning/speed_wanted'
 */
#endif                                 /* RTW_HEADER_Skydog_path_planning_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
