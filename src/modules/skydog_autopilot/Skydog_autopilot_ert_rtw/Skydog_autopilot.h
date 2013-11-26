/* 
 * File: Skydog_autopilot.h
 *  
 * Code generated for Simulink model 'Skydog_autopilot'.
 * 
 * Model version                  : 1.121
 * Simulink Coder version         : 8.1 (R2011b) 08-Jul-2011
 * TLC version                    : 8.1 (Jul  9 2011)
 * C/C++ source code generated on : Sun Nov 24 22:56:20 2013
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
  #endif /* Skydog_autopilot_COMMON_INCLUDES_ */
  

    #include "Skydog_autopilot_types.h"    
/* Includes for objects with custom storage classes. */
#include "SkydogSignals.h"





    /* Macros for accessing real-time model data structure */
    

    #ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm) ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val) ((rtm)->errorStatus = (val))
#endif


  
    
    
      






    
    
    
      /* External inputs (root inport signals with auto storage) */
                typedef struct {
            
        
        
	   real32_T Roll_acc_r; 	/* '<Root>/Roll_acc_r' */
          } ExternalInputs_Skydog_autopilot;
      
      
        /* Real-time Model Data Structure */
      struct RT_MODEL_Skydog_autopilot {
        const char_T * volatile errorStatus;

    };

      
        
          




            
        /* External inputs (root inport signals with auto storage) */
                  extern ExternalInputs_Skydog_autopilot Skydog_autopilot_U;



    


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
   * hilite_system('SkyDog_Autopilot_template/Skydog_autopilot')    - opens subsystem SkyDog_Autopilot_template/Skydog_autopilot
   * hilite_system('SkyDog_Autopilot_template/Skydog_autopilot/Kp') - opens and selects block Kp
   *
   * Here is the system hierarchy for this model
   *
    * '<Root>' : 'SkyDog_Autopilot_template'
      * '<S1>'   : 'SkyDog_Autopilot_template/Skydog_autopilot'
      * '<S2>'   : 'SkyDog_Autopilot_template/Skydog_autopilot/Autopilot'
      * '<S3>'   : 'SkyDog_Autopilot_template/Skydog_autopilot/Stabilization'
   */



  #endif /* RTW_HEADER_Skydog_autopilot_h_ */

/* 
 * File trailer for generated code.
 * 
 * [EOF]
 */

