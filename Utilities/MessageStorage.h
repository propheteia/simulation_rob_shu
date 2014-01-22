#ifndef __MESSAGESTORAGE_H
#define __MESSAGESTORAGE_H

#include <iostream>
#include <string>

#include "StaticModule.h"
#include "Logger.h"
#include "worldmodel.h"

#define MS StaticModule<MessageStorage>::getInstance()

using namespace std;

class MessageStorage
{
private:
	std::string plan2PathPlanning;
	std::string pathPlanning2Action;
	float plan2PathPlanningUpdateCycle;
	float pathPlanning2ActionUpdateCycle;
	
	string guiDebuggerMsg;
	
public:
	MessageStorage();
	void resetAll();
	
	void setPlan2PathPlanningMsg(std::string msg);
	void setPathPlanning2ActionMsg(std::string msg);
	std::string getPlan2PathPlanningMsg();
	std::string getPathPlanning2ActionMsg();
	
	float getPlan2PathPlanningUpdateCycle(){return plan2PathPlanningUpdateCycle;}
	float getPathPlanning2ActionUpdateCycle(){return pathPlanning2ActionUpdateCycle;}
	
	void clearGuiDebuggerMsg(){guiDebuggerMsg.clear();}
	void appendGuiDebuggerMsg(string msg){guiDebuggerMsg.append(msg);}
	string getGuiDebuggerMsg(){return guiDebuggerMsg;}
};

#endif //__MESSAGESTORAGE_H
