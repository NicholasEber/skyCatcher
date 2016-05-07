
/*
** Spring 2016 Launch
*/
#include <Bridge.h>
#include <FileIO.h>
#include <Arduino.h>
#include <TrapSat.h>
#include <TempSensor.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP183.h>

void makeNumStarts();
/*
** Global Variables
*/
int NumOfRestarts;
int lStat = 0;         // variable for reading the pushbutton status
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA; // should be ~1000


/*
** initialize the hardware
*/
Servo lidServo;  // create servo object to control a servo to open lid
Servo lockServo; // controls the servo to lock the lid
Adafruit_BMP183 bmp = Adafruit_BMP183(10, 11, 12, 13); //altimieter
TempSensor IntTempSensor(TRAPSAT_INT_TEMP_PIN, TRAPSAT_TEMP_VS);
TempSensor ExtTempSensor(TRAPSAT_EXT_TEMP_PIN, TRAPSAT_TEMP_VS);

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Final Code -- no serial Terminal!
  Serial.println("Moving on.\n");
  Bridge.begin(115200);
  FileSystem.begin();

  pinMode(LID_MOTOR_PIN, INPUT);
  pinMode(LOCK_MOTOR_PIN, INPUT);
  pinMode(BUTTON_PIN,INPUT);
  lidServo.attach(LID_MOTOR_PIN);  // attaches the servo on pin 9 to the servo object
  lidServo.attach(LOCK_MOTOR_PIN);  // attaches the servo on pin 10 to the servo object

  /*
  ** unlocking and locking lid geting ready to fly.
  ** 15 sec after power to close lid and lock
  */
  Serial.println("un-locking lid.");
  lockServo.write(0);
  delay(15000);
  Serial.println("Locking lid.");
  lockServo.write(180);
   
  /*
  ** Two Minute Delay
  ** Giving the Linux Processor Time to Boot
  */
  delay(12000);
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
  
  Serial.println("Ending startup.\n");
}

void loop() {

  TempSensor IntTempSensor(TRAPSAT_INT_TEMP_PIN, TRAPSAT_TEMP_VS);
  TempSensor ExtTempSensor(TRAPSAT_EXT_TEMP_PIN, TRAPSAT_TEMP_VS);
  /*
  ** Clear the Altitude Buffer
  */
  char altiMessage[TRAPSAT_MSG_LEN];
  memset(altiMessage, 0, sizeof(altiMessage));
  
  /*
  ** get info
  */
  lStat = digitalRead(BUTTON_PIN);// 0=OPEN, 1=Close
  float alti= bmp.getAltitude(seaLevelPressure);
  float pres= bmp.getPressure();
  float iTemp= IntTempSensor.getTempC();
  float eTemp= ExtTempSensor.getTempC();
  int xpin= analogRead(X_PIN);
  int ypin= analogRead(Y_PIN);
  int zpin= analogRead(Z_PIN);
  
  
  /*
  ** Call Motor to open  -  need to change position based on lid
  */
  Serial.println("Check to open.\n");  
  if(/*(alti > 30480) /*|| ?time?*/ lStat==LOW) /*100000 feet*/
   {
    // step one revolution in one direction:
    Serial.println("Opening Lids");
    /*for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree*/
      lockServo.write(0);
      delay(1500);                       // waits 1.5 for the servo to reach the position
      lidServo.write(0);              // tell servo to go to position in variable 'pos'

    
    
   }
   
    /*
    ** Call Motor to close  -  need to change position based on lid
    */
   Serial.println("Check closing.\n");
   if(/*(alti < 30480) /*|| ?time?*/ lStat==HIGH) /*100000 feet*/
   {
    // step revolution direction:
    Serial.println("Closing Lids");
    /*for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees*/
 
      lidServo.write(180);              // tell servo to go to position in variable 'pos'                
      delay(1000);                     // waits 10sec for the servo to reach the position
      lockServo.write(0);
   }

    Serial.println("start log.\n");
    //recored: time, Altitude,Pressure, Internal Temp, External Temp, Xpin, Ypin, Zpin, Lid status
    sprintf(altiMessage, ": %s, %s, %s, %s, %s, %s, %s, %s\n", String(alti).c_str(),
                                           String(pres).c_str(),String(iTemp).c_str(),
                                           String(eTemp).c_str(),String(xpin).c_str(),
                                           String(ypin).c_str(),String(zpin).c_str(),
                                           String(lStat).c_str());
    altiMessage[TRAPSAT_MSG_LEN - 1] = '\0';
    runLogEvent(altiMessage);
    Serial.println(altiMessage);
    float volt= IntTempSensor.getVoltOut();
    Serial.println(volt);  
  /*
  // accelerometer 
  // print the sensor values:
  Serial.print(analogRead(X_PIN));
  // print a tab between values:
  Serial.print("\t");
  Serial.print(analogRead(Y_PIN));
  // print a tab between values:
  Serial.print("\t");
  Serial.print(analogRead(Z_PIN));
  Serial.println();
  // delay before next reading:
  delay(100);
  */

    
 //runLogEvent("System Finished!\n");
  delay(4000);
}
