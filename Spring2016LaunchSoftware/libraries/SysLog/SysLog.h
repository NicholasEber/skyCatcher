/*
** SysLog - System Log 
** Version 5
** Target - Arduino Yun
*/

#ifndef SYSLOG_H_
#define SYSLOG_H_

#include <Arduino.h>
#include <Bridge.h>
#include <FileIO.h>
#include <TrapSat.h>

/*
** Global Defines
*/
#define DEFAULT_LOG_PATH "/mnt/sd/"
#define EVENTLOG_FILENAME "eventlog.txt"
#define ERRORLOG_FILENAME "errorlog.txt"
#define EXIT_OPEN_ERR 0
#define EXIT_OPEN_SUCCESS 1
#define EXIT_WRITE_SUCCESS 2

class SysLog
{
public:
	SysLog();
	SysLog(String logDir);
	void setDebug();
	void clearDebug();
	int logEvent(String tempMessage);
	int logError(String tempMessage);
	String getTimeStamp();

private:
	String logMessage;
	String errMessage;
	String LogDirectory;
	String EventFilePath;
	String ErrorFilePath;
	bool printDebug;

	void logPrint(String output);
	void setLogDir(String tempDir);
	void setEventFilePath(String tempDir, String tempFile);
	void setErrorFilePath(String tempDir, String tempFile);
	void initLog();
	int initEvent();
	int initError();
};

#endif
