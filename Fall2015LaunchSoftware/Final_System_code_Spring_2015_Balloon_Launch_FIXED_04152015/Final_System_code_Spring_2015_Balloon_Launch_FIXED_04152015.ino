

/*
** System Test INO v Fall Launch
*/

#include <Bridge.h>
#include <FileIO.h>
#include <Arduino.h>
#include <TrapSat.h>
#include <TempSensor.h>
#include <TrapCam.h>
#include <AltSoftSerial.h>
#include <VC0706_AltSerial_V2.h> 



/*
** Global Variables
*/
int NumOfRestarts;
bool fired;

void setup() {
  Serial.begin(9600);
    //while (!Serial);  // Final Code -- no serial Terminal!
    //Serial.println("Moving on.\n");
  Bridge.begin();
  FileSystem.begin();
  
  /*
  ** Two Minute Delay
  ** Giving the Linux Processor Time to Boot
  */
  delay(120000);
  
  /*
  ** Check For System Restart And Adjust
  */
  makeNumStarts();
  incNumStarts();
  NumOfRestarts = getNumStarts();
  
  char startMsg[TRAPSAT_MSG_LEN] = "";
  sprintf(startMsg, "System Start (%d)\n", getNumStarts());
  startMsg[TRAPSAT_MSG_LEN - 1] = '\0';
  runLogEvent(startMsg);
  fired = false;
  //pinMode(TRAPSAT_FLASH_PIN, OUTPUT);  //No Flash this time
}

void loop() {
  /*
  ** Initialize Temperature Sensors
  */
  TempSensor IntTempSensor(TRAPSAT_INT_TEMP_PIN, TRAPSAT_TEMP_VS);
  TempSensor ExtTempSensor(TRAPSAT_EXT_TEMP_PIN, TRAPSAT_TEMP_VS);
  
  /*
  ** Initialize Camera
  */
  TrapCam Camera;
  //Camera.clearSerLog(); /* For Launch, removes debugging flag that enables Serial */
  
  /*
  ** Clear the Temperature Buffer
  */
  char tempMessage[TRAPSAT_MSG_LEN];
  memset(tempMessage, 0, sizeof(tempMessage));
  
  while(millis() < 18000000) /* 5 Hours */
  {
    /*
    ** Create File Name
    */
    String campath = String(TRAPSAT_CAM_DIR_PATH) + String(NumOfRestarts) + "_" + Camera.getTimeStamp() + ".jpeg"; 
    Serial.println(campath);
    
    /*
    ** Turn Flash On
    */
    //digitalWrite(TRAPSAT_FLASH_PIN, HIGH);   // No Flash this time
    
    /*
    ** Call Camera Function
    */
    if( Camera.makePic(campath) )
    {
        runLogEvent("Picture Captured\n");
    }
    else
    {
        runLogError("Camera Error: makePic() Returned False\n");
    }
    
    /*
    ** Turn Flash Off
    */
    //digitalWrite(TRAPSAT_FLASH_PIN, LOW); // No Flash this time
    
    /*
    ** Take Temperature Readings
    */
    sprintf(tempMessage, "Internal Temp: %s C\n", String(IntTempSensor.getTempC()).c_str());
    tempMessage[TRAPSAT_MSG_LEN - 1] = '\0';
    runLogEvent(tempMessage);
    Serial.println(tempMessage);
    
    sprintf(tempMessage, "Internal Temp Vs: %s V\n", String(IntTempSensor.getVoltOut()).c_str());
    tempMessage[TRAPSAT_MSG_LEN - 1] = '\0';
    runLogEvent(tempMessage);
    Serial.println(tempMessage);
    
    sprintf(tempMessage, "External Temp: %s C\n", String(ExtTempSensor.getTempC()).c_str());
    tempMessage[TRAPSAT_MSG_LEN - 1] = '\0';
    runLogEvent(tempMessage);
    Serial.println(tempMessage);
    
    sprintf(tempMessage, "External Temp Vs: %s V\n", String(ExtTempSensor.getVoltOut()).c_str());
    tempMessage[TRAPSAT_MSG_LEN - 1] = '\0';
    runLogEvent(tempMessage);
    Serial.println(tempMessage);
    
    if((millis() > (130 * 60000)) && !fired) /* 130 minutes */
    {
        Serial.println("Firing NICHROME!");
        fireItUp();
    }
    
    delay(1000); /* One Second delay */
  }
  
  //Serial.println("Test Finished!");
  runLogEvent("System Finished!\n");
  
  /*
  ** HOLD
  */
  while(1);
}
