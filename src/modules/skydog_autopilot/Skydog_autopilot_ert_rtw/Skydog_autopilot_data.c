/*
 * File: Skydog_autopilot_data.c
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

/* Block parameters (auto storage) */
Parameters_Skydog_autopilot Skydog_autopilot_P = {
  1.0F,                                /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<S5>/Gain'
                                        */
  1.0F,                                /* Computed Parameter: Saturation1_UpperSat
                                        * Referenced by: '<S5>/Saturation1'
                                        */
  0.0F,                                /* Computed Parameter: Saturation1_LowerSat
                                        * Referenced by: '<S5>/Saturation1'
                                        */
  1.0F,                                /* Computed Parameter: Gain_Gain_k
                                        * Referenced by: '<S2>/Gain'
                                        */
  0.6F,                                /* Computed Parameter: Gain_Gain_l
                                        * Referenced by: '<S4>/Gain'
                                        */
  0.01F,                               /* Computed Parameter: DiscreteTimeIntegrator_gainval
                                        * Referenced by: '<S4>/Discrete-Time Integrator'
                                        */
  0.0F,                                /* Computed Parameter: DiscreteTimeIntegrator_IC
                                        * Referenced by: '<S4>/Discrete-Time Integrator'
                                        */
  0.52359879F,                         /* Computed Parameter: DiscreteTimeIntegrator_UpperSat
                                        * Referenced by: '<S4>/Discrete-Time Integrator'
                                        */
  -0.52359879F,                        /* Computed Parameter: DiscreteTimeIntegrator_LowerSat
                                        * Referenced by: '<S4>/Discrete-Time Integrator'
                                        */
  0.34906584F,                         /* Computed Parameter: Saturation1_UpperSat_j
                                        * Referenced by: '<S4>/Saturation1'
                                        */
  -0.34906584F,                        /* Computed Parameter: Saturation1_LowerSat_i
                                        * Referenced by: '<S4>/Saturation1'
                                        */
  0.34906584F,                         /* Computed Parameter: Saturation4_UpperSat
                                        * Referenced by: '<S3>/Saturation4'
                                        */
  -0.34906584F,                        /* Computed Parameter: Saturation4_LowerSat
                                        * Referenced by: '<S3>/Saturation4'
                                        */
  8.5F,                                /* Computed Parameter: Gain_Gain_b
                                        * Referenced by: '<S6>/Gain'
                                        */
  0.01F,                               /* Computed Parameter: DiscreteTimeIntegrator_gainva_d
                                        * Referenced by: '<S6>/Discrete-Time Integrator'
                                        */
  0.0F,                                /* Computed Parameter: DiscreteTimeIntegrator_IC_h
                                        * Referenced by: '<S6>/Discrete-Time Integrator'
                                        */
  4.5F,                                /* Computed Parameter: DiscreteTimeIntegrator_UpperS_o
                                        * Referenced by: '<S6>/Discrete-Time Integrator'
                                        */
  -4.5F,                               /* Computed Parameter: DiscreteTimeIntegrator_LowerS_a
                                        * Referenced by: '<S6>/Discrete-Time Integrator'
                                        */
  3.0F,                                /* Computed Parameter: Saturation1_UpperSat_k
                                        * Referenced by: '<S6>/Saturation1'
                                        */
  -3.0F,                               /* Computed Parameter: Saturation1_LowerSat_c
                                        * Referenced by: '<S6>/Saturation1'
                                        */
  -3.0F,                               /* Computed Parameter: Gain_Gain_p
                                        * Referenced by: '<S8>/Gain'
                                        */
  0.261799395F,                        /* Computed Parameter: Saturation1_UpperSat_i
                                        * Referenced by: '<S8>/Saturation1'
                                        */
  -0.261799395F,                       /* Computed Parameter: Saturation1_LowerSat_g
                                        * Referenced by: '<S8>/Saturation1'
                                        */
  0.261799395F,                        /* Computed Parameter: Saturation2_UpperSat
                                        * Referenced by: '<S3>/Saturation2'
                                        */
  -0.261799395F,                       /* Computed Parameter: Saturation2_LowerSat
                                        * Referenced by: '<S3>/Saturation2'
                                        */
  0.34906584F,                         /* Computed Parameter: Saturation5_UpperSat
                                        * Referenced by: '<S3>/Saturation5'
                                        */
  -0.34906584F,                        /* Computed Parameter: Saturation5_LowerSat
                                        * Referenced by: '<S3>/Saturation5'
                                        */
  8.0F,                                /* Computed Parameter: Gain_Gain_m
                                        * Referenced by: '<S10>/Gain'
                                        */
  0.01F,                               /* Computed Parameter: DiscreteTimeIntegrator_gainva_g
                                        * Referenced by: '<S10>/Discrete-Time Integrator'
                                        */
  0.0F,                                /* Computed Parameter: DiscreteTimeIntegrator_IC_i
                                        * Referenced by: '<S10>/Discrete-Time Integrator'
                                        */
  4.5F,                                /* Computed Parameter: DiscreteTimeIntegrator_UpperS_k
                                        * Referenced by: '<S10>/Discrete-Time Integrator'
                                        */
  -4.5F,                               /* Computed Parameter: DiscreteTimeIntegrator_LowerS_c
                                        * Referenced by: '<S10>/Discrete-Time Integrator'
                                        */
  3.0F,                                /* Computed Parameter: Saturation1_UpperSat_c
                                        * Referenced by: '<S10>/Saturation1'
                                        */
  -3.0F,                               /* Computed Parameter: Saturation1_LowerSat_m
                                        * Referenced by: '<S10>/Saturation1'
                                        */
  3.0F,                                /* Computed Parameter: Gain_Gain_e
                                        * Referenced by: '<S9>/Gain'
                                        */
  0.366519153F,                        /* Computed Parameter: Saturation1_UpperSat_b
                                        * Referenced by: '<S9>/Saturation1'
                                        */
  -0.366519153F,                       /* Computed Parameter: Saturation1_LowerSat_ij
                                        * Referenced by: '<S9>/Saturation1'
                                        */
  0.366519153F,                        /* Computed Parameter: Saturation_UpperSat
                                        * Referenced by: '<S3>/Saturation'
                                        */
  -0.366519153F,                       /* Computed Parameter: Saturation_LowerSat
                                        * Referenced by: '<S3>/Saturation'
                                        */
  1.0F,                                /* Computed Parameter: Saturation6_UpperSat
                                        * Referenced by: '<S3>/Saturation6'
                                        */
  -1.0F,                               /* Computed Parameter: Saturation6_LowerSat
                                        * Referenced by: '<S3>/Saturation6'
                                        */
  0.01F,                               /* Computed Parameter: Gain_Gain_ba
                                        * Referenced by: '<S11>/Gain'
                                        */
  0.01F,                               /* Computed Parameter: DiscreteTimeIntegrator_gainva_f
                                        * Referenced by: '<S11>/Discrete-Time Integrator'
                                        */
  0.0F,                                /* Computed Parameter: DiscreteTimeIntegrator_IC_k
                                        * Referenced by: '<S11>/Discrete-Time Integrator'
                                        */
  0.366519153F,                        /* Computed Parameter: DiscreteTimeIntegrator_UpperS_a
                                        * Referenced by: '<S11>/Discrete-Time Integrator'
                                        */
  -0.366519153F,                       /* Computed Parameter: DiscreteTimeIntegrator_LowerS_l
                                        * Referenced by: '<S11>/Discrete-Time Integrator'
                                        */
  0.244346097F,                        /* Computed Parameter: Saturation1_UpperSat_jh
                                        * Referenced by: '<S11>/Saturation1'
                                        */
  -0.244346097F,                       /* Computed Parameter: Saturation1_LowerSat_cj
                                        * Referenced by: '<S11>/Saturation1'
                                        */
  0.244346097F,                        /* Computed Parameter: Saturation1_UpperSat_e
                                        * Referenced by: '<S3>/Saturation1'
                                        */
  -0.244346097F,                       /* Computed Parameter: Saturation1_LowerSat_ga
                                        * Referenced by: '<S3>/Saturation1'
                                        */
  0.01F,                               /* Computed Parameter: DiscreteTimeIntegrator_gainva_h
                                        * Referenced by: '<S5>/Discrete-Time Integrator'
                                        */
  0.0F,                                /* Computed Parameter: DiscreteTimeIntegrator_IC_m
                                        * Referenced by: '<S5>/Discrete-Time Integrator'
                                        */
  0.52359879F,                         /* Computed Parameter: DiscreteTimeIntegrator_UpperS_b
                                        * Referenced by: '<S5>/Discrete-Time Integrator'
                                        */
  -0.52359879F,                        /* Computed Parameter: DiscreteTimeIntegrator_LowerS_m
                                        * Referenced by: '<S5>/Discrete-Time Integrator'
                                        */
  1.0F,                                /* Computed Parameter: Saturation3_UpperSat
                                        * Referenced by: '<S3>/Saturation3'
                                        */
  0.0F,                                /* Computed Parameter: Saturation3_LowerSat
                                        * Referenced by: '<S3>/Saturation3'
                                        */
  1.0F,                                /* Computed Parameter: Gain_Gain_p1
                                        * Referenced by: '<S1>/Gain'
                                        */
  1.0F,                                /* Computed Parameter: Gain1_Gain
                                        * Referenced by: '<S1>/Gain1'
                                        */
  0.3F,                                /* Computed Parameter: Gain1_Gain_n
                                        * Referenced by: '<S4>/Gain1'
                                        */
  0.0F,                                /* Computed Parameter: Gain1_Gain_j
                                        * Referenced by: '<S5>/Gain1'
                                        */
  8.0F,                                /* Computed Parameter: Gain1_Gain_k
                                        * Referenced by: '<S6>/Gain1'
                                        */
  5.0F,                                /* Computed Parameter: Gain1_Gain_e
                                        * Referenced by: '<S10>/Gain1'
                                        */
  0.0F,                                /* Computed Parameter: Gain1_Gain_m
                                        * Referenced by: '<S11>/Gain1'
                                        */
  1,                                   /* Computed Parameter: Switch1_Threshold
                                        * Referenced by: '<S7>/Switch1'
                                        */
  1,                                   /* Computed Parameter: Switch_Threshold
                                        * Referenced by: '<S7>/Switch'
                                        */
  1,                                   /* Computed Parameter: Switch2_Threshold
                                        * Referenced by: '<S7>/Switch2'
                                        */
  1                                    /* Computed Parameter: Switch3_Threshold
                                        * Referenced by: '<S7>/Switch3'
                                        */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
