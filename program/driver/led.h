#ifndef __MODULE_BOARD_S_H
#define __MODULE_BOARD_S_H

#include "stm32f4xx.h"

#define ENABLE 1
#define DISABLE 0

#if (Use_QCopter_board || Use_stm32f407_board)

#define KEY   PBI(2)
#define LED_R 15
#define LED_G 14
#define LED_B 13
	
#endif

#if Use_stm32f429_board

#define LED_R 14
#define LED_G 13
#define LED_B 12
	
#endif

void LED_Config(void);
void SetLED(int LED, int state);
void LED_Toggle(int LED);

#endif

