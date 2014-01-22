/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei                          *
 *   Copyright (C) 2009 by Zhu_Ming,Zheng Yonglei,Qu Junjun,Liu Zhengliang *
 *                         Pan Yan,Yaoyuan                                 *
 *   zhuming535984@gmail.com,zhengyonglei@gmail.com                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "NaoSpecialAction.h"
#include "self.h"
#include "nao.h"

NaoSpecialAction::NaoSpecialAction(const SpecialActionInterfaces& interfaces ):SpecialAction(interfaces),mInit(true),state(disable)
{
}


void NaoSpecialAction::Init()
{
	mIsDone = false;
    mPhase              = DIVINGSAVE1;
    mPhaseCycle         = 0;
    mPhaseStartCycle    = WM.getSelf().CurrentCycle;

    state = DivingAction;

}

void NaoSpecialAction::CalculateVel(JointID id, float angle, float maxVel)
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

void NaoSpecialAction::ApplyJVel(bool left, float slowgain)
{
    boost::shared_array<float> IKJointAngle = NAO->GetIKJointAngle();

	
    if (left) // left foot
    {
        mJointVel[Nao::JID_LLEG_1] = (IKJointAngle[Nao::JID_LLEG_1] - mJointAngle[Nao::JID_LLEG_1]) / slowgain;
        mJointVel[Nao::JID_LLEG_2] = (IKJointAngle[Nao::JID_LLEG_2] - mJointAngle[Nao::JID_LLEG_2]) / slowgain;
        mJointVel[Nao::JID_LLEG_3] = (IKJointAngle[Nao::JID_LLEG_3] - mJointAngle[Nao::JID_LLEG_3]) / slowgain;
        mJointVel[Nao::JID_LLEG_4] = (IKJointAngle[Nao::JID_LLEG_4] - mJointAngle[Nao::JID_LLEG_4]) / slowgain;
        mJointVel[Nao::JID_LLEG_5] = (IKJointAngle[Nao::JID_LLEG_5] - mJointAngle[Nao::JID_LLEG_5]) / slowgain;
        mJointVel[Nao::JID_LLEG_6] = (IKJointAngle[Nao::JID_LLEG_6] - mJointAngle[Nao::JID_LLEG_6]) / slowgain;
		
    }
    else // right foot
    {
        mJointVel[Nao::JID_RLEG_1] = (IKJointAngle[Nao::JID_RLEG_1] - mJointAngle[Nao::JID_RLEG_1]) / slowgain;
        mJointVel[Nao::JID_RLEG_2] = (IKJointAngle[Nao::JID_RLEG_2] - mJointAngle[Nao::JID_RLEG_2]) / slowgain;
        mJointVel[Nao::JID_RLEG_3] = (IKJointAngle[Nao::JID_RLEG_3] - mJointAngle[Nao::JID_RLEG_3]) / slowgain;
        mJointVel[Nao::JID_RLEG_4] = (IKJointAngle[Nao::JID_RLEG_4] - mJointAngle[Nao::JID_RLEG_4]) / slowgain;
        mJointVel[Nao::JID_RLEG_5] = (IKJointAngle[Nao::JID_RLEG_5] - mJointAngle[Nao::JID_RLEG_5]) / slowgain;
        mJointVel[Nao::JID_RLEG_6] = (IKJointAngle[Nao::JID_RLEG_6] - mJointAngle[Nao::JID_RLEG_6]) / slowgain;
		
    }
}

bool NaoSpecialAction::executeParameterized()
{
    aLOG << "NaoAction ERROR" << endl;
	if (mInit)
	{
		mIsDone = false;
		Init();
		mInit = false;
	}
	if(actionRequest.specialActionRequest.specialActionType == SpecialActionRequest::init)
	{
		InitPosition();
	}
	else
	{
		if (state == DivingAction)
		{
			DivingSave();
			switch (mPhase)
			{
			case DIVINGSAVE1:
				divingSave1();
				break;
			case DIVINGSAVE2:
				divingSave2();
				break;
			case DIVINGSAVE3:
				divingSave3();
				break;
	
			}
		}
	}
	if (mIsDone)
	{	
		mInit = true;
		return false;
	}

	return true;
}

void NaoSpecialAction::InitPosition()
{
	if(WM.getSelf().CurrentCycle - mPhaseStartCycle < 10)
	{
		CalculateVel(Nao::JID_LARM_1, -90.0, 40);
		CalculateVel(Nao::JID_RARM_1, -90.0, 40);
		CalculateVel(Nao::JID_LARM_2, 0.0, 40);
		CalculateVel(Nao::JID_RARM_2, 0.0, 40);
		CalculateVel(Nao::JID_LARM_3, 0.0, 40);
		CalculateVel(Nao::JID_RARM_3, 0.0, 40);
		CalculateVel(Nao::JID_LARM_4, 0.0, 40);
		CalculateVel(Nao::JID_RARM_4, 0.0, 40);
		CalculateVel(Nao::JID_LLEG_1, 0.0, 40);
		CalculateVel(Nao::JID_RLEG_1, 0.0, 40);
		CalculateVel(Nao::JID_LLEG_2, 0.0, 40);
		CalculateVel(Nao::JID_RLEG_2, 0.0, 40);
		CalculateVel(Nao::JID_LLEG_3, 0.0, 40);
		CalculateVel(Nao::JID_RLEG_3, 0.0, 40);
		CalculateVel(Nao::JID_LLEG_4, 0.0, 40);
		CalculateVel(Nao::JID_RLEG_4, 0.0, 40);
		CalculateVel(Nao::JID_LLEG_5, 0.0, 40);
		CalculateVel(Nao::JID_RLEG_5, 0.0, 40);
		CalculateVel(Nao::JID_LLEG_6, 0.0, 40);
		CalculateVel(Nao::JID_RLEG_6, 0.0, 40);
	}
	else
		mIsDone = true;
}

