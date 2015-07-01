#include "QuadCopterConfig.h"

void LED_Config(void)
{
	
#if Use_QCopter_board
	GPIO_InitTypeDef GPIO_InitStruct = {
		/* LED_R PC13 */  /* LED_G PC14 */  /* LED_B PC15 */
		.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_PuPd = GPIO_PuPd_NOPULL
	};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif

#if Use_stm32f407_board 	
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_OType =GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Init(GPIOD, &GPIO_InitStruct);
#endif

#if Use_stm32f429_board
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_OType =GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_Init(GPIOG, &GPIO_InitStruct);
#endif



	SetLED(LED_G, ENABLE);
	SetLED(LED_R, ENABLE);
	SetLED(LED_B, ENABLE);
}

void SetLED(int LED, int state)
{

#if Use_QCopter_board
	PCO(LED) = state;
#endif

#if Use_stm32f407_board 	
	PDO(LED) = state;	
#endif

#if Use_stm32f429_board
	PGO(LED) = state;	
#endif

}

void LED_Toggle(int LED)
{

#if Use_QCopter_board
	PCO(LED) = ~PCO(LED);
#endif

#if Use_stm32f407_board 	
	PDO(LED) = ~PDO(LED);
#endif

#if Use_stm32f429_board
	PGO(LED) = ~PGO(LED);
#endif
}
