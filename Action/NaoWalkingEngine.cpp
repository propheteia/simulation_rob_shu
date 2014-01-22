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

#include "NaoWalkingEngine.h"
#include "xmlParser.h"

using namespace std;
using namespace salt;
using namespace boost;
//using namespace zeitgeist;

NaoWalkingEngine::NaoWalkingEngine(const WalkingEngineInterfaces& interfaces):WalkingEngine(interfaces),
        IsIKfailNow(false),mSlowGain(0.1f),mWalkingSlowGain(0.08f),mDs3D(0.0f,0.0f,0.0f),Tc(0.16f),Ds(0.0f),mInit(true)
{
    wT = new Trajectory();
    currentRequest.walkParams.x() = 0.0f;
    currentRequest.walkParams.y() = 0.0f;
    currentRequest.rotation = 0.0f;
}

NaoWalkingEngine::~NaoWalkingEngine()
{
    delete wT;
}

void NaoWalkingEngine::Init()
{
    mPhase              = PREPARE;
    mPhaseCycle         = 0;
    mPhaseStartCycle    = WM.getSelf().CurrentCycle;
    mLeftFirstMove      = true;//false;
    mTotalStep          = 0;
    IsIKfailNow         = false;

    currentRequest.walkParams.x() = 0.0f;
    currentRequest.walkParams.y() = 0.0f;
    currentRequest.rotation = 0.0f;

	static bool inited = false;
	if(!inited)
	{
		if(1 == WM.getSelf().GetUnum())
		{
			mWalkingSlowGain = CONF.getXMLValue("/Strive/WalkingEngine/Golie/WalkingSlowGain").asFloat();
			Tc = CONF.getXMLValue("/Strive/WalkingEngine/Golie/Tc").asFloat();
		}
		else
		{
			mWalkingSlowGain = CONF.getXMLValue("/Strive/WalkingEngine/OtherPlayer/WalkingSlowGain").asFloat();
			Tc = CONF.getXMLValue("/Strive/WalkingEngine/OtherPlayer/Tc").asFloat();
		}
		LOG << "WalkingEngineParam Set Finished!" << mWalkingSlowGain << '\t' << Tc << endl;
		inited = true;
	}
}

void NaoWalkingEngine::ApplyTrajectory(bool lefthold)
{
    if (! NAO->IK_leg(mTorsoMatrix, mHoldFootMatrix, lefthold))
    {
        IsIKfailNow = true;
        return ;
    }
    ApplyJVel(lefthold);

    if (! NAO->IK_leg(mTorsoMatrix, mMoveFootMatrix, ! lefthold))
    {
        IsIKfailNow = true;
        return ;
    }

    ApplyJVel(! lefthold);
    IsIKfailNow = false;
}


bool NaoWalkingEngine::GeneralTrajectory()
{
    if (! wT->General())
    {
        cerr
        << "(Walk) ERROR: Trajectory general failed.\n";
        return false;
    }

    return true;
}

bool NaoWalkingEngine::executeParameterized(const WalkRequest& walkRequest, double positionInWalkingCycle)
{
    if (lastActionType != ActionRequest::walk)//reset the currentRequest for staleness
        mInit = true;
    if (mInit)
    {
        Init();
        mInit = false;
    }
    smoothActionRequest(walkRequest, currentRequest);

    aLOG << "ChangeWalkParams: " << endl;
    ChangeWalkParams(currentRequest);
    GeneralTrajectory();

	WM.getSelf().SetWalkingDsForLocalize(mDs3D,rotation);//for localization 

    if (actionInfo.stopWalking == false )
        Walking();
    else
    {
        Stop();
    }
    switch (mPhase)
    {
    case PREPARE:
        Prepare();
        break;

    case FIRST_STEP:
        FirstStep();
        break;

    case NORMAL_STEP:
        NormalStep();
        break;

    case LAST_STEP:
        LastStep();
        break;

    case STOP:
        Stop();
        break;
    default:
        break;
    }
    /** swing arms */
    SwingArms();
    return false;
}

void NaoWalkingEngine::Walking()
{
    mSlowGain = mWalkingSlowGain;

    if (mPhase == PREPARE && IsPrepareDone())
    {
        mPhase = FIRST_STEP;
        mPhaseStartCycle = WM.getSelf().CurrentCycle;
        mPhaseCycle = 0;
    }

    if ( (mPhase == FIRST_STEP && IsFirstStepDone()) ||
            (mPhase == NORMAL_STEP && IsNormalStepDone())
       )
    {
        mPhase = NORMAL_STEP;
        mPhaseStartCycle = WM.getSelf().CurrentCycle;
        mPhaseCycle = round(wT->GetTc()/0.02f);
        mTotalStep += 1;
    }
    //cout << "FirstStep  " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
    aLOG << "Ds: " << wT->GetDs() << endl;
}

