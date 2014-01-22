#ifndef __DECISION_H
#define __DECISION_H

#include "Logger.h"
#include "string"

class Plan
{
protected:
	bool isFinished;
	int startCycle;
	int endCycle;
	int runCount;
	std::string name;
	
public:
	Plan(){}
	
	bool getIsFinished(){return isFinished;}//After the whole action is done, 
	int getStartCycle(){return startCycle;}
	int getEndCycle(){return endCycle;}
	int getRunCount(){return runCount;}
	std::string getName(){return name;}
	
	virtual void run() = 0;
	virtual bool isNeeded() = 0;
	virtual void genMsg() = 0;
};

#endif //__DECISION_H
