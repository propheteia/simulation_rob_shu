#include "AdvancedAgent.h"

AdvancedAgent::AdvancedAgent() :
	autoSendMsg(true)
{
	LOG<<"AdvancedAgent::AdvancedAgent()-->An AdvancedAgent is constructed."<<endl;
	runMode = RM_NONE;
	tactics = T_DEFENSE;
}

void AdvancedAgent::run()
{
	LOG<<"AdvancedAgent::run()"<<endl;
	
	//setTactics();
	//matchPlan();
	if (autoSendMsg)
		sendInfo2GuiDebugger();
}

void AdvancedAgent::selectRunModeByFile()
{
	if (CONF.getXMLValue("/Strive/AdvancedAgent/MatchPlan").asBool())
		runMode = RM_MATCH;
	else if (CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/TestPathPlanning").asBool())
		runMode = RM_TEST_PATHPLANNING;
	else if (CONF.getXMLValue("/Strive/AdvancedAgent/TestPlan").asBool())
		runMode = RM_TEST_DECISION;
}

void AdvancedAgent::test()
{
	LOG<<"AdvancedAgent::run()"<<endl;
	selectRunModeByFile();
	switch(runMode)
	{
		case RM_TEST_PATHPLANNING:
			testPathPlanning();
			break;
		case RM_TEST_DECISION:
			testDecison();
			break;
		case RM_MATCH:
			matchPlan();
			break;
		case RM_NONE:
			cout<<"AdvancedAgent::run()-->No mode to run. Exiting..."<<endl;
			exit(0);
	}
}

void AdvancedAgent::testPathPlanning()
{
	LOG<<"AdvancedAgent::testPathPlanning()"<<endl;
	//pathPlanning.test();
	PP.test();
}

void AdvancedAgent::testDecison()
{
	LOG<<"AdvancedAgent::testDecison()"<<endl;
}

void AdvancedAgent::matchPlan()
{
	LOG<<"AdvancedAgent::matchPlan()"<<endl;
	
	if(!planManager.selectPlan())//select and do the selected plan
		cout<<"AdvancedAgent::matchPlan()-->Not plan selected."<<endl;
	else
		PP.run();//generate the path planning message
}

Tactics AdvancedAgent::getTactics()
{
	return tactics;
}

void AdvancedAgent::setTactics()
{
}

void AdvancedAgent::sendInfo2GuiDebugger()
{
	DEBUG<<"AdvancedAgent::sendMsg2GuiDebugger"<<endl;
	//cout<<"AdvancedAgent::sendInfo2GuiDebugger-->(my_pos("<<WM.getSelf().pos<<"))"<<endl;
	//if (WM.getTmm(1).isValid())
	//	cout<<"AdvancedAgent::sendInfo2GuiDebugger-->WM.getTmm("<<1<<").pos = "<<WM.getTmm(0).pos<<endl;
	if (DEBUG_NET.isValid())
	{
		ostringstream o_msg;
		o_msg<<"<r>";
			o_msg<<"<c>"<<int(WM.getSelf().CurrentCycle)<<"</c>";
			o_msg<<"<rp>";
				o_msg<<"<dir>"<<WM.getSelf().GetTorsoPitchAngle()<<"</dir>";
				o_msg<<"<pos>";
					o_msg<<"<t>Strive3D</t>";
					o_msg<<"<u>"<<WM.getSelf().GetUnum()<<"</u>";
					o_msg<<"<h>"<<WM.getSelf().pos<<"</h>";
					o_msg<<"<lla>"<<WM.getSelf().GetLeftArmGloblePosition()<<"</lla>";
					o_msg<<"<rla>"<<WM.getSelf().GetRightArmGloblePosition()<<"</rla>";
					o_msg<<"<lf>"<<WM.getSelf().GetLeftFootGloblePosition()<<"</lf>";
					o_msg<<"<rf>"<<WM.getSelf().GetRightFootGloblePosition()<<"</rf>";
				o_msg<<"</pos>";
			o_msg<<"</rp>";
			//o_msg<<"<c>"<<int(WM.getMySimTime()*100)<<"</c>";
			o_msg<<"<av>";
			for (int i=0; i<MAX_TEAM_SIZE; i++)
			{
				if (WM.getTmm(i).isValid())
				{
					o_msg<<"<p>";
						o_msg<<"<t>Strive3D</t>";
						o_msg<<"<u>"<<WM.getTmm(i).GetUnum()<<"</u>";
						o_msg<<"<h>"<<WM.getTmm(i).pos<<"</h>";
						//o_msg<<"<h>"<<WM.getTmm(i).GetHeadGloblePosition()<<"</h>";
						o_msg<<"<lla>"<<WM.getTmm(i).GetLeftArmGloblePosition()<<"</lla>";
						o_msg<<"<rla>"<<WM.getTmm(i).GetRightArmGloblePosition()<<"</rla>";
						o_msg<<"<lf>"<<WM.getTmm(i).GetLeftFootGloblePosition()<<"</lf>";
						o_msg<<"<rf>"<<WM.getTmm(i).GetRightFootGloblePosition()<<"</rf>";
					o_msg<<"</p>";
				}
			}
			for (int i=0; i<MAX_TEAM_SIZE; i++)
			{
				if (WM.getOpp(i).isValid())
				{
					o_msg<<"<p>";
						o_msg<<"<t>Oponent</t>";
						o_msg<<"<u>"<<WM.getOpp(i).GetUnum()<<"</u>";
						o_msg<<"<h>"<<WM.getOpp(i).pos<<"</h>";
						//o_msg<<"<h>"<<WM.getOpp(i).GetHeadGloblePosition()<<"</h>";
						o_msg<<"<lla>"<<WM.getOpp(i).GetLeftArmGloblePosition()<<"</lla>";
						o_msg<<"<rla>"<<WM.getOpp(i).GetRightArmGloblePosition()<<"</rla>";
						o_msg<<"<lf>"<<WM.getOpp(i).GetLeftFootGloblePosition()<<"</lf>";
						o_msg<<"<rf>"<<WM.getOpp(i).GetRightFootGloblePosition()<<"</rf>";
					o_msg<<"</p>";
				}
			}
			o_msg<<"</av>";
		o_msg<<"</r>";
		//cout<<"AdvancedAgent::sendInfo2GuiDebugger-->o_msg="<<o_msg.str()<<endl;
		DEBUG_NET.PutMessage(o_msg.str());
	}
}
