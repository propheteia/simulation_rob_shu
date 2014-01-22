#ifndef __POSITIONINGPLAN_H
#define __POSITIONINGPLAN_H

#include "Plan.h"
#include "vector.h"
#include "ball.h"
//#include "self.h"
#include "worldmodel.h"

using namespace salt;

class PositioningPlan:public Plan
{
private:
	
public:
	PositioningPlan();
	
	virtual void run();
	virtual bool isNeeded();
	virtual void genMsg();
};

#endif //__POSITIONINGPLAN_H
