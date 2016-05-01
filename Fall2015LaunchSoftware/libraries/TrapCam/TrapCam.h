// Non-Arduino Libraries
#include "VC0706_AltSerial_V2.h"
#include "AltSoftSerial.h"
// Arduino Libraries
#include "Arduino.h"
#include "FileIO.h"
#include "Process.h"
#include "YunServer.h"
#include "Bridge.h"
#include "Console.h"

//#define	SCNLOG	true	
#define PIC_PATH "/mnt/sda1/trapsat/data/"


class TrapCam
{
  public:
  	bool SCNLOG;

  	TrapCam();
  	void clearSerLog();
  	boolean pic_init(String);
  	boolean makePic(String);
  	String timePicPath();
  	
  
  //private:
  	String getTimeStamp();
  	
};
