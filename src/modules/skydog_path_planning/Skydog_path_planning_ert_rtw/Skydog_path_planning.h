/*
 * File: Skydog_path_planning.h
 *
 * Code generated for Simulink model 'Skydog_path_planning'.
 *
 * Model version                  : 1.148
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Mon Dec 02 22:04:28 2013
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
#include <stddef.h>
#include "rtwtypes.h"
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

/* Parameters (auto storage) */
struct Parameters_Skydog_path_planning_ {
  real32_T Gain3_Gain;                 /* Computed Parameter: Gain3_Gain
                                        * Referenced by: '<S1>/Gain3'
                                        */
  real32_T Gain1_Gain;                 /* Computed Parameter: Gain1_Gain
                                        * Referenced by: '<S1>/Gain1'
                                        */
  real32_T Gain2_Gain;                 /* Computed Parameter: Gain2_Gain
                                        * Referenced by: '<S1>/Gain2'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_Skydog_path_planning {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_Skydog_path_planning Skydog_path_planning_P;

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
 * hilite_system('SkyDog_Autopilot_template/Skydog_path_planning')    - opens subsystem SkyDog_Autopilot_template/Skydog_path_planning
 * hilite_system('SkyDog_Autopilot_template/Skydog_path_planning/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SkyDog_Autopilot_template'
 * '<S1>'   : 'SkyDog_Autopilot_template/Skydog_path_planning'
 * '<S2>'   : 'SkyDog_Autopilot_template/Skydog_path_planning/Path_planning'
 */
#endif                                 /* RTW_HEADER_Skydog_path_planning_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
