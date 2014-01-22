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

#include <Logger.h>
#include <cassert>
using namespace std;

//Logger section
Logger::LogStreams *Logger::logStreams = NULL;

// class LogStream
LogStream::LogStream(ostream *out, bool infoOn): out(out),first(true),infoOn(infoOn)
{
	cout << "infoOn: " << infoOn << endl;
	//initPid = getpid();
//	cout<<"LogStream::LogStream"<<endl;
	if(infoOn)
	{
		timer=new OnlineTimer();
		timer->resetCycle(0);
		if (out)
		{
			//cout<<"LogStream::LogStream-->out="<<out<<endl;
			*out << fixed << setprecision(3);
			*out << "Strive3D(2013) Logger file.\nThis logger system is created by Zheng Yonglei of Strive3D team on the basis of Mersad team's released source code.\nThis file is created in "<<timer->getDate();
		}
	}
}

LogStream::~LogStream()
{
}

bool LogStream::isOutValid()
{
	return (bool)out;
}

void LogStream::newCycle(long cycle) //Start to log a new cycle, print the current cycle at the first line of this cycle's log
{
//	cout<<"LogStream::newCycle"<<endl;
	timer->resetCycle(cycle);
	if (out)
	{
		unsigned cycle = timer ? timer->getCycle() : 0;
		//*out << endl << "[" << setw(FIELD_WIDTH) << setfill(FIELD_FILL) << cycle << "]\t" <<timer->getDate()<< endl;
		*out << endl << "[" << setw(FIELD_WIDTH) << setfill(FIELD_FILL) << cycle << "][cycle interval= "<<timer->cycleInterval()<<"]"<< endl;
	}
}

void LogStream::newCycle()
{
	timer->resetCycle();
}

LogStream &LogStream::operator<<(ostream &(*man)(ostream &))
{
	if (out)
	{
		*out << man;
		if (man == (ostream &(*)(ostream &))endl)
			first = true;
	}
	return *this;
}

LogStream &LogStream::operator<<(ios_base &(*man)(ios_base &))
{
	if (out)
		*out << man;
	return *this;
}


// class LogNull
LogNull::LogNull(): LogStream(NULL)
{
}

LogNull::~LogNull()
{
}


// class LogFile
LogFile::LogFile(const string &fileName, bool infoOn): 
	LogStream(new ofstream(fileName.c_str()), infoOn), 
	fileName(fileName)
{
}

LogFile::~LogFile()
{
	delete out;
}


// class LogStdOut
LogStdOut::LogStdOut(): LogStream(&cout)
{
}

LogStdOut::~LogStdOut()
{
}


// class LogStdErr
LogStdErr::LogStdErr(): LogStream(&cerr)
{
}

LogStdErr::~LogStdErr()
{
}


// class Logger::LogStreams
Logger::LogStreams::LogStreams()
{
}

Logger::LogStreams::~LogStreams()
{
	for (map<string, LogStream *>::iterator it = streams.begin();
		it != streams.end(); it++)
		delete it->second;
}

void Logger::LogStreams::add(const string &name, LogStream *stream)
{
	map<string, LogStream *>::iterator it = streams.find(name);
	if (it != streams.end()) return;
	
	streams[name] = stream;
}

LogStream &Logger::LogStreams::operator[](const string &name) const
{
	map<string, LogStream *>::const_iterator it = streams.find(name);
	if (it == streams.end())
		return (LogStream &)logNull;
	return *it->second;
}


// class Logger
Logger::Logger()
{
	//logLine="**************************************************";
	if (!logStreams)
		logStreams = new LogStreams();
}

Logger::~Logger()
{
}

void Logger::add(const string &name, LogStream *stream)
{
	assert(logStreams);
	logStreams->add(name, stream);	
}

LogStream &Logger::operator[](const string &name) const
{
	assert(logStreams);
	return (*logStreams)[name];
}

/*void Logger::init()
{
//	cout<<"Logger::init()"<<endl;
	if (s_instance)
	{
		cout<<"Logger was already initialized"<<endl;
		assert(0);
	}
	
	s_instance=new Logger();
}*/