void NaoWalkingEngine::Stop()
{
    mSlowGain = mWalkingSlowGain;
    if (mPhase == PREPARE && IsPrepareDone())
    {
        mPhase = STOP;
        return;
    }

    if ( (mPhase == FIRST_STEP && IsFirstStepDone()) ||
            (mPhase == NORMAL_STEP && IsNormalStepDone())
       )
    {
        mPhase = LAST_STEP;
        mPhaseStartCycle = WM.getSelf().CurrentCycle;
        mPhaseCycle = 0;
        mTotalStep += 1;
    }

    if (mPhase == LAST_STEP && IsLastStepDone())
    {
        mPhase = STOP;
        mPhaseStartCycle = WM.getSelf().CurrentCycle;
        mPhaseCycle = 0;
    }

    if (mPhase == STOP)
    {
        mIsDone = true;
    }
}

void NaoWalkingEngine::Prepare()
{
    float t = 0.0f;
    mIsDone = false;
//     mSlowGain = 0.08f;
    mSlowGain = 0.2f;
    bool lefthold = true;
    wT->GetStartTrajectoryMatrix(t, mTorsoMatrix,
                                 mHoldFootMatrix, mMoveFootMatrix, lefthold);

    ApplyTrajectory(true);
}

void NaoWalkingEngine::FirstStep()
{
    mPhaseCycle = WM.getSelf().CurrentCycle - mPhaseStartCycle;
    mPhaseTime = float(mPhaseCycle) * 0.02f;

    if ((mPhaseTime < 0.0f) || (mPhaseTime > wT->GetTs()))
    {
        return ;
    }

    bool lefthold = !mLeftFirstMove;
    wT->GetStartTrajectoryMatrix(mPhaseTime, mTorsoMatrix,
                                 mHoldFootMatrix, mMoveFootMatrix, lefthold);
    //cout << "FirstStep  " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
    ApplyTrajectory(lefthold);
}

bool NaoWalkingEngine::IsFirstStepDone()
{
    return mPhaseCycle >= round(wT->GetTs()/0.02f);
}

void NaoWalkingEngine::NormalStep()
{
    mPhaseCycle = WM.getSelf().CurrentCycle + round(wT->GetTc()/0.02f) - mPhaseStartCycle;
    mPhaseTime = float(mPhaseCycle) * 0.02f;

    if (mPhaseTime < wT->GetTc() || mPhaseTime > 2 * wT->GetTc())
    {
        return ;
    }

    bool lefthold = IsLeftFootHold();

    wT->GetTrajectoryMatrix(mPhaseTime, mTorsoMatrix,
                            mHoldFootMatrix, mMoveFootMatrix, lefthold);

    ApplyTrajectory(lefthold);
}

bool NaoWalkingEngine::IsNormalStepDone()
{
    // cout << __FUNCTION__ << ' ' << mPhaseCycle << ' '
    //     << round(2.0f * mTrajectory->GetTc()/0.02f) << endl;
    return mPhaseCycle >= 2.0f *  round(wT->GetTc()/0.02f) - 1;
}

/** It seems that there is something wrong in this function
 *  which result in furling the last foot with problem
 */
void NaoWalkingEngine::LastStep()
{
    mPhaseCycle = WM.getSelf().CurrentCycle - mPhaseStartCycle;
    mPhaseTime = float(mPhaseCycle) * 0.02f;

    float t = mPhaseTime;
    bool lefthold = IsLeftFootHold();

    if (t <= wT->GetTd())
    {
        t += wT->GetTc();

        wT->GetTrajectoryMatrix(t, mTorsoMatrix,
                                mHoldFootMatrix, mMoveFootMatrix, lefthold);
    }
    else
    {
        mSlowGain = 0.06;
        t -= wT->GetTd();
        if (( t < 0.0f )|| ( t > wT->GetTe())) return ;

        wT->GetEndTrajectoryMatrix(t, mTorsoMatrix,
                                   mHoldFootMatrix, mMoveFootMatrix, lefthold);
    }

    ApplyTrajectory(lefthold);
}

