/*==============================================================================================*/
/*==============================================================================================*/
#include "QuadCopterConfig.h"

/* Connection methods of Ultrasonic */
#define ULT_USE_UART2	1
#define ULT_USE_PWM		0

Ultrasonic_t Ultrasonic = {
	.lenHigh = 0,
	.lenLow = 0,
	.d = 0
};

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

	serial.printf("Distance: ");
	serial.printf("%d",Ultrasonic.d);
	serial.printf(" cm\n\r");
	vTaskDelay(30);
#endif

}
