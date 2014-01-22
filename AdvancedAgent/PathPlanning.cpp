#include "PathPlanning.h"

PathPlanning::PathPlanning()
{
	//LOG.newCycle(0);
	LOG<<"PathPlanning::PathPlanning()-->A PathPlanning is constructed."<<endl;
	curCycle = INVALID_VALUE;
	simCycleMax = INVALID_VALUE;
	matchSimCycleMax = INVALID_VALUE;
	simStep = INVALID_VALUE;
	curCycle = INVALID_VALUE;
	destDistThreshold = INVALID_VALUE;
	mySpeedMax = INVALID_VALUE;
	destForce = INVALID_VALUE;
	oppForceIndex = INVALID_VALUE;
	virtualDestAngle = INVALID_VALUE;
	isBallPosValid = false;
	isDesPosValid = false;
	hasOpp = false;
	MyVel.clear();
	OppPos.clear();
	MyPos.clear();
	wayPoints.clear();
}

void PathPlanning::run()
{
	LOG<<"PathPlanning::run()"<<endl;
	if (checkMsg())
	{
		setCommonArg();
		//set geometry by messages sent from server
		setGeometryInMatch();

		//simulating
		for (int i=0;i<matchSimCycleMax;i++)
		{
			LOG<<STARS<<"New Cycle"<<STARS<<endl;
			curCycle = i;
			simulate();
			//printMFile();
			if ((DestPos - MyPos[i]).Length() < destDistThreshold)
			{
				LOG<<"PathPlanning::test()-->All simulation finished. Simulation cycle  count = "<<i+1<<endl;
				break;
			}
		}
		
		simFinished = true;
		//printMFile();
		if(generateWayPoint())
			generateMessage();
	}
}

void PathPlanning::prepareData()
{
	//Calculate opponents' distance to me
	OppPosDis2Me.clear();
	float dotVal;
	nearestValidOpp = -1;//nearestValidOpp = -1 means there is no opponent is in front me
	float me2OppDistMin = 1000;
	for (int i=0;i<OppPos.size();i++)
	{
		Vector2f me2Opp = OppPos[i] - MyPos[curCycle-1];
		Vector2f unitMe2Opp = me2Opp/(me2Opp.Length());
		Vector2f me2Dest = DestPos - MyPos[curCycle-1];
		Vector2f unitMe2Dest = me2Dest/(me2Dest.Length());
		dotVal = unitMe2Opp.Dot(unitMe2Dest);
		LOG<<"PathPlanning::prepareData()-->dotVal(opp["<<i<<"]) = "<<dotVal<<endl;
		if (dotVal > 0.707 && me2Opp.Length() < me2OppDistMin)//The opponent is in front of me
		{
			me2OppDistMin = me2Opp.Length();
			nearestValidOpp = i;
		}
	}
	if (nearestValidOpp >= 0)
	LOG<<"PathPlanning::prepareData()-->nearestValidOpp is "<<nearestValidOpp<<"\tPos = "<<OppPos[nearestValidOpp]<<endl;
}

void PathPlanning::setVirtualDest()
{
	//LOG<<"PathPlanning::setVirtualDest()"<<endl;
	needVirtualDestPos = false;
	if (nearestValidOpp >= 0)
	{
		Vector2f me2Opp = OppPos[nearestValidOpp] - MyPos[curCycle-1];
		Vector2f unitMe2Opp = me2Opp/(me2Opp.Length());
		Vector2f me2Dest = DestPos - MyPos[curCycle-1];
		Vector2f unitMe2Dest = me2Dest/(me2Dest.Length());
		float dotVel = unitMe2Opp.Dot(unitMe2Dest);
		if (dotVel > cos(salt::gDegToRad(virtualDestAngle)))
		{
			LOG<<"PathPlanning::setVirtualDest()-->needVirtualDestPos = true"<<endl;
			needVirtualDestPos = true;
			if (Cross(Vector2f(0,0),unitMe2Dest,unitMe2Opp) > 0)
			{
				LOG<<"PathPlanning::setVirtualDest()-->Anti-clock-wise"<<endl;
				float angle = unitMe2Dest.GetAngleDeg() - virtualDestAngle;
				LOG<<"PathPlanning::setVirtualDest()-->angle = "<<angle<<endl;
				virtualDestPos = MakeVector2f(angle);
				virtualDestPos = virtualDestPos*(dotVel * me2Opp.Length())/cos(salt::gDegToRad(virtualDestAngle));
			}
			else
			{
				LOG<<"PathPlanning::setVirtualDest()-->Clock-wise"<<endl;
				float angle = unitMe2Dest.GetAngleDeg() + virtualDestAngle;
				LOG<<"PathPlanning::setVirtualDest()-->angle = "<<angle<<endl;
				virtualDestPos = MakeVector2f(angle);
				virtualDestPos = virtualDestPos*(dotVel * me2Opp.Length())/cos(salt::gDegToRad(virtualDestAngle));
			}
			virtualDestPos += MyPos[curCycle-1];
			LOG<<"PathPlanning::setVirtualDest()-->virtualDestPos = "<<virtualDestPos<<endl;
		}
	}
}

