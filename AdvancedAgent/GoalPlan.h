#ifndef __GOALPLAN_H
#define __GOALPLAN_H

#include "Plan.h"
#include "types.h"

class GoalPlan:public Plan
{
	private:
	
	public:
		GoalPlan();
	
		virtual void run();
		virtual bool isNeeded();
		virtual void genMsg();
};

#endif //__GOALPLAN_H
