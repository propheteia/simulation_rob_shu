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

#include <iostream>

#include "worldmodel.h"
//#include <zeitgeist/logserver/logserver.h>
#include <sstream>
#include "self.h"
#include "fieldinfo.h"
#include "ball.h"
#include "geometry.h"
#include <cctype>
#include "Utilities.h"
#include "newBall.h"

using namespace std;
using namespace boost;
using namespace salt;

WorldModel::WorldModel() :mHearCycles(0),mSimTime(0.0f),mLastSimTime(0.0f)
{	
	mParser = new Parser();
	mSelf = new Self();
	mBall = new Ball();
	mFieldInfo = new FieldInfo();
	// add players model
	for (int i = 0; i <MAX_TEAM_SIZE; i++)
	{
		mTmm[i] = new Player;
		mTmm[i]->setUnum(i+1);
		mTmm[i]->setIsValid(false);
	}
	mRoleForward1 = mTmm[0];
	for (int i = 0; i <MAX_TEAM_SIZE; i++)
	{
		mOpp[i] = new Player;
		mOpp[i]->setUnum(i+1);
		mOpp[i]->setIsValid(false);
	}
	// some flags
	mCanLocalize = false;
	mICanSeeBall = false;
	mCanHitout = false;
	Init();
    mOppTeamNearestToBall = NULL; 
    // added on 1.23
    dr = new Drawing();
    nBall = new NBall();
    
}

WorldModel::~WorldModel()
{

}

void WorldModel::Init()
{	
    mVisionPerceptorMatrix.Identity();
    SetupVisionObjectMap();
}

void WorldModel::SetupVisionObjectMap()
{
    mVisionObjectMap.clear();
    mVisionObjectMap["F1L"]         = FLAG_1_L; //0
    mVisionObjectMap["F1R"]         = FLAG_1_R; //1
    mVisionObjectMap["F2L"]         = FLAG_2_L; //2
    mVisionObjectMap["F2R"]         = FLAG_2_R; //3
    mVisionObjectMap["G1L"]         = GOAL_1_L; //4
    mVisionObjectMap["G1R"]         = GOAL_1_R; //5
    mVisionObjectMap["G2L"]         = GOAL_2_L; //6
    mVisionObjectMap["G2R"]         = GOAL_2_R; //7
    mVisionObjectMap["B"]           = BALL;

    mVisionObjectMap["TEAMMATE1HEAD"]      = TEAMMATE_1_HEAD;
    mVisionObjectMap["TEAMMATE1LEFTARM"]   = TEAMMATE_1_LEFTARM;
    mVisionObjectMap["TEAMMATE1RIGHTARM"]  = TEAMMATE_1_RIGHTARM;
    mVisionObjectMap["TEAMMATE1LEFTFOOT"]  = TEAMMATE_1_LEFTFOOT;
    mVisionObjectMap["TEAMMATE1RIGHTFOOT"] = TEAMMATE_1_RIGHTFOOT;

    mVisionObjectMap["TEAMMATE2HEAD"]      = TEAMMATE_2_HEAD;
    mVisionObjectMap["TEAMMATE2LEFTARM"]   = TEAMMATE_2_LEFTARM;
    mVisionObjectMap["TEAMMATE2RIGHTARM"]  = TEAMMATE_2_RIGHTARM;
    mVisionObjectMap["TEAMMATE2LEFTFOOT"]  = TEAMMATE_2_LEFTFOOT;
    mVisionObjectMap["TEAMMATE2RIGHTFOOT"] = TEAMMATE_2_RIGHTFOOT;

    mVisionObjectMap["TEAMMATE3HEAD"]      = TEAMMATE_3_HEAD;
    mVisionObjectMap["TEAMMATE3LEFTARM"]   = TEAMMATE_3_LEFTARM;
    mVisionObjectMap["TEAMMATE3RIGHTARM"]  = TEAMMATE_3_RIGHTARM;
    mVisionObjectMap["TEAMMATE3LEFTFOOT"]  = TEAMMATE_3_LEFTFOOT;
    mVisionObjectMap["TEAMMATE3RIGHTFOOT"] = TEAMMATE_3_RIGHTFOOT;

    mVisionObjectMap["TEAMMATE4HEAD"]      = TEAMMATE_4_HEAD;
    mVisionObjectMap["TEAMMATE4LEFTARM"]   = TEAMMATE_4_LEFTARM;
    mVisionObjectMap["TEAMMATE4RIGHTARM"]  = TEAMMATE_4_RIGHTARM;
    mVisionObjectMap["TEAMMATE4LEFTFOOT"]  = TEAMMATE_4_LEFTFOOT;
    mVisionObjectMap["TEAMMATE4RIGHTFOOT"] = TEAMMATE_4_RIGHTFOOT;

    mVisionObjectMap["TEAMMATE5HEAD"]      = TEAMMATE_5_HEAD;
    mVisionObjectMap["TEAMMATE5LEFTARM"]   = TEAMMATE_5_LEFTARM;
    mVisionObjectMap["TEAMMATE5RIGHTARM"]  = TEAMMATE_5_RIGHTARM;
    mVisionObjectMap["TEAMMATE5LEFTFOOT"]  = TEAMMATE_5_LEFTFOOT;
    mVisionObjectMap["TEAMMATE5RIGHTFOOT"] = TEAMMATE_5_RIGHTFOOT;
    
    mVisionObjectMap["TEAMMATE6HEAD"]      = TEAMMATE_6_HEAD;
    mVisionObjectMap["TEAMMATE6LEFTARM"]   = TEAMMATE_6_LEFTARM;
    mVisionObjectMap["TEAMMATE6RIGHTARM"]  = TEAMMATE_6_RIGHTARM;
    mVisionObjectMap["TEAMMATE6LEFTFOOT"]  = TEAMMATE_6_LEFTFOOT;
    mVisionObjectMap["TEAMMATE6RIGHTFOOT"] = TEAMMATE_6_RIGHTFOOT;

    mVisionObjectMap["TEAMMATE7HEAD"]      = TEAMMATE_7_HEAD;
    mVisionObjectMap["TEAMMATE7LEFTARM"]   = TEAMMATE_7_LEFTARM;
    mVisionObjectMap["TEAMMATE7RIGHTARM"]  = TEAMMATE_7_RIGHTARM;
    mVisionObjectMap["TEAMMATE7LEFTFOOT"]  = TEAMMATE_7_LEFTFOOT;
    mVisionObjectMap["TEAMMATE7RIGHTFOOT"] = TEAMMATE_7_RIGHTFOOT;
    
    mVisionObjectMap["TEAMMATE8HEAD"]      = TEAMMATE_8_HEAD;
    mVisionObjectMap["TEAMMATE8LEFTARM"]   = TEAMMATE_8_LEFTARM;
    mVisionObjectMap["TEAMMATE8RIGHTARM"]  = TEAMMATE_8_RIGHTARM;
    mVisionObjectMap["TEAMMATE8LEFTFOOT"]  = TEAMMATE_8_LEFTFOOT;
    mVisionObjectMap["TEAMMATE8RIGHTFOOT"] = TEAMMATE_8_RIGHTFOOT;

    mVisionObjectMap["TEAMMATE9HEAD"]      = TEAMMATE_9_HEAD;
    mVisionObjectMap["TEAMMATE9LEFTARM"]   = TEAMMATE_9_LEFTARM;
    mVisionObjectMap["TEAMMATE9RIGHTARM"]  = TEAMMATE_9_RIGHTARM;
    mVisionObjectMap["TEAMMATE9LEFTFOOT"]  = TEAMMATE_9_LEFTFOOT;
    mVisionObjectMap["TEAMMATE9RIGHTFOOT"] = TEAMMATE_9_RIGHTFOOT;

    mVisionObjectMap["TEAMMATE10HEAD"]     = TEAMMATE_10_HEAD;
    mVisionObjectMap["TEAMMATE10LEFTARM"]  = TEAMMATE_10_LEFTARM;
    mVisionObjectMap["TEAMMATE10RIGHTARM"] = TEAMMATE_10_RIGHTARM;
    mVisionObjectMap["TEAMMATE10LEFTFOOT"] = TEAMMATE_10_LEFTFOOT;
    mVisionObjectMap["TEAMMATE10RIGHTFOOT"]= TEAMMATE_10_RIGHTFOOT;

    mVisionObjectMap["TEAMMATE11HEAD"]     = TEAMMATE_11_HEAD;
    mVisionObjectMap["TEAMMATE11LEFTARM"]  = TEAMMATE_11_LEFTARM;
    mVisionObjectMap["TEAMMATE11RIGHTARM"] = TEAMMATE_11_RIGHTARM;
    mVisionObjectMap["TEAMMATE11LEFTFOOT"] = TEAMMATE_11_LEFTFOOT;
    mVisionObjectMap["TEAMMATE11RIGHTFOOT"]= TEAMMATE_11_RIGHTFOOT;

    mVisionObjectMap["OPPONENT1HEAD"]      = OPPONENT_1_HEAD;
    mVisionObjectMap["OPPONENT1LEFTARM"]   = OPPONENT_1_LEFTARM;
    mVisionObjectMap["OPPONENT1RIGHTARM"]  = OPPONENT_1_RIGHTARM;
    mVisionObjectMap["OPPONENT1LEFTFOOT"]  = OPPONENT_1_LEFTFOOT;
    mVisionObjectMap["OPPONENT1RIGHTFOOT"] = OPPONENT_1_RIGHTFOOT;
    
    mVisionObjectMap["OPPONENT2HEAD"]      = OPPONENT_2_HEAD;
    mVisionObjectMap["OPPONENT2LEFTARM"]   = OPPONENT_2_LEFTARM;
    mVisionObjectMap["OPPONENT2RIGHTARM"]  = OPPONENT_2_RIGHTARM;
    mVisionObjectMap["OPPONENT2LEFTFOOT"]  = OPPONENT_2_LEFTFOOT;
    mVisionObjectMap["OPPONENT2RIGHTFOOT"] = OPPONENT_2_RIGHTFOOT;

    mVisionObjectMap["OPPONENT3HEAD"]      = OPPONENT_3_HEAD;
    mVisionObjectMap["OPPONENT3LEFTARM"]   = OPPONENT_3_LEFTARM;
    mVisionObjectMap["OPPONENT3RIGHTARM"]  = OPPONENT_3_RIGHTARM;
    mVisionObjectMap["OPPONENT3LEFTFOOT"]  = OPPONENT_3_LEFTFOOT;
    mVisionObjectMap["OPPONENT3RIGHTFOOT"] = OPPONENT_3_RIGHTFOOT;
    
    mVisionObjectMap["OPPONENT4HEAD"]      = OPPONENT_4_HEAD;
    mVisionObjectMap["OPPONENT4LEFTARM"]   = OPPONENT_4_LEFTARM;
    mVisionObjectMap["OPPONENT4RIGHTARM"]  = OPPONENT_4_RIGHTARM;
    mVisionObjectMap["OPPONENT4LEFTFOOT"]  = OPPONENT_4_LEFTFOOT;
    mVisionObjectMap["OPPONENT4RIGHTFOOT"] = OPPONENT_4_RIGHTFOOT;

    mVisionObjectMap["OPPONENT5HEAD"]      = OPPONENT_5_HEAD;
    mVisionObjectMap["OPPONENT5LEFTARM"]   = OPPONENT_5_LEFTARM;
    mVisionObjectMap["OPPONENT5RIGHTARM"]  = OPPONENT_5_RIGHTARM;
    mVisionObjectMap["OPPONENT5LEFTFOOT"]  = OPPONENT_5_LEFTFOOT;
    mVisionObjectMap["OPPONENT5RIGHTFOOT"] = OPPONENT_5_RIGHTFOOT;

    mVisionObjectMap["OPPONENT6HEAD"]      = OPPONENT_6_HEAD;
    mVisionObjectMap["OPPONENT6LEFTARM"]   = OPPONENT_6_LEFTARM;
    mVisionObjectMap["OPPONENT6RIGHTARM"]  = OPPONENT_6_RIGHTARM;
    mVisionObjectMap["OPPONENT6LEFTFOOT"]  = OPPONENT_6_LEFTFOOT;
    mVisionObjectMap["OPPONENT6RIGHTFOOT"] = OPPONENT_6_RIGHTFOOT;

    mVisionObjectMap["OPPONENT7HEAD"]      = OPPONENT_7_HEAD;
    mVisionObjectMap["OPPONENT7LEFTARM"]   = OPPONENT_7_LEFTARM;
    mVisionObjectMap["OPPONENT7RIGHTARM"]  = OPPONENT_7_RIGHTARM;
    mVisionObjectMap["OPPONENT7LEFTFOOT"]  = OPPONENT_7_LEFTFOOT;
    mVisionObjectMap["OPPONENT7RIGHTFOOT"] = OPPONENT_7_RIGHTFOOT;

    mVisionObjectMap["OPPONENT8HEAD"]      = OPPONENT_8_HEAD;
    mVisionObjectMap["OPPONENT8LEFTARM"]   = OPPONENT_8_LEFTARM;
    mVisionObjectMap["OPPONENT8RIGHTARM"]  = OPPONENT_8_RIGHTARM;
    mVisionObjectMap["OPPONENT8LEFTFOOT"]  = OPPONENT_8_LEFTFOOT;
    mVisionObjectMap["OPPONENT8RIGHTFOOT"] = OPPONENT_8_RIGHTFOOT;

    mVisionObjectMap["OPPONENT9HEAD"]      = OPPONENT_9_HEAD;
    mVisionObjectMap["OPPONENT9LEFTARM"]   = OPPONENT_9_LEFTARM;
    mVisionObjectMap["OPPONENT9RIGHTARM"]  = OPPONENT_9_RIGHTARM;
    mVisionObjectMap["OPPONENT9LEFTFOOT"]  = OPPONENT_9_LEFTFOOT;
    mVisionObjectMap["OPPONENT9RIGHTFOOT"] = OPPONENT_9_RIGHTFOOT;

    mVisionObjectMap["OPPONENT10HEAD"]     = OPPONENT_10_HEAD;
    mVisionObjectMap["OPPONENT10LEFTARM"]  = OPPONENT_10_LEFTARM;
    mVisionObjectMap["OPPONENT10RIGHTARM"] = OPPONENT_10_RIGHTARM;
    mVisionObjectMap["OPPONENT10LEFTFOOT"] = OPPONENT_10_LEFTFOOT;
    mVisionObjectMap["OPPONENT10RIGHTFOOT"]= OPPONENT_10_RIGHTFOOT;

    mVisionObjectMap["OPPONENT11HEAD"]     = OPPONENT_11_HEAD;
    mVisionObjectMap["OPPONENT11LEFTARM"]  = OPPONENT_11_LEFTARM;
    mVisionObjectMap["OPPONENT11RIGHTARM"] = OPPONENT_11_RIGHTARM;
    mVisionObjectMap["OPPONENT11LEFTFOOT"] = OPPONENT_11_LEFTFOOT;
    mVisionObjectMap["OPPONENT11RIGHTFOOT"]= OPPONENT_11_RIGHTFOOT;
}