void PathPlanning::calculateForce()
{
	LOG<<"PathPlanning::calculateForce()"<<endl;
	LOG<<"PathPlanning::calculateForce()-->curCycle = "<<curCycle<<endl;
	prepareData();
	//Set a virtual destination if is needed
	setVirtualDest();
	
	MyForce.Zero();
	Vector2f curDestPos;
	if (needVirtualDestPos)
		curDestPos = virtualDestPos;
	else
		curDestPos = DestPos;
	//int forceNum = 0;
	if (isDesPosValid) //Calculate the influence from destination. This is the major factor.
	{
		LOG<<"PathPlanning::calculateForce()-->MyPos[curCycle-1] = "<<MyPos[curCycle-1]<<"\tcurDestPos = "<<curDestPos<<endl;
		Vector2f direction = curDestPos - MyPos[curCycle-1];
		//BallPos+MyStartPos;
		float distance = direction.Length();
		direction = direction/distance;
		//LOG<<"PathPlanning::calculateForce()-->direction(DestPos) = "<<direction<<endl;
		LOG<<"PathPlanning::calculateForce()-->distance(DestPos) = "<<distance<<endl;
		//float absForce = distFactor * pow(distance,-2);
		//float absForce = distFactor * distance;
		//absForce for destination position
		//float absForce = 10.0;
		
		Vector2f tempForce = direction * destForce;
		MyForce += tempForce;
		LOG<<"PathPlanning::calculateForce()-->MyForce(DestPos) = "<<tempForce<<endl;
		//Vector field = distFactor * 
	}
	
	//float oppDistFactor = -200.0;
	for (int i=0;i<oppNum;i++)//Calculate the influence from opponents. 
	{
		LOG<<"PathPlanning::calculateForce()-->oppNum = "<<oppNum<<endl;
		Vector2f direction = OppPos[i] -  MyPos[curCycle-1];
		float distance = direction.Length() - oppSize;
		direction = direction/distance;
		//LOG<<"PathPlanning::calculateForce()-->direction(DestPos) = "<<direction<<endl;
		LOG<<"PathPlanning::calculateForce()-->distance(OppPos["<<i<<"]) = "<<distance<<endl;
		
		//float absForce = distFactor * pow(distance,-2);
		float absForce = oppDistFactor * pow(distance,oppForceIndex);
		Vector2f tempForce = direction * absForce;
		MyForce += tempForce;
		LOG<<"PathPlanning::calculateForce()-->MyForce(OppPos["<<i<<"]) = "<<tempForce<<endl;
	}
	
	LOG<<"PathPlanning::calculateForce()-->MyForce(All) = "<<MyForce<<endl;
	
}

void PathPlanning::simulate()
{
	LOG<<"PathPlanning::simulate()"<<endl;
	if (curCycle > 0)
	{
		calculateForce();
		MyVel.push_back(MyVel[curCycle-1] + MyForce*simStep/massFactor);
		float absVel = MyVel[curCycle].Length();
		if (absVel > mySpeedMax)
		{
			LOG<<"PathPlanning::simulate()-->Speed exceeded."<<endl;
			Vector2f unitVel = MyVel[curCycle]/absVel;
			MyVel[curCycle] = unitVel * mySpeedMax;
		}
		//MyPos.push_back(MyPos[curCycle-1] + MyForce*simStep);
		MyPos.push_back(MyPos[curCycle-1] + MyVel[curCycle]*simStep);
	}
	
	LOG<<"PathPlanning::simulate()-->MyVel["<<curCycle<<"] = "<<MyVel[curCycle]<<endl;
	LOG<<"PathPlanning::simulate()-->MyPos["<<curCycle<<"] = "<<MyPos[curCycle]<<endl;
}

