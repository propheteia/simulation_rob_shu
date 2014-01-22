/*
 *  Copyright 2002-2005, Mersad Team, Allameh Helli High School (NODET).
 *
 *  This program is free software, you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Library General Public License for more details.
 *
 *  This file is created by: Mohsen Izadi
 *  and is modified by: Ahmad Boorghany
 *
 *  Released on Monday 1 August 2005, 10 Mordad 1384 by Mersad RoboCup Team.
 *  For more information please read README file.
*/

#ifndef __LOGGER_H
#define __LOGGER_H

//#include <Timer.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <iomanip>

#include <unistd.h>

#include "StaticModule.h"

//#define LOGGER StaticModule<Logger>::getInstance()
//#define INIT_LOGGER StaticModule<Logger>::init();

#define FIELD_FILL '0'
//#define FIELD_WIDTH 4
#define FIELD_WIDTH 6
#define PRECISION 2

#define LOG StaticModule<Logger>::getInstance()["MainLog"]
#define pLOG StaticModule<Logger>::getInstance()["ParserLog"]
#define aLOG StaticModule<Logger>::getInstance()["ActionLog"]
#define DEBUG StaticModule<Logger>::getInstance()["DebugLog"]
#define bLOG StaticModule<Logger>::getInstance()["BehaviorControlLog"]
#define tLOG StaticModule<Logger>::getInstance()["TrainerLog"]

#define oLOG StaticModule<Logger>::getInstance()["OutputLog"]

#define STARS "**************************************************************"

using namespace std;


//Timer section
class Timer
{
	protected:
		long cycle;
		
		time_t nowTime;
		struct tm *timenow;

	public:
		Timer();
		virtual ~Timer();

		virtual long now() const = 0;
		virtual std::string now_string() const = 0;
		//virtual std::string curMS() const = 0;
		//virtual std::string curS() const = 0;
		virtual std::string cycleInterval() = 0;
		virtual long getCycle() const;
		virtual void resetCycle(long cycle);
		virtual std::string getDate() = 0;
		virtual void resetCycle() = 0;
};

class OnlineTimer: public Timer
{
	protected:
		timeval beginning;
		timeval lastCycle;

	public:
		OnlineTimer();
		virtual ~OnlineTimer();

		virtual long now() const;
		virtual std::string now_string() const;
		//virtual std::string curMS() const;
		//virtual std::string curS() const;
		virtual std::string cycleInterval();
		virtual void resetCycle(long cycle);
		virtual std::string getDate();
		
		virtual void resetCycle();
};

class VirtualTimer: public Timer//Not in use now
{
	protected:
		long curMS;

	public:
		VirtualTimer();
		virtual ~VirtualTimer();

		void setCurMS(long curMSArg);

		virtual long now() const;
		virtual void resetCycle(long cycle);
};
//End of Timer section

//Logger section
class LogStream
{
protected:
	std::ostream *out;
	Timer *timer;
	bool first;
	bool infoOn;
	//pid_t initPid;

public:
	LogStream(std::ostream *out, bool infoOn = true);
	virtual ~LogStream();

	bool isOutValid();
	void newCycle(long cycle);
	void newCycle();
	
	template<class T>
	LogStream &operator<<(const T &value)
	{
		if (out)
		{
			if (first && infoOn)
			{
				first = false;
				//unsigned now = timer ? timer->now() : 0;
				//float now = timer ? timer->now() : 0;
				*out << "  " << std::setw(6) << std::setfill(FIELD_FILL) << timer->now_string().c_str() << ". ";
			//	if (initPid != getpid())
			//		*out <<"[RecvProc] "; //This is message receiving process.
				
			}
			*out << value;
		}
		return *this;
	}

	LogStream &operator<<(std::ostream &(*man)(std::ostream &));
	LogStream &operator<<(std::ios_base &(*man)(std::ios_base &));
};

class LogNull: public LogStream
{
public:
	LogNull();
	virtual ~LogNull();
};

class LogFile: public LogStream
{
protected:
	std::string fileName;

public:
	LogFile(const std::string &fileName, bool infoOn = true);
	virtual ~LogFile();
};

class LogStdOut: public LogStream
{
public:
	LogStdOut();
	virtual ~LogStdOut();
};

class LogStdErr: public LogStream
{
public:
	LogStdErr();
	virtual ~LogStdErr();
};

class Logger
{
protected:
	class LogStreams
	{
	protected:
		std::map<std::string, LogStream *> streams;
		LogNull logNull;

	public:
		LogStreams();
		virtual ~LogStreams();

		void add(const std::string &name, LogStream *stream);
		LogStream &operator[](const std::string &name) const;
	};

	static LogStreams *logStreams;

public:
	Logger();
	virtual ~Logger();
	
	void add(const std::string &name, LogStream *stream);
	LogStream &operator[](const std::string &name) const;
};
//End of logger section

#endif // __LOGGER_H

//How to use
/*
#include "Logger.h"
#include <string>
using namespace std;
int main()
{
	//initializing the logger system
	Logger::init();
	Logger& logger=Logger::getInstance();
	std::string logFileName =  "./test.log";
	logger.add("MainLog", new LogFile(logFileName));

	//applying the logger system
	LOG.newCycle(0);
	LOG<<"HumanoidAgent::run()"<<endl;
	LOG<<STARS<<endl;
	
	LOG.newCycle(1);
	LOG<<"HumanoidAgent::run()"<<endl;
}
*/