bool NaoWalkingEngine::IsLastStepDone()
{
    return mPhaseCycle >= round( (wT->GetTe() + wT->GetTd())/0.02f );
}

void NaoWalkingEngine::smoothActionRequest(const WalkRequest& request, WalkRequest& currentRequest)
{
    //this is done per frame, so maximum speed change is m/s
    //and degree/s:
    double smoothGain_x = 0.0015 ; //the origin is 0.0025
    double smoothGain_y = 0.0010 ; //the origin is 0.0015
    double smoothRotation = 5; //the origin is 5
    if ((request.walkParams.x() - currentRequest.walkParams.x()) > smoothGain_x)
    {
        currentRequest.walkParams.x() += smoothGain_x;
    }
    else if ((request.walkParams.x() - currentRequest.walkParams.x()) < -smoothGain_x)
    {
        currentRequest.walkParams.x() -= smoothGain_x;
    }
    else
    {
        currentRequest.walkParams.x() = request.walkParams.x();
    }

    if ((request.walkParams.y() - currentRequest.walkParams.y()) > smoothGain_y)
    {
        currentRequest.walkParams.y() += smoothGain_y;
    }
    else if ((request.walkParams.y() - currentRequest.walkParams.y()) < -smoothGain_y)
    {
        currentRequest.walkParams.y() -= smoothGain_y;
    }
    else
    {
        currentRequest.walkParams.y() = request.walkParams.y();
    }

    if ((request.rotation - currentRequest.rotation) > 5.0)
    {
        currentRequest.rotation += 5.0;
    }
    else if ((request.rotation - currentRequest.rotation) < -5.0)
    {
        currentRequest.rotation -= 5.0;
    }
    else
    {
        currentRequest.rotation = request.rotation;
    }
}

void NaoWalkingEngine::ApplyJVel(bool left)
{
    boost::shared_array<float> IKJointAngle = NAO->GetIKJointAngle();
    if (left) // left foot
    {
        mJointVel[Nao::JID_LLEG_1] = (IKJointAngle[Nao::JID_LLEG_1] - mJointAngle[Nao::JID_LLEG_1]) / mSlowGain;
        mJointVel[Nao::JID_LLEG_2] = (IKJointAngle[Nao::JID_LLEG_2] - mJointAngle[Nao::JID_LLEG_2]) / mSlowGain;
        mJointVel[Nao::JID_LLEG_3] = (IKJointAngle[Nao::JID_LLEG_3] - mJointAngle[Nao::JID_LLEG_3]) / mSlowGain;
        mJointVel[Nao::JID_LLEG_4] = (IKJointAngle[Nao::JID_LLEG_4] - mJointAngle[Nao::JID_LLEG_4]) / mSlowGain;
        mJointVel[Nao::JID_LLEG_5] = (IKJointAngle[Nao::JID_LLEG_5] - mJointAngle[Nao::JID_LLEG_5]) / mSlowGain;
        mJointVel[Nao::JID_LLEG_6] = (IKJointAngle[Nao::JID_LLEG_6] - mJointAngle[Nao::JID_LLEG_6]) / mSlowGain;
    }
    else // right foot
    {
        mJointVel[Nao::JID_RLEG_1] = (IKJointAngle[Nao::JID_RLEG_1] - mJointAngle[Nao::JID_RLEG_1]) / mSlowGain;
        mJointVel[Nao::JID_RLEG_2] = (IKJointAngle[Nao::JID_RLEG_2] - mJointAngle[Nao::JID_RLEG_2]) / mSlowGain;
        mJointVel[Nao::JID_RLEG_3] = (IKJointAngle[Nao::JID_RLEG_3] - mJointAngle[Nao::JID_RLEG_3]) / mSlowGain;
        mJointVel[Nao::JID_RLEG_4] = (IKJointAngle[Nao::JID_RLEG_4] - mJointAngle[Nao::JID_RLEG_4]) / mSlowGain;
        mJointVel[Nao::JID_RLEG_5] = (IKJointAngle[Nao::JID_RLEG_5] - mJointAngle[Nao::JID_RLEG_5]) / mSlowGain;
        mJointVel[Nao::JID_RLEG_6] = (IKJointAngle[Nao::JID_RLEG_6] - mJointAngle[Nao::JID_RLEG_6]) / mSlowGain;
    }
}