void PathPlanning::test()
{
	LOG<<"PathPlanning::test()"<<endl;
	setCommonArg();
	//setGeometryByConfig is used only in test mode
	setGeometryByConfig();
	//simulating
	for (int i=0;i<simCycleMax;i++)
	{
		//LOG.newCycle(i+1);
		//update simulation cycle
		LOG<<STARS<<"New Cycle"<<STARS<<endl;
		curCycle = i;
		simulate();
		printMFile();
		if ((DestPos - MyPos[i]).Length() < destDistThreshold)
		{
			LOG<<"PathPlanning::test()-->All simulation finished. Simulation cycle  count = "<<i+1<<endl;
			break;
		}
	}
	
	simFinished = true;
	printMFile();
	if(generateWayPoint())
		generateMessage();
}

void PathPlanning::setCommonArg()
{
	LOG<<"PathPlanning::setCommonArg()"<<endl;
	simCycleMax = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/simCycleMax").asInt();
	simStep = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/simStep").asFloat();
	mySpeedMax = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/mySpeedMax").asFloat();
	destForce = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/destForce").asFloat();
	hasOpp = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/hasOpp").asBool();
	massFactor = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/massFactor").asFloat();
	oppSize = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/oppSize").asFloat();
	destDistThreshold = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/destDistThreshold").asFloat();
	oppForceIndex = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/oppForceIndex").asFloat();
	virtualDestAngle = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/virtualDestAngle").asFloat();
}

