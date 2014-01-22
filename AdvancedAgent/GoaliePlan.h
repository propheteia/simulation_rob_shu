#ifndef __GOALIEPLAN_H
#define __GOALIEPLAN_H

#include <sstream>

#include "Plan.h"
#include "worldmodel.h"
#include "Logger.h"
#include "ball.h"
#include "vector.h"
#include "MessageStorage.h"

using namespace salt;

class GoaliePlan:public Plan
{
	private:
		Vector2f lastBallPos;
		Vector2f blockPoint;
		Vector2f ourGoal;
		Vector2f ourGoal2Ball;
		Vector2f ballPos2;
		
		//ostringstream o_msg;
	
	public:
		GoaliePlan();
	
		virtual void run();
		virtual bool isNeeded();
		virtual void genMsg();
		
		void getBlockPoint();
};

#endif //__GOALIEPLAN_H
