/*
** TrapSat Header To Rule Them All
** 
** 
*/

#ifndef TRAPSAT_H_
#define TRAPSAT_H_

#include <Arduino.h>

#define TRAPSAT_COUNT_PATH "/trapsat_restarts.txt"
#define TRAPSAT_LOG_DIR "/mnt/sda1/trapsat/logs/" /* 21 chars*/
#define TRAPSAT_CAM_DIR_PATH "/mnt/sda1/trapsat/data/"
#define TRAPSAT_INT_TEMP_PIN 0
#define TRAPSAT_EXT_TEMP_PIN 1
#define TRAPSAT_NIC_SW_PIN 8
#define TRAPSAT_TEMP_VS 4.65
#define TRAPSAT_FLASH_PIN 2
#define GROUND_PIN 18		// analog input pin 4 -- ground
#define POWER_PIN 19		// analog input pin 5 -- voltage
#define X_PIN A3			// x-axis of the accelerometer
#define Y_PIN A2			// y-axis
#define Z_PIN A1			// z-axis (only on 3-axis models)


#define TRAPSAT_MSG_LEN 80

#endif  /* TRAPSAT_H_ */
