/*
 * File: Skydog_path_planning.h
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
  real_T Memory1_PreviousInput;        /* '<S2>/Memory1' */
  real_T Memory_PreviousInput;         /* '<S2>/Memory' */
  struct {
    void *LoggedData;
  } Scope_PWORK;                       /* '<S2>/Scope' */

  struct {
    void *LoggedData;
  } ToWorkspace1_PWORK;                /* '<S2>/To Workspace1' */

  struct {
    void *LoggedData;
  } ToWorkspace4_PWORK;                /* '<S2>/To Workspace4' */

  struct {
    void *LoggedData;
  } ToWorkspace6_PWORK;                /* '<S2>/To Workspace6' */

  struct {
    void *LoggedData;
  } ToWorkspace7_PWORK;                /* '<S2>/To Workspace7' */
} D_Work_Skydog_path_planning;

/* Parameters (auto storage) */
struct Parameters_Skydog_path_planning_ {
  real_T Constant_Value[60];           /* Expression: wps.variables.values
                                        * Referenced by: '<S2>/Constant'
                                        */
  real_T k_init_Value;                 /* Expression: 1
                                        * Referenced by: '<S2>/k_init'
                                        */
  real_T Memory1_X0;                   /* Expression: 0
                                        * Referenced by: '<S2>/Memory1'
                                        */
  real_T t0_init_Value;                /* Expression: 0
                                        * Referenced by: '<S2>/t0_init'
                                        */
  real_T Memory_X0;                    /* Expression: 0
                                        * Referenced by: '<S2>/Memory'
                                        */
  uint8_T Gain6_Gain;                  /* Computed Parameter: Gain6_Gain
                                        * Referenced by: '<S2>/Gain6'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_Skydog_path_planning {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_Skydog_path_planning Skydog_path_planning_P;

/* Block states (auto storage) */
extern D_Work_Skydog_path_planning Skydog_path_planning_DWork;

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
 * hilite_system('SkyDog_Autopilot_discrete/Skydog_path_planning')    - opens subsystem SkyDog_Autopilot_discrete/Skydog_path_planning
 * hilite_system('SkyDog_Autopilot_discrete/Skydog_path_planning/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SkyDog_Autopilot_discrete'
 * '<S2>'   : 'SkyDog_Autopilot_discrete/Skydog_path_planning'
 * '<S3>'   : 'SkyDog_Autopilot_discrete/Skydog_path_planning/P_wps_nfz'
 * '<S4>'   : 'SkyDog_Autopilot_discrete/Skydog_path_planning/acc_lat_roll_angle'
 * '<S5>'   : 'SkyDog_Autopilot_discrete/Skydog_path_planning/altitude_wanted'
 * '<S6>'   : 'SkyDog_Autopilot_discrete/Skydog_path_planning/lambda_eta'
 * '<S7>'   : 'SkyDog_Autopilot_discrete/Skydog_path_planning/speed_wanted'
 */
#endif                                 /* RTW_HEADER_Skydog_path_planning_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
