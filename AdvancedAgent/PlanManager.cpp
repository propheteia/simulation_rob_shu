#include "PlanManager.h"

PlanManager::PlanManager()
{
	LOG<<"PlanManager::PlanManager()-->A PlanManager is constructed."<<endl;
	positioningDecisoin = new PositioningPlan();
	goaliePlan = new GoaliePlan();
	dribblePlan = new DribblePlan();
	goalPlan = new GoalPlan();
	while (!plans.empty())
		plans.pop();
}

bool PlanManager::selectPlan()
{
	LOG<<"PlanManager::selectPlan()"<<endl;
	bool planSelected = false;
	if (WM.getSelf().GetUnum() == 1)//Player 1 is the goalie
	{
		planSelected = true;
		goaliePlan->run();
	}
	else
	{
		//resume an old plan
		/*if (!plans.empty())
		{
			LOG<<"PlanManager::selectPlan()-->resume old plan: "<<plans.top()->getName()<<endl;
			while(!planSelected && !plans.empty())
			{
				if (plans.top()->isNeeded())
				{
					plans.top()->run();
					planSelected = true;
				}
				else
					plans.pop();
			}
		}
		
		if (!planSelected)//start new plan
		{
			//check other plans first
			//
			if(positioningDecisoin->isNeeded())
			{
				plans.push(positioningDecisoin);
				LOG<<"PlanManager::selectPlan()-->start a new plan: "<<plans.top()->getName()<<endl;
				plans.top()->run();
				planSelected = true;
			}
		}
		
		//remove finished 
		if (!plans.empty() && plans.top()->getIsFinished())
			plans.pop();*/
		
		if(goalPlan->isNeeded())
		{
			LOG<<"PlanManager::selectPlan()-->start a GoalPlan"<<endl; 
			goalPlan->run();
			planSelected = true;
		}
		else if (dribblePlan->isNeeded())
		{
			LOG<<"PlanManager::selectPlan()-->start a DribblePlan"<<endl; 
			dribblePlan->run();
			planSelected = true;
		}
		else//if(positioningDecisoin->isNeeded())
		{
			LOG<<"PlanManager::selectPlan()-->start a PositioningPlan"<<endl; 
			positioningDecisoin->run();
			planSelected = true;
		}
	}
		
		
	
	return planSelected;
}
