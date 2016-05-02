/*
** SysLog - System Log
** Version 5
** Target - Arduino Yun
*/

/*
** Oct 14, 2014 Edits
**  % getTimeStamp() changed from returning Date and Time
**     to returning Time Since Start 00h00m00s000ms
**  % getTimeStamp() no longer returns formatting characters
**     i.e. '[', '[', and ' '
*/

#include "SysLog.h"

/*
** Public Function
** printDebug modifiers
*/
void SysLog::setDebug()
{
	printDebug = true;
}

void SysLog::clearDebug()
{
	printDebug = false;
}

/*
** Optional Extra Output for Debugging
** Make this a private function in the class
*/
void SysLog::logPrint(String output)
{
	if( printDebug )
	{
		Serial.print(output);
	}
}

/*
** Make sure Event Log exists
** Make this an private function in the class
*/
int SysLog::initEvent()
{
	/*
	** Convert File Path to C String
	*/
	File EventLog = FileSystem.open(EventFilePath.c_str(),FILE_APPEND);
	if( EventLog )
	{
		logPrint("EventLog Created With Write Permissions\n");
		EventLog.close();
		return EXIT_OPEN_SUCCESS;
	}
	else
	{
		logPrint("EventLog Not Able To Be Opened!\n");
		return EXIT_OPEN_ERR;
	}
}

/*
** Make sure Error Log exists
** Make this an private function in the class
*/
int SysLog::initError()
{
	/*
	** Convert File Path to C String
	*/
	File ErrorLog = FileSystem.open(ErrorFilePath.c_str(),FILE_APPEND);
	if( ErrorLog )
	{
		logPrint("ErrorLog Created With Write Permissions\n");
		ErrorLog.close();
		return EXIT_OPEN_SUCCESS;
	}
	else
	{
		logPrint("ErrorLog Not Able To Be Opened!\n");
		return EXIT_OPEN_ERR;
	}
}

/*
** Call All Initialization Functions
** Make this an private function in the class
*/ 
void SysLog::initLog()
{
	Bridge.begin();
	FileSystem.begin();
	String logMsg = "";

	/*
	** Check for Directory Path, Update Debug Message
	*/
	if(FileSystem.mkdir(LogDirectory.c_str()))
	{
		logMsg = "MKDIR GOOD Log Directory:" + LogDirectory + "\n";
	}
	else
	{
		logMsg = "MKDIR FAIL Log Directory:" + LogDirectory + "\n";
	}
	/*
	**
	*/
	logPrint(logMsg);

	initEvent();
	initError();
}

/*
** Set Event/Error Log Directory Path
** Changes LogDirectory
** Make this an private function in the class
*/
void SysLog::setLogDir(String tempDir)
{
	/*
	** Test to see if tempDir is a valid directory
	** if not use default"Overloaded Constructor Called\n"
	*/
	if( (tempDir[0] == '/') && (tempDir[tempDir.length()-1] == '/') )
	{
		logPrint("Custom Dir Loaded\n");
		LogDirectory = tempDir;
	}
	else
	{
		logPrint("Default Dir Used\n");
		LogDirectory = DEFAULT_LOG_PATH;
	}

	setEventFilePath(LogDirectory, EVENTLOG_FILENAME);
	setErrorFilePath(LogDirectory, ERRORLOG_FILENAME);
}

/*
** Update the full file path for EventLog
** Make this an private function in the class
*/
void SysLog::setEventFilePath(String tempDir, String tempFile)
{
	EventFilePath = tempDir;
	EventFilePath += tempFile;
}

/*
** Update the full file path for ErrorLog
** Make this an private function in the class
*/
void SysLog::setErrorFilePath(String tempDir, String tempFile)
{
	ErrorFilePath = tempDir;
	ErrorFilePath += tempFile;
}

/*
** Create Default Event/Error Log Location
** Make this an public function in the class
*/
SysLog::SysLog()
{
	logMessage = "";
	errMessage = "";
	LogDirectory = "";
	EventFilePath = "";
	ErrorFilePath = "";
	printDebug = true;

	/*
	** No Arg, Default Log Path used
	*/
	setLogDir(String(DEFAULT_LOG_PATH));

	/*
	** Set Event/Error FilePath 
	*/
	initLog();
}

/*
** Overload with New Dir Path
** Make this an public function in the class
*/
SysLog::SysLog(String logDir)
{
	logMessage = "";
	errMessage = "";
	LogDirectory = "";
	EventFilePath = "";
	ErrorFilePath = "";
	printDebug = true;

	logPrint("Overloaded Constructor Called\n");

	/*
	** Yes Arg, Custom Log Path used
	*/
	setLogDir(logDir);

	/*
	** Set Event/Error FilePath 
	*/
	initLog();
}

/*
** Public Class
** Return a string that holds the timestamp to the caller
*/
String SysLog::getTimeStamp()
{
	String timeStamp;
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
	
	timeStamp = String(hourSince) + "h";
	timeStamp += String(minuteSince) + "m";
	timeStamp += String(secondSince) + "s";
	timeStamp += String(rest) + "ms";
	
	// 00h00m00s000ms

	return timeStamp;
}

/*
** Write to ErrorLog
** Make this an public function in the class
*/
int SysLog::logError(String tempMessage)
{
	/*
	** Convert File Path to C String
	*/
	File ErrorLog = FileSystem.open(ErrorFilePath.c_str(),FILE_APPEND);
	if( ErrorLog )
	{
		logPrint("ErrorLog Opened With Write Permissions\n");
	}
	else
	{
		logPrint("ErrorLog Not Able To Be Written\n");
		return EXIT_OPEN_ERR;
	}
	
	/*
	** Concatonate String
	** Later we should add size checking if necessary
	*/
	errMessage = "[";
	errMessage += getTimeStamp();
	errMessage += "] ";
	errMessage += tempMessage;

	/*
	** Write To File
	*/
	ErrorLog.print(errMessage);
	logPrint(errMessage);

	ErrorLog.close();
	return EXIT_WRITE_SUCCESS;
}

/*
** Write to EventLog
** Make this an public function in the class
*/
int SysLog::logEvent(String tempMessage)
{
	/*
	** Convert File Path to Character Array
	*/
	File EventLog = FileSystem.open(EventFilePath.c_str(),FILE_APPEND);
	if( EventLog )
	{
		logPrint("EventLog Opened With Write Permissions\n");
	}
	else
	{
		logPrint("EventLog Not Able To Be Written\n");
		return EXIT_OPEN_ERR;
	}

	/*
	** Concatonate String
	** Later we should add size checking if necessary
	*/
	logMessage = "[";
	logMessage += getTimeStamp();
	logMessage += "] ";
	logMessage += tempMessage;

	/*
	** Write To File
	*/
	EventLog.print(logMessage);
	logPrint(logMessage);

	EventLog.close();
	return EXIT_WRITE_SUCCESS;
}
