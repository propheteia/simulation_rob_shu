#include "DribblePlan.h"

DribblePlan::DribblePlan()
{
	name = "DribblePlan";
	isFinished = false;
}

bool DribblePlan::isNeeded()
{
	//just for test
	return false;
}

void DribblePlan::run()
{
	LOG<<"DribblePlan::run()"<<endl;
}

void DribblePlan::genMsg()
{
	LOG<<"DribblePlan::genMsg()"<<endl;
}