/*Logger& Logger::getInstance()
{
	assert(s_instance);
	return *s_instance;
}*/

//End of logger section

//Timer section

// class Timer
Timer::Timer(): cycle(-1)
{
//	cout<<"Timer::Timer()"<<endl;
	// resetCycle(); // logically, should be here!
	time(&nowTime);
	timenow = localtime(&nowTime);
}

Timer::~Timer()
{
}

void Timer::resetCycle(long cycle)
{
//	cout<<"Timer::resetCycle"<<endl;
	this->cycle = cycle;
}

long Timer::getCycle() const
{
	return cycle;
}


// class OnlineTimer
OnlineTimer::OnlineTimer()
{
//	<<"OnlineTimer::OnlineTimer()"<<endl;
	resetCycle(-1);
	gettimeofday(&lastCycle, NULL);
}

OnlineTimer::~OnlineTimer()
{
}

void OnlineTimer::resetCycle(long cycle)
{
//	cout<<"OnlineTimer::resetCycle"<<endl;
	Timer::resetCycle(cycle);
	gettimeofday(&beginning, NULL);
}

void OnlineTimer::resetCycle()
{
	gettimeofday(&beginning, NULL);
}

long OnlineTimer::now() const
{
	timeval now;
	gettimeofday(&now, NULL);
	return (long) (((now.tv_usec - beginning.tv_usec) / 1000.0 + (now.tv_sec - beginning.tv_sec) * 1000) + 0.5);
}

std::string OnlineTimer::now_string() const
{
	timeval now;
	gettimeofday(&now, NULL);
	
	long time = (((now.tv_usec - beginning.tv_usec) + (now.tv_sec - beginning.tv_sec) * 1000000) + 0.5);
		
	long usec = time - (time/1000)*1000;
	long msec = time - (time/1000000)*1000000;
	msec = (msec - usec)/1000;
	long sec = time/1000000;
	
	ostringstream o_time;
	o_time<< setw(1) << setfill('0')  << sec <<":"<<setw(3) <<setfill('0') << msec << ":" <<setw(3)<<usec ;
	std::string s_time = o_time.str();
	//cout<<s_time<<endl;
	return s_time;
}

std::string OnlineTimer::cycleInterval()
{
	timeval now;
	gettimeofday(&now, NULL);
	
	long time = (((now.tv_usec - lastCycle.tv_usec) + (now.tv_sec - lastCycle.tv_sec) * 1000000) + 0.5);
	
	gettimeofday(&lastCycle, NULL);
		
	long usec = time - (time/1000)*1000;
	long msec = time - (time/1000000)*1000000;
	msec = (msec - usec)/1000;
	long sec = time/1000000;
	
	ostringstream o_time;
	o_time<< setw(1) << setfill('0')  << sec <<":"<<setw(3) <<setfill('0') << msec << ":" <<setw(3)<<usec ;
	std::string s_time = o_time.str();
	//cout<<s_time<<endl;
	return s_time;
}

/*std::string OnlineTimer::curMS() const
{
	timeval now;
	gettimeofday(&now, NULL);
	ostringstream o_ms;
	o_ms << now.tv_usec;
	
	return o_ms.str();
}

std::string OnlineTimer::curS() const
{
	timeval now;
	gettimeofday(&now, NULL);
	ostringstream o_s;
	o_s << now.tv_sec;
	
	return o_s.str();
}
*/
std::string OnlineTimer::getDate()
{
	//time_t now;
	//struct tm  *timenow;
	//time(&now);
	//timenow = localtime(&now);
	ostringstream o_result;
	o_result<<asctime(timenow);
	return o_result.str();
}

// class VirtualTimer
VirtualTimer::VirtualTimer()
{
}

VirtualTimer::~VirtualTimer()
{
}

void VirtualTimer::resetCycle(long cycle)
{
	Timer::resetCycle(cycle);
	curMS = 0;
}

long VirtualTimer::now() const
{
	return curMS;
}

void VirtualTimer::setCurMS(long curMSArg)
{
	curMS = curMSArg;
}
//End of timer section