void WorldModel::SetupVisionSenseMap()
{
    mVisionSenseMap.clear();
    int tmindex = 1;

    const float flagHeight = 0.0f; // 0.375f;
    const float goalFlagX = mFieldInfo->fieldlength/2.0; // + mFieldInfo->goaldepth/2.0; NOTE
    const float goalFlagHeight = mFieldInfo->goalheight;

    mVisionSenseMap[FLAG_1_L].realPos  = Vector3f(-mFieldInfo->fieldlength/2.0*tmindex, 
                                                  +mFieldInfo->fieldwidth/2.0*tmindex, 
                                                  flagHeight);
    mVisionSenseMap[FLAG_1_R].realPos  = Vector3f(+mFieldInfo->fieldlength/2.0*tmindex, 
                                                  +mFieldInfo->fieldwidth/2.0*tmindex, 
                                                  flagHeight);
    mVisionSenseMap[FLAG_2_L].realPos  = Vector3f(-mFieldInfo->fieldlength/2.0*tmindex, 
                                                  -mFieldInfo->fieldwidth/2.0*tmindex, 
                                                  flagHeight);
    mVisionSenseMap[FLAG_2_R].realPos  = Vector3f(+mFieldInfo->fieldlength/2.0*tmindex, 
                                                  -mFieldInfo->fieldwidth/2.0*tmindex, 
                                                  flagHeight);
    mVisionSenseMap[GOAL_1_L].realPos  = Vector3f(-goalFlagX*tmindex, 
                                                  +mFieldInfo->goalwidth/2.0*tmindex,
                                                  goalFlagHeight);
    mVisionSenseMap[GOAL_1_R].realPos  = Vector3f(+goalFlagX*tmindex, 
                                                  +mFieldInfo->goalwidth/2.0*tmindex,
                                                  goalFlagHeight);
    mVisionSenseMap[GOAL_2_L].realPos  = Vector3f(-goalFlagX*tmindex, 
                                                  -mFieldInfo->goalwidth/2.0*tmindex,
                                                  goalFlagHeight);
    mVisionSenseMap[GOAL_2_R].realPos  = Vector3f(+goalFlagX*tmindex, 
                                                  -mFieldInfo->goalwidth/2.0*tmindex,
                                                  goalFlagHeight);
}

void WorldModel::setMySimTime ( float simTime)
{	mSimTime = simTime;
}

float WorldModel::getMySimTime()
{
	return mSimTime;
}

VisionObject WorldModel::setVisionObjectMap ( string objName )
{	VisionObject vo;
        TVisionObjectMap::iterator vo_iter = mVisionObjectMap.find(objName);
        if (vo_iter == mVisionObjectMap.end())
        {
            	pLOG << "[WorldModel] unknown object name: " << objName << endl;
        }
        vo = mVisionObjectMap[objName];
	return vo;
}

void WorldModel::setVisionSenseMap ( VisionObject VO,float distance,float theta,float phi)
{	mVisionSenseMap[VO].distance  = distance;
    mVisionSenseMap[VO].theta     = theta;
    mVisionSenseMap[VO].phi       = phi;
}

