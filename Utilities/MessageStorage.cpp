#include "MessageStorage.h"

MessageStorage::MessageStorage()
{
	LOG<<"MessageStorage::MessageStorage-->MessageStorage is constructed."<<endl;
	resetAll();
	plan2PathPlanningUpdateCycle = -1;
	pathPlanning2ActionUpdateCycle = -1;
}

void MessageStorage::resetAll()
{
	plan2PathPlanning.clear();
	pathPlanning2Action.clear();
	guiDebuggerMsg.clear();
}

void MessageStorage::setPlan2PathPlanningMsg(std::string msg)
{
	//plan2PathPlanningUpdateCycle = ...
	plan2PathPlanning.clear();
	plan2PathPlanning = msg;
	plan2PathPlanningUpdateCycle = WM.getMySimTime();
}

void MessageStorage::setPathPlanning2ActionMsg(std::string msg)
{
	//pathPlanning2ActionUpdateCycle = ...
	pathPlanning2Action.clear();
	pathPlanning2Action = msg;
	pathPlanning2ActionUpdateCycle = WM.getMySimTime();
}

std::string MessageStorage::getPlan2PathPlanningMsg()
{
	return plan2PathPlanning;
}

std::string MessageStorage::getPathPlanning2ActionMsg()
{
	return pathPlanning2Action;
}
