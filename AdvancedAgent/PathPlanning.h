#ifndef __PATH_PLANNING_H
#define __PATH_PLANNING_H

#include <vector>
#include <cassert>
#include <cmath>
#include <sstream>
#include <fstream>
#include <cmath>
#include "Logger.h"
#include "StaticModule.h"
#include "vector.h"
#include "xmlParser.h"
#include "MessageStorage.h"
#include "geometry.h"
#include "Parser.h"
#include "AdvancedAgent.h"
#include "worldmodel.h"

#define PP StaticModule<PathPlanning>::getInstance()
#define INVALID_VALUE -100.0

using namespace salt;

//class PathPlanning;

class PathPlanning
{
protected:
	
	Vector2f MyStartPos;
	Vector2f MyStartVel;
	Vector2f MyForce;
	Vector2f BallPos;
	Vector2f DestPos;
	Vector2f virtualDestPos;
	bool needVirtualDestPos;
	vector<Vector2f>MyVel;
	vector<Vector2f>OppPos;
	vector<Vector2f>ObsPos;//obstacle position
	vector<float>OppPosDis2Me;
	int nearestValidOpp;
	vector<Vector2f>MyPos;
	
	vector<Vector2f>wayPoints;//This is the final result given by the path planning system, which is used to generate the way points' s-expressions.
	vector<Vector2f>wayPointsVelUnit;//The unit velocities of the agent in the waypints.
	
	bool isBallPosValid;
	bool isDesPosValid;
	int oppNum;
	
	int simCycleMax;
	int matchSimCycleMax;
	int curCycle;
	float simStep;
	bool simFinished;
	float destDistThreshold;
	float virtualDestAngle;
	
	float oppDistFactor;
	float oppForceIndex;
	float oppSize;
	float massFactor;
	float mySpeedMax;
	float destForce;
	bool hasOpp;
	
	fstream m_file;//used in matlab
	
public:
	PathPlanning();
	
	void run();
	void calculateForce();
	void prepareData();
	void simulate();
	void test();
	void setCommonArg();
	void setGeometryByConfig();//Just for test
	void setGeometryInMatch();//for match
	void setVirtualDest();
	
	void printMFile();//M-file is Matlab format file. It is used to show the real path generated by this class.
	void prepareMFile();
	
	bool generateWayPoint();
	void generateMessage();
	
	bool checkMsg();
};

#endif //__PATH_PLANNING_H