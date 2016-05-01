
/*
** Spring 2016 Launch
*/
#include <Bridge.h>
#include <FileIO.h>
#include <Arduino.h>
#include <TrapSat.h>
#include <Servo.h>
#include <Sensor.h>
#include <Adafruit_BMP183.h>

void makeNumStarts();
/*
** Global Variables
*/
int NumOfRestarts;
bool OPEN;
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA; // should be ~1000

/*
** initialize the hardware
*/
Servo myservo;  // create servo object to control a servo
Adafruit_BMP183 bmp = Adafruit_BMP183(10, 11, 12, 13); 

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Final Code -- no serial Terminal!
  Serial.println("Moving on.\n");
  Bridge.begin(115200);
  FileSystem.begin();
    
  /*
  ** Two Minute Delay
  ** Giving the Linux Processor Time to Boot
  */
  delay(120000);
  Serial.println("Linux Processor Booted.\n");
  
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
  OPEN = false;
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
  Serial.println("Ending startup.\n");
}

void loop() {
  
  /*
  ** Clear the Altitude Buffer
  */
  char altiMessage[TRAPSAT_MSG_LEN];
  memset(altiMessage, 0, sizeof(altiMessage));

  /*
  ** get current Altitude
  */
  float alti= bmp.getAltitude(seaLevelPressure);
  
  /*
  ** Call Motor to open
  */
  Serial.println("Check to open.\n");  
  if(alti > 30480 && !OPEN) /*100000 feet*/
   {
    // step one revolution in one direction:
    Serial.println("Opening Lids");

    
    for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    
    sprintf(altiMessage, "Altitude: %s feet: Opening Lids\n", String((alti*3.28084)).c_str());
    altiMessage[TRAPSAT_MSG_LEN - 1] = '\0';
    runLogEvent(altiMessage);
    
    Serial.println(altiMessage); 
    OPEN= true;
    delay(500);
   }
   
    /*
    ** Call Motor to close
    */
   Serial.println("Check closing.\n");
   if(bmp.getAltitude(seaLevelPressure) < 30480 && OPEN) /*100000 feet*/
   {
    // step revolution direction:
    Serial.println("Closing Lids");
    
    for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    } 
  
    sprintf(altiMessage, "Altitude: %s feet: Closing Lids\n", String((alti*3.28084)).c_str());
    altiMessage[TRAPSAT_MSG_LEN - 1] = '\0';
    runLogEvent(altiMessage);

    OPEN= false;
   }

   Serial.println("start log.\n");
    sprintf(altiMessage, "Altitude: %s feet\n", String((alti*3.28084)).c_str());
    altiMessage[TRAPSAT_MSG_LEN - 1] = '\0';
    runLogEvent(altiMessage);
    Serial.println(altiMessage);    
  
    
    /*
    ** Take Temperature Readings
    
    sprintf(tempMessage, "Internal Temp: %s C\n", String(IntTempSensor.getTempC()).c_str());
    tempMessage[TRAPSAT_MSG_LEN - 1] = '\0';
    runLogEvent(tempMessage);
    Serial.println(tempMessage);
    
    */


    
 //runLogEvent("System Finished!\n");
  delay(5000);
}
