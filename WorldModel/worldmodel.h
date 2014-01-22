/*
   Copyright (C) 2007, 2008 by Team Apollo
   Nanjing University of Posts and Telecommunications, China

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef WORLDMODEL_H
#define WORLDMODEL_H
#include <fstream>
#include <vector>
#include <cassert>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "types.h"
#include "matrix.h"
#include "Logger.h"
#include <queue>
#include "Parser.h"
#include "StaticModule.h"
#include "convexhullmanager.h"
#include "ball.h"
#include "drawing.h"
#include "newBall.h"
#define WM StaticModule<WorldModel>::getInstance()
#define MAX_TEAM_SIZE 11
using namespace salt;
using namespace std;

class Player;
class Self;
class Ball;
class FieldInfo;
class Parser;

class WorldModel
{
public:
	WorldModel();
	~WorldModel();
	void Init();
	void SetupVisionSenseMap();
	void Update(const std::string& message);
	void VisionOn() { mVisionOn = true; }
	bool getVisionState() { return mVisionOn; }
	bool getHitout() {return mCanHitout;}
	void HearingOn() { mHearingOn = true; }
	void setMySimTime  ( float simTime );
	float getMySimTime();
	VisionObject setVisionObjectMap ( std::string objName );
	void setVisionSenseMap (  VisionObject VO,float distance,float theta,float phi );
	const VisionSense & getVisionSense( VisionObject VO ) 
   	{
	    return mVisionSenseMap[VO];
	}
	void setHearInfo( HearInfo& hearinfo );
	Self& getSelf(){return *mSelf;}
	Ball& getBall(){return *mBall;}
	FieldInfo& getFieldInfo(){return *mFieldInfo;}
	
	Player& getTmm(int index);
	Player& getOpp(int index);
	
	Player& getTmmByNum(int uniNum){return getTmm(uniNum-1);}
	Player& getOppByNum(int uniNum){return getOpp(uniNum-1);}
	
	ConvexHullManager & getConvexHullManager() { return mConvexHullManager; }

	ConvexHullManager & getSafeConvexHullManager() { return mSafeConvexHullManager; }
	
	bool SeeBall() { return mICanSeeBall; }
	bool CanLocalize() { return mCanLocalize; }
	
	Player& getOurTeamNearestToBall()	{ return *mOurTeamNearestToBall; }
	Player& getOurTeamSecondToBall ()	{ return *mOurTeamSecondToBall; }
	Player& getOppTeamNearestToBall()	{ return *mOppTeamNearestToBall; }
	Player& getPlayerNearestToBall()	{ return *mPlayerNearestToBall;	}
	Player& getmOppTeamNearestToGoalie(){ return *mOppTeamNearestToGoalie; }
	Player& getOurForward1()	{ return *mRoleForward1; }
	Player& getOurForward2(){ return *mRoleForward2; }


	void updatedBetweenBallandOpp();
	bool Oppreadyforshoot();
	bool Oppshooting();

	bool Prepare;
	
	
	//added
	salt::Matrix getVisionPerceptorMatrix(){
	  return 	mVisionPerceptorMatrix;
	}
		
protected:
	
	// void OnUnLink();
	
	void ShiftInfo();
	
	void ResetInfo();
	
	void SetupVisionObjectMap();
		
	void CalculateVisionObjectLocalPos();
	
	salt::Matrix LocalizeWithThreeFlagsTriangle(	const VisionObject vo1,
										const VisionObject vo2,
										const VisionObject vo3);
	
	salt::Matrix LocalizeWithThreeFlagsLine(const VisionObject vo1,
									const VisionObject vo2,
									const VisionObject vo3);
						
	bool UpdateWithHearInfomation();
	
	bool Localize();
	
	void UpdateSelf();
	
	void UpdateBall();
	
	void UpdatePlayers();
	
	void UpdateFieldInfo();

	void AnalysePlayerPosition();
	
	void RoleAssignment_forward1();	

	void RoleAssignment_forward2();
	
	void IsCanHitout();
	
	//added by wenbin
	void updateNewBall();
	bool CanSeeBall(){
	  if(mVisionSenseMap[BALL].distance > 0)
	    iCanSeeBall = true;
	  else
	    iCanSeeBall = false;
	  return iCanSeeBall;
	}

private:
	Drawing *dr;
	NBall *nBall;
	bool iCanSeeBall;
	
	
	Parser *mParser;//Neil 2009.3

	Self *mSelf;
	Ball *mBall;
	FieldInfo *mFieldInfo;
	HearInfo mHearInfo;
	//2009.12.20 BY Qu Junjun
	Player* mOurTeamNearestToBall;
	Player* mOurTeamSecondToBall;
	Player*	mOppTeamNearestToBall;
	Player* mOppTeamNearestToGoalie;
	Player* mPlayerNearestToBall;
	Player* mRoleForward1;
	Player* mRoleForward2;
	//Zheng Yonglei 2009.1.19 [start]
	Player* mTmm[MAX_TEAM_SIZE];
	Player* mOpp[MAX_TEAM_SIZE];
	//Zheng Yonglei 2009.1.19 [end]

	typedef std::map<VisionObject, VisionSense> TVisionSenseMap;
	TVisionSenseMap mVisionSenseMap;
	
	typedef std::map<std::string, VisionObject> TVisionObjectMap;
	TVisionObjectMap mVisionObjectMap;
	
	/** current simulation time */
	float           	mSimTime;
	
	/** simulation time of last cycle */
	float          	 mLastSimTime;
	
	salt::Matrix   	mVisionPerceptorMatrix, mPrevVisionPerceptorMatrix;

	Matrix transformMat;//for change the Robot's world coordinate to Vision coordinate 

	int			mHearCycles;
	
	bool			mVisionOn;
	
	bool			mHearingOn;
	
	bool 			mCanLocalize;
	
	bool			mICanSeeBall;
	
	bool 			mCanHitout;
	
	ConvexHullManager mConvexHullManager; //By Qu Junjun 2009.10.4
	
	ConvexHullManager mSafeConvexHullManager;

//2009,12,23 by ianpan.
	queue<double> setdBetweenBallandOpp;
	queue<double> comparedbetweenBallandOpp;
};

#endif
