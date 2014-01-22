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
#ifndef WALK_H
#define WALK_H

#include "WalkingEngine.h"
#include "trajectory.h"
#include "worldmodel.h"
#include "self.h"

class NaoWalkingEngine : public WalkingEngine
{
public:
    enum TPhase {
        PREPARE,
        FIRST_STEP,
        NORMAL_STEP,
        MARK_TIME_STEP,
        LAST_STEP,
        STOP
    };

public:
    NaoWalkingEngine( const WalkingEngineInterfaces& interfaces);
    ~NaoWalkingEngine();
	 void Init();
    virtual bool executeParameterized(const WalkRequest& walkRequest, double positionInWalkingCycle);


    /** total steps finished */
    int GetTotalStep() const { return mTotalStep; }

    /** whether left foot move first */
    bool IsLeftFootMoveFirst() const { return mLeftFirstMove; }

    /** whether left foot on the ground */
    bool IsLeftFootHold() const { return  mLeftFirstMove ? (mTotalStep % 2 == 1) :
                                                           (mTotalStep % 2 == 0); }

    void SetWalkingSlowGain(float slowGain)
    {
        this->mWalkingSlowGain = slowGain;
    }

	/** calculate velocity need achieve the desired angle
     	*  for each Joint Angle according to JID
     	*/
    	void CalculateVel(JointID id, float angle, float maxVel);

	void smoothActionRequest(const WalkRequest& request, WalkRequest& currentRequest);//by Neil
protected:

    void Walking();

    void Stop();

    void SwingArms();

    void Prepare();
    bool IsPrepareDone() ;

    void FirstStep();
    bool IsFirstStepDone();

    void NormalStep();
    bool IsNormalStepDone();

    void LastStep();
    bool IsLastStepDone();

    bool IsStopDone();

    bool GeneralTrajectory();

    void ApplyJVel(bool left);

    void ApplyTrajectory(bool lefthold);

    void SetWalkParams();
	void ChangeWalkParams( WalkRequest currentRequest );

	Trajectory *wT;

protected:


    TPhase              mPhase;

    /** according to time */
    double              mPhaseTime;
    double              mPhaseStartTime;

    /** according to cycle */
    int                 mPhaseCycle;
    int                 mPhaseStartCycle;

    bool                mLeftFirstMove;

    /** false: when inconsistent walk parameters exist,
        calculation of inverse kinematics will fail
    */
    bool                IsIKfailNow;

    int                 mTotalStep;

    float               mSteadyParam;

    salt::Matrix        mTorsoMatrix;

    salt::Matrix        mHoldFootMatrix;

    salt::Matrix        mMoveFootMatrix;

    /** general slow gain, the final one used */
    float               mSlowGain;

    /** special slow gain for walking (not prepare or stop) */
    float               mWalkingSlowGain;

	bool mInit;
	bool mIsDone;

	WalkRequest currentRequest;
	salt::Vector3f mDs3D;//by Neil
	double rotation;
	float Ds;
	float Tc;
};


#endif