void NaoWalkingEngine::SetWalkParams()
{
    const float modelParams[10] =
    {
        0.035f, 0.08f, 0.08f,
        0.1201f, 0.1f, 0.015f, 0.015f,
        -10.0f, 0.115f, 0.055f
    };
    if (! wT->ModelInited())
        wT->InitModelParams(modelParams);

    float walkParams[10] =
    {
        0.0f, 0.0f,
        0.2f, 0.04f, 0.10f,
        0.0f, 0.0f,
        0.23f, 0.08f, 0.0f
    };

    wT->SetParams(walkParams);
}

void NaoWalkingEngine::ChangeWalkParams(WalkRequest currentRequest)
{
    mDs3D[1] = currentRequest.walkParams.x();
    mDs3D[0] = currentRequest.walkParams.y();
    rotation = currentRequest.rotation;
    Ds = Vector2f(0.0f, mDs3D.y()).Length();
    aLOG << "walkParamsX: " << mDs3D[0] << " walkParamsY: " << mDs3D[1] <<  "walkParamsR" << rotation << endl;
    float walkParams[10] =
    {
        mDs3D[0], mDs3D[1],
        Tc, 0.2*Tc, 0.5*Tc,
        0.48*Ds, 0.34f*Ds,
        0.23f, 0.08f, rotation
    };

    wT->SetParams(walkParams);


}


/** //NOTE the IK check of angle has been calculated in 'Prepare'
 *  here is time-costed, they should be stored.
*/
bool NaoWalkingEngine::IsPrepareDone()
{
    float t = 0.0f;

    bool lefthold = true;
    wT->GetStartTrajectoryMatrix(t, mTorsoMatrix,
                                 mHoldFootMatrix, mMoveFootMatrix, lefthold);

    float ld, rd;
    boost::shared_array<float> IKJointAngle = NAO->GetIKJointAngle();
    if (! NAO->IK_leg(mTorsoMatrix, mHoldFootMatrix, lefthold)) return false;
    ld = gRadToDeg(gAbs(mJointAngle[Nao::JID_LLEG_4] - IKJointAngle[Nao::JID_LLEG_4]));

    if (! NAO->IK_leg(mTorsoMatrix, mMoveFootMatrix, ! lefthold)) return false;
    rd = gRadToDeg(gAbs(mJointAngle[Nao::JID_RLEG_4] - IKJointAngle[Nao::JID_RLEG_4]));

    return (ld < 1.5 && rd < 1.5);
}