void WorldModel::ShiftInfo()
{
    mLastSimTime = mSimTime;
}

void WorldModel::ResetInfo()
{
    aLOG<<"### Reset"<<endl;

	WM.getSelf().mLFForce.Zero();  
	WM.getSelf().mRFForce.Zero();
	WM.getSelf().mLFCenter.Zero(); 
	WM.getSelf().mRFCenter.Zero();
	WM.getSelf().mGyroAngularVel.Zero();
    
	mVisionOn = false;
	mHearingOn = false;
	mConvexHullManager.Clear();
	mSafeConvexHullManager.Clear();
	mOurTeamNearestToBall = mSelf;
	mPlayerNearestToBall = mSelf;
	//rest Player
	for (int i = (int)(FLAG_1_L); i <= (int)(OPPONENT_11_RIGHTFOOT); ++i)
	{
		VisionSense &vs = mVisionSenseMap[static_cast<VisionObject>(i)];
		vs.distance = -1000;
		vs.distanceToSelf = -1000;
	}
	
}

void WorldModel::Update(const string& message)
{
    aLOG<<"############ Update Start!  #############"<<endl;
	//prepare info
	ShiftInfo();
	ResetInfo();
	//parse message 
	mParser->parseAll(message);
	//calculate local pos of everything I see
	CalculateVisionObjectLocalPos();
	//update: localize ,update matrix and renew ( calculate pos,state and area)
	UpdateSelf();
    //Set local and global pos (if can localize),  renew
	UpdateBall();
	updateNewBall();
    //update hear info
	UpdateWithHearInfomation();
	if(mCanLocalize)
	{
        // Set local pos and global pos of player's body part including myself 
        // Head pos represent Player's pos
		UpdatePlayers();
	}
	if(mVisionOn)
	UpdateFieldInfo();
	AnalysePlayerPosition();
	updatedBetweenBallandOpp();

	if(SeeBall())
	{
		aLOG<<"### Ball Info"<<endl;
		aLOG << "Ball Pos: " << mBall->pos << endl;
		aLOG << "The Nearest Player To Ball : ";
		Player & nearest = getPlayerNearestToBall();
		aLOG << nearest.GetUnum() << endl;
		aLOG << "Ball's vel: " << mBall->vel << endl;
	}
	else
		aLOG << "I Can't See The Ball!" << endl;
	mSafeConvexHullManager.AddArea(mBall->area);
    //renew (state and area)
	for (int i=0; i< MAX_TEAM_SIZE; i++)
	{
		aLOG<<"### TeamMate Info"<<endl;
		if(mTmm[i]->isValid())// I know player's position
		{
			mTmm[i]->renew();
			mConvexHullManager.AddArea(mTmm[i]->area);
			mSafeConvexHullManager.AddArea((mTmm[i]->GetSafeArea()));
			aLOG << "TeamMate " << mTmm[i]->GetUnum() << " State: " << mTmm[i]->GetState() << endl;
		}
		if(mOpp[i]->isValid())
		{
			mOpp[i]->renew();
			mConvexHullManager.AddArea(mOpp[i]->area);
			mSafeConvexHullManager.AddArea((mOpp[i]->GetSafeArea()));
			aLOG << "TeamMate " << mOpp[i]->GetUnum() << " State: " << mOpp[i]->GetState() << endl;
		}
	}
	if(mVisionOn)
	{
		mConvexHullManager.MergeConvexHull();
		mSafeConvexHullManager.MergeConvexHull();
		vector< vector<Vector3f> > & ConvexHullList = getSafeConvexHullManager().GetConvexHull();
		for(int i = 0; i < ConvexHullList.size(); i++)
		{
			aLOG << "ConvexHull No." << i << endl;		
			for(int j = 0; j < ConvexHullList[i].size(); j++)
			{
				aLOG << ConvexHullList[i][j] << endl;;
			}
		}
	}
    //if I can't localize ,use hear info 
    if(!mCanLocalize && SeeBall())
    {
        aLOG << "### Using Hearing Infomation!" << endl;
        mSelf->mTorsoYawAngle = mSelf->TorsoYawAngleByHearing;
        mSelf->pos = mSelf->posbyhearing;
        mBall->pos = mBall->posbyhearing;
        aLOG << "Time: " << WM.getFieldInfo().match_time <<  "\tTorsoYawAngle: " << mSelf->mTorsoYawAngle << endl;
        aLOG << "Selfpos: " << mSelf->pos << "\tBallpos: " << mBall->pos << endl;
    }
	if (mSimTime > 1.0f && gAbs(mSimTime - mLastSimTime - 0.02) > 1e-3)
		cerr << "(Time Jump) "<< mLastSimTime << " " << mSimTime << endl;
    aLOG<<"################ Update Finished  #################"<<endl;
    dr->draw_trajectory(mBall->pos);
}

void WorldModel::UpdateSelf()
{
    aLOG<<"### UpdateSelf"<<endl;
	if(mVisionOn)
  	    mCanLocalize = Localize();
    
	mSelf->UpdateRobotMatrix(mVisionPerceptorMatrix);
	aLOG << "Time: " << mFieldInfo->match_time << endl;

	if( !mCanLocalize )
	{
		aLOG << "***********No Enough Data! Can't Localize!************" << endl;
	}
	else
		aLOG << "***********I Can Localize!************" << endl;
	mSelf->renew();
}

void WorldModel::updateNewBall()
{
  if (CanSeeBall())
  {
    nBall->updateFromVision(mVisionSenseMap[BALL]);
    nBall->printVisionModel();
  }
}

void WorldModel::UpdateBall()
{	
    aLOG<<"### UpdateBall"<<endl;
	mBall->setLocalPos(mVisionSenseMap[BALL].localPos);
	mBall->setDistance(mVisionSenseMap[BALL].distanceToSelf);
	if(mCanLocalize && mBall->GetDistanceToSelf() > 0)
	{
		mBall->setPos(mVisionPerceptorMatrix * (mVisionSenseMap[BALL].localPos));
	}
	mBall->renew();
	if(mVisionOn)
	{
		if(mVisionSenseMap[BALL].distance > 0)	
			mICanSeeBall = true;
		else
			mICanSeeBall = false;
	}
     cout<<"************** OLD BALL ***********************"<<endl;
     cout<<"G_POS:"<<mBall->pos<<endl;
     cout<<"L_POS:"<<mBall->pos_local<<endl;
     cout<<"DIST: "<<mBall->distance<<endl;
}

