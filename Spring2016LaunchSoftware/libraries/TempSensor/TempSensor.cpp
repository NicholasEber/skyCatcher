#include "TempSensor.h"

TempSensor::TempSensor()
{
	setPin(0);
	setVoltSource(DEFAULT_VOLT_SOURCE);
}

TempSensor::TempSensor(int p)
{
	setPin(p);
	setVoltSource(DEFAULT_VOLT_SOURCE);
}

TempSensor::TempSensor(int p, float v)
{
	setPin(p);
	setVoltSource(v);
}

void TempSensor::setPin(int p)
{
	if(p >= MIN_PIN_NUM && p <= MAX_PIN_NUM)
	{
		myPin = p;
	}
	else
	{
		myPin = DEFAULT_PIN;
	}
	Serial.println("My Pin Set To "+String(myPin));
}

void TempSensor::setVoltSource(float v)
{
	if(v >= MIN_VOLT_SOURCE && v <= MAX_VOLT_SOURCE)
	{
		myVs = v;
	}
	else
	{
		myVs = DEFAULT_VOLT_SOURCE;
	}
	Serial.println("My Vs Set To "+String(myVs));
}

float TempSensor::getVoltOut()
{
	//int reading = analogRead(myPin);
	//float voltage = reading * myVs;
	//voltage = voltage / 1024.0;
	//return voltage;
	
	return ((analogRead(myPin) * myVs) / 1024.0);
}

float TempSensor::getTempC()
{
    //int reading = analogRead(myPin);
    //float voltage = reading * myVs;
    //voltage = voltage / 1024.0;
    /* 
    ** converting from 10 mv per degree with 500 mV offset
    ** to degrees ((voltage - 500mV) times 100)
    */
    //float temperatureC = (voltage - 0.5) * 100 ;
    //return temperatureC;
    return (((analogRead(myPin) * myVs / 1024.0) - 0.5) * 100);
}

float TempSensor::getTempF()
{
	/*
	** Return:
	** Call getTempC() and immediately convert it to F
	*/
    return ((getTempC() * 9.0 / 5.0) + 32.0);
}
