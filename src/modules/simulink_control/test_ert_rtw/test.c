/*
 * File: test.c
 *
 * Code generated for Simulink model 'test'.
 *
 * Model version                  : 1.7
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Tue Oct 22 22:50:12 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "test.h"
#include "test_private.h"

/* Exported data definition */

/* Definition for custom storage class: ExportToFile */
real_T inVar;
real_T outVar;

/* Real-time model */
RT_MODEL_test test_M_;
RT_MODEL_test *const test_M = &test_M_;

/* Model step function */
void test_step(void)
{
  /* Sum: '<S1>/Sum' incorporates:
   *  Constant: '<S1>/Constant'
   *  Gain: '<S1>/Gain'
   *  Inport: '<Root>/In1'
   */
  outVar = test_P.Gain_Gain * inVar + test_P.Constant_Value;
}

/* Model initialize function */
void test_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(test_M, (NULL));

  /* block I/O */

  /* custom signals */
  outVar = 0.0;

  /* external inputs */
  inVar = 0.0;
}

/* Model terminate function */
void test_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
