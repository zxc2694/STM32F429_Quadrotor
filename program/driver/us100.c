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
	float parameter_pwm2dis = 1;	
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

	// PA1 GPIO config ---------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_1,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_Speed = GPIO_Speed_2MHz,
		.GPIO_OType =GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA0 PWM config (TIM2 ch1)--------------------------------------
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	uint16_t PrescalerValue = (uint16_t)((SystemCoreClock / 4) / 1000000) - 1;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* TIM2 PWM6  PA0 (Ultrasonic In) */ /* GPIO PA1 (Ultrasonic Out) */  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect TIM pin to AF2 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM1);

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY - 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStruct.TIM_Period = 0xFFFF;              // 週期 = 2.5ms, 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = 0;             // 除頻84 = 1M ( 1us )
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // 上數
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
	TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;

	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	TIM_Cmd(TIM2, ENABLE);
	/* Enable the Interrupt Request */
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

}
#endif

/*==============================================================================================*/
/*==============================================================================================*
**函數 : us100_distant
**功能 : get 1 calculated distant data from the data received by USART / PWM
**輸入 : Ultrasonic.lenHigh, Ultrasonic.lenLow / system.variable[Dis].value
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

	vTaskDelay(30);
#endif

#if ULT_USE_PWM

	GPIO_WriteBit(GPIOA, GPIO_Pin_1, DISABLE);
	vTaskDelay(2);
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, ENABLE);
	vTaskDelay(20);
	// send a pulse which >=5us , tell the sensor to start surveying
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, DISABLE);

	Ultrasonic.d = system.variable[Dis].value * parameter_pwm2dis ;

#endif
}
