#ifndef __DECISIONMANAGER_H
#define __DECISIONMANAGER_H

#include <stack>

#include "Logger.h"
#include "Plan.h"
#include "self.h"
#include "PositioningPlan.h"
#include "DribblePlan.h"
#include "GoaliePlan.h"
#include "GoalPlan.h"

class PlanManager
{
private:
	stack<Plan*>plans;
	PositioningPlan* positioningDecisoin;
	GoaliePlan* goaliePlan;
	DribblePlan* dribblePlan;
	GoalPlan* goalPlan;
//	bool planDone;
	
public:
	PlanManager();
	
	bool selectPlan();//only one plan type can be selected in a single cycle
};

#endif //__DECISIONMANAGER_H
