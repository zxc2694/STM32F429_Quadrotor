#ifndef __QCOPTER_CTRL_H
#define __QCOPTER_CTRL_H

#include "stm32f4xx.h"
#include "QuadCopterConfig.h"


#define MIN_PWM_INPUT 1000
#define MAX_PWM_INPUT 5000

#if USE_FUTABA_CONTROLLER
#define MAX_PWM1_INPUT 3643.0
#define MID_PWM1_INPUT 2847.0
#define MIN_PWM1_INPUT 2052.0

#define MAX_PWM2_INPUT 3643.0
#define MID_PWM2_INPUT 2847.0
#define MIN_PWM2_INPUT 2052.0

#define MAX_PWM3_INPUT 3684.0
#define MID_PWM3_INPUT 2884.0
#define MIN_PWM3_INPUT 2084.0

#define MAX_PWM4_INPUT 3684.0
#define MID_PWM4_INPUT 2879.0
#define MIN_PWM4_INPUT 2075.0

#define MAX_PWM5_INPUT 3682.0
#define MIN_PWM5_INPUT 2077.0
#endif

#if USE_WFLY_CONTROLLER
#define MAX_PWM1_INPUT 3641.0
#define MID_PWM1_INPUT 2847.0
#define MIN_PWM1_INPUT 2054.0

#define MAX_PWM2_INPUT 3641.0
#define MID_PWM2_INPUT 2847.0
#define MIN_PWM2_INPUT 2054.0

#define MAX_PWM3_INPUT 3681.0
#define MIN_PWM3_INPUT 2086.0

#define MAX_PWM4_INPUT 3682.0
#define MID_PWM4_INPUT 2879.0
#define MIN_PWM4_INPUT 2077.0

#define MAX_PWM5_INPUT 3682.0
#define MIN_PWM5_INPUT 2077.0
#endif

#if USE_DEVO_v10_CONTROLLER
#define MAX_PWM1_INPUT 3574.0
#define MID_PWM1_INPUT 2800.0 	// AILE // Roll
#define MIN_PWM1_INPUT 2054.0

#define MAX_PWM2_INPUT 3543.0
#define MID_PWM2_INPUT 2853.0 	// ELEV // Pitch
#define MIN_PWM2_INPUT 2050.0

#define MAX_PWM3_INPUT 3583.0
#define MIN_PWM3_INPUT 2077.0 	// THRO // Thrust

#define MAX_PWM4_INPUT 3591.0
#define MID_PWM4_INPUT 2836.0 	// RUDD // Yaw
#define MIN_PWM4_INPUT 2081.0

#define MAX_PWM5_INPUT 3587.0 	
#define MIN_PWM5_INPUT 2076.0 	// GEAR // Protect Switch
#endif

/*
	          left hand                          right hand
           _______________                     _______________
          /       ↑       \                   /       ↑       \
         /     PWM2(Pitch) \                 /    PWM3(Thrust) \    
        /         |         \               /         |         \
       /          |          \             /          |          \
       <<---------+--------->> PWM4(Yaw)   <<---------+--------->> PWM1(Roll) 
       \          |          /             \          |          /
        \         |         /               \         |         /
         \        |        /                 \        |        /
          \______ ↓ ______/                   \______ ↓ ______/
*/

#define MAX_CTRL_ROLL 45
#define MIN_CTRL_ROLL -45

#define MAX_CTRL_PITCH 45
#define MIN_CTRL_PITCH -45

#define MAX_CTRL_YAW 180
#define MIN_CTRL_YAW -180
typedef enum RC_State {
	GET_SIGNAL = 0x00,
	NO_SIGNAL = 0x01
} RC_State;
#define ENGINE_OFF 1
#define ENGINE_ON 0

void Update_RC_Control(int16_t *Roll, int16_t  *Pitch, int16_t  *Yaw, int16_t  *Thr, int16_t *safety); 
RC_State remote_signal_check();

extern vs16 PWM_M1;
extern vs16 PWM_M2;
extern vs16 PWM_M3;
extern vs16 PWM_M4;

#endif
