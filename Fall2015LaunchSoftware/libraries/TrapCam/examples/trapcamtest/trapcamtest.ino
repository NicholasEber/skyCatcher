#include <FileIO.h>
/*#include <Process.h>
#include <YunServer.h>
#include <HttpClient.h>
#include <YunClient.h>
#include <Bridge.h>
#include <Console.h>
*/
#include <AltSoftSerial.h>

#ifndef VC0706_ALTSERIAL_V2_H_
#include <VC0706_AltSerial_V2.h>
#endif

#ifndef TRAPCAM_V03_H_
#include <TrapCam_V03.h>
#endif

void setup() 
{
  // put your setup code here, to run once:
  
  Bridge.begin();  // necessary for some reason, even though called in TrapCam.
  
  while(!Serial) ; // Wait for the Serial connection to open 
                    // Se we can see the output.
 

}

void loop() {
  // put your main code here, to run repeatedly:
   TrapCam_V03 camera;  // New camera object.
  
  //camera.getTimeStamp(); // Private function, not operable.
  
  String campath = camera.timePicPath();  // creates a string that is the full filepath of our image
                                          // + the name of the file in timestamp esque format .jpg
  Serial.print("\n campath is ");        
  Serial.println(campath);        // Let's look at it to verify that it works, plus then we can see the directory path.
  
  camera.pic_init(campath);        // ititialize the camera, give it the directory path+filename we want our picture to have
  Serial.println("^ init ********* makePic v");
  camera.makePic(campath);        // take the picture!
}
