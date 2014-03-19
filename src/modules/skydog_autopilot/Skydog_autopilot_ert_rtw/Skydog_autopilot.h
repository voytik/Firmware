/*
 * File: Skydog_autopilot.h
 *
 * Code generated for Simulink model 'Skydog_autopilot'.
 *
 * Model version                  : 1.216
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Wed Mar 19 21:15:43 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Skydog_autopilot_h_
#define RTW_HEADER_Skydog_autopilot_h_
#ifndef Skydog_autopilot_COMMON_INCLUDES_
# define Skydog_autopilot_COMMON_INCLUDES_
#include <stddef.h>
#include <string.h>
#include "rtwtypes.h"
#endif                                 /* Skydog_autopilot_COMMON_INCLUDES_ */

#include "Skydog_autopilot_types.h"

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
  struct {
    void *LoggedData;
  } Scope6_PWORK;                      /* '<S1>/Scope6' */

  struct {
    void *LoggedData;
  } Scope_PWORK;                       /* '<S2>/Scope' */

  struct {
    void *LoggedData;
  } Scope1_PWORK;                      /* '<S2>/Scope1' */

  struct {
    void *LoggedData;
  } Scope_PWORK_e;                     /* '<S3>/Scope' */

  struct {
    void *LoggedData;
  } Scope1_PWORK_d;                    /* '<S3>/Scope1' */

  struct {
    void *LoggedData;
  } Scope2_PWORK;                      /* '<S3>/Scope2' */

  struct {
    void *LoggedData;
  } Scope3_PWORK;                      /* '<S3>/Scope3' */

  struct {
    void *LoggedData;
  } Scope5_PWORK;                      /* '<S3>/Scope5' */

  real32_T DiscreteTimeIntegrator_DSTATE;/* '<S4>/Discrete-Time Integrator' */
  real32_T DiscreteTimeIntegrator_DSTATE_b;/* '<S6>/Discrete-Time Integrator' */
  real32_T DiscreteTimeIntegrator_DSTATE_f;/* '<S10>/Discrete-Time Integrator' */
  real32_T DiscreteTimeIntegrator_DSTAT_fr;/* '<S11>/Discrete-Time Integrator' */
  real32_T DiscreteTimeIntegrator_DSTATE_d;/* '<S5>/Discrete-Time Integrator' */
} D_Work_Skydog_autopilot;

/* Real-time Model Data Structure */
struct RT_MODEL_Skydog_autopilot {
  const char_T * volatile errorStatus;
};

/* Block states (auto storage) */
extern D_Work_Skydog_autopilot Skydog_autopilot_DWork;

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern real32_T Alti_control_I;        /* Variable: Alti_control_I
                                        * Referenced by: '<S4>/Gain1'
                                        */
extern real32_T Alti_control_P;        /* Variable: Alti_control_P
                                        * Referenced by: '<S4>/Gain'
                                        */
extern real32_T Pitch_control_I;       /* Variable: Pitch_control_I
                                        * Referenced by: '<S6>/Gain1'
                                        */
extern real32_T Pitch_control_P;       /* Variable: Pitch_control_P
                                        * Referenced by: '<S6>/Gain'
                                        */
extern real32_T Pitch_rate_control_P;  /* Variable: Pitch_rate_control_P
                                        * Referenced by: '<S8>/Gain'
                                        */
extern real32_T Roll_control_I;        /* Variable: Roll_control_I
                                        * Referenced by: '<S10>/Gain1'
                                        */
extern real32_T Roll_control_P;        /* Variable: Roll_control_P
                                        * Referenced by: '<S10>/Gain'
                                        */
extern real32_T Roll_rate_control_P;   /* Variable: Roll_rate_control_P
                                        * Referenced by: '<S9>/Gain'
                                        */
extern real32_T Roll_yaw_FF;           /* Variable: Roll_yaw_FF
                                        * Referenced by: '<S2>/Gain'
                                        */
extern real32_T Speed_control_I;       /* Variable: Speed_control_I
                                        * Referenced by: '<S5>/Gain1'
                                        */
extern real32_T Speed_control_P;       /* Variable: Speed_control_P
                                        * Referenced by: '<S5>/Gain'
                                        */
extern real32_T Yaw_rate_control_I;    /* Variable: Yaw_rate_control_I
                                        * Referenced by: '<S11>/Gain1'
                                        */
extern real32_T Yaw_rate_control_P;    /* Variable: Yaw_rate_control_P
                                        * Referenced by: '<S11>/Gain'
                                        */

/* Model entry point functions */
extern void Skydog_autopilot_initialize(void);
extern void Skydog_autopilot_step(void);
extern void Skydog_autopilot_terminate(void);

/* Real-time Model object */
extern struct RT_MODEL_Skydog_autopilot *const Skydog_autopilot_M;

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
 * hilite_system('SkyDog_Autopilot_discrete/Skydog_autopilot')    - opens subsystem SkyDog_Autopilot_discrete/Skydog_autopilot
 * hilite_system('SkyDog_Autopilot_discrete/Skydog_autopilot/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SkyDog_Autopilot_discrete'
 * '<S1>'   : 'SkyDog_Autopilot_discrete/Skydog_autopilot'
 * '<S2>'   : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Autopilot'
 * '<S3>'   : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Stabilization'
 * '<S4>'   : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Autopilot/alti_control'
 * '<S5>'   : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Autopilot/speed_control'
 * '<S6>'   : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Stabilization/Pitch_control'
 * '<S7>'   : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Stabilization/mode_sel'
 * '<S8>'   : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Stabilization/pitch _rate_control'
 * '<S9>'   : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Stabilization/roll _rate_control'
 * '<S10>'  : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Stabilization/roll_control '
 * '<S11>'  : 'SkyDog_Autopilot_discrete/Skydog_autopilot/Stabilization/yaw _rate_control'
 */
#endif                                 /* RTW_HEADER_Skydog_autopilot_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
