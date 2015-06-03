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

	serial.putc('U');
	serial2.putc('1');
	//vTaskDelay(500);

	//calculating the distance
	//if(serial2.getc()){
		Ultrasonic.lenHigh = serial.getc();
		serial2.putc('2');
		Ultrasonic.lenLow = serial.getc();
		serial2.putc('3');
		Ultrasonic.d = Ultrasonic.lenHigh*256 + Ultrasonic.lenLow;
	//}
}