void NaoSpecialAction::DivingSave()
{
    aLOG << "DivingSave: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
    if ( mPhase == DIVINGSAVE1 && WM.getSelf().CurrentCycle - mPhaseStartCycle > 5 )
    {
        mPhase = DIVINGSAVE2;
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
    }
	else if ( mPhase == DIVINGSAVE2 && WM.getSelf().CurrentCycle - mPhaseStartCycle > 70 )
    {
        mPhase = DIVINGSAVE3;
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
    }
	else if ( mPhase == DIVINGSAVE3 && WM.getSelf().CurrentCycle - mPhaseStartCycle > 15 )
	{
		mIsDone = true;
	}

}

void NaoSpecialAction::divingSave1()
{
	cout<<"divingsave1  "<<endl;
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	oLOG << "specialActionType in action: " << actionRequest.specialActionRequest.specialActionType << endl;
	if(actionRequest.specialActionRequest.specialActionType == SpecialActionRequest::divingSaveLeft)
	{
		torsor.Pos() = Vector3f(0.01, -0.05, 0.345);

		leftfoot.Pos() = Vector3f(-0.055, 0.025, 0.07);

		rightfoot.Pos() = Vector3f(0.065, 0.025, 0.0);
	}
	else if(actionRequest.specialActionRequest.specialActionType == SpecialActionRequest::divingSaveRight)
	{
		torsor.Pos() = Vector3f(-0.01, 0.0, 0.345);

		leftfoot.Pos() = Vector3f(-0.065, 0.025, 0.0);

		rightfoot.Pos() = Vector3f(0.055, 0.025, 0.07);
	}
	cout << "torsor: " << '\n' << torsor 
		 << "leftfoot: " << '\n' << leftfoot
		 << "rightfoot: " << '\n' << rightfoot << endl;
	NAO->IK_leg(torsor, leftfoot, rightfoot);

	ApplyJVel(true, 0.05);
	ApplyJVel(false, 0.05);
}
void NaoSpecialAction::divingSave2()
{
    cout<<"divingsave2"<<endl;
    CalculateVel(Nao::JID_LARM_1, 0, 40);
    CalculateVel(Nao::JID_RARM_1, 0, 40);
    CalculateVel(Nao::JID_LARM_2, 0.0, 20);
    CalculateVel(Nao::JID_RARM_2, 0.0, 20);
    CalculateVel(Nao::JID_LARM_3, -20.0, 40);
    CalculateVel(Nao::JID_RARM_3, -20.0, 40);
    CalculateVel(Nao::JID_LARM_4, 0.0, 10);
    CalculateVel(Nao::JID_RARM_4, 0.0, 10);
    CalculateVel(Nao::JID_LLEG_1, -90.0, 45);
    CalculateVel(Nao::JID_RLEG_1, -90.0, 45);
    CalculateVel(Nao::JID_LLEG_2, 45.0, 45);
    CalculateVel(Nao::JID_RLEG_2, -45.0, 45);
    CalculateVel(Nao::JID_LLEG_3, 45.0, 45);
    CalculateVel(Nao::JID_RLEG_3, 45.0, 45);
    CalculateVel(Nao::JID_LLEG_4, 0.0, 45);
    CalculateVel(Nao::JID_RLEG_4, 0.0, 45);
    CalculateVel(Nao::JID_LLEG_5, 0.0, 10);
    CalculateVel(Nao::JID_RLEG_5, 0.0, 10);
    CalculateVel(Nao::JID_LLEG_6, .0, 10);
    CalculateVel(Nao::JID_RLEG_6, 0.0, 10);
}

void NaoSpecialAction::divingSave3()
{
    cout<<"divingsave3"<<endl;
    CalculateVel(Nao::JID_LARM_1, -50.0, 10);
    CalculateVel(Nao::JID_RARM_1, -50.0, 10);
    CalculateVel(Nao::JID_LARM_2, 0.0, 20);
    CalculateVel(Nao::JID_RARM_2, 0.0, 20);
    CalculateVel(Nao::JID_LARM_3, -90.0, 10);
    CalculateVel(Nao::JID_RARM_3, 90.0, 10);
    CalculateVel(Nao::JID_LARM_4, 0.0, 10);
    CalculateVel(Nao::JID_RARM_4, 0.0, 10);
    CalculateVel(Nao::JID_LLEG_1, -90.0, 50);
    CalculateVel(Nao::JID_RLEG_1, -90.0, 50);
    CalculateVel(Nao::JID_LLEG_2, 45.0, 90);
    CalculateVel(Nao::JID_RLEG_2, -45.0, 90);
    CalculateVel(Nao::JID_LLEG_3, 45.0, 50);
    CalculateVel(Nao::JID_RLEG_3, 45.0, 50);
    CalculateVel(Nao::JID_LLEG_4, 0.0, 10);
    CalculateVel(Nao::JID_RLEG_4, 0.0, 10);
    CalculateVel(Nao::JID_LLEG_5, 0.0, 10);
    CalculateVel(Nao::JID_RLEG_5, 0.0, 10);
    CalculateVel(Nao::JID_LLEG_6, .0, 10);
    CalculateVel(Nao::JID_RLEG_6, 0.0, 10);
}
