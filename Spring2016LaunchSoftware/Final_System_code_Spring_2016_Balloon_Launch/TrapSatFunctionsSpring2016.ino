#include <TrapSat.h>

void makeNumStarts()
{
    if( !FileSystem.exists(TRAPSAT_COUNT_PATH) )
    {
        //Serial.print("File Didn't Existing. Making file..\n");
        File NumStartsFile = FileSystem.open(TRAPSAT_COUNT_PATH, FILE_WRITE);
        //NumStartsFile.print('I');
        NumStartsFile.close();
    }
}

void incNumStarts()
{
    File NumStartsFile = FileSystem.open(TRAPSAT_COUNT_PATH, FILE_APPEND);
    NumStartsFile.print('I');
    NumStartsFile.close();
}

int getNumStarts()
{
    File NumStartsFile = FileSystem.open(TRAPSAT_COUNT_PATH, FILE_READ);
    int num = 0;
    while (NumStartsFile.read() == 'I')
    {
        num++;
    }
    NumStartsFile.close();
    return num;
}

char* getTimeStamp()
{
        const int bytes = 19;
	char timeStamp[bytes] = "";
	unsigned long sinceBegin = millis();
	unsigned long rest = 0;

	/* 3,600,000 ms in 1 hour */
	int hourSince = sinceBegin / 3600000;
	rest = sinceBegin % 3600000;

	/* 60,000 ms in 1 minute */
	int minuteSince = rest / 60000;
	rest = rest % 60000;

	/* 1,000 ms in 1 second */
	int secondSince	= rest / 1000;
	rest = rest % 1000;
	
  sprintf(timeStamp, "%0d, %0dh%0dm%0ds%0dms", getNumStarts(), hourSince, minuteSince, secondSince, rest);
  timeStamp[bytes - 1] = '\0';
	// 000_00h00m00s000ms
	return timeStamp;
}

void runLogEvent(char* msg)
{
    //Serial.print("Calling runLogEvent()\n");
    char logMsg[80] = "";
    strcat(logMsg, getTimeStamp());
    strcat(logMsg, " ");
    strcat(logMsg, msg);

    Process LogEvent;
    LogEvent.begin("/root/SysLogger/SysEventLogger.sh");
    LogEvent.addParameter( logMsg );
    LogEvent.run();
    LogEvent.close();
}

void runLogError(char* msg)
{
    Serial.print("Calling runLogError()\n");
    char logMsg[80] = "";
    strcat(logMsg, getTimeStamp());
    strcat(logMsg, " ");
    strcat(logMsg, msg);

    Process LogError;
    LogError.begin("/root/SysLogger/SysErrorLogger.sh");
    LogError.addParameter( logMsg );
    LogError.run();
    LogError.close();
}
