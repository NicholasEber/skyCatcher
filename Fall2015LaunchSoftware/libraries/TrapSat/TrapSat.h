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


#define TRAPSAT_MSG_LEN 80

#endif  /* TRAPSAT_H_ */