void NaoWalkingEngine::SwingArms()
{
    CalculateVel(Nao::JID_LARM_1, -90.0, 80);
    CalculateVel(Nao::JID_RARM_1, -90.0, 80);
    CalculateVel(Nao::JID_LARM_2, 7.0, 20);
    CalculateVel(Nao::JID_RARM_2, -7.0, 20);
    CalculateVel(Nao::JID_LARM_3, -90.0, 20);
    CalculateVel(Nao::JID_RARM_3, 90.0, 20);
    CalculateVel(Nao::JID_LARM_4, -35.0, 20);
    CalculateVel(Nao::JID_RARM_4, 35.0, 20);

    if (mPhase != NORMAL_STEP) return ;
    
//     Vector3f com = WM.getSelf().GetRobotCoM();
//     float z = com.z();
//     float ax = 9.8 * com.x()/z;
//     float ay = 9.8 * com.y()/z;
//     Vector3f agent = WM.getSelf().pos;
//     Vector3f ball = WM.getBall().pos;
//     Vector2f balltoagent = ball.to2D() - agent.to2D();
//     double BallToDestination = gNormalizeDeg( balltoagent.GetAngleDeg()-WM.getSelf().GetTorsoYawAngle());

    float tc    = wT->GetTc();
    float ds = wT->GetDs();
    float angle = ((mPhaseCycle * 0.02f - tc) / tc * 80 - 40)/**gAbs(mDs3D[1])/0.12*/;
    float delta = -90;
    float angleArm2 = ((mPhaseCycle * 0.02f - tc) / tc * 7 )/**mDs3D[0]/0.04*/;
    float angleArm4 = ((mPhaseCycle * 0.02f - tc) / tc * 20 - 10)/**gAbs(mDs3D[1])/0.12*/;

    if (! IsLeftFootHold()) angle *= -1;
    CalculateVel(Nao::JID_LARM_1,  angle+delta, 20);
    CalculateVel(Nao::JID_RARM_1, -angle+delta, 20);
    CalculateVel(Nao::JID_LARM_2, 7.0-angleArm2, 20);
    CalculateVel(Nao::JID_RARM_2, -7.0-angleArm2, 20);
    CalculateVel(Nao::JID_LARM_4, -35.0-angleArm4, 20);
    CalculateVel(Nao::JID_RARM_4, 35.0-angleArm4, 20);
//     if (BallToDestination > 0)
//     {
//       if (ax > -0.8)
//       {
//       CalculateVel(Nao::JID_LARM_1,  angle+delta, 20);
//       CalculateVel(Nao::JID_RARM_1, -angle+delta, 20);
//       CalculateVel(Nao::JID_LARM_4, -35.0-angleArm4, 20);
//       CalculateVel(Nao::JID_RARM_4, 35.0-angleArm4, 20);
//       }
//       else if (ax <= -0.8)
//       {
//       if (-ax *15 <60)
//       {
//       CalculateVel(Nao::JID_LARM_1,  angle+delta+ax*15, 20);
//       CalculateVel(Nao::JID_RARM_1, -angle+delta+ax*15, 20);
//       }
//       else
//       {
//       CalculateVel(Nao::JID_LARM_1,  angle+delta-60, 20);
//       CalculateVel(Nao::JID_RARM_1, -angle+delta-60, 20);	
//       }
//       CalculateVel(Nao::JID_LARM_4, -35.0-angleArm4, 20);
//       CalculateVel(Nao::JID_RARM_4, 35.0-angleArm4, 20);
//       }
//     }
//     else
//     {
//       if (ax > -0.2)
//       {
//       CalculateVel(Nao::JID_LARM_1,  angle+delta, 20);
//       CalculateVel(Nao::JID_RARM_1, -angle+delta, 20);
//       CalculateVel(Nao::JID_LARM_4, -35.0-angleArm4, 20);
//       CalculateVel(Nao::JID_RARM_4, 35.0-angleArm4, 20);
//       }
//       else if (ax <= -0.2)
//       {
//       if (-ax *15 <60)
//       {
//       CalculateVel(Nao::JID_LARM_1,  angle+delta-ax*15, 20);
//       CalculateVel(Nao::JID_RARM_1, -angle+delta-ax*15, 20);
//       }
//       else
//       {
//       CalculateVel(Nao::JID_LARM_1,  angle+delta+60, 20);
//       CalculateVel(Nao::JID_RARM_1, -angle+delta+60, 20);	
//       }
//       CalculateVel(Nao::JID_LARM_4, -35.0-angleArm4, 20);
//       CalculateVel(Nao::JID_RARM_4, 35.0-angleArm4, 20);
//       }
//     }
//     
//     if (ay > 2.5 && ay < -2.5)
//     {
//       CalculateVel(Nao::JID_LARM_2, 7.0-angleArm2, 20);
//       CalculateVel(Nao::JID_RARM_2, -7.0-angleArm2, 20);
//     }
//     else if (ay >= 2.5 )
//     {
//       if (ay*5 < 60)
//       CalculateVel(Nao::JID_LARM_2,7.0-4*angleArm2+ay*5,20 );
//       else
//       CalculateVel(Nao::JID_LARM_2,7.0-4*angleArm2+60,20 );
//       CalculateVel(Nao::JID_RARM_2, -7.0-angleArm2, 20);
//     }
//     else if (ay <= -2.5)
//     {
//       CalculateVel(Nao::JID_LARM_2, 7.0-angleArm2, 20);
//       if (-ay*5 < 60)
//       CalculateVel(Nao::JID_RARM_2, -7.0-4*angleArm2 + ay*5, 20 );
//       else
//       CalculateVel(Nao::JID_RARM_2, -7.0-4*angleArm2 -60, 20 );
//     }
}

void NaoWalkingEngine::CalculateVel(JointID id, float angle, float maxVel)
{
    if (maxVel < 0)
    {
        aLOG << "NaoAction ERROR: (CalculateVel) "
        << "maxVel < 0" << endl;
        mJointVel[id] = 0.0f;
        return ;
    }

    float curAngle = gRadToDeg(mJointAngle[id]);
    float minus = gNormalizeDeg(angle - curAngle);
    float vel = 0.0;

    vel = gAbs(minus) > maxVel ? maxVel * gSign(minus) : minus;
    vel = std::min(gDegToRad(vel) * 10.0f, 100.0f);
    //aLOG << "minus: " << minus << "       vel: " << vel << endl;
    mJointVel[id] = vel;
}
