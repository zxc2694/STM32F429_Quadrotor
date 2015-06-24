/*==============================================================================================*/
/*==============================================================================================*/
#include "QuadCopterConfig.h"

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
**使用 : us100_distant();
**==============================================================================================*/
/*==============================================================================================*/
void us100_distant(){

	//reading data
	serial2.putc('U');
	vTaskDelay(500);

	serial2.putc('U');
	vTaskDelay(500);
	serial.putc('1');

	//calculating the distance
	//if(serial2.getc()){
		Ultrasonic.lenHigh = serial2.getc();
		serial.putc('2');
		Ultrasonic.lenLow = serial2.getc();
		serial.putc('3');
		Ultrasonic.d = Ultrasonic.lenHigh*256 + Ultrasonic.lenLow;
	//}
}
