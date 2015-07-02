#ifndef __IMU_CORRECT_H
#define __IMU_CORRECT_H

#include "stm32f4_system.h"

#define CONTROLLER_DISABLE 0
#define CONTROLLER_ENABLE 1

void sensor_read();
void correct_sensor();
void AHRS_and_RC_updata(int16_t *Thr, int16_t *Pitch, int16_t *Roll, int16_t *Yaw, int16_t *safety);
void PID_vertical_Zd();
#endif
