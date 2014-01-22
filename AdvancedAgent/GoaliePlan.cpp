#include "GoaliePlan.h"

GoaliePlan::GoaliePlan()
{
	name = "GoaliePlan";
	isFinished = false;
	lastBallPos = Vector2f(0.0,0.0);
	blockPoint = Vector2f(0.0,0.0);
	ourGoal = Vector2f(-12.0,0.0);
	ballPos2 = Vector2f(0.0,0.0);
}

bool GoaliePlan::isNeeded()
{
	//Consider distance from ball to our goal
	
	return true;
}

void GoaliePlan::run()
{
	LOG<<"GoaliePlan::run()"<<endl;
	
	ballPos2 = Vector2f(WM.getBall().pos.x(),WM.getBall().pos.y());
	ourGoal2Ball = ballPos2 - ourGoal;
	float distOurGoal2Ball = ourGoal2Ball.Length();
	//if (distOurGoal2Ball >= 8)
	if (distOurGoal2Ball >= 18)
	{
		LOG<<"GoaliePlan::run()-->distOurGoal2Ball >= 18"<<endl;
		isFinished = true;
	}
	else
	{
		LOG<<"GoaliePlan::run()-->distOurGoal2Ball < 18"<<endl;
		LOG<<"GoaliePlan::run()-->lastBallPos = "<<lastBallPos<<"\tballPos2 =  "<<ballPos2<<endl;
		if ((lastBallPos - ballPos2).Length() > 0.3)
		{
			getBlockPoint();
			genMsg();
			lastBallPos = ballPos2;
		}
		isFinished = false;
	}
}

void GoaliePlan::genMsg()
{
	LOG<<"GoaliePlan::genMsg()"<<endl;
	ostringstream o_msg;
	o_msg << "(Block "<< "(b "<<blockPoint<<")";
	LOG<<"GoaliePlan::genMsg()-->o_msg = "<<o_msg.str()<<endl;
	MS.setPlan2PathPlanningMsg(o_msg.str());
}

void GoaliePlan::getBlockPoint()
{
	LOG<<"GoaliePlan::getBlockPoint()"<<endl;
	Vector2f unitOurGoal2Ball = ourGoal2Ball/(ourGoal2Ball.Length());
	LOG<<"GoaliePlan::getBlockPoint()-->unitOurGoal2Ball = "<<unitOurGoal2Ball<<endl;
	blockPoint = unitOurGoal2Ball * (ourGoal2Ball.Length() - 0.5);
	blockPoint += ourGoal;
}
