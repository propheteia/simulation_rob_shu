#include "PositioningPlan.h"

PositioningPlan::PositioningPlan()
{
	name = "PositioningPlan";
	isFinished = false;
}

bool PositioningPlan::isNeeded()
{
	//PositioningPlan is the last choice
	return true;
	
}

void PositioningPlan::run()
{
	LOG<<"PositioningPlan::run()"<<endl;
	//ideas
	/*
	1.set agent's position according to the ball's position
	*/
	Vector3f ballPos = WM.getBall().pos;
	LOG<<"PositioningPlan::run()-->ballPos = "<<ballPos<<endl;
	LOG<<"PositioningPlan::run()-->selfPos = "<<WM.getSelf().pos<<endl;
}

void PositioningPlan::genMsg()
{
	LOG<<"PositioningPlan::genMsg()"<<endl;
}
