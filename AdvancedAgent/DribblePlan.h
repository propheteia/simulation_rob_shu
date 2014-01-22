#ifndef __DRIBBLEPLAN_H
#define __DRIBBLEPLAN_H

#include "Plan.h"

class DribblePlan:public Plan
{
	private:
	
	public:
		DribblePlan();
	
		virtual void run();
		virtual bool isNeeded();
		virtual void genMsg();
};

#endif //__DRIBBLEPLAN_H
