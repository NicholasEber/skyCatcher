#include "TrapCam.h"


TrapCam::TrapCam()
{
	SCNLOG = true;
	//AltSoftSerial cameraconnection;
	//VC0706_AltSerial_V2 cam = VC0706_AltSerial_V2(&cameraconnection);
}

void TrapCam::clearSerLog()
{
	SCNLOG = false;
}

boolean TrapCam::pic_init(String path)
{
	Bridge.begin();
	FileSystem.begin();
	
	//Create the jpg file
	Process mkfile;
	mkfile.begin("touch");
	mkfile.addParameter(path);
	mkfile.run();
	mkfile.close();

	File testPicFile = FileSystem.open(path.c_str(), FILE_WRITE);
	if(!testPicFile)
	{
		if(SCNLOG)
			Serial.println("FILE NOT CREATED BY INIT!");
		testPicFile.close();
		return false; // Proabably add to error log in implementation file.
	}
	else
	{
		if(SCNLOG)
			Serial.println("FILE CREATED BY INIT!");
		testPicFile.close();
	}

	/*if(cam.begin()) 
	{
		if(SCNLOG)
    		Serial.println("Camera Found");
  	} 
  	else 
  	{
  		if(SCNLOG)
   			Serial.println("No camera found?");
    	return false;
  	}*/


}

boolean	TrapCam::makePic(String path)
{
	Bridge.begin();
	FileSystem.begin();

	AltSoftSerial cameraconnection;
    VC0706_AltSerial_V2 cam = VC0706_AltSerial_V2(&cameraconnection);

	File picpath = FileSystem.open(path.c_str(), FILE_WRITE);
	if(!picpath)
	{
		if(SCNLOG)
			Serial.println("FILE NOT OPENED BY makePic!");
		return false;
	}

	if(!cam.begin())	// If camera isn't found...
	{
		if(SCNLOG)
			Serial.println("Camera not found.");
		return false;
	}

	// Set the picture size
	cam.setImageSize(VC0706_640x480);

	delay(1000); // wait a sec...

	if (! cam.takePicture()) //Try to take a picture!
	{
    	if(SCNLOG)
    		Serial.println("Failed to snap!");
    	return false;
	}
  	else
  	{
  		if(SCNLOG)
    		Serial.println("Picture taken!");
  	}

	// write the image to the SD card!

	 // Get the size of the image (frame) taken
	 uint16_t jpglen = cam.frameLength();
	 if(SCNLOG)
	 {
		 Serial.println("Storing ");
		 Serial.println(jpglen, DEC);
		 Serial.println(" byte image.");
	 }

	 int32_t time = millis();
	 pinMode(8, OUTPUT);
	 // Read all the data up to # bytes!
	 byte wCount = 0; // For counting # of writes

	 while(jpglen > 0)
	 {
	   // read 32 bytes at a time;
	   uint8_t *buffer;
	   uint8_t bytesToRead = min(64, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
	   buffer = cam.readPicture(bytesToRead);
	   picpath.write(buffer, bytesToRead);
	   if(++wCount >= 64)
	   { // Every 2K, give a little feedback so it doesn't appear locked up
	     if(SCNLOG)
	     	Serial.print('.');
	     wCount = 0;
	   }
	   jpglen -= bytesToRead;
	 }

	 picpath.flush();
	 picpath.close();  	

	 if(SCNLOG)
	 {
	 	time = millis() - time;
 		Serial.println("\n Stored Picture!");
		Serial.print(time); Serial.println(" ms elapsed");
	 }

	 return true;

}

String TrapCam::getTimeStamp()  	
{
  String timeStamp;
  Process timeProcess;

  /*
  ** date is a command line utility to get the date and the time.
  ** 
  ** !!!!!!!!!!!!!!!!!!!!!1!WARNING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  **
  ** this function should not be used to name the jpeg images unless
  ** the board is connected to a valid WiFi network with internet or UTC 
  ** time access. Otherwise these values will be irrelivant and useless!
  */
  timeProcess.begin("date");

  timeProcess.addParameter("+%D__%T");
  timeProcess.run();

  while (timeProcess.available() > 0)
  {
    char ch = timeProcess.read();
    if (ch != '\n')
    {
      timeStamp += ch;
    }
  }
  //timeStamp += "]";

  /*
  ** Length Of timeStamp should be: 16
  ** 09/08/1111:55:15
  ** 09_08_1111_55_15
  */
  
  for(int i =0; i < 16; i++)
  {
    if(timeStamp[i] == '/' || timeStamp[i] == ':') 	// Take out the '/' and ':' characters.
      timeStamp[i] = '_';							// we can't use them for a filename.
  }
  return timeStamp;
}

String TrapCam::timePicPath()
{
	String picpath = PIC_PATH + getTimeStamp() + ".jpg";
  	return picpath;
}
