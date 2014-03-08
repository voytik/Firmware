/*
 * File: Skydog_autopilot.h
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

/* Parameters (auto storage) */
struct Parameters_Skydog_autopilot_ {
  real32_T Gain_Gain;                  /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<S5>/Gain'
                                        */
  real32_T Saturation1_UpperSat;       /* Computed Parameter: Saturation1_UpperSat
                                        * Referenced by: '<S5>/Saturation1'
                                        */
  real32_T Saturation1_LowerSat;       /* Computed Parameter: Saturation1_LowerSat
                                        * Referenced by: '<S5>/Saturation1'
                                        */
  real32_T Gain_Gain_k;                /* Computed Parameter: Gain_Gain_k
                                        * Referenced by: '<S2>/Gain'
                                        */
  real32_T Gain_Gain_l;                /* Computed Parameter: Gain_Gain_l
                                        * Referenced by: '<S4>/Gain'
                                        */
  real32_T DiscreteTimeIntegrator_gainval;/* Computed Parameter: DiscreteTimeIntegrator_gainval
                                           * Referenced by: '<S4>/Discrete-Time Integrator'
                                           */
  real32_T DiscreteTimeIntegrator_IC;  /* Computed Parameter: DiscreteTimeIntegrator_IC
                                        * Referenced by: '<S4>/Discrete-Time Integrator'
                                        */
  real32_T DiscreteTimeIntegrator_UpperSat;/* Computed Parameter: DiscreteTimeIntegrator_UpperSat
                                            * Referenced by: '<S4>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_LowerSat;/* Computed Parameter: DiscreteTimeIntegrator_LowerSat
                                            * Referenced by: '<S4>/Discrete-Time Integrator'
                                            */
  real32_T Saturation1_UpperSat_j;     /* Computed Parameter: Saturation1_UpperSat_j
                                        * Referenced by: '<S4>/Saturation1'
                                        */
  real32_T Saturation1_LowerSat_i;     /* Computed Parameter: Saturation1_LowerSat_i
                                        * Referenced by: '<S4>/Saturation1'
                                        */
  real32_T Saturation4_UpperSat;       /* Computed Parameter: Saturation4_UpperSat
                                        * Referenced by: '<S3>/Saturation4'
                                        */
  real32_T Saturation4_LowerSat;       /* Computed Parameter: Saturation4_LowerSat
                                        * Referenced by: '<S3>/Saturation4'
                                        */
  real32_T Gain_Gain_b;                /* Computed Parameter: Gain_Gain_b
                                        * Referenced by: '<S6>/Gain'
                                        */
  real32_T DiscreteTimeIntegrator_gainva_d;/* Computed Parameter: DiscreteTimeIntegrator_gainva_d
                                            * Referenced by: '<S6>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_IC_h;/* Computed Parameter: DiscreteTimeIntegrator_IC_h
                                        * Referenced by: '<S6>/Discrete-Time Integrator'
                                        */
  real32_T DiscreteTimeIntegrator_UpperS_o;/* Computed Parameter: DiscreteTimeIntegrator_UpperS_o
                                            * Referenced by: '<S6>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_LowerS_a;/* Computed Parameter: DiscreteTimeIntegrator_LowerS_a
                                            * Referenced by: '<S6>/Discrete-Time Integrator'
                                            */
  real32_T Saturation1_UpperSat_k;     /* Computed Parameter: Saturation1_UpperSat_k
                                        * Referenced by: '<S6>/Saturation1'
                                        */
  real32_T Saturation1_LowerSat_c;     /* Computed Parameter: Saturation1_LowerSat_c
                                        * Referenced by: '<S6>/Saturation1'
                                        */
  real32_T Gain_Gain_p;                /* Computed Parameter: Gain_Gain_p
                                        * Referenced by: '<S8>/Gain'
                                        */
  real32_T Saturation1_UpperSat_i;     /* Computed Parameter: Saturation1_UpperSat_i
                                        * Referenced by: '<S8>/Saturation1'
                                        */
  real32_T Saturation1_LowerSat_g;     /* Computed Parameter: Saturation1_LowerSat_g
                                        * Referenced by: '<S8>/Saturation1'
                                        */
  real32_T Saturation2_UpperSat;       /* Computed Parameter: Saturation2_UpperSat
                                        * Referenced by: '<S3>/Saturation2'
                                        */
  real32_T Saturation2_LowerSat;       /* Computed Parameter: Saturation2_LowerSat
                                        * Referenced by: '<S3>/Saturation2'
                                        */
  real32_T Saturation5_UpperSat;       /* Computed Parameter: Saturation5_UpperSat
                                        * Referenced by: '<S3>/Saturation5'
                                        */
  real32_T Saturation5_LowerSat;       /* Computed Parameter: Saturation5_LowerSat
                                        * Referenced by: '<S3>/Saturation5'
                                        */
  real32_T Gain_Gain_m;                /* Computed Parameter: Gain_Gain_m
                                        * Referenced by: '<S10>/Gain'
                                        */
  real32_T DiscreteTimeIntegrator_gainva_g;/* Computed Parameter: DiscreteTimeIntegrator_gainva_g
                                            * Referenced by: '<S10>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_IC_i;/* Computed Parameter: DiscreteTimeIntegrator_IC_i
                                        * Referenced by: '<S10>/Discrete-Time Integrator'
                                        */
  real32_T DiscreteTimeIntegrator_UpperS_k;/* Computed Parameter: DiscreteTimeIntegrator_UpperS_k
                                            * Referenced by: '<S10>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_LowerS_c;/* Computed Parameter: DiscreteTimeIntegrator_LowerS_c
                                            * Referenced by: '<S10>/Discrete-Time Integrator'
                                            */
  real32_T Saturation1_UpperSat_c;     /* Computed Parameter: Saturation1_UpperSat_c
                                        * Referenced by: '<S10>/Saturation1'
                                        */
  real32_T Saturation1_LowerSat_m;     /* Computed Parameter: Saturation1_LowerSat_m
                                        * Referenced by: '<S10>/Saturation1'
                                        */
  real32_T Gain_Gain_e;                /* Computed Parameter: Gain_Gain_e
                                        * Referenced by: '<S9>/Gain'
                                        */
  real32_T Saturation1_UpperSat_b;     /* Computed Parameter: Saturation1_UpperSat_b
                                        * Referenced by: '<S9>/Saturation1'
                                        */
  real32_T Saturation1_LowerSat_ij;    /* Computed Parameter: Saturation1_LowerSat_ij
                                        * Referenced by: '<S9>/Saturation1'
                                        */
  real32_T Saturation_UpperSat;        /* Computed Parameter: Saturation_UpperSat
                                        * Referenced by: '<S3>/Saturation'
                                        */
  real32_T Saturation_LowerSat;        /* Computed Parameter: Saturation_LowerSat
                                        * Referenced by: '<S3>/Saturation'
                                        */
  real32_T Saturation6_UpperSat;       /* Computed Parameter: Saturation6_UpperSat
                                        * Referenced by: '<S3>/Saturation6'
                                        */
  real32_T Saturation6_LowerSat;       /* Computed Parameter: Saturation6_LowerSat
                                        * Referenced by: '<S3>/Saturation6'
                                        */
  real32_T Gain_Gain_ba;               /* Computed Parameter: Gain_Gain_ba
                                        * Referenced by: '<S11>/Gain'
                                        */
  real32_T DiscreteTimeIntegrator_gainva_f;/* Computed Parameter: DiscreteTimeIntegrator_gainva_f
                                            * Referenced by: '<S11>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_IC_k;/* Computed Parameter: DiscreteTimeIntegrator_IC_k
                                        * Referenced by: '<S11>/Discrete-Time Integrator'
                                        */
  real32_T DiscreteTimeIntegrator_UpperS_a;/* Computed Parameter: DiscreteTimeIntegrator_UpperS_a
                                            * Referenced by: '<S11>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_LowerS_l;/* Computed Parameter: DiscreteTimeIntegrator_LowerS_l
                                            * Referenced by: '<S11>/Discrete-Time Integrator'
                                            */
  real32_T Saturation1_UpperSat_jh;    /* Computed Parameter: Saturation1_UpperSat_jh
                                        * Referenced by: '<S11>/Saturation1'
                                        */
  real32_T Saturation1_LowerSat_cj;    /* Computed Parameter: Saturation1_LowerSat_cj
                                        * Referenced by: '<S11>/Saturation1'
                                        */
  real32_T Saturation1_UpperSat_e;     /* Computed Parameter: Saturation1_UpperSat_e
                                        * Referenced by: '<S3>/Saturation1'
                                        */
  real32_T Saturation1_LowerSat_ga;    /* Computed Parameter: Saturation1_LowerSat_ga
                                        * Referenced by: '<S3>/Saturation1'
                                        */
  real32_T DiscreteTimeIntegrator_gainva_h;/* Computed Parameter: DiscreteTimeIntegrator_gainva_h
                                            * Referenced by: '<S5>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_IC_m;/* Computed Parameter: DiscreteTimeIntegrator_IC_m
                                        * Referenced by: '<S5>/Discrete-Time Integrator'
                                        */
  real32_T DiscreteTimeIntegrator_UpperS_b;/* Computed Parameter: DiscreteTimeIntegrator_UpperS_b
                                            * Referenced by: '<S5>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_LowerS_m;/* Computed Parameter: DiscreteTimeIntegrator_LowerS_m
                                            * Referenced by: '<S5>/Discrete-Time Integrator'
                                            */
  real32_T Saturation3_UpperSat;       /* Computed Parameter: Saturation3_UpperSat
                                        * Referenced by: '<S3>/Saturation3'
                                        */
  real32_T Saturation3_LowerSat;       /* Computed Parameter: Saturation3_LowerSat
                                        * Referenced by: '<S3>/Saturation3'
                                        */
  real32_T Gain_Gain_p1;               /* Computed Parameter: Gain_Gain_p1
                                        * Referenced by: '<S1>/Gain'
                                        */
  real32_T Gain1_Gain;                 /* Computed Parameter: Gain1_Gain
                                        * Referenced by: '<S1>/Gain1'
                                        */
  real32_T Gain1_Gain_n;               /* Computed Parameter: Gain1_Gain_n
                                        * Referenced by: '<S4>/Gain1'
                                        */
  real32_T Gain1_Gain_j;               /* Computed Parameter: Gain1_Gain_j
                                        * Referenced by: '<S5>/Gain1'
                                        */
  real32_T Gain1_Gain_k;               /* Computed Parameter: Gain1_Gain_k
                                        * Referenced by: '<S6>/Gain1'
                                        */
  real32_T Gain1_Gain_e;               /* Computed Parameter: Gain1_Gain_e
                                        * Referenced by: '<S10>/Gain1'
                                        */
  real32_T Gain1_Gain_m;               /* Computed Parameter: Gain1_Gain_m
                                        * Referenced by: '<S11>/Gain1'
                                        */
  int16_T Switch1_Threshold;           /* Computed Parameter: Switch1_Threshold
                                        * Referenced by: '<S7>/Switch1'
                                        */
  int16_T Switch_Threshold;            /* Computed Parameter: Switch_Threshold
                                        * Referenced by: '<S7>/Switch'
                                        */
  int16_T Switch2_Threshold;           /* Computed Parameter: Switch2_Threshold
                                        * Referenced by: '<S7>/Switch2'
                                        */
  int16_T Switch3_Threshold;           /* Computed Parameter: Switch3_Threshold
                                        * Referenced by: '<S7>/Switch3'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_Skydog_autopilot {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_Skydog_autopilot Skydog_autopilot_P;

/* Block states (auto storage) */
extern D_Work_Skydog_autopilot Skydog_autopilot_DWork;

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
 * hilite_system('SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot')    - opens subsystem SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot
 * hilite_system('SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SkyDog_Autopilot_discrete_integrators1'
 * '<S1>'   : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot'
 * '<S2>'   : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Autopilot'
 * '<S3>'   : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Stabilization'
 * '<S4>'   : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Autopilot/alti_control'
 * '<S5>'   : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Autopilot/speed_control'
 * '<S6>'   : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Stabilization/Pitch_control'
 * '<S7>'   : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Stabilization/mode_sel'
 * '<S8>'   : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Stabilization/pitch _rate_control'
 * '<S9>'   : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Stabilization/roll _rate_control'
 * '<S10>'  : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Stabilization/roll_control '
 * '<S11>'  : 'SkyDog_Autopilot_discrete_integrators1/Skydog_autopilot/Stabilization/yaw _rate_control'
 */
#endif                                 /* RTW_HEADER_Skydog_autopilot_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
