#include "QuadCopterConfig.h"

/* Connection methods of Ultrasonic */
#define ULT_USE_UART2	0
#define ULT_USE_PWM		1 
// if you wanna use us100 via PWM, remember to initialize the pins with "us100_config()" first.

Ultrasonic_t Ultrasonic = {
	.lenHigh = 0,
	.lenLow = 0,
	.d = 0
};

#if ULT_USE_PWM	
	uint32_t parameter_ctr2sec = 3464/9000000;	
	// speed of sound in cm / 90 MHz = (331.4+25*0.6)*100 / 90 MHz
#endif

/*==============================================================================================
/*==============================================================================================
** function : us100_config
** usage : PWM configuration and parameter setting for us100
** input : -
** output : -
** form : us100_config();
==============================================================================================*/
/*============================================================================================*/
#if ULT_USE_PWM
void us100_config(){

	/* TIM8_CH2 PWM7 PC6 (MODE_OUT) */   /* TIM8_CH3 PWM6 PC7 (MODE_IN) */

	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	uint16_t PrescalerValue = (uint16_t)((SystemCoreClock / 4) / 1000000) - 1;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* Connect TIM pin to AF2 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Timer Configuration */
	TIM_DeInit(TIM8);
	TIM_TimeBaseStruct.TIM_Period = 0xFFFF;        // 週期 = 2.5ms, 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = 0;             // 除頻84 = 1M ( 1us )
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // 上數
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStruct);
	TIM_PrescalerConfig(TIM8, PrescalerValue, TIM_PSCReloadMode_Immediate);

	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_Pulse = 0;

	TIM_OC2Init(TIM8, &TIM_OCInitStruct); //設定Channel2
	TIM_OC3Init(TIM8, &TIM_OCInitStruct); //設定Channel3

	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM8, ENABLE);

	/* TIM enable counter */
	TIM_Cmd(TIM8, ENABLE);

	// TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	// TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;	// 0x000C, so f=90MHz
	// TIM_ICInitStructure.TIM_ICFilter = 0x0;
	// TIM_ICInit(TIM8, &TIM_ICInitStructure);			// for PC7 TIM8_CH2
	// TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	// TIM_ICInit(TIM8, &TIM_ICInitStructure);			// for PC6 TIM8_CH3

	// NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY - 2;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);

	// TIM_ITConfig(TIM8, TIM_IT_CC2, ENABLE);
	// TIM_ITConfig(TIM8, TIM_IT_CC3, ENABLE);
}
#endif

/*==============================================================================================*/
/*==============================================================================================*
**函數 : us100_distant
**功能 : get 1 calculated distant data from the data received by USART 
**輸入 : Ultrasonic.lenHigh, Ultrasonic.lenLow 
**輸出 : Ultrasonic.d (mm)
**使用 : print_us100_distant();
**==============================================================================================*/
/*==============================================================================================*/
void print_us100_distance(){

#if ULT_USE_UART2

	serial2.putc('U');
	vTaskDelay(70);
	 
	Ultrasonic.lenHigh = serial2.getc();
	Ultrasonic.lenLow = serial2.getc();
	Ultrasonic.d = (Ultrasonic.lenHigh*256 + Ultrasonic.lenLow)*0.1;

	system.variable[Dis].value = Ultrasonic.d;

//	serial.printf("Distance: ");
//	serial.printf("%d",Ultrasonic.d);
//	serial.printf(" cm\n\r");
	vTaskDelay(30);
#endif

#if ULT_USE_PWM

	/* TIM8_CH2 PWM7 PC6 (MODE_OUT) */   /* TIM8_CH3 PWM6 PC7 (MODE_IN) */

	uint16_t counter=0;

	GPIO_WriteBit(GPIOC, GPIO_Pin_7, DISABLE);
	vTaskDelay(2);
	GPIO_WriteBit(GPIOC, GPIO_Pin_7, ENABLE);
	vTaskDelay(50);		
	// send a pulse which >=5us , tell the sensor to start surveying
	GPIO_WriteBit(GPIOC, GPIO_Pin_7, DISABLE);
	
	while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0);

	counter=TIM_GetCounter(TIM8);

	while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 1);

	counter=TIM_GetCounter(TIM8)-counter;
	// record the length of pulse

	if (counter < 0){	
	// while this function run into the counter of timer reset, the failure occur, so restart the whole function again.
		counter=counter+65535;
	}

	Ultrasonic.d = counter * parameter_ctr2sec;
	system.variable[Dis].value = Ultrasonic.d;

#endif
}
