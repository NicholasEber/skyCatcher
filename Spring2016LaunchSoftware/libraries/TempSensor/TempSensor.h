#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include <Arduino.h>

#define MIN_PIN_NUM 0
#define MAX_PIN_NUM 5
#define MIN_VOLT_SOURCE 0.0
#define MAX_VOLT_SOURCE 10.0

#define DEFAULT_PIN 0
#define DEFAULT_VOLT_SOURCE 4.4

class TempSensor
{
public:
	TempSensor();
	TempSensor(int p);
	TempSensor(int p, float v);
	void setVoltSource(float v);
	float getVoltOut();
	float getTempC();
	float getTempF();

private:
	int myPin;
	int myVs;
private:
	void setPin(int p);
};

#endif

/*
My Time: 22:45
Yun Time: 00:02:13 EDT 2011


date +"%H:%I:%S"
return: 00:12:54
date +"%H%I%S"
return: 001259

My Time: 22:56
Thu Sep  8 00:13:33 EDT 2011

*/