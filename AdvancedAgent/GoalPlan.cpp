#include "GoalPlan.h"

GoalPlan::GoalPlan()
{
	name = "GoalPlan";
	isFinished = false;
}

bool GoalPlan::isNeeded()
{
	//just for test
	return false;
}

void GoalPlan::run()
{
	LOG<<"GoalPlan::run()"<<endl;
}

void GoalPlan::genMsg()
{
	LOG<<"GoalPlan::genMsg()"<<endl;
}
