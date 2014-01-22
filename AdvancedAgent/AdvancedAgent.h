#ifndef __ADVANCEDAGENT_H
#define __ADVANCEDAGENT_H

#include "StaticModule.h"
#include "Logger.h"
#include "PlanManager.h"
#include "xmlParser.h"
#include "PathPlanning.h"
#include "worldmodel.h"
#include "types.h"
#include "Net.h"

#define AA StaticModule<AdvancedAgent>::getInstance()

//class PathPlanning;

enum RunMode
{
	RM_TEST_PATHPLANNING,
	RM_TEST_DECISION,
	RM_MATCH,
 	RM_NONE
};

enum Tactics
{
	T_DEFENSE,
	T_OFFENSE,
	T_NONE
};

//class PathPlanning;

class AdvancedAgent
{
private:
	RunMode runMode;
	Tactics tactics;
	
	//PathPlanning pathPlanning;
	PlanManager planManager;

public:
	AdvancedAgent();
	
	Tactics getTactics();
	void setTactics();
	
	void selectRunModeByFile();
	void test();
	void testPathPlanning();
	void testDecison();
	void matchPlan();
	
	void run();
	
	void sendInfo2GuiDebugger();

	string debug_msg;
	bool autoSendMsg;
};

#endif //__ADVANCEDAGENT_H