void PathPlanning::setGeometryByConfig()
{
	LOG<<"PathPlanning::setGeometryByConfig()"<<endl;
	float x,y;
	
	//Set MyStartPos (start position)
	x = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/MyStartPos/x").asFloat();
	y = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/MyStartPos/y").asFloat();
	MyStartPos = Vector2f(x,y);
	
	//Set MyStartVel (velocity)
	x = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/MyStartVel/x").asFloat();
	y = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/MyStartVel/y").asFloat();
	MyStartVel = Vector2f(x,y);
	
	//Set DestPos (destination position)
	x = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/DestPos/x").asFloat();
	y = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/DestPos/y").asFloat();
	DestPos = Vector2f(x,y);
	isDesPosValid = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/DestPos/isValid").asBool();
	
	//Set BallPos (not use now)
	x = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/BallPos/x").asFloat();
	y = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/BallPos/y").asFloat();
	BallPos = Vector2f(x,y);
	isBallPosValid = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/BallPos/isValid").asBool();
	
	MyPos.clear();
	MyPos.push_back(MyStartPos);
	MyVel.clear();
	MyVel.push_back(MyStartVel);
	
	//no opponent
	OppPos.clear();
	if (hasOpp)
	{
		int validOppNum = 0;
		oppNum = CONF.getXMLValueCount("/Strive/AdvancedAgent/PathPlanning/OppPos");
		for (int i=0;i<oppNum;i++)
		{
			if (CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/OppPos/isValid",i).asBool())
			{
				validOppNum++;
				x = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/OppPos/x",i).asFloat();
				y = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/OppPos/y",i).asFloat();
				OppPos.push_back(Vector2f(x,y));
			}
		}
		oppNum = validOppNum;
		
		oppDistFactor = CONF.getXMLValue("/Strive/AdvancedAgent/PathPlanning/oppDistFactor").asFloat();
	}
	
	curCycle = 0;
	simFinished = false;
	
	prepareMFile();
}

void PathPlanning::setGeometryInMatch()
{
	LOG<<"PathPlanning::setGeometryInMatch"<<endl;
	ObsPos.clear();
	for (int i=0;i<MAX_TEAM_SIZE;i++)
	{
		if (WM.getOpp(i).isValid())
		{
			LOG<<"PathPlanning::run()-->opponent["<<i<<"] is valid"<<endl;
			if (AA.getTactics() == T_OFFENSE)
				ObsPos.push_back(WM.getOpp(i).pos.to2D());
		}
	}
		
	for (int i=0;i<MAX_TEAM_SIZE;i++)
	{
		if (WM.getTmm(i).isValid())
		{
			LOG<<"PathPlanning::run()-->teammate["<<i<<"] is valid"<<endl;
			if (AA.getTactics() == T_OFFENSE)
				ObsPos.push_back(WM.getTmm(i).pos.to2D());
		}
	}
}

void PathPlanning::prepareMFile()
{
	m_file.precision(7);
	m_file.open("matlab/data.m",ios::out);
}

void PathPlanning::printMFile()
{
	if (curCycle == 0)
	{
		m_file<<"%Strive3D soccer simulation team.(2008)"<<endl<<"%This file is created by Zheng Yonglei"<<endl;
		m_file<<"MyStartPos = ["<<MyStartPos<<"];"<<endl;
		m_file<<"DestPos = ["<<DestPos<<"];"<<endl;
		m_file<<"OppPos = {"<<endl;
		for (int i=0;i<oppNum;i++)
		{
			m_file<<OppPos[i]<<endl;
		}
		m_file<<"};"<<endl;
		
		m_file << "MyPos = {"<<endl;
		m_file <<MyPos[curCycle]<<endl;
	}
	else if (!simFinished)
	{
		m_file <<MyPos[curCycle]<<endl;
	}
	else
	{
		m_file << "};" <<endl;
		m_file.close();
	}
}

bool PathPlanning::generateWayPoint()
{
	LOG<<"PathPlanning::generateWayPoint()"<<endl;
	wayPoints.clear();
	wayPointsVelUnit.clear();
	
	for(int i=0;i<MyPos.size();i++)
	{
		if (i%4 == 0 && i<=MyPos.size()-2)
		{
			wayPoints.push_back(MyPos[i]);
			Vector2f vel = MyPos[i+1] - MyPos[i];
			Vector2f unitVel = vel/(vel.Length());
			wayPointsVelUnit.push_back(unitVel);
		}
	}
	//assert(wayPointsVelUnit.size() == wayPoints.size());
	
	//Print results
	for (int i=1;i<wayPoints.size();i++)
	{
		LOG<<"PathPlanning::generateWayPoint()-->Distance between wayPoint["<<i-1<<"] and wayPoint["<<i<<"] is "<<(wayPoints[i-1] - wayPoints[i]).Length()<<endl;
	}
	
	if (wayPoints.size() > 1)
		return true;
	else
		return false;
}

void PathPlanning::generateMessage()
{
	/*
	eg.
	(WayPoints (1 2.32 32.4) (2 3.1 33.1) (3 40.23 3.1) ...)
	*/
	LOG<<"PathPlanning::generateMessage()"<<endl;
	ostringstream o_msg;
	std::string s_msg;
	if (wayPoints.empty())
	{
		//cout<<"PathPlanning::generateMessage()-->Error! No way points."<<endl;
		LOG<<"PathPlanning::generateMessage()-->Error! No way points."<<endl;
		return;
	}
	
	o_msg<<"(WayPoints";
	for (int i=0;i<wayPoints.size();i++)
	{
		o_msg<<" (w "<<wayPoints[i]<<" "<<wayPointsVelUnit[i]<<")";
	}
	o_msg<<")";
	s_msg = o_msg.str();
	MS.setPathPlanning2ActionMsg(s_msg);
	LOG<<"PathPlanning::generateMessage()-->message = "<<s_msg<<endl;
	PARSER.parserWayPoints(s_msg);
}

bool PathPlanning::checkMsg()
{
	LOG<<"PathPlanning::checkMsg()"<<endl;
	LOG<<"WM.getMySimTime() = "<<WM.getMySimTime()<<"\tMS.getPlan2PathPlanningUpdateCycle() = "<<MS.getPlan2PathPlanningUpdateCycle()<<endl;
	if (WM.getMySimTime() == MS.getPlan2PathPlanningUpdateCycle())
	{
		LOG<<"PathPlanning::checkMsg()-->Plan2PathPlanning message updated."<<endl;
		return true;
	}
	else
		return false;
}