void WorldModel::UpdatePlayers()//Neil 2009.3
{
	if(mVisionOn)
	{
		for (int i = 0; i <MAX_TEAM_SIZE; i++)
		{
			mTmm[i]->BlindEye();
		}
		
		for (int i = 0; i <MAX_TEAM_SIZE; i++)
		{
			mOpp[i]->BlindEye();
		}
	}
	if(mSelf->GetName() == "<unnamed>")
		mSelf->setName(mFieldInfo->our_teamname);
	for (int i = (int)(TEAMMATE_1_HEAD); i <= (int)(TEAMMATE_11_RIGHTFOOT); ++i)
	{
		VisionSense& vs = mVisionSenseMap[static_cast<VisionObject>(i)];
		if (vs.distance < 0 ) 
			continue;
		int index = (i - (int)(TEAMMATE_1_HEAD)) / 5; 
		int limb = (i - (int)(TEAMMATE_1_HEAD)) % 5;
		aLOG << "i: " << i << " index: " << index << " limb: " << limb << endl;
		if (index == WM.getSelf().GetUnum()-1)
		{
			aLOG<<"WorldModel::UpdatePlayers-->my uninum = "<<WM.getSelf().GetUnum()<<endl;
			switch ( limb )
			{
				case 0:
					break;
				case 1:
					WM.getSelf().SetLeftArm(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
					aLOG << "WorldModel::UpdatePlayers-->Self pos = "<<WM.getSelf().pos
					<<" LeftArm(Local) = "<<WM.getSelf().GetLeftArmLocalPosition()
					<<" LeftArm(Globle) = "<<WM.getSelf().GetLeftArmGloblePosition()<<endl;	
					break;
				case 2:
					WM.getSelf().SetRightArm(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
					aLOG << "WorldModel::UpdatePlayers-->Self pos = "<<WM.getSelf().pos
					<<" RightArm(Local) = "<<WM.getSelf().GetRightArmLocalPosition()
					<<" RightArm(Globle) = "<<WM.getSelf().GetRightArmGloblePosition()<<endl;	
					break;
				case 3:
					WM.getSelf().SetLeftFoot(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
					aLOG << "WorldModel::UpdatePlayers-->Self pos = "<<WM.getSelf().pos
					<<" LeftFoot(Local) = "<<WM.getSelf().GetLeftFootLocalPosition()
					<<" LeftFoot(Globle) = "<<WM.getSelf().GetLeftFootGloblePosition()<<endl;	
					break;
				case 4:
					WM.getSelf().SetRightFoot(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
					aLOG << "WorldModel::UpdatePlayers-->Self pos = "<<WM.getSelf().pos
					<<" RightFoot(Local) = "<<WM.getSelf().GetRightFootLocalPosition()
					<<" RightFoot(Globle) = "<<WM.getSelf().GetRightFootGloblePosition()<<endl;	
					break;
			}
			continue;
		}
		if(mTmm[index]->GetName() == "<unnamed>")
			mTmm[index]->setName(mFieldInfo->our_teamname);
		switch ( limb )
		{
			case 0:		//Head
				mTmm[index]->SetHead(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mTmm["<<index<<"] pos = "<<mTmm[index]->pos
				<<" Head(Local) = "<<mTmm[index]->GetHeadLocalPosition()
				<<" Head(Globle) = "<<mTmm[index]->GetHeadGloblePosition()<<endl;	
				break;
			case 1:		//LeftArm
				mTmm[index]->SetLeftArm(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mTmm["<<index<<"] pos = "<<mTmm[index]->pos
				<<" LeftArm(Local) = "<<mTmm[index]->GetLeftArmLocalPosition()
				<<" LeftArm(Globle) = "<<mTmm[index]->GetLeftArmGloblePosition()<<endl;	
				break;
			case 2:		//RightArm
				mTmm[index]->SetRightArm(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mTmm["<<index<<"] pos = "<<mTmm[index]->pos
				<<" RightArm(Local) = "<<mTmm[index]->GetRightArmLocalPosition()
				<<" RightArm(Globle) = "<<mTmm[index]->GetRightArmGloblePosition()<<endl;	
				break;
			case 3:		//LeftFoot
				mTmm[index]->SetLeftFoot(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mTmm["<<index<<"] pos = "<<mTmm[index]->pos
				<<" LeftFoot(Local) = "<<mTmm[index]->GetLeftFootLocalPosition()
				<<" LeftFoot(Globle) = "<<mTmm[index]->GetLeftFootGloblePosition()<<endl;	
				break;
			case 4:		//RightFoot
				mTmm[index]->SetRightFoot(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mTmm["<<index<<"] pos = "<<mTmm[index]->pos
				<<" RightFoot(Local) = "<<mTmm[index]->GetRightFootLocalPosition()
				<<" RightFoot(Globle) = "<<mTmm[index]->GetRightFootGloblePosition()<<endl;	
				break;
		}	
	}
	
	for (int i = (int)(OPPONENT_1_HEAD); i <= (int)(OPPONENT_11_RIGHTFOOT); ++i)
	{
		VisionSense& vs = mVisionSenseMap[static_cast<VisionObject>(i)];
		if (vs.distance < 0 ) 
			continue;
		
		int index = (i - (int)(OPPONENT_1_HEAD)) / 5;
		int limb = (i - (int)(OPPONENT_1_HEAD)) % 5;
		if(mOpp[index]->GetName() == "<unnamed>")
			mOpp[index]->setName(mFieldInfo->opp_teamname);
		switch ( limb )
		{
			case 0:		//Head
				mOpp[index]->SetHead(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mOpp["<<index<<"] pos = "<<mOpp[index]->pos
				<<" Head(Local) = "<<mOpp[index]->GetHeadLocalPosition()
				<<" Head(Globle) = "<<mOpp[index]->GetHeadGloblePosition()<<endl;	
				break;
			case 1:		//LeftArm
				mOpp[index]->SetLeftArm(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mOpp["<<index<<"] pos = "<<mOpp[index]->pos
				<<" LeftArm(Local) = "<<mOpp[index]->GetLeftArmLocalPosition()
				<<" LeftArm(Globle) = "<<mOpp[index]->GetLeftArmGloblePosition()<<endl;	
				break;
			case 2:		//RightArm
				mOpp[index]->SetRightArm(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mOpp["<<index<<"] pos = "<<mOpp[index]->pos
				<<" RightArm(Local) = "<<mOpp[index]->GetRightArmLocalPosition()
				<<" RightArm(Globle) = "<<mOpp[index]->GetRightArmGloblePosition()<<endl;	
				break;
			case 3:		//LeftFoot
				mOpp[index]->SetLeftFoot(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mOpp["<<index<<"] pos = "<<mOpp[index]->pos	
				<<" LeftFoot(Local) = "<<mOpp[index]->GetLeftFootLocalPosition()
				<<" LeftFoot(Globle) = "<<mOpp[index]->GetLeftFootGloblePosition()<<endl;	
				break;
			case 4:		//RightFoot
				mOpp[index]->SetRightFoot(vs.localPos, mVisionPerceptorMatrix * vs.localPos);
				aLOG <<"WorldModel::UpdatePlayers-->mOpp["<<index<<"] pos = "<<mOpp[index]->pos	
				<<" RightFoot(Local) = "<<mOpp[index]->GetRightFootLocalPosition()
				<<" RightFoot(Globle) = "<<mOpp[index]->GetRightFootGloblePosition()<<endl;	
				break;
		}
	}
}

void WorldModel::UpdateFieldInfo()
{
	if(mVisionSenseMap[GOAL_1_R].distance > 0)
	{	
	    mFieldInfo->oppleftgoallocal = mVisionSenseMap[GOAL_1_R].localPos;
	}

	if(mVisionSenseMap[GOAL_2_R].distance > 0)
	{
	    mFieldInfo->opprightgoallocal = mVisionSenseMap[GOAL_2_R].localPos;
	}

	mFieldInfo->oppgoalcenterlocal = mVisionSenseMap[GOAL_1_R].localPos + mVisionSenseMap[GOAL_2_R].localPos;
	mFieldInfo->oppgoalcenterlocal = mFieldInfo->oppgoalcenterlocal * 0.5;

	mFieldInfo->ourleftgoallocal = mVisionSenseMap[GOAL_1_L].localPos;
	mFieldInfo->ourrightgoallocal = mVisionSenseMap[GOAL_2_L].localPos;
	mFieldInfo->ourgoalcenterlocal = mVisionSenseMap[GOAL_1_L].localPos + mVisionSenseMap[GOAL_2_L].localPos;
	mFieldInfo->ourgoalcenterlocal = mFieldInfo->ourgoalcenter * 0.5;

	aLOG << "******************************UpdateFieldInfo*******************************" << endl;
	aLOG << "oppleftgoallocal: " << mFieldInfo->oppleftgoallocal << endl;
	aLOG << "opprightgoallocal: " << mFieldInfo->opprightgoallocal << endl;
	aLOG << "oppgoalcenterlocal: " << mFieldInfo->oppgoalcenterlocal << endl;
}

void WorldModel::CalculateVisionObjectLocalPos()
{
    for (int i = (int)(FLAG_1_L); i < (int)(OPPONENT_11_RIGHTFOOT); ++i)
    {
        VisionSense& vs = mVisionSenseMap[static_cast<VisionObject>(i)];
        if (vs.distance < 0) 
		continue;
		
        float distance = vs.distance;
        float theta = gDegToRad(gNormalizeDeg(vs.theta));
        float phi   = gDegToRad(gNormalizeDeg(vs.phi));

        vs.localPosInVision[0] = distance * gCos(phi) * gCos(theta);
        vs.localPosInVision[1] = distance * gCos(phi) * gSin(theta);
        vs.localPosInVision[2] = distance * gSin(phi);

	vs.distanceToSelf = vs.distance*gCos(gDegToRad(vs.phi));
	
	 /**add by Neil*/
	vs.localPos = NAO->GetRobotCameraMatrix(Nao::PART_HEAD) * vs.localPosInVision;

	aLOG << "Object:" << i <<" x:" <<  vs.localPosInVision[0] << " y:"<< vs.localPosInVision[1] << " z:"<< vs.localPosInVision[2] << endl;
	aLOG << "Object:" << i <<" x:" <<  vs.localPos[0] << " y:"<< vs.localPos[1] << " z:"<< vs.localPos[2] << endl;

    }
	aLOG << "**************************************************" << endl;
}


Matrix WorldModel::LocalizeWithThreeFlagsTriangle(const VisionObject vo1,
                                                  const VisionObject vo2,
                                                  const VisionObject vo3)
{
    Vector3f pos;
    Matrix m;
    m.Identity();
    VisionSense vs1 = mVisionSenseMap[vo1];
    VisionSense vs2 = mVisionSenseMap[vo2];
    VisionSense vs3 = mVisionSenseMap[vo3];
	if(vs1.realPos.x() != vs2.realPos.x())
	{
		if(vs1.realPos.x() == vs3.realPos.x())
		{
			VisionSense vstmp = vs2;
			vs2 = vs3;
			vs3 = vstmp;
		}
		else if(vs2.realPos.x() == vs3.realPos.x())
		{
			VisionSense vstmp = vs1;
			vs1 = vs3;
			vs3 = vstmp;
		}
	}
	if(gAbs(vs1.realPos.y() - vs3.realPos.y()) > 1e-3)
	{
		VisionSense vstmp = vs1;
			vs1 = vs2;
			vs2 = vstmp;
	}
	aLOG << "LocalizeWithThreeFlagsTriangle: " << endl 
		 << "vs1: " << vs1.realPos << " vs2: " << vs2.realPos << " vs3: " << vs3.realPos << endl;
	aLOG << "vs1.distance: " << vs1.distance << " vs2.distance: " << vs2.distance << " vs3.distance: " << vs3.distance << endl;
    float a1 = vs1.distance * vs1.distance - vs2.distance * vs2.distance;
    float b1 = 2.0f * (-vs1.realPos.y()) - 2.0f * (-vs2.realPos.y());
    float a2 = vs1.distance * vs1.distance - vs3.distance * vs3.distance;
    float b2 = 2.0f * (-vs1.realPos.x()) - 2.0f * (-vs3.realPos.x() );
    aLOG << "a1: " << a1 << " b1: " << b1 << " a2: " << a2 << " b2: " << b2 << endl;
    if (gAbs(b1) < 1e-3 ||
        gAbs(b2) < 1e-3)
    {
        return m;
    }
    pos[0] = a2 / b2 + (vs1.realPos.x() + vs3.realPos.x()) / 2;
	a1 += (-2 * pos[0] * (vs2.realPos.x() - vs1.realPos.x()) 
		  - vs2.realPos.x() * vs2.realPos.x() + vs1.realPos.x() * vs1.realPos.x());
    pos[1] = a1 / b1 + (vs1.realPos.y() + vs2.realPos.y()) / 2;
    pos[2] = vs1.realPos.z();
// 	aLOG << "pos[0]: " << pos[0] << " pos[1]: " << pos[1] << " pos[2]: " << pos[2] << endl;
    float under;
    under = b2 * -0.5;
//     aLOG << "under1: " << under << endl;
    m(0, 0) = (vs1.localPos.x() - vs3.localPos.x()) / under;
    m(0, 1) = (vs1.localPos.y() - vs3.localPos.y()) / under;
    m(0, 2) = (vs1.localPos.z() - vs3.localPos.z()) / under;
//     aLOG << "m(0, 0): " << m(0, 0) << " m(0, 1): " << m(0, 1) << " m(0, 2): " << m(0, 2) << endl;
 
    under = b1 * -0.5;
//     aLOG << "under2: " << under << endl;
    m(1, 0) = (vs1.localPos.x() - vs2.localPos.x()) / under;
    m(1, 1) = (vs1.localPos.y() - vs2.localPos.y()) / under;
    m(1, 2) = (vs1.localPos.z() - vs2.localPos.z()) / under;
//     aLOG << "m(1, 0): " << m(1, 0) << " m(1, 1): " << m(1, 1) << " m(1, 2): " << m(1, 2) << endl;

    Vector3f vx(m(0, 0), m(0, 1), m(0, 2));
    Vector3f vy(m(1, 0), m(1, 1), m(1, 2));
    Vector3f vz = vx.Cross(vy);
//     aLOG << "vx: " << vx << " vy: " << vy << " vz: " << endl;
    m(2, 0) = vz.x(); m(2, 1) = vz.y(); m(2, 2) = vz.z();

    pos[2] = vs1.realPos.z()
             - m(2, 0) * vs1.localPos.x()
             - m(2, 1) * vs1.localPos.y()
             - m(2, 2) * vs1.localPos.z();

    m.Pos() = pos;
    return m;
}


/** if using this function to localize, the accuracy of z pos is low */
Matrix WorldModel::LocalizeWithThreeFlagsLine(const VisionObject vo1,
                                              const VisionObject vo2,
                                              const VisionObject vo3)
{
    Vector3f pos(0.0f, 0.0f, 0.0f);
    Matrix mat;
    mat.Identity(); 
    VisionSense vs1 = mVisionSenseMap[vo1];
    VisionSense vs2 = mVisionSenseMap[vo2];
    VisionSense vs3 = mVisionSenseMap[vo3];
    VisionSense vstmp;
	if(vs2.realPos.z() == vs3.realPos.z())
	{
		vstmp = vs3;
		vs3 = vs1;
		vs1 = vstmp;
	}
	aLOG << "LocalizeWithThreeFlagsLine: " << endl 
		 << "vs1: " << vs1.realPos << " vs2: " << vs2.realPos << "vs3: " << vs3.realPos << endl;
	aLOG << "vs1.distance: " << vs1.distance << "vs2.distance: " << vs2.distance << "vs3.distance: " << vs3.distance << endl;
    float a = vs1.distance * vs1.distance - vs2.distance * vs2.distance;
	aLOG << "a1: " << a << endl;
    float b = 2.0f * (-vs1.realPos.y()) - 2.0f * (-vs2.realPos.y());
	aLOG << "b1: " << b << endl;
    assert(gAbs(b) > 1e-5);
    pos[1] = a / b + (vs1.realPos.y() + vs2.realPos.y()) / 2;
	aLOG << "pos[1]: " << pos[1] << endl;
    a = vs2.distance * vs2.distance - vs3.distance * vs3.distance -
         (pos[1]-vs2.realPos.y())*(pos[1]-vs2.realPos.y()) +
         (pos[1]-vs3.realPos.y())*(pos[1]-vs3.realPos.y()) -
          vs2.realPos.z()*vs2.realPos.z() + vs3.realPos.z()*vs3.realPos.z();
	aLOG << "a2: " << a << endl;
    b = 2.0f * (-vs2.realPos.z()) - 2.0f * (-vs3.realPos.z());
	aLOG << "b2: " << b << endl;
    assert(gAbs(b) > 1e-5);
    pos[2] = a / b;
	aLOG << "pos[2]: " << pos[2] << endl;

    float c =  vs1.distance * vs1.distance -
               (pos[1]-vs1.realPos.y())*(pos[1]-vs1.realPos.y()) -
               (pos[2]-vs1.realPos.z())*(pos[2]-vs1.realPos.z());
	aLOG << "c: " << c << endl;
    if (c < 0) return mat;

	float sign = -gSign((vs1.realPos.y() - vs2.realPos.y()) * (vs1.theta - vs2.theta));
    float x1 =  gSqrt(c) + vs1.realPos.x();
    float x2 = -gSqrt(c) + vs1.realPos.x();
	aLOG << "x1: " << x1 << "   x2: " << x2 << " sign: " << sign << endl;
    pos[0] = sign * gSqrt(c) + vs1.realPos.x();
	aLOG << "pos[0]: " << pos[0] << endl;
    Vector3f l1 = vs1.localPos;
    Vector3f r1 = vs1.realPos - pos;
    Vector3f l2 = vs2.localPos;
    Vector3f r2 = vs2.realPos - pos;
    Vector3f l3 = vs3.localPos;
    Vector3f r3 = vs3.realPos - pos;

    Matrix m;
    m.Identity();
    m(0, 0) = l1.x(); m(0, 1) = l1.y(); m(0, 2) = l1.z();
    m(1, 0) = l2.x(); m(1, 1) = l2.y(); m(1, 2) = l2.z();
    m(2, 0) = l3.x(); m(2, 1) = l3.y(); m(2, 2) = l3.z();
    if (! InverseMatrix(m))
    {
        cerr
             << "(WorldModel) localize matrix inverse error" << endl;
    }
    // TODO: comments here 
    Vector3f c1(r1.x(), r2.x(), r3.x());
    Vector3f c2(r1.y(), r2.y(), r3.y());
    Vector3f c3(r1.z(), r2.z(), r3.z());

    Vector3f s1, s2, s3;
    s1 = m.Rotate(c1);
    s2 = m.Rotate(c2);
    s3 = m.Rotate(c3);

    mat(0, 0) = s1.x(); mat(0, 1) = s1.y(); mat(0, 2) = s1.z();
    mat(1, 0) = s2.x(); mat(1, 1) = s2.y(); mat(1, 2) = s2.z();
    mat(2, 0) = s3.x(); mat(2, 1) = s3.y(); mat(2, 2) = s3.z();

    mat.Pos() = pos;
    return mat;
}

bool WorldModel::UpdateWithHearInfomation()
{
	if(mHearingOn)
	{
// 		if(mHearInfo.sender == "self")
// 			return false;
		if(( 'l' == mHearInfo.message[0] && getFieldInfo().team_index == TI_LEFT) || ( 'r' == mHearInfo.message[0]	&& getFieldInfo().team_index == TI_RIGHT))//Player Information
		{
			mHearCycles = 0;
			stringstream ss;
			int num;
			int forward1_num;
			int forward2_num;
			Vector3f pos;
			Vector3f ballpos;
			char buf[20];
			unsigned long ulposX;
			unsigned long ulposY;
			unsigned long ulballposX;
			unsigned long ulballposY;
			aLOG << "message length: " << mHearInfo.message.size() << endl;
			if(mHearInfo.message.size() < 20)
			{
				aLOG << "Can't Understand The heared Message!" << endl;
				return false;
			}
			aLOG << "Hear Successful!" <<endl;
			strncpy(buf, mHearInfo.message.c_str(), mHearInfo.message.size());
			buf[20] = '\0';
			num = buf[1] - '0';
			forward1_num = buf[2] - '0';
			forward2_num = buf[3] - '0';
			memcpy(&ulposX, buf+4, sizeof(float));
			memcpy(&ulposY, buf+8, sizeof(float));
			memcpy(&ulballposX, buf+12, sizeof(float));
			memcpy(&ulballposY, buf+16, sizeof(float));
			pos.x() = ul2float(ulposX);
			pos.y() = ul2float(ulposY);
			pos.z() = 0.0;
			ballpos.x() = ul2float(ulballposX);
			ballpos.y() = ul2float(ulballposY);
			ballpos.z() = 0.0;
			mBall->posbyhearing = ballpos;
			mRoleForward1 = mTmm[forward1_num -1];
			mRoleForward2 = mTmm[forward2_num -1];
			aLOG << "buffer: " << buf << endl;
			aLOG << "num: " << num << "    forward1_num:" << forward1_num <<"    forward2_num:" << forward2_num << "    pos: " << pos << "    ballpos: " << ballpos << endl;
			if(num == getSelf().GetUnum())
			{
				mSelf->posbyhearing = pos;
				aLOG << "Hearing: " << "selfpos: " << pos << "ballpos: " << mBall->posbyhearing << "balllocal: " << mBall->GetLocalPos()
				<< "yawangle: " << mSelf->mTorsoYawAngle << endl;
				Vector3f balllocal3f = mBall->GetLocalPos();
				Vector3f ballbycommunication = mBall->posbyhearing;
				Vector2f balllocal = balllocal3f.to2D();
				Vector2f agent2ball =  ballbycommunication.to2D() - pos.to2D();
				float TorsoYawAngle = gNormalizeDeg(agent2ball.GetAngleDeg() - balllocal.GetAngleDeg());
				aLOG << "yawangle2: " << TorsoYawAngle << endl;
				if(!mCanLocalize)
                	mSelf->TorsoYawAngleByHearing = TorsoYawAngle;
 			}
			else 
			{
				mTmm[num - 1]->posbyhearing = pos;
			}
		}
	}
} 

bool WorldModel::Localize()
{	
    Vector3f pos(0.0f, 0.0f, 0.0f);
    Matrix mat;
 int count = 0;	
	VisionObject flag[3];
	Vector3f coordinate[3];
	for(int i = (int)(FLAG_1_L); i <= GOAL_2_R; i++)
	{	
		VisionSense& vs = mVisionSenseMap[static_cast<VisionObject>(i)];
		if (vs.distance >= 0 ) 
		{
			flag[count] = (VisionObject)(i);
			coordinate[count] = vs.realPos;
			aLOG << "flag[" << count << "]" << "=" << flag[count] << ": " << coordinate[count] << '\t' << vs.localPos << endl;
			count++;
		}
		if(count >= 3)
		{	
			break;
		}
	}
	if(count < 3)
	{
		return false;
	}	
	else if(gAbs(coordinate[0].x() - coordinate[1].x()) < 1e-5
	   		&& gAbs(coordinate[0].x() - coordinate[2].x()) < 1e-5
	   		&& gAbs(coordinate[1].x() - coordinate[2].x()) < 1e-5
	  		)
	{
	  	aLOG << "LocalizeWithThreeFlagsLine" << endl;
		aLOG << "coordinate[0] = " << coordinate[0] << endl;
		aLOG << "coordinate[1] = " << coordinate[1] << endl;
		aLOG << "coordinate[2] = " << coordinate[2] << endl;
	  	mat = LocalizeWithThreeFlagsLine(flag[0], flag[1], flag[2]);
	}
	else 
	{
	  	aLOG << "LocalizeWithThreeFlagsTriangle" << endl;
	  	mat = LocalizeWithThreeFlagsTriangle(flag[0], flag[1], flag[2]);
	}
    mVisionPerceptorMatrix = mat;
	aLOG<<"WorldModel::UpdateSelf-->Self = " << endl 
		 << mVisionPerceptorMatrix.Right() << endl
		 << mVisionPerceptorMatrix.Up() << endl
		 << mVisionPerceptorMatrix.Forward() << endl
		 << mVisionPerceptorMatrix.Pos() << endl;
    return true;
}

Player& WorldModel::getTmm(int index)
{
	if (index < 0 || index >= MAX_TEAM_SIZE)
	{
		cout<<"WorldModel::getTmm-->Error! No teammate with index  "<<index<<endl;
		assert(0);
	}
	else
		return *(mTmm[index]);
}

Player& WorldModel::getOpp(int index)
{
	if (index < 0 || index >= MAX_TEAM_SIZE)
	{
		cout<<"WorldModel::getOpp-->Error! No opponent with index  "<<index<<endl;
		assert(0);
	}
	else
		return *(mOpp[index]);
}

void WorldModel::setHearInfo( HearInfo& hearinfo )
{
	string tmp = hearinfo.message;
	assert(!tmp.empty());
	if( ('l' == tmp[0] && WM.getFieldInfo().team_index == TI_LEFT) || ('r' == tmp[0] && WM.getFieldInfo().team_index == TI_RIGHT))
	{
		mHearInfo = hearinfo;
		mHearInfo.message = tmp;
		aLOG << "WorldModel::HearInfo-->time: " << mHearInfo.time << "   SimTime: " << WM.getMySimTime() << endl;
		aLOG << "WorldModel::HearInfo-->sender " << mHearInfo.sender << ':' << mHearInfo.message << endl;
	}
	else
	{
		aLOG << "Strive3D Other Team's Message!" << endl;
	}
}

void WorldModel::AnalysePlayerPosition()
{
	int num_of_tmm_before_me = 0;
	float	OurminDistance = 1000;
	float 	OursecondminDistance = 1000;
	float	OppminDistanceToGolie = 1000;
	Vector3f ballpos = mBall->pos;
	Vector3f playerpos = mSelf->pos;
	float disttoball;
	float disttogoalie;
	float	mydisttoball = gSqrt((ballpos.x() - playerpos.x()) * (ballpos.x() - playerpos.x()) + (ballpos.y() - playerpos.y()) * (ballpos.y() - playerpos.y()));
	OurminDistance = mydisttoball;
	OursecondminDistance = mydisttoball;
	mOurTeamNearestToBall = mSelf;
	mOurTeamSecondToBall = mSelf;
	for (int i=0; i< MAX_TEAM_SIZE; i++)
	{
		if(mTmm[i]->isValid() && mTmm[i]->GetUnum() != mSelf->GetUnum())
		{
			playerpos = mTmm[i]->pos;
			disttoball = gSqrt((ballpos.x() - playerpos.x()) * (ballpos.x() - playerpos.x()) + (ballpos.y() - playerpos.y()) * (ballpos.y() - playerpos.y()));
			if(disttoball < OurminDistance)
			{
				OurminDistance = disttoball;
				mOurTeamNearestToBall = mTmm[i];
			}
			if(disttoball < mydisttoball)
			{
				if(mTmm[i]->GetUnum() != 1 && mTmm[i]->GetUnum() != 8 && mTmm[i]->GetUnum() != 9 )
					num_of_tmm_before_me++;
			}
		}
	}
	aLOG << "mOurTeamNearestToBall: " << &mOurTeamNearestToBall << '\t' << mOurTeamNearestToBall->GetUnum() << endl;
	for (int i =0; i< MAX_TEAM_SIZE; i++)
	{
		if(mTmm[i]->isValid() && mTmm[i]->GetUnum() != mSelf->GetUnum())
		{
			playerpos = mTmm[i]->pos;
			disttoball = gSqrt((ballpos.x() - playerpos.x()) * (ballpos.x() - playerpos.x()) + (ballpos.y() - playerpos.y()) * (ballpos.y() - playerpos.y()));			
			if(disttoball >( OurminDistance+0.1) && disttoball < OursecondminDistance)
			{
				OursecondminDistance = disttoball;
				mOurTeamSecondToBall= mTmm[i];
			}
		}
	}
	aLOG << "mOurTeamSecondToBall: " << &mOurTeamSecondToBall << '\t' << mOurTeamSecondToBall->GetUnum() << endl;
	float OppminDistance = 1000;
	for (int i=0; i< MAX_TEAM_SIZE; i++)
	{
		if(mOpp[i]->isValid())
		{
			playerpos = mOpp[i]->pos;
			disttoball = gSqrt((ballpos.x() - playerpos.x()) * (ballpos.x() - playerpos.x()) + (ballpos.y() - playerpos.y()) * (ballpos.y() - playerpos.y()));
			Vector3f OppToGolie = mFieldInfo->oppgoalcenterlocal - mOpp[i]->GetHeadLocalPosition();
			disttogoalie = OppToGolie.Length();
			if(disttoball < OppminDistance)
			{
				OppminDistance = disttoball;
				mOppTeamNearestToBall = mOpp[i];
				aLOG << "mOppTeamNearestToBall: " << &mOppTeamNearestToBall << '\t' << mOppTeamNearestToBall->GetUnum() << endl;
			}
			if(disttogoalie < OppminDistanceToGolie)
			{
				OppminDistanceToGolie = disttogoalie;
				mOppTeamNearestToGoalie = mOpp[i];
				aLOG << "mOppTeamNearestToGoalie: " << &mOppTeamNearestToGoalie << '\t' << mOppTeamNearestToGoalie->GetUnum() << endl;
			}
		}
	}
	mPlayerNearestToBall = OurminDistance < OppminDistance ? mOurTeamNearestToBall : mOppTeamNearestToBall;

	aLOG << "mHearCycles:" <<mHearCycles <<"      mRoleForward1: " <<mRoleForward1->GetUnum() << endl;
	if(mHearCycles > 4 || mRoleForward1->GetUnum() == 1)
	{
		aLOG << "ROLEASSIGN1111111"<<endl;
		if(mSelf->GetUnum() == 1)
		{
			RoleAssignment_forward1();
			RoleAssignment_forward2();
			mSelf->roleinteam = Self::golie;
			aLOG << "mSelf.roleinteam: golie" << endl;
		}
		else if(mSelf->GetUnum() == 8)
		{
			IsCanHitout();
			mSelf->roleinteam = Self::guard1;
			aLOG << "mSelf.roleinteam: guard1" << endl;
		}
	
		else if(mSelf->GetUnum() == 9)
		{
			IsCanHitout();
			mSelf->roleinteam = Self::guard2;
			aLOG << "mSelf.roleinteam: guard2" << endl;
		}
		else
		{
			switch(num_of_tmm_before_me)
			{
				case 0:
					mSelf->roleinteam = Self::forward1;
					aLOG << "mSelf.roleinteam: forward1" << endl;
					break;
				case 1:
					mSelf->roleinteam = Self::forward2;
					aLOG << "mSelf.roleinteam: forward2" << endl;
					break;
				case 2:
					mSelf->roleinteam = Self::center;
					aLOG << "mSelf.roleinteam: center" << endl;
					break;
				case 3:
					mSelf->roleinteam = Self::guard3;
					aLOG << "mSelf.roleinteam: guard3" << endl;
					break;
				case 4:
					mSelf->roleinteam = Self::guard4;
					aLOG << "mSelf.roleinteam: guard4" << endl;
					break;
				case 5:
					mSelf->roleinteam = Self::guard5;
					aLOG << "mSelf.roleinteam: guard5" << endl;
					break;
                case 6:
                    mSelf->roleinteam = Self::guard6;
                    aLOG << "mSelf.roleinteam: guard6" <<endl;
                    break;
                case 7:
                    mSelf->roleinteam = Self::guard7;
                    aLOG << "mSelf.roleinteam: guard7" <<endl;
                    break;
				default:
					mSelf->roleinteam = Self::unknow;
					aLOG << "mSelf.roleinteam: unknow" << endl;
			}
		}
	}
	else if( mRoleForward2->GetUnum() == 1)
	{
		aLOG<<"ROLEASSIGN2222222"<<endl;
		mHearCycles++;
		if(mSelf->GetUnum() == 1)
		{
			RoleAssignment_forward1();
			RoleAssignment_forward2();
			mSelf->roleinteam = Self::golie;
			aLOG << "mSelf.roleinteam: golie" << endl;
		}
		else if(mSelf->GetUnum() == 8)
		{
			IsCanHitout();
			mSelf->roleinteam = Self::guard1;
			aLOG << "mSelf.roleinteam: guard1" << endl;
		}
		else if(mSelf->GetUnum() == 9)
		{
		  	IsCanHitout();
			mSelf->roleinteam = Self::guard2;
			aLOG << "mSelf.roleinteam: guard2" << endl;
		}
		else 
		{
			if(mSelf->GetUnum() == mRoleForward1->GetUnum())
			{
				mSelf->roleinteam = Self::forward1;
				aLOG <<"mSelf.roleinteam:forward1" << endl;
			}
			else
			{
				switch(num_of_tmm_before_me)
				{
					case 0:
						mSelf->roleinteam = Self::forward2;
						aLOG << "mSelf.roleinteam: forward2" << endl;
						break;
					case 1:
						mSelf->roleinteam = Self::forward2;
						aLOG << "mSelf.roleinteam: forward2" << endl;
						break;
					case 2:
						mSelf->roleinteam = Self::center;
						aLOG << "mSelf.roleinteam: center" << endl;
						break;
					case 3:
						mSelf->roleinteam = Self::guard3;
						aLOG << "mSelf.roleinteam: guard3" << endl;
						break;
					case 4:
						mSelf->roleinteam = Self::guard4;
						aLOG << "mSelf.roleinteam: guard4" << endl;
						break;
					case 5:
						mSelf->roleinteam = Self::guard5;
						aLOG << "mSelf.roleinteam: guard5" << endl;
						break;
                    case 6:
                        mSelf->roleinteam = Self::guard6;
                        aLOG << "mSelf.roleinteam: guard6" <<endl;
                        break;
                    case 7:
                        mSelf->roleinteam = Self::guard7;
                        aLOG << "mSelf.roleinteam: guard7" <<endl;
                        break;
					default:
						mSelf->roleinteam = Self::unknow;
						aLOG << "mSelf.roleinteam: unknow" << endl;
				}
			}
		}
	}
	else
	{
		aLOG<<"ROLEASSIGN333333"<<endl;
		mHearCycles++;
		if(mSelf->GetUnum() == 1)
		{
			RoleAssignment_forward1();
			RoleAssignment_forward2();
			mSelf->roleinteam = Self::golie;
			aLOG << "mSelf.roleinteam: golie" << endl;
		}
		else if(mSelf->GetUnum() == 8)
		{
		  	IsCanHitout();
			mSelf->roleinteam = Self::guard1;
			aLOG << "mSelf.roleinteam: guard1" << endl;
		}
		else if(mSelf->GetUnum() == 9)
		{
		  	IsCanHitout();
			mSelf->roleinteam = Self::guard2;
			aLOG << "mSelf.roleinteam: guard2" << endl;
		}
		else if(mSelf->GetUnum() == mRoleForward1->GetUnum())
		{
			mSelf->roleinteam = Self::forward1;
			aLOG <<"mSelf.roleinteam:forward1" << endl;
		}
		else 
		{
			if(mSelf->GetUnum() == mRoleForward2->GetUnum())
			{
				mSelf->roleinteam = Self::forward2;
				aLOG <<"mSelf.roleinteam:forward2" << endl;
			}
			else
			{
				switch(num_of_tmm_before_me)
				{
					case 0:
						mSelf->roleinteam = Self::center;
						aLOG << "mSelf.roleinteam: center" << endl;
						break;
					case 1:
						mSelf->roleinteam = Self::center;
						aLOG << "mSelf.roleinteam: center" << endl;
						break;
					case 2:
						mSelf->roleinteam = Self::center;
						aLOG << "mSelf.roleinteam: center" << endl;
						break;
					case 3:
						mSelf->roleinteam = Self::guard3;
						aLOG << "mSelf.roleinteam: guard3" << endl;
						break;
					case 4:
						mSelf->roleinteam = Self::guard4;
						aLOG << "mSelf.roleinteam: guard4" << endl;
						break;
					case 5:
						mSelf->roleinteam = Self::guard5;
						aLOG << "mSelf.roleinteam: guard5" << endl;
			 			break;
                    case 6:
                        mSelf->roleinteam = Self::guard6;
                        aLOG << "mSelf.roleinteam: guard6" <<endl;
                        break;
                    case 7:
                        mSelf->roleinteam = Self::guard7;
                        aLOG << "mSelf.roleinteam: guard7" <<endl;
                        break;
					default:
						mSelf->roleinteam = Self::unknow;
						aLOG << "mSelf.roleinteam: unknow" << endl;
				}
			}
		}
	}
}

void WorldModel::RoleAssignment_forward1()
{
 	float	smallestcost = 1000.0;
	float	self_pathcost =1000.0;
	float	player_pathcost =1000.0;
	float	 k_dist = 0.6;
	float	 k_angle = 0.4;
	float 	disttmmtoball = 1000.0;
	float	disttoball = 1000.0;
	float	angleToDest = 180.0;
	Player&	tmmnearnesttoball = getOurTeamNearestToBall();
	Vector2f tmmpos = tmmnearnesttoball.pos.to2D();
	Vector2f goalpos = getFieldInfo().oppgoalcenter.to2D();
	Vector2f ballpos = mBall->pos.to2D();
	Vector2f selfpos = mSelf->pos.to2D();
	Vector2f vecttmmtoball = ballpos - tmmpos;
	Vector2f vectballtogoal = goalpos - ballpos;
	Vector2f vectselftoball = ballpos - selfpos;
	disttoball = vectselftoball.Length();
	angleToDest = gAbs(gNormalizeDeg(vectballtogoal.GetAngleDeg() -vectselftoball.GetAngleDeg()));
	self_pathcost = k_dist * disttoball - k_angle * gCos(gDegToRad( angleToDest)) ;
	aLOG << "forward1--self:" << endl;
	aLOG << "disttoball: " << disttoball <<"   1:" << k_dist * disttoball <<"  angletodest: " << angleToDest <<"     cos:"<<gCos(gDegToRad( angleToDest)) <<"   2:"<< k_angle * gCos(gDegToRad( angleToDest)) << endl;
	aLOG << "pathcost :" << self_pathcost<<endl;
	smallestcost = self_pathcost;
	mRoleForward1 = mSelf;
//	if(vecttmmtoball.Length() <= 0.4)
//		mRoleForward1 = mOurTeamNearestToBall;
//	else
//	{
		for (int i =0; i< MAX_TEAM_SIZE; i++)
		{
			if(mTmm[i]->isValid() && mTmm[i]->GetUnum() != mSelf->GetUnum() && mTmm[i]->GetUnum() != 8 && mTmm[i]->GetUnum() != 9)
			{
				Vector2f playerpos = mTmm[i]->pos.to2D();
				Vector2f vectplayertoball = ballpos - playerpos;
				disttoball = vectplayertoball.Length();
				angleToDest = gAbs(gNormalizeDeg(vectballtogoal.GetAngleDeg() -vectplayertoball.GetAngleDeg()));
				player_pathcost =	k_dist * disttoball - k_angle * gCos(gDegToRad( angleToDest)) ;
				aLOG << "forward1--player:" << mTmm[i]->GetUnum() << endl;
				aLOG << "disttoball: " << disttoball <<"   1:" << k_dist * disttoball <<"  angletodest: " << angleToDest <<"     cos:"<<gCos(gDegToRad( angleToDest)) <<"   2:"<< k_angle * gCos(gDegToRad( angleToDest)) << endl;
				aLOG << "pathcost :" << player_pathcost<<endl;
				if(player_pathcost < smallestcost)
				{	
					smallestcost = player_pathcost;
					mRoleForward1 = mTmm[i];
				}
			}
		}	
//	}
}

void WorldModel::RoleAssignment_forward2()
{
	float dist = 2.0;
	float MinDistance = 1000;
	float distselftodest;
	Vector2f selfpos = mSelf->pos.to2D();
	Vector2f goalpos = getFieldInfo().ourgoalcenter.to2D();
	Vector2f ballpos = mBall->pos.to2D();
	Vector2f vectballtogoal = goalpos - ballpos;
	if(dist > vectballtogoal.Length())
		dist = vectballtogoal.Length() * 2 / 3;
	Vector2f destpos = ballpos + Vector2f(dist * gCos(vectballtogoal.GetAngleRad()), dist * gSin(vectballtogoal.GetAngleRad()));	
	Vector2f vectselftodest = destpos - selfpos;
	distselftodest = vectselftodest.Length();
	MinDistance = distselftodest;
	mRoleForward2 = mSelf;
	for (int i=0; i< MAX_TEAM_SIZE; i++)
	{
		if(mTmm[i]->isValid() && mTmm[i]->GetUnum() != mSelf->GetUnum() && mTmm[i]->GetUnum() != 8 && mTmm[i]->GetUnum() != 9 && mTmm[i]->GetUnum() != mRoleForward1->GetUnum())
		{
			Vector2f playerpos = mTmm[i]->pos.to2D();
			Vector2f vectplayertodest = destpos - playerpos;
			float distplayertodest = vectplayertodest.Length();
			if(distplayertodest < MinDistance)
			{
				MinDistance = distplayertodest;
				mRoleForward2 = mTmm[i];
			}
		}
	}
}

void WorldModel::IsCanHitout()
{
	float	smallestcost = 1000.0;
	float	self_pathcost =1000.0;
	float	player_pathcost =1000.0;
	float	 k_dist = 0.6;
	float	 k_angle = 0.4;
	float 	disttmmtoball = 1000.0;
	float	disttoball = 1000.0;
	float	angleToDest = 180.0;
	Player&	tmmnearnesttoball = getOurTeamNearestToBall();
	Vector2f tmmpos = tmmnearnesttoball.pos.to2D();
	Vector2f goalpos = getFieldInfo().oppgoalcenter.to2D();
	Vector2f ballpos = mBall->pos.to2D();
	Vector2f selfpos = mSelf->pos.to2D();
	Vector2f vecttmmtoball = ballpos - tmmpos;
	Vector2f vectballtogoal = goalpos - ballpos;
	Vector2f vectselftoball = ballpos - selfpos;
	disttoball = vectselftoball.Length();
	angleToDest = gAbs(gNormalizeDeg(vectballtogoal.GetAngleDeg() -vectselftoball.GetAngleDeg()));
	self_pathcost = k_dist * disttoball - k_angle * gCos(gDegToRad( angleToDest)) ;
	aLOG << "forward1--self:" << endl;
	aLOG << "disttoball: " << disttoball <<"   1:" << k_dist * disttoball <<"  angletodest: " << angleToDest <<"     cos:"<<gCos(gDegToRad( angleToDest)) <<"   2:"<< k_angle * gCos(gDegToRad( angleToDest)) << endl;
	aLOG << "pathcost :" << self_pathcost<<endl;
	smallestcost = self_pathcost;
	aLOG << "self_pathcost:" << self_pathcost << endl;
	for (int i =0; i< MAX_TEAM_SIZE; i++)
	{
		if(mTmm[i]->isValid() && mTmm[i]->GetUnum() != mSelf->GetUnum())
		{
			Vector2f playerpos = mTmm[i]->pos.to2D();
			Vector2f vectplayertoball = ballpos - playerpos;
			disttoball = vectplayertoball.Length();
			angleToDest = gAbs(gNormalizeDeg(vectballtogoal.GetAngleDeg() -vectplayertoball.GetAngleDeg()));
			player_pathcost = k_dist * disttoball - k_angle * gCos(gDegToRad( angleToDest)) ;
			if(player_pathcost < smallestcost)	
				smallestcost = player_pathcost;				
		}
	}
	if(smallestcost < self_pathcost + 0.1 && smallestcost > self_pathcost - 0.1)
		mCanHitout = true;
	else
		mCanHitout = false;
}

//2009,12,23 by ianpan start
void WorldModel::updatedBetweenBallandOpp()
{
	if( mOppTeamNearestToBall != NULL)
	{
		aLOG<<"OOOOOOOOOOOOOOOOOOOOOOOOppupdate"<<endl;
		double d = gSqrt ( ((getOppTeamNearestToBall().pos[0] - 	getBall().pos[0])*(getOppTeamNearestToBall().pos[0] - getBall().pos[0])) + ((getOppTeamNearestToBall().pos[1] - getBall().pos[1])*(getOppTeamNearestToBall().pos[1] - getBall().pos[1])) );
	
		setdBetweenBallandOpp.push(d);

		if(setdBetweenBallandOpp.size() > 20)
		setdBetweenBallandOpp.pop();
	}
}	
bool WorldModel::Oppshooting()
{	Player & oppnearesttoball =WM.getOppTeamNearestToBall(); //
		if ( &oppnearesttoball == NULL)
			return false;
		else if(oppnearesttoball.GetState() == TUMBLE)
			return false;
	aLOG<<"Oppshooting judging"<<endl;
	if(setdBetweenBallandOpp.size() > 10)
	{
		comparedbetweenBallandOpp = setdBetweenBallandOpp;
		double avg =0, tail;
		while( !comparedbetweenBallandOpp.empty() )
		{	
			tail = comparedbetweenBallandOpp.front();
			comparedbetweenBallandOpp.pop();
			avg += tail;
		
		}
		avg /= setdBetweenBallandOpp.size();
		if (WM.getBall().pos.to2D().x() < -7.5 && gAbs(avg - tail) > 0.28 &&  (gAbs(WM.getBall().pos.x()) < gAbs(WM.getSelf().pos.x())) && (oppnearesttoball.GetState() != TUMBLE )  )
			{cerr<<"!!!!!!!!!!!!!!!!!!!!!oppshooting!!!!!!!!!!!!!!!!!!!!!!"<<endl<<gAbs(avg - tail)<<endl;return true;}
		else 
			{aLOG<<"!!!!!!!!!!!!!!!!!!!!!!!!!!oppNOTshooting!!!!!!!!!!!!!!!!!!!!"<<endl;return false;}
	}	
	else return false;
}

bool WorldModel::Oppreadyforshoot()
{
	aLOG<<"Oppreadyforshoot judging"<<endl;
	if(setdBetweenBallandOpp.size() > 10)
	{
		comparedbetweenBallandOpp = setdBetweenBallandOpp;
		double avg2 =0, tail2 , minus = 0;
		while( !comparedbetweenBallandOpp.empty() )
		{	
			tail2 = comparedbetweenBallandOpp.front();
			comparedbetweenBallandOpp.pop();
			avg2 += tail2;
		
		}
		avg2 /= setdBetweenBallandOpp.size();
		comparedbetweenBallandOpp = setdBetweenBallandOpp;
		while( !comparedbetweenBallandOpp.empty() )
		{	
			tail2 = comparedbetweenBallandOpp.front();
			comparedbetweenBallandOpp.pop();
			minus += (tail2-avg2)*(tail2-avg2);
		
		}
		minus /= setdBetweenBallandOpp.size();
		minus = gSqrt(minus);
		if (gAbs(avg2) <= 0.20 && minus <0.08)
			{aLOG<<"!!!!!!!!!!!!!!!!!!!!!oppreadyforshoot!!!!!!!!!!!!!!!!!!!!!!"<<endl;return true;}
		else 
			{aLOG<<"!!!!!!!!!!!!!!!!!!!!!!!!!!oppNOTreadyforshoot!!!!!!!!!!!!!!!!!!!!"<<endl;return false;}
	}	
	else return false;
}	
//2009,12,23 ianpan end
