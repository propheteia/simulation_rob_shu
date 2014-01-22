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


#include "NaoDynamicKick.h"
#include "self.h"
#include "nao.h"
#include "worldmodel.h"
#include "ball.h"

NaoDynamicKickEngine::NaoDynamicKickEngine(const KickEngineInterfaces& interfaces ):KickEngine(interfaces),mInit(true),state(disable)
{	
}

void NaoDynamicKickEngine::Init()
{
     	mPhase              = KICKL1;
	mPhase2             = KICKR1;
	mPhase3		    = PASS1;
	mPhase4		    = PASSQ1;
    	mPhaseCycle         = 0;
    	mPhaseStartCycle    = WM.getSelf().CurrentCycle;
	if(actionRequest.kickRequest.type == 1)
		state = KICKLEFT;
	else if (actionRequest.kickRequest.type == 0)
		state = KICKRIGHT;
	else if (actionRequest.kickRequest.type == 2)
		state = PASS;
	else
		state = PASSQ;
}

void NaoDynamicKickEngine::CalculateVel(JointID id, float angle, float maxVel)
{
	//WM.getSelf().mCoMIsStable();
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

void NaoDynamicKickEngine::CalculateVel2(JointID id, float angle, float maxVel)
{
	WM.getSelf().mCoMIsStable();
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

	vel =  maxVel * gSign(minus) ;
	vel = std::min(gDegToRad(vel) * 10.0f, 100.0f);
	//aLOG << "minus: " << minus << "       vel: " << vel << endl;
	mJointVel[id] = vel;
}
void NaoDynamicKickEngine::CalculateVel3(JointID id, float angle, float maxVel)
{
	WM.getSelf().mCoMIsStable();
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

	vel = gAbs(minus) > maxVel ? maxVel * gSign(minus) : 3.5*minus;
	vel = std::min(gDegToRad(vel) * 15.0f,100.0f);
	//aLOG << "minus: " << minus << "       vel: " << vel << endl;
	mJointVel[id] = vel;
}

void NaoDynamicKickEngine::ApplyJVel(bool left, float slowgain)
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

		
bool NaoDynamicKickEngine::executeParameterized()
{
	if(mInit)
	{	Init();
		mInit = false;
	}
	
	if(state == KICKLEFT)
	{
		KickingLeft();
		aLOG << " KickLeft Phase" << mPhase <<endl;
		switch (mPhase)
    	{
			case KICKL1:
				kickLeft1();
				break;
			case KICKL2:
				kickLeft2();
				break;
			case KICKL3:
				kickLeft3();
				break;
			case KICKL4:
				kickLeft4();
				break;
			default:
				break;		
		}
	}
	if(state == KICKRIGHT)
	{
		KickingRight();
		aLOG << " KickRight Phase" << mPhase2 <<endl;
		switch (mPhase2)
    	{
			case KICKR1:
				kickRight1();
				break;
			case KICKR2:
				kickRight2();
				break;
			case KICKR3:
				kickRight3();
				break;
			case KICKR4:
				kickRight4();
				break;
			default:
				break;		
		}
	}
	
	if(state == PASS)
	{
		Passing();
		aLOG << " Passing Phase" << mPhase <<endl;
		switch (mPhase3)
    		{
        		case PASS1:
				pass1();
			break;
			case PASS2:
				pass2();
			break;
			case PASS3:
				pass3();
			break;
			case PASS4:
				pass4();
			break;
			case PASS5:
				pass5();
			break;
			
		
		
		}
	}
	
	if (state == PASSQ)
	{
		PassingQuick();
		aLOG << " PassingQuick Phase" << mPhase <<endl;
		switch (mPhase4)
    		{
        		case PASSQ1:
				passq1();
			break;
			case PASSQ2:
				passq2();
			break;
			case PASSQ3:
				passq3();
			break;
			case PASSQ4:
				passq4();
			break;
			case PASSQ5:
				passq5();
			break;
		}
	}


	
	if (mIsDone)
	{	mInit = true;
		return false;
	} 
	return true;
}

void NaoDynamicKickEngine::KickingLeft()
{	aLOG << "KickLeft: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	if( mPhase == KICKL1 && CycleInPhase >=15 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
 		mPhase = KICKL3;
	}
//	if( mPhase == KICKL2 && CycleInPhase>=20)
//	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
//		mPhase = KICKL3;
//	}
	if( mPhase == KICKL3 && CycleInPhase > 20 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase =KICKL4;
	}
	if( mPhase == KICKL4 && CycleInPhase > 50 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = KICKL5;
		mIsDone = true;
	}
}

void NaoDynamicKickEngine::KickingRight()
{	aLOG << "KickRight: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	if( mPhase2 == KICKR1 && CycleInPhase >=15 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
 		mPhase2 = KICKR3;
	}
//	if( mPhase2 == KICKR2 && CycleInPhase>=20)
//	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
//		mPhase2 = KICKR3;
//	}
	if( mPhase2 == KICKR3 && CycleInPhase > 20 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase2 =KICKR4;
	}
	if( mPhase2 == KICKR4 && CycleInPhase > 50 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase2 = KICKR5;
		mIsDone = true;
	}
}

void NaoDynamicKickEngine::Passing()
{	aLOG << "Passing: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
	
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	if( mPhase3 == PASS1 && CycleInPhase >=20 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
 		mPhase3 = PASS2;
	}
	if( mPhase3 == PASS2 && CycleInPhase>=20)
	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase3 = PASS3;
	}
	if( mPhase3 == PASS3 && CycleInPhase > 20 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase3 = PASS4;
	}
	if( mPhase3 == PASS4 && CycleInPhase > 50 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase3 = PASS5;
		mIsDone = true;
	}

}

void NaoDynamicKickEngine::PassingQuick()
{	aLOG << "PassingQuick: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
	
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	if( mPhase4 == PASSQ1 && CycleInPhase >=10 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
 		mPhase4 = PASSQ3;
	}
// 	if( mPhase4 == PASSQ2 && CycleInPhase>=20)
// 	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
// 		mPhase4 = PASSQ3;
// 	}
	if( mPhase4 == PASSQ3 && CycleInPhase > 20 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase4 = PASSQ4;
	}
	if( mPhase4 == PASSQ4 && CycleInPhase > 50 )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase4 = PASSQ5;
		mIsDone = true;
	}

}

 void NaoDynamicKickEngine::kickLeft1()
{	
	mIsDone = false;
	cout<<"Kick1"<<endl;
// 	CalculateVel(Nao::JID_LARM_1, -120.0, 3);  CalculateVel(Nao::JID_RARM_1, -120.0, 3);
//     	CalculateVel(Nao::JID_LARM_2, 20.0, 3);    CalculateVel(Nao::JID_RARM_2, -10.0, 3);
//    	CalculateVel(Nao::JID_LARM_3, -80.0, 3);  CalculateVel(Nao::JID_RARM_3, 80.0, 3);
//     	CalculateVel(Nao::JID_LARM_4, -80.0, 3);  CalculateVel(Nao::JID_RARM_4, 80.0, 3);
// 	CalculateVel(Nao::JID_LLEG_1, 0.0, 3);  CalculateVel(Nao::JID_RLEG_1, 0.0, 3);
//     	CalculateVel(Nao::JID_LLEG_2, 0.0, 3);    CalculateVel(Nao::JID_RLEG_2, 0.0, 3);
//    	CalculateVel(Nao::JID_LLEG_3, 32.0, 3);  CalculateVel(Nao::JID_RLEG_3, 32.0, 3);
//     	CalculateVel(Nao::JID_LLEG_4, -61.0, 3);  CalculateVel(Nao::JID_RLEG_4, -61.0, 3);
// 	CalculateVel(Nao::JID_LLEG_5, 32.0, 3);  CalculateVel(Nao::JID_RLEG_5, 32.0, 3);
//     	CalculateVel(Nao::JID_LLEG_6, 0.0, 3);  CalculateVel(Nao::JID_RLEG_6, 0.0, 3);
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	
	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);

	leftfoot.Pos() = Vector3f(-0.055, 0.0, 0.-0.36);

	rightfoot.Pos() = Vector3f(0.055, 0.0, -0.36);

	NAO->IK_leg(torsor, leftfoot, rightfoot);

 	ApplyJVel(true, 0.15);
 	ApplyJVel(false, 0.15);
 }
 
void NaoDynamicKickEngine::kickLeft2()
{
	cout<<"Kick2"<<endl;
  	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 2);
    	CalculateVel(Nao::JID_LLEG_2, 20.0, 2);    CalculateVel(Nao::JID_RLEG_2, 19.0, 2);
   	CalculateVel(Nao::JID_LLEG_3, 6.0, 4);  CalculateVel(Nao::JID_RLEG_3, 5.0, 6);
    	CalculateVel(Nao::JID_LLEG_4, -10.0, 4);  CalculateVel(Nao::JID_RLEG_4, -9.0, 5);
	CalculateVel(Nao::JID_LLEG_5, 6.0, 4);  CalculateVel(Nao::JID_RLEG_5, 5.0, 5);
    	CalculateVel(Nao::JID_LLEG_6, -20.0, 2);  CalculateVel(Nao::JID_RLEG_6, -19.0, 3);
// 	Matrix torsor(Matrix::GetIdentity());
// 	Matrix leftfoot(Matrix::GetIdentity());
// 	Matrix rightfoot(Matrix::GetIdentity());
// 	
// 	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);
// 	
// 	leftfoot.Pos() = Vector3f(-0.055, 0.0, 0.-0.36);
// 
// 	rightfoot.Pos() = Vector3f(0.055, 0.0, -0.36);
// 
// 	NAO->IK_leg(torsor, leftfoot, rightfoot);
// 
// 	ApplyJVel(true, 0.2);
// 	ApplyJVel(false, 0.2);
}

void NaoDynamicKickEngine::kickLeft3()
{ 
    Vector3f com = WM.getSelf().GetRobotCoM();
    float z = com.z();
    float ax = 9.8 * com.x()/z;
    float ay = 9.8 * com.y()/z;
    
	//!During Cycle 0~19 Do Actions as Following(Moving Center of Mass)
  	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
// 	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 2);
//     	CalculateVel(Nao::JID_LLEG_2, 10.0, 2);    CalculateVel(Nao::JID_RLEG_2, 19.0, 2);
//    	CalculateVel(Nao::JID_LLEG_3, 6.0, 4);  CalculateVel(Nao::JID_RLEG_3, 5.0, 6);
//     	CalculateVel(Nao::JID_LLEG_4, -10.0, 4);  CalculateVel(Nao::JID_RLEG_4, -9.0, 5);
// 	CalculateVel(Nao::JID_LLEG_5, 6.0, 4);  CalculateVel(Nao::JID_RLEG_5, 5.0, 5);
//     	CalculateVel(Nao::JID_LLEG_6, -10.0, 2);  CalculateVel(Nao::JID_RLEG_6, -10.0, 3);
		
	//!At Cycle 20(the Final Cycle in this Phase) Calculate the Motion Spline for Kick(Cubic Spline Curve)
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	if(CycleInPhase > 19)
	{
		
		float Force = 1.0;	//!Force: 0.0~1.0
//		float Direction = 10.0;	//!Direction: 0.0~30.0
		Vector3f BallLocalPostoBody = WM.getBall().GetLocalPos() - WM.getSelf().GetHeadLocalPosition() ;
		BallLocalPostoBody.z() = BallLocalPostoBody.x();
		BallLocalPostoBody.x() = -BallLocalPostoBody.y();
		BallLocalPostoBody.y() = BallLocalPostoBody.z();
		BallLocalPostoBody.z() = 0.00;
		Vector3f BallLocalPostoRightFoot = WM.getBall().GetLocalPos() - WM.getSelf().GetRightFootLocalPosition();
		BallLocalPostoRightFoot.z() = BallLocalPostoRightFoot.x();
		BallLocalPostoRightFoot.x() = -BallLocalPostoRightFoot.y();
		BallLocalPostoRightFoot.y() = BallLocalPostoRightFoot.z();
// 		BallLocalPostoRightFoot.z() = 0.042;
		BallLocalPostoRightFoot.z() = 0;
		Vector3f BallLocalPos = WM.getBall().GetLocalPos();
		BallLocalPos.z() = BallLocalPos.x();
		BallLocalPos.x() = -BallLocalPos.y();
		BallLocalPos.y() = BallLocalPos.z();

		aLOG << "BallLocalPostoRightFoot: " << BallLocalPostoRightFoot << endl;
		//!T is the Key Cycles. After 0.8s is the Kick Phase in which the Kick foot moves powerfully.
		float T[6] = 			{0.00, 0.30, 0.60, 0.80, 0.88, 1.0};
		//!For Kick Power Fixing
		T[5] = T[5]/(0.9 + Force * 0.1);
		
// 		float TorsorX[6] = 		{-0.007, -0.007, -0.003, -0.0032, -0.0031, -0.0065};
		float TorsorX[6] = 		{-0.006, -0.006, -0.001, -0.0012, -0.0011, -0.0055};
// 		TorsorX[0] = BallLocalPostoBody.x() + 0.027;
// 		TorsorX[1] = BallLocalPostoBody.x() + 0.027;
// 		TorsorX[2] = BallLocalPostoBody.x() + 0.03;
// 		TorsorX[0] = BallLocalPos.x() + 0.01;
// 		TorsorX[1] = BallLocalPos.x() + 0.01;
// 		TorsorX[2] = BallLocalPos.x() + 0.006;
// 		TorsorX[3] = BallLocalPostoBody.x() + 0.0253;
// 		TorsorX[4] = BallLocalPostoBody.x() + 0.0244;
// 		TorsorX[5] = BallLocalPostoBody.x() + 0.021;
		float TorsorY[6] = 		{-0.025, -0.025, -0.0222, -0.0216, -0.0275, 0.0};
		TorsorY[0] = BallLocalPostoBody.y() - 0.16;
		TorsorY[1] = BallLocalPostoBody.y() - 0.16;
// 		TorsorY[2] = BallLocalPostoBody.y() - 0.1572;
// 		TorsorY[3] = BallLocalPostoBody.y() - 0.1566;
// 		TorsorY[4] = BallLocalPostoBody.y() - 0.1425;
// 		TorsorY[5] = BallLocalPostoBody.y() - 0.135;
		float TorsorZ[6] = 		{0.366, 0.366, 0.366, 0.366, 0.366, 0.366};
		float TorsorRx[6] = 	{0.00, 0.00, 0.00, 1.2, -1.8, 9.0};
		float TorsorRy[6] = 	{0.00, 0.00, 0.00, 3.1, 4.0, 3.0};
		float TorsorRz[6] = 	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		
		
		float KickFootX[6] =	{-0.11, -0.11, -0.1075, -0.1, -0.08, -0.05};
		//!For Spline Fixing When Direction is not 0.
// 		KickFootX[0] = BallLocalPos.x()-0.035*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[1] = BallLocalPos.x()-0.035*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[2] = BallLocalPos.x()-0.03025*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[3] = BallLocalPos.x()-0.025*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[4] = BallLocalPos.x()-0.015*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[5] = BallLocalPos.x()+0.015*(1-actionRequest.kickRequest.Direction/30.0);
		
// 		KickFootX[0] = BallLocalPostoRightFoot.x()-0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[1] = BallLocalPostoRightFoot.x()-0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[2] = BallLocalPostoRightFoot.x()-0.04025*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[3] = BallLocalPostoRightFoot.x()-0.04*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[4] = BallLocalPostoRightFoot.x()-0.02*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[5] = BallLocalPostoRightFoot.x()+0.015*(1-actionRequest.kickRequest.Direction/30.0);
		float KickFootY[6] = 	{0.0072, 0.0072, 0.0079, -0.04, 0.00, 0.2};
// 		KickFootY[0] = BallLocalPostoRightFoot.y() - 0.14;
// 		KickFootY[1] = BallLocalPostoRightFoot.y() - 0.14;
// 		KickFootY[2] = BallLocalPostoRightFoot.y() - 0.15;
// 		KickFootY[3] = BallLocalPostoRightFoot.y() - 0.18;
// 		KickFootY[4] = BallLocalPostoRightFoot.y() - 0.135;
// 		KickFootY[5] = BallLocalPostoRightFoot.y() + 0.06;
// 		
// 		KickFootY[0] = BallLocalPos.y() - 0.16;
// 		KickFootY[1] = BallLocalPos.y() - 0.16;
// 		KickFootY[2] = BallLocalPos.y() - 0.18;
// 		KickFootY[3] = BallLocalPos.y() - 0.21;
// 		KickFootY[4] = BallLocalPos.y() - 0.155;
// 		KickFootY[5] = BallLocalPos.y() + 0.1;

// 		float KickFootZ[6] = 	{0.0082, 0.02, 0.04, 0.07, 0.029, 0.070};
		float KickFootZ[6] = 	{0.0082, 0.02, 0.04, 0.07, 0.029, 0.070};
		float KickFootRx[6] =	{0.0, 0.0, -7.0, -12.0, 7.0, 12.0};
		float KickFootRy[6] =	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		float KickFootRz[6] =	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		
		KickFootRz[4] = actionRequest.kickRequest.Direction/3.0;
		KickFootRz[5] = actionRequest.kickRequest.Direction;
		
		mSplineTorsorX.SetBreakPoints(T, TorsorX, 6);
		mSplineTorsorY.SetBreakPoints(T, TorsorY, 6);
		mSplineTorsorZ.SetBreakPoints(T, TorsorZ, 6);
		mSplineTorsorRx.SetBreakPoints(T, TorsorRx, 6);
		mSplineTorsorRy.SetBreakPoints(T, TorsorRy, 6);
		mSplineTorsorRz.SetBreakPoints(T, TorsorRz, 6);
		
		if(!mSplineTorsorX.General())
			return;
		if(!mSplineTorsorY.General())
			return;
		if(!mSplineTorsorZ.General())
			return;
		if(!mSplineTorsorRx.General())
			return;
		if(!mSplineTorsorRy.General())
			return;
		if(!mSplineTorsorRz.General())
			return;
		
		mSplineKickFootX.SetBreakPoints(T, KickFootX, 6);
		mSplineKickFootY.SetBreakPoints(T, KickFootY, 6);
		mSplineKickFootZ.SetBreakPoints(T, KickFootZ, 6);
		mSplineKickFootRx.SetBreakPoints(T, KickFootRx, 6);
		mSplineKickFootRy.SetBreakPoints(T, KickFootRy, 6);
		mSplineKickFootRz.SetBreakPoints(T, KickFootRz, 6);
		
		if(!mSplineKickFootX.General())
			return;
		if(!mSplineKickFootY.General())
			return;
		if(!mSplineKickFootZ.General())
			return;
		if(!mSplineKickFootRx.General())
			return;
		if(!mSplineKickFootRy.General())
			return;
		if(!mSplineKickFootRz.General())
			return;
		
// 		for(float t = 0.00; t < 1.0; t += 0.02)
// 		{
// 			oLOG << t << ' ' 	<< mSplineTorsorX.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorY.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorZ.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorRx.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorRy.GetYGivenX(t) << ' '
// 								<< mSplineTorsorRz.GetYGivenX(t) << ' ' 
// 								<< mSplineKickFootX.GetYGivenX(t) << ' '
// 								<< mSplineKickFootY.GetYGivenX(t) << ' '
// 								<< mSplineKickFootZ.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRx.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRy.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRz.GetYGivenX(t) << ' ' << endl;
// 		}
	}
}

void NaoDynamicKickEngine::kickLeft4()
{	
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	float t = CycleInPhase * 0.02;
	
	//! Motions of Arms
    CalculateVel(Nao::JID_LARM_1, -120.0, 30);  CalculateVel(Nao::JID_RARM_1, -120.0, 30);
    CalculateVel(Nao::JID_LARM_2, 20.0, 30);    CalculateVel(Nao::JID_RARM_2, -10.0, 30);
    CalculateVel(Nao::JID_LARM_3, -80.0, 30);  CalculateVel(Nao::JID_RARM_3, 80.0, 30);
    CalculateVel(Nao::JID_LARM_4, -80.0, 30);  CalculateVel(Nao::JID_RARM_4, 80.0, 30);
	
	//! Get the Matrixs at Current Cycle from the Kick Spline
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	
	torsor.Pos() = Vector3f(mSplineTorsorX.GetYGivenX(t),
							mSplineTorsorY.GetYGivenX(t),
							mSplineTorsorZ.GetYGivenX(t));
	torsor.RotateX(gDegToRad(mSplineTorsorRx.GetYGivenX(t)));
	torsor.RotateY(gDegToRad(mSplineTorsorRy.GetYGivenX(t)));
	torsor.RotateZ(gDegToRad(mSplineTorsorRz.GetYGivenX(t)));
	
	leftfoot.Pos() = Vector3f(mSplineKickFootX.GetYGivenX(t),
							mSplineKickFootY.GetYGivenX(t),
							mSplineKickFootZ.GetYGivenX(t));
	leftfoot.RotateX(gDegToRad(mSplineKickFootRx.GetYGivenX(t)));
	leftfoot.RotateY(gDegToRad(mSplineKickFootRy.GetYGivenX(t)));
	leftfoot.RotateZ(gDegToRad(mSplineKickFootRz.GetYGivenX(t)));
	
	NAO->IK_leg(torsor, leftfoot, rightfoot);

	if(t < 0.8)
	{
		ApplyJVel(true, 0.1);
		ApplyJVel(false, 0.1);
	}
	else
	{
		ApplyJVel(true, 0.04);
		ApplyJVel(false, 0.02);
	}

// 	oLOG << WM.getFieldInfo().match_time << ' ';
// 	for(JointID j = Nao::JID_HEAD_1; j <= Nao::JID_RLEG_6; j++)
// 	{
// 		oLOG << mJointAngle[j] << ' ';
// 	}
// 	oLOG << endl;

}
// void NaoDynamicKickEngine::kickLeft2()
// {	
// 	Matrix torsor(Matrix::GetIdentity());
// 	Matrix leftfoot(Matrix::GetIdentity());
// 	Matrix rightfoot(Matrix::GetIdentity());
// 	torsor.Right() = Vector3f(1, 0, 0);
// 	torsor.Up() = Vector3f(0, 1, 0);
// 	torsor.Forward() = Vector3f(0, 0, 1);
// 	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);
// 
// 	leftfoot.Right() = Vector3f(1, 0, 0);
// 	leftfoot.Up() = Vector3f(0, 1, 0);
// 	leftfoot.Forward() = Vector3f(0, 0, 1);
// 	leftfoot.Pos() = Vector3f(0.055, 0.0, 0.-0.34);
// 
// 	rightfoot.Right() = Vector3f(1, 0, 0);
// 	rightfoot.Up() = Vector3f(0, 1, 0);
// 	rightfoot.Forward() = Vector3f(0, 0, 1);
// 	rightfoot.Pos() = Vector3f(0.065, 0.0, -0.3);
// 
// 	NAO->IK_leg(torsor, leftfoot, rightfoot);
// 
// 	ApplyJVel(true, 0.5);
// 	ApplyJVel(false, 0.2);
// 
// }

void NaoDynamicKickEngine::kickLeft5()
{	
	Matrix torsor(0.998597, -0.00345847, 0.0528305, -0.0207504,
0.00259652, 0.999863, 0.0163753, 0.0024535,
-0.0528799, -0.0162151, 0.998469, 0.331,
0, 0, 0, 0);
	Matrix leftfoot(0.998316, -0.0273728, 0.0511464, -0.125051,
0.0207842, 0.991917, 0.125177, 0.0167485,
-0.0541594, -0.123903, 0.990815, -0.00254771,
0, 0, 0, 0);
	Matrix rightfoot(1, 1.04145e-09, 2.70066e-08, -0.0175075,
1.18175e-09, 1, 7.32791e-09, 0.024634,
1.0293e-08, 8.89331e-10, 1, -0.0355508,
0, 0, 0, 0);

// 	torsor.Right() = Vector3f(1, 0, 0);
// 	torsor.Up() = Vector3f(0, 1, 0);
// 	torsor.Forward() = Vector3f(0, 0, 1);
// 	torsor.Pos() = Vector3f(-0.045, 0.02, 0.37);
// 
// 	leftfoot.Right() = Vector3f(1, 0, 0);
// 	leftfoot.Up() = Vector3f(0, 1, 0);
// 	leftfoot.Forward() = Vector3f(0, 0, 1);
// 	leftfoot.Pos() = Vector3f(-0.055, 0.025, 0.015);
// 
// 	rightfoot.RotationX(0.3);
// // 	rightfoot.Right() = Vector3f(1, 0, 0);
// // 	rightfoot.Up() = Vector3f(0, 1, 0);
// // 	rightfoot.Forward() = Vector3f(0, 0, 1);
// 	rightfoot.Pos() = Vector3f(0.01, 0.2, 0.09);

	NAO->IK_leg(torsor, leftfoot, rightfoot);

	ApplyJVel(true, 0.05);
	ApplyJVel(false, 0.05);
}

void NaoDynamicKickEngine::kickLeft6()
{	cout<<"Kick6"<<endl;
	CalculateVel(Nao::JID_LARM_1, 0.0, 20);  CalculateVel(Nao::JID_RARM_1, 0.0, 20);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 20);    CalculateVel(Nao::JID_RARM_2, 0.0, 20);
   	CalculateVel(Nao::JID_LARM_3, 0.0, 20);  CalculateVel(Nao::JID_RARM_3, 0.0, 20);
    	CalculateVel(Nao::JID_LARM_4, 0.0, 20);  CalculateVel(Nao::JID_RARM_4, 0.0, 20);
	CalculateVel(Nao::JID_LLEG_1, 1.0, 7);  CalculateVel(Nao::JID_RLEG_1, 1.0, 7);
	CalculateVel(Nao::JID_LLEG_2, 0.0, 30);  CalculateVel(Nao::JID_RLEG_2, 6.0, 30);
	CalculateVel(Nao::JID_LLEG_3, 8.0, 30);  CalculateVel(Nao::JID_RLEG_3, 24.0, 30);
	CalculateVel(Nao::JID_LLEG_4, -56.0, 30);  CalculateVel(Nao::JID_RLEG_4, -63.0, 30);
	CalculateVel(Nao::JID_LLEG_5, 42.0, 30);  CalculateVel(Nao::JID_RLEG_5, 10.0, 30);
	CalculateVel(Nao::JID_LLEG_6, 11.0, 10);  CalculateVel(Nao::JID_RLEG_6, 11.0, 10);


}
void NaoDynamicKickEngine::kickLeft7()
{	cout<<"Kick6"<<endl;
	CalculateVel(Nao::JID_LARM_1, 0.0, 80);  CalculateVel(Nao::JID_RARM_1, 0.0, 80);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 20);    CalculateVel(Nao::JID_RARM_2, 0.0, 20);
   	CalculateVel(Nao::JID_LARM_3, 0.0, 20);  CalculateVel(Nao::JID_RARM_3, 0.0, 20);
    	CalculateVel(Nao::JID_LARM_4, 0.0, 20);  CalculateVel(Nao::JID_RARM_4, 0.0, 20);
	CalculateVel(Nao::JID_LLEG_1, 1.0, 7);  CalculateVel(Nao::JID_RLEG_1, 1.0, 7);
	CalculateVel(Nao::JID_LLEG_2, 0.0, 10);  CalculateVel(Nao::JID_RLEG_2, 6.0, 10);
	CalculateVel(Nao::JID_LLEG_3, 14.0, 30);  CalculateVel(Nao::JID_RLEG_3, 24.0, 30);
	CalculateVel(Nao::JID_LLEG_4, -60.0, 30);  CalculateVel(Nao::JID_RLEG_4, -64.0, 30);
	CalculateVel(Nao::JID_LLEG_5, 44.0, 30);  CalculateVel(Nao::JID_RLEG_5, 0.0, 30);
	CalculateVel(Nao::JID_LLEG_6, 11.0, 10);  CalculateVel(Nao::JID_RLEG_6, 11.0, 10);


}

void NaoDynamicKickEngine::kickRight1()
{	
	mIsDone = false;
// 	cout<<"Kick1"<<endl;
// 	CalculateVel(Nao::JID_LARM_1, -120.0, 3);  CalculateVel(Nao::JID_RARM_1, -120.0, 3);
//     	CalculateVel(Nao::JID_LARM_2, 20.0, 3);    CalculateVel(Nao::JID_RARM_2, -10.0, 3);
//    	CalculateVel(Nao::JID_LARM_3, -80.0, 3);  CalculateVel(Nao::JID_RARM_3, 80.0, 3);
//     	CalculateVel(Nao::JID_LARM_4, -80.0, 3);  CalculateVel(Nao::JID_RARM_4, 80.0, 3);
// 	CalculateVel(Nao::JID_LLEG_1, 0.0, 3);  CalculateVel(Nao::JID_RLEG_1, 0.0, 3);
//     	CalculateVel(Nao::JID_LLEG_2, 0.0, 3);    CalculateVel(Nao::JID_RLEG_2, 0.0, 3);
//    	CalculateVel(Nao::JID_LLEG_3, 32.0, 3);  CalculateVel(Nao::JID_RLEG_3, 32.0, 3);
//     	CalculateVel(Nao::JID_LLEG_4, -61.0, 3);  CalculateVel(Nao::JID_RLEG_4, -61.0, 3);
// 	CalculateVel(Nao::JID_LLEG_5, 32.0, 3);  CalculateVel(Nao::JID_RLEG_5, 32.0, 3);
//     	CalculateVel(Nao::JID_LLEG_6, 0.0, 3);  CalculateVel(Nao::JID_RLEG_6, 0.0, 3);
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	
	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);

	leftfoot.Pos() = Vector3f(-0.055, 0.0, -0.36);

	rightfoot.Pos() = Vector3f(0.055, 0.0, -0.36);

	NAO->IK_leg(torsor, leftfoot, rightfoot);

	ApplyJVel(true, 0.15);
	ApplyJVel(false, 0.15);
}

void NaoDynamicKickEngine::kickRight2()
{
	cout<<"Kick2"<<endl;
  	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 2);
    	CalculateVel(Nao::JID_LLEG_2, 19.0, 2);    CalculateVel(Nao::JID_RLEG_2, 20.0, 2);
   	CalculateVel(Nao::JID_LLEG_3, 5.0, 6);  CalculateVel(Nao::JID_RLEG_3, 6.0, 4);
    	CalculateVel(Nao::JID_LLEG_4, -9.0, 5);  CalculateVel(Nao::JID_RLEG_4, -10.0, 4);
	CalculateVel(Nao::JID_LLEG_5, 5.0, 5);  CalculateVel(Nao::JID_RLEG_5, 6.0, 4);
    	CalculateVel(Nao::JID_LLEG_6, 20.0, 3);  CalculateVel(Nao::JID_RLEG_6, 19.0, 2);
// 	Matrix torsor(Matrix::GetIdentity());
// 	Matrix leftfoot(Matrix::GetIdentity());
// 	Matrix rightfoot(Matrix::GetIdentity());
// 	
// 	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);
// 	
// 	leftfoot.Pos() = Vector3f(-0.055, 0.0, 0.-0.36);
// 
// 	rightfoot.Pos() = Vector3f(0.055, 0.0, -0.36);
// 
// 	NAO->IK_leg(torsor, leftfoot, rightfoot);
// 
// 	ApplyJVel(true, 0.2);
// 	ApplyJVel(false, 0.2);
}

void NaoDynamicKickEngine::kickRight3()
{
	//!During Cycle 0~19 Do Actions as Following(Moving Center of Mass)
	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 2);
	CalculateVel(Nao::JID_LLEG_2, -19.0, 2);    CalculateVel(Nao::JID_RLEG_2, -10.0, 2);
   	CalculateVel(Nao::JID_LLEG_3, 5.0, 6);  CalculateVel(Nao::JID_RLEG_3, 6.0, 4);
    	CalculateVel(Nao::JID_LLEG_4, -9.0, 5);  CalculateVel(Nao::JID_RLEG_4, -10.0, 4);
	CalculateVel(Nao::JID_LLEG_5, 5.0, 5);  CalculateVel(Nao::JID_RLEG_5, 6.0, 4);
    	CalculateVel(Nao::JID_LLEG_6, 10.0, 3);  CalculateVel(Nao::JID_RLEG_6, 10.0, 2);
	
		
	//!At Cycle 20(the Final Cycle in this Phase) Calculate the Motion Spline for Kick(Cubic Spline Curve)
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	if(CycleInPhase > 19)
	{
		float Force = 1.0;	//!Force: 0.0~1.0
//		float Direction = 0.0;	//!Direction: 0.0~30.0 
		Vector3f BallLocalPostoBody_1 = WM.getBall().GetLocalPos() - WM.getSelf().GetHeadLocalPosition()  ;
		BallLocalPostoBody_1.z() = BallLocalPostoBody_1.x();
		BallLocalPostoBody_1.x() = -BallLocalPostoBody_1.y();
		BallLocalPostoBody_1.y() = BallLocalPostoBody_1.z();
		BallLocalPostoBody_1.z() = 0.00;
		Vector3f BallLocalPostoLeftFoot = WM.getBall().GetLocalPos() - WM.getSelf().GetLeftFootLocalPosition();
		BallLocalPostoLeftFoot.z() = BallLocalPostoLeftFoot.x();
		BallLocalPostoLeftFoot.x() = -BallLocalPostoLeftFoot.y();
		BallLocalPostoLeftFoot.y() = BallLocalPostoLeftFoot.z();
		//BallLocalPostoLeftFoot.z() = 0.042;		//20%
		BallLocalPostoLeftFoot.z() = 0.00;		//5%
		Vector3f BallLocalPos = WM.getBall().GetLocalPos();
		BallLocalPos.z() = BallLocalPos.x();
		BallLocalPos.x() = -BallLocalPos.y();
		BallLocalPos.y() = BallLocalPos.z();

		//!T is the Key Cycles. After 0.8s is the Kick Phase in which the Kick foot moves powerfully.
		float T[6] = 			{0.00, 0.30, 0.60, 0.80, 0.88, 1.0};
		//!For Kick Power Fixing
		T[5] = T[5]/(0.9 + Force * 0.1);
		
		float TorsorX[6] = 		{-0.007, -0.007, -0.003, -0.0032, -0.0031, -0.0065};
// 		TorsorX[0] = BallLocalPostoBody_1.x() - 0.003;
// 		TorsorX[1] = BallLocalPostoBody_1.x() - 0.003;
// 		TorsorX[2] = BallLocalPostoBody.x() - 0.006;
// 		TorsorX[3] = BallLocalPostoBody.x() - 0.0073;
// 		TorsorX[4] = BallLocalPostoBody.x() - 0.0064;
// 		TorsorX[5] = BallLocalPostoBody_1.x() - 0.003;
		float TorsorY[6] = 		{-0.025, -0.025, -0.0222, -0.0216, -0.0275, 0.0};
		TorsorY[0] = BallLocalPostoBody_1.y() - 0.16;
		TorsorY[1] = BallLocalPostoBody_1.y() - 0.16;
// 		TorsorY[2] = BallLocalPostoBody_1.y() - 0.1572;
// 		TorsorY[3] = BallLocalPostoBody.y() - 0.1566;
// 		TorsorY[4] = BallLocalPostoBody.y() - 0.1425;
// 		TorsorY[5] = BallLocalPostoBody.y() - 0.135;
		float TorsorZ[6] = 		{0.366, 0.366, 0.366, 0.366, 0.366, 0.366};
		float TorsorRx[6] = 	{0.00, 0.00, 0.00, 1.2, -1.8, 9.0};
		float TorsorRy[6] = 	{0.00, 0.00, 0.00, -3.1, -4.0, -3.0};
		float TorsorRz[6] = 	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		
		float KickFootX[6] =	{0.11, 0.11, 0.1075, 0.1, 0.08, 0.05};
		//!For Spline Fixing When Direction is not 0.
// 		KickFootX[0] = BallLocalPos.x()+0.035*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[1] = BallLocalPos.x()+0.035*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[2] = BallLocalPos.x()+0.03025*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[3] = BallLocalPos.x()+0.025*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[4] = BallLocalPos.x()+0.015*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[5] = BallLocalPos.x()-0.015*(1-actionRequest.kickRequest.Direction/30.0); 
		
// 		KickFootX[0] = BallLocalPostoLeftFoot.x()+0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[1] = BallLocalPostoLeftFoot.x()+0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[2] = BallLocalPostoLeftFoot.x()+0.04025*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[3] = BallLocalPostoLeftFoot.x()+0.04*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[4] = BallLocalPostoLeftFoot.x()+0.02*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[5] = BallLocalPostoLeftFoot.x()-0.015*(1-actionRequest.kickRequest.Direction/30.0); 
		float KickFootY[6] = 	{0.0072, 0.0072, 0.0079, -0.04, 0.00, 0.2};
// 		KickFootY[0] = BallLocalPostoLeftFoot.y() - 0.14;
// 		KickFootY[1] = BallLocalPostoLeftFoot.y() - 0.14;
// 		KickFootY[2] = BallLocalPostoLeftFoot.y() - 0.15;
// 		KickFootY[3] = BallLocalPostoLeftFoot.y() - 0.18;
// 		KickFootY[4] = BallLocalPostoLeftFoot.y() - 0.135;
// 		KickFootY[5] = BallLocalPostoLeftFoot.y() + 0.06;

		float KickFootZ[6] = 	{0.0082, 0.02, 0.04, 0.07, 0.029, 0.070};
		float KickFootRx[6] =	{0.0, 0.0, -7.0, -12.0, 7.0, 12.0};
		float KickFootRy[6] =	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		float KickFootRz[6] =	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		
		KickFootRz[4] = -actionRequest.kickRequest.Direction/3.0;
		KickFootRz[5] = -actionRequest.kickRequest.Direction;
		
		mSplineTorsorX.SetBreakPoints(T, TorsorX, 6);
		mSplineTorsorY.SetBreakPoints(T, TorsorY, 6);
		mSplineTorsorZ.SetBreakPoints(T, TorsorZ, 6);
		mSplineTorsorRx.SetBreakPoints(T, TorsorRx, 6);
		mSplineTorsorRy.SetBreakPoints(T, TorsorRy, 6);
		mSplineTorsorRz.SetBreakPoints(T, TorsorRz, 6);
		
		if(!mSplineTorsorX.General())
			return;
		if(!mSplineTorsorY.General())
			return;
		if(!mSplineTorsorZ.General())
			return;
		if(!mSplineTorsorRx.General())
			return;
		if(!mSplineTorsorRy.General())
			return;
		if(!mSplineTorsorRz.General())
			return;
		
		mSplineKickFootX.SetBreakPoints(T, KickFootX, 6);
		mSplineKickFootY.SetBreakPoints(T, KickFootY, 6);
		mSplineKickFootZ.SetBreakPoints(T, KickFootZ, 6);
		mSplineKickFootRx.SetBreakPoints(T, KickFootRx, 6);
		mSplineKickFootRy.SetBreakPoints(T, KickFootRy, 6);
		mSplineKickFootRz.SetBreakPoints(T, KickFootRz, 6);
		
		if(!mSplineKickFootX.General())
			return;
		if(!mSplineKickFootY.General())
			return;
		if(!mSplineKickFootZ.General())
			return;
		if(!mSplineKickFootRx.General())
			return;
		if(!mSplineKickFootRy.General())
			return;
		if(!mSplineKickFootRz.General())
			return;
		
// 		for(float t = 0.00; t < 1.0; t += 0.02)
// 		{
// 			oLOG << t << ' ' 	<< mSplineTorsorX.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorY.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorZ.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorRx.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorRy.GetYGivenX(t) << ' '
// 								<< mSplineTorsorRz.GetYGivenX(t) << ' ' 
// 								<< mSplineKickFootX.GetYGivenX(t) << ' '
// 								<< mSplineKickFootY.GetYGivenX(t) << ' '
// 								<< mSplineKickFootZ.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRx.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRy.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRz.GetYGivenX(t) << ' ' << endl;
// 		}
	}
}

void NaoDynamicKickEngine::kickRight4()
{
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	float t = CycleInPhase * 0.02;
	
	//! Motions of Arms
    CalculateVel(Nao::JID_LARM_1, -120.0, 30);  CalculateVel(Nao::JID_RARM_1, -120.0, 30);
    CalculateVel(Nao::JID_LARM_2, 20.0, 30);    CalculateVel(Nao::JID_RARM_2, -10.0, 30);
    CalculateVel(Nao::JID_LARM_3, -80.0, 30);  CalculateVel(Nao::JID_RARM_3, 80.0, 30);
    CalculateVel(Nao::JID_LARM_4, -80.0, 30);  CalculateVel(Nao::JID_RARM_4, 80.0, 30);
	
	//! Get the Matrixs at Current Cycle from the Kick Spline
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	
 	torsor.Pos() = Vector3f(mSplineTorsorX.GetYGivenX(t),
							mSplineTorsorY.GetYGivenX(t),
							mSplineTorsorZ.GetYGivenX(t));
	torsor.RotateX(gDegToRad(mSplineTorsorRx.GetYGivenX(t)));
	torsor.RotateY(gDegToRad(mSplineTorsorRy.GetYGivenX(t)));
	torsor.RotateZ(gDegToRad(mSplineTorsorRz.GetYGivenX(t)));
	
	rightfoot.Pos() = Vector3f(mSplineKickFootX.GetYGivenX(t),
							mSplineKickFootY.GetYGivenX(t),
							mSplineKickFootZ.GetYGivenX(t));
	rightfoot.RotateX(gDegToRad(mSplineKickFootRx.GetYGivenX(t)));
	rightfoot.RotateY(gDegToRad(mSplineKickFootRy.GetYGivenX(t)));
	rightfoot.RotateZ(gDegToRad(mSplineKickFootRz.GetYGivenX(t)));
	
	NAO->IK_leg(torsor, leftfoot, rightfoot);

	if(t < 0.8)
	{
		ApplyJVel(true, 0.1);
		ApplyJVel(false, 0.1);
	}
	else
	{
		ApplyJVel(true, 0.02);
		ApplyJVel(false, 0.04);
	}

}

void NaoDynamicKickEngine::kickRight5()
{	
	Vector3f ball = WM.getBall().GetLocalPos();
	Vector3f destpos = ball + Vector3f(-0.17, 0.0, 0.0);
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	//torsor.Pos() = Vector3f(-0.055, 0, 0.36);
	torsor.Pos() = Vector3f(0.0,0.0, 0);

	rightfoot.Right() = Vector3f(1, 0, 0);
	rightfoot.Up() = Vector3f(0, 1, 0);
	rightfoot.Forward() = Vector3f(0, 0, 1);
	//leftfoot.Pos() = Vector3f(-0.055, 0.025, 0.0);
	rightfoot.Pos() = Vector3f(-0.06, 0.0, -0.34);

// 	rightfoot.RotationZ(-gPI/3);
	//rightfoot.Pos() = Vector3f(0.055, -0.025, 0.035);
	leftfoot.Pos() = Vector3f(destpos.y(), destpos.x(), -0.32);
	oLOG << "left foot:" << endl;
// 	leftfoot.Dump(oLOG);

	/*oLOG << "ZMP: " << WM.getSelf().getZMP() << endl;*/	
	boost::shared_array<Robot::RobotPartInfo> partinfo = NAO->GetRobotPartInfo();
	oLOG << "ball Pos:" << ball << endl;
	oLOG << "dest Pos:" << destpos << endl;
	oLOG << "LLEG6 Pos:\n" << partinfo[Nao::JID_LLEG_6].localPos << '\n' << endl;
	oLOG << "LLEG6 Rot:" << endl;
// 	partinfo[Nao::JID_LLEG_6].localRot.Dump(oLOG);

	NAO->IK_leg(torsor, leftfoot, rightfoot);
	
	ApplyJVel(true, 0.05);
	ApplyJVel(false, 0.05);
	
// 	Matrix torsor(0.998597, -0.00345847, 0.0528305, -0.0207504,
// 0.00259652, 0.999863, 0.0163753, 0.0024535,
// -0.0528799, -0.0162151, 0.998469, 0.331,
// 0, 0, 0, 0);
// 	Matrix leftfoot(0.998316, -0.0273728, 0.0511464, -0.125051,
// 0.0207842, 0.991917, 0.125177, 0.0167485,
// -0.0541594, -0.123903, 0.990815, -0.00254771,
// 0, 0, 0, 0);
// 	Matrix rightfoot(1, 1.04145e-09, 2.70066e-08, -0.0175075,
// 1.18175e-09, 1, 7.32791e-09, 0.024634,
// 1.0293e-08, 8.89331e-10, 1, -0.0355508,
// 0, 0, 0, 0);
}


 void NaoDynamicKickEngine::pass1()
{	
	mIsDone = false;
	cout<<"Kick1"<<endl;
// 	CalculateVel(Nao::JID_LARM_1, -120.0, 3);  CalculateVel(Nao::JID_RARM_1, -120.0, 3);
//     	CalculateVel(Nao::JID_LARM_2, 20.0, 3);    CalculateVel(Nao::JID_RARM_2, -10.0, 3);
//    	CalculateVel(Nao::JID_LARM_3, -80.0, 3);  CalculateVel(Nao::JID_RARM_3, 80.0, 3);
//     	CalculateVel(Nao::JID_LARM_4, -80.0, 3);  CalculateVel(Nao::JID_RARM_4, 80.0, 3);
// 	CalculateVel(Nao::JID_LLEG_1, 0.0, 3);  CalculateVel(Nao::JID_RLEG_1, 0.0, 3);
//     	CalculateVel(Nao::JID_LLEG_2, 0.0, 3);    CalculateVel(Nao::JID_RLEG_2, 0.0, 3);
//    	CalculateVel(Nao::JID_LLEG_3, 32.0, 3);  CalculateVel(Nao::JID_RLEG_3, 32.0, 3);
//     	CalculateVel(Nao::JID_LLEG_4, -61.0, 3);  CalculateVel(Nao::JID_RLEG_4, -61.0, 3);
// 	CalculateVel(Nao::JID_LLEG_5, 32.0, 3);  CalculateVel(Nao::JID_RLEG_5, 32.0, 3);
//     	CalculateVel(Nao::JID_LLEG_6, 0.0, 3);  CalculateVel(Nao::JID_RLEG_6, 0.0, 3);
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	
	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);

	leftfoot.Pos() = Vector3f(-0.055, 0.0, 0.-0.36);

	rightfoot.Pos() = Vector3f(0.055, 0.0, -0.36);

	NAO->IK_leg(torsor, leftfoot, rightfoot);

 	ApplyJVel(true, 0.2);
 	ApplyJVel(false, 0.2);
 }
 
void NaoDynamicKickEngine::pass2()
{
	cout<<"Kick2"<<endl;
  	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 2);
    	CalculateVel(Nao::JID_LLEG_2, 20.0, 2);    CalculateVel(Nao::JID_RLEG_2, 19.0, 2);
   	CalculateVel(Nao::JID_LLEG_3, 6.0, 4);  CalculateVel(Nao::JID_RLEG_3, 5.0, 6);
    	CalculateVel(Nao::JID_LLEG_4, -10.0, 4);  CalculateVel(Nao::JID_RLEG_4, -9.0, 5);
	CalculateVel(Nao::JID_LLEG_5, 6.0, 4);  CalculateVel(Nao::JID_RLEG_5, 5.0, 5);
    	CalculateVel(Nao::JID_LLEG_6, -20.0, 2);  CalculateVel(Nao::JID_RLEG_6, -19.0, 3);
// 	Matrix torsor(Matrix::GetIdentity());
// 	Matrix leftfoot(Matrix::GetIdentity());
// 	Matrix rightfoot(Matrix::GetIdentity());
// 	
// 	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);
// 	
// 	leftfoot.Pos() = Vector3f(-0.055, 0.0, 0.-0.36);
// 
// 	rightfoot.Pos() = Vector3f(0.055, 0.0, -0.36);
// 
// 	NAO->IK_leg(torsor, leftfoot, rightfoot);
// 
// 	ApplyJVel(true, 0.2);
// 	ApplyJVel(false, 0.2);
}

void NaoDynamicKickEngine::pass3()
{
	//!During Cycle 0~19 Do Actions as Following(Moving Center of Mass)
  	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
// 	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 2);
//     	CalculateVel(Nao::JID_LLEG_2, 10.0, 2);    CalculateVel(Nao::JID_RLEG_2, 19.0, 2);
//    	CalculateVel(Nao::JID_LLEG_3, 6.0, 4);  CalculateVel(Nao::JID_RLEG_3, 5.0, 6);
//     	CalculateVel(Nao::JID_LLEG_4, -10.0, 4);  CalculateVel(Nao::JID_RLEG_4, -9.0, 5);
// 	CalculateVel(Nao::JID_LLEG_5, 6.0, 4);  CalculateVel(Nao::JID_RLEG_5, 5.0, 5);
//     	CalculateVel(Nao::JID_LLEG_6, -10.0, 2);  CalculateVel(Nao::JID_RLEG_6, -10.0, 3);
		
	//!At Cycle 20(the Final Cycle in this Phase) Calculate the Motion Spline for Kick(Cubic Spline Curve)
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	if(CycleInPhase > 19)
	{
		
		float Force = 1.0;	//!Force: 0.0~1.0
//		float Direction = 10.0;	//!Direction: 0.0~30.0
		Vector3f BallLocalPostoBody = WM.getBall().GetLocalPos() - WM.getSelf().GetHeadLocalPosition() ;
		BallLocalPostoBody.z() = BallLocalPostoBody.x();
		BallLocalPostoBody.x() = -BallLocalPostoBody.y();
		BallLocalPostoBody.y() = BallLocalPostoBody.z();
		BallLocalPostoBody.z() = 0.00;
		Vector3f BallLocalPostoRightFoot = WM.getBall().GetLocalPos() - WM.getSelf().GetRightFootLocalPosition();
		BallLocalPostoRightFoot.z() = BallLocalPostoRightFoot.x();
		BallLocalPostoRightFoot.x() = -BallLocalPostoRightFoot.y();
		BallLocalPostoRightFoot.y() = BallLocalPostoRightFoot.z();
// 		BallLocalPostoRightFoot.z() = 0.042;
		BallLocalPostoRightFoot.z() = 0;
		Vector3f BallLocalPos = WM.getBall().GetLocalPos();
		BallLocalPos.z() = BallLocalPos.x();
		BallLocalPos.x() = -BallLocalPos.y();
		BallLocalPos.y() = BallLocalPos.z();

		aLOG << "BallLocalPostoRightFoot: " << BallLocalPostoRightFoot << endl;
		//!T is the Key Cycles. After 0.8s is the Kick Phase in which the Kick foot moves powerfully.
		float T[6] = 			{0.00, 0.30, 0.60, 0.80, 0.88, 1.0};
		//!For Kick Power Fixing
		T[5] = T[5]/(0.9 + /*actionRequest.kickRequest.*/Force * 0.1);
		
		float TorsorX[6] = 		{-0.007, -0.007, -0.003, -0.0032, -0.0031, -0.0065};
// 		float TorsorX[6] = 		{0.004, 0.004, 0.000, 0.0002, 0.0001, 0.0035};
// 		TorsorX[0] = BallLocalPostoBody.x() + 0.027;
// 		TorsorX[1] = BallLocalPostoBody.x() + 0.027;
// 		TorsorX[2] = BallLocalPostoBody.x() + 0.03;
// 		TorsorX[0] = BallLocalPos.x() + 0.01;
// 		TorsorX[1] = BallLocalPos.x() + 0.01;
// 		TorsorX[2] = BallLocalPos.x() + 0.006;
// 		TorsorX[3] = BallLocalPostoBody.x() + 0.0253;
// 		TorsorX[4] = BallLocalPostoBody.x() + 0.0244;
// 		TorsorX[5] = BallLocalPostoBody.x() + 0.021;
		float TorsorY[6] = 		{-0.025, -0.025, -0.0222, -0.0216, -0.0275, 0.0};
// 		TorsorY[0] = BallLocalPostoBody.y() - 0.16;
// 		TorsorY[1] = BallLocalPostoBody.y() - 0.16;
// 		TorsorY[2] = BallLocalPostoBody.y() - 0.1572;
// 		TorsorY[3] = BallLocalPostoBody.y() - 0.1566;
// 		TorsorY[4] = BallLocalPostoBody.y() - 0.1425;
// 		TorsorY[5] = BallLocalPostoBody.y() - 0.135;
		float TorsorZ[6] = 		{0.366, 0.366, 0.366, 0.366, 0.366, 0.366};
// 		float TorsorRx[6] = 	{0.00, 0.00, 0.00, 1.2, -1.8, 9.0};
		float TorsorRx[6] = 	{0.00, 0.00, 0.00, 1.2, -1.8, 9.0};
		float TorsorRy[6] = 	{0.00, 0.00, 0.00, 3.1, 4.0, 3.0};
		float TorsorRz[6] = 	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		
		
		float KickFootX[6] =	{-0.11, -0.11, -0.1075, -0.1, -0.08, -0.05};
		//!For Spline Fixing When Direction is not 0.
// 		KickFootX[0] = BallLocalPos.x()-0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[1] = BallLocalPos.x()-0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[2] = BallLocalPos.x()-0.04025*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[3] = BallLocalPos.x()-0.040*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[4] = BallLocalPos.x()-0.01*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[5] = BallLocalPos.x()+0.015*(1-actionRequest.kickRequest.Direction/30.0);
		
// 		KickFootX[0] = BallLocalPostoRightFoot.x()-0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[1] = BallLocalPostoRightFoot.x()-0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[2] = BallLocalPostoRightFoot.x()-0.04025*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[3] = BallLocalPostoRightFoot.x()-0.04*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[4] = BallLocalPostoRightFoot.x()-0.01*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[5] = BallLocalPostoRightFoot.x()+0.015*(1-actionRequest.kickRequest.Direction/30.0);
		float KickFootY[6] = 	{0.0072, 0.0072, 0.0079, -0.04, 0.00, 0.2};
// 		KickFootY[0] = BallLocalPostoRightFoot.y() - 0.14;
// 		KickFootY[1] = BallLocalPostoRightFoot.y() - 0.14;
// 		KickFootY[2] = BallLocalPostoRightFoot.y() - 0.15;
// 		KickFootY[3] = BallLocalPostoRightFoot.y() - 0.18;
// 		KickFootY[4] = BallLocalPostoRightFoot.y() - 0.135;
// 		KickFootY[5] = BallLocalPostoRightFoot.y() + 0.06;

// 		float KickFootZ[6] = 	{0.0082, 0.02, 0.04, 0.07, 0.029, 0.070};
		float KickFootZ[6] = 	{0.0082, 0.02, 0.04, 0.07, 0.029, 0.070};
		float KickFootRx[6] =	{0.0, 0.0, -7.0, -12.0, 7.0, 12.0};
		float KickFootRy[6] =	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		float KickFootRz[6] =	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		
		KickFootRz[4] = actionRequest.kickRequest.Direction/3.0;
		KickFootRz[5] = actionRequest.kickRequest.Direction;
		
		mSplineTorsorX.SetBreakPoints(T, TorsorX, 6);
		mSplineTorsorY.SetBreakPoints(T, TorsorY, 6);
		mSplineTorsorZ.SetBreakPoints(T, TorsorZ, 6);
		mSplineTorsorRx.SetBreakPoints(T, TorsorRx, 6);
		mSplineTorsorRy.SetBreakPoints(T, TorsorRy, 6);
		mSplineTorsorRz.SetBreakPoints(T, TorsorRz, 6);
		
		if(!mSplineTorsorX.General())
			return;
		if(!mSplineTorsorY.General())
			return;
		if(!mSplineTorsorZ.General())
			return;
		if(!mSplineTorsorRx.General())
			return;
		if(!mSplineTorsorRy.General())
			return;
		if(!mSplineTorsorRz.General())
			return;
		
		mSplineKickFootX.SetBreakPoints(T, KickFootX, 6);
		mSplineKickFootY.SetBreakPoints(T, KickFootY, 6);
		mSplineKickFootZ.SetBreakPoints(T, KickFootZ, 6);
		mSplineKickFootRx.SetBreakPoints(T, KickFootRx, 6);
		mSplineKickFootRy.SetBreakPoints(T, KickFootRy, 6);
		mSplineKickFootRz.SetBreakPoints(T, KickFootRz, 6);
		
		if(!mSplineKickFootX.General())
			return;
		if(!mSplineKickFootY.General())
			return;
		if(!mSplineKickFootZ.General())
			return;
		if(!mSplineKickFootRx.General())
			return;
		if(!mSplineKickFootRy.General())
			return;
		if(!mSplineKickFootRz.General())
			return;
		
// 		for(float t = 0.00; t < 1.0; t += 0.02)
// 		{
// 			oLOG << t << ' ' 	<< mSplineTorsorX.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorY.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorZ.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorRx.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorRy.GetYGivenX(t) << ' '
// 								<< mSplineTorsorRz.GetYGivenX(t) << ' ' 
// 								<< mSplineKickFootX.GetYGivenX(t) << ' '
// 								<< mSplineKickFootY.GetYGivenX(t) << ' '
// 								<< mSplineKickFootZ.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRx.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRy.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRz.GetYGivenX(t) << ' ' << endl;
// 		}
	}
}

void NaoDynamicKickEngine::pass4()
{	
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	float t = CycleInPhase * 0.02;
	
	//! Motions of Arms
    CalculateVel(Nao::JID_LARM_1, -120.0, 30);  CalculateVel(Nao::JID_RARM_1, -120.0, 30);
    CalculateVel(Nao::JID_LARM_2, 20.0, 30);    CalculateVel(Nao::JID_RARM_2, -10.0, 30);
    CalculateVel(Nao::JID_LARM_3, -80.0, 30);  CalculateVel(Nao::JID_RARM_3, 80.0, 30);
    CalculateVel(Nao::JID_LARM_4, -80.0, 30);  CalculateVel(Nao::JID_RARM_4, 80.0, 30);
	
	//! Get the Matrixs at Current Cycle from the Kick Spline
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	
	torsor.Pos() = Vector3f(mSplineTorsorX.GetYGivenX(t),
							mSplineTorsorY.GetYGivenX(t),
							mSplineTorsorZ.GetYGivenX(t));
	torsor.RotateX(gDegToRad(mSplineTorsorRx.GetYGivenX(t)));
	torsor.RotateY(gDegToRad(mSplineTorsorRy.GetYGivenX(t)));
	torsor.RotateZ(gDegToRad(mSplineTorsorRz.GetYGivenX(t)));
	
	leftfoot.Pos() = Vector3f(mSplineKickFootX.GetYGivenX(t),
							mSplineKickFootY.GetYGivenX(t),
							mSplineKickFootZ.GetYGivenX(t));
	leftfoot.RotateX(gDegToRad(mSplineKickFootRx.GetYGivenX(t)));
	leftfoot.RotateY(gDegToRad(mSplineKickFootRy.GetYGivenX(t)));
	leftfoot.RotateZ(gDegToRad(mSplineKickFootRz.GetYGivenX(t)));
	
	NAO->IK_leg(torsor, leftfoot, rightfoot);

	if(t < 0.8)
	{
		ApplyJVel(true, 0.1);
		ApplyJVel(false, 0.1);
	}
	else
	{
		ApplyJVel(true, 0.04);
		ApplyJVel(false, 0.02);
	}

// 	oLOG << WM.getFieldInfo().match_time << ' ';
// 	for(JointID j = Nao::JID_HEAD_1; j <= Nao::JID_RLEG_6; j++)
// 	{
// 		oLOG << mJointAngle[j] << ' ';
// 	}
// 	oLOG << endl;

}
// void NaoDynamicKickEngine::kickLeft2()
// {	
// 	Matrix torsor(Matrix::GetIdentity());
// 	Matrix leftfoot(Matrix::GetIdentity());
// 	Matrix rightfoot(Matrix::GetIdentity());
// 	torsor.Right() = Vector3f(1, 0, 0);
// 	torsor.Up() = Vector3f(0, 1, 0);
// 	torsor.Forward() = Vector3f(0, 0, 1);
// 	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);
// 
// 	leftfoot.Right() = Vector3f(1, 0, 0);
// 	leftfoot.Up() = Vector3f(0, 1, 0);
// 	leftfoot.Forward() = Vector3f(0, 0, 1);
// 	leftfoot.Pos() = Vector3f(0.055, 0.0, 0.-0.34);
// 
// 	rightfoot.Right() = Vector3f(1, 0, 0);
// 	rightfoot.Up() = Vector3f(0, 1, 0);
// 	rightfoot.Forward() = Vector3f(0, 0, 1);
// 	rightfoot.Pos() = Vector3f(0.065, 0.0, -0.3);
// 
// 	NAO->IK_leg(torsor, leftfoot, rightfoot);
// 
// 	ApplyJVel(true, 0.5);
// 	ApplyJVel(false, 0.2);
// 
// }

void NaoDynamicKickEngine::pass5()
{	
	Matrix torsor(0.998597, -0.00345847, 0.0528305, -0.0207504,
0.00259652, 0.999863, 0.0163753, 0.0024535,
-0.0528799, -0.0162151, 0.998469, 0.331,
0, 0, 0, 0);
	Matrix leftfoot(0.998316, -0.0273728, 0.0511464, -0.125051,
0.0207842, 0.991917, 0.125177, 0.0167485,
-0.0541594, -0.123903, 0.990815, -0.00254771,
0, 0, 0, 0);
	Matrix rightfoot(1, 1.04145e-09, 2.70066e-08, -0.0175075,
1.18175e-09, 1, 7.32791e-09, 0.024634,
1.0293e-08, 8.89331e-10, 1, -0.0355508,
0, 0, 0, 0);

// 	torsor.Right() = Vector3f(1, 0, 0);
// 	torsor.Up() = Vector3f(0, 1, 0);
// 	torsor.Forward() = Vector3f(0, 0, 1);
// 	torsor.Pos() = Vector3f(-0.045, 0.02, 0.37);
// 
// 	leftfoot.Right() = Vector3f(1, 0, 0);
// 	leftfoot.Up() = Vector3f(0, 1, 0);
// 	leftfoot.Forward() = Vector3f(0, 0, 1);
// 	leftfoot.Pos() = Vector3f(-0.055, 0.025, 0.015);
// 
// 	rightfoot.RotationX(0.3);
// // 	rightfoot.Right() = Vector3f(1, 0, 0);
// // 	rightfoot.Up() = Vector3f(0, 1, 0);
// // 	rightfoot.Forward() = Vector3f(0, 0, 1);
// 	rightfoot.Pos() = Vector3f(0.01, 0.2, 0.09);

	NAO->IK_leg(torsor, leftfoot, rightfoot);

	ApplyJVel(true, 0.05);
	ApplyJVel(false, 0.05);
}

 void NaoDynamicKickEngine::passq1()
{	
	mIsDone = false;
	cout<<"Kick1"<<endl;
// 	CalculateVel(Nao::JID_LARM_1, -120.0, 3);  CalculateVel(Nao::JID_RARM_1, -120.0, 3);
//     	CalculateVel(Nao::JID_LARM_2, 20.0, 3);    CalculateVel(Nao::JID_RARM_2, -10.0, 3);
//    	CalculateVel(Nao::JID_LARM_3, -80.0, 3);  CalculateVel(Nao::JID_RARM_3, 80.0, 3);
//     	CalculateVel(Nao::JID_LARM_4, -80.0, 3);  CalculateVel(Nao::JID_RARM_4, 80.0, 3);
// 	CalculateVel(Nao::JID_LLEG_1, 0.0, 3);  CalculateVel(Nao::JID_RLEG_1, 0.0, 3);
//     	CalculateVel(Nao::JID_LLEG_2, 0.0, 3);    CalculateVel(Nao::JID_RLEG_2, 0.0, 3);
//    	CalculateVel(Nao::JID_LLEG_3, 32.0, 3);  CalculateVel(Nao::JID_RLEG_3, 32.0, 3);
//     	CalculateVel(Nao::JID_LLEG_4, -61.0, 3);  CalculateVel(Nao::JID_RLEG_4, -61.0, 3);
// 	CalculateVel(Nao::JID_LLEG_5, 32.0, 3);  CalculateVel(Nao::JID_RLEG_5, 32.0, 3);
//     	CalculateVel(Nao::JID_LLEG_6, 0.0, 3);  CalculateVel(Nao::JID_RLEG_6, 0.0, 3);
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	
	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);

	leftfoot.Pos() = Vector3f(-0.055, 0.0, 0.-0.36);

	rightfoot.Pos() = Vector3f(0.055, 0.0, -0.36);

	NAO->IK_leg(torsor, leftfoot, rightfoot);

 	ApplyJVel(true, 0.1);
 	ApplyJVel(false, 0.1);
 }
 
void NaoDynamicKickEngine::passq2()
{
	cout<<"Kick2"<<endl;
  	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 2);
    	CalculateVel(Nao::JID_LLEG_2, 20.0, 2);    CalculateVel(Nao::JID_RLEG_2, 19.0, 2);
   	CalculateVel(Nao::JID_LLEG_3, 6.0, 4);  CalculateVel(Nao::JID_RLEG_3, 5.0, 6);
    	CalculateVel(Nao::JID_LLEG_4, -10.0, 4);  CalculateVel(Nao::JID_RLEG_4, -9.0, 5);
	CalculateVel(Nao::JID_LLEG_5, 6.0, 4);  CalculateVel(Nao::JID_RLEG_5, 5.0, 5);
    	CalculateVel(Nao::JID_LLEG_6, -20.0, 2);  CalculateVel(Nao::JID_RLEG_6, -19.0, 3);
// 	Matrix torsor(Matrix::GetIdentity());
// 	Matrix leftfoot(Matrix::GetIdentity());
// 	Matrix rightfoot(Matrix::GetIdentity());
// 	
// 	torsor.Pos() = Vector3f(0.0, 0.0, 0.0);
// 	
// 	leftfoot.Pos() = Vector3f(-0.055, 0.0, 0.-0.36);
// 
// 	rightfoot.Pos() = Vector3f(0.055, 0.0, -0.36);
// 
// 	NAO->IK_leg(torsor, leftfoot, rightfoot);
// 
// 	ApplyJVel(true, 0.2);
// 	ApplyJVel(false, 0.2);
}

void NaoDynamicKickEngine::passq3()
{
	//!During Cycle 0~19 Do Actions as Following(Moving Center of Mass)
  	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
// 	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 2);
//     	CalculateVel(Nao::JID_LLEG_2, 10.0, 2);    CalculateVel(Nao::JID_RLEG_2, 19.0, 2);
//    	CalculateVel(Nao::JID_LLEG_3, 6.0, 4);  CalculateVel(Nao::JID_RLEG_3, 5.0, 6);
//     	CalculateVel(Nao::JID_LLEG_4, -10.0, 4);  CalculateVel(Nao::JID_RLEG_4, -9.0, 5);
// 	CalculateVel(Nao::JID_LLEG_5, 6.0, 4);  CalculateVel(Nao::JID_RLEG_5, 5.0, 5);
//     	CalculateVel(Nao::JID_LLEG_6, -10.0, 2);  CalculateVel(Nao::JID_RLEG_6, -10.0, 3);
		
	//!At Cycle 20(the Final Cycle in this Phase) Calculate the Motion Spline for Kick(Cubic Spline Curve)
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	if(CycleInPhase > 19)
	{
		
		float Force = 1.0;	//!Force: 0.0~1.0
//		float Direction = 10.0;	//!Direction: 0.0~30.0
		Vector3f BallLocalPostoBody = WM.getBall().GetLocalPos() - WM.getSelf().GetHeadLocalPosition() ;
		BallLocalPostoBody.z() = BallLocalPostoBody.x();
		BallLocalPostoBody.x() = -BallLocalPostoBody.y();
		BallLocalPostoBody.y() = BallLocalPostoBody.z();
		BallLocalPostoBody.z() = 0.00;
		Vector3f BallLocalPostoRightFoot = WM.getBall().GetLocalPos() - WM.getSelf().GetRightFootLocalPosition();
		BallLocalPostoRightFoot.z() = BallLocalPostoRightFoot.x();
		BallLocalPostoRightFoot.x() = -BallLocalPostoRightFoot.y();
		BallLocalPostoRightFoot.y() = BallLocalPostoRightFoot.z();
// 		BallLocalPostoRightFoot.z() = 0.042;
		BallLocalPostoRightFoot.z() = 0;
		Vector3f BallLocalPos = WM.getBall().GetLocalPos();
		BallLocalPos.z() = BallLocalPos.x();
		BallLocalPos.x() = -BallLocalPos.y();
		BallLocalPos.y() = BallLocalPos.z();

		aLOG << "BallLocalPostoRightFoot: " << BallLocalPostoRightFoot << endl;
		//!T is the Key Cycles. After 0.8s is the Kick Phase in which the Kick foot moves powerfully.
		float T[6] = 			{0.00, 0.30, 0.60, 0.80, 0.88, 1.0};
		//!For Kick Power Fixing
		T[5] = T[5]/(0.9 + /*actionRequest.kickRequest.*/Force * 0.1);
		
		float TorsorX[6] = 		{-0.007, -0.007, -0.003, -0.0032, -0.0031, -0.0065};
// 		float TorsorX[6] = 		{0.000, 0.000, 0.000, 0.000, 0.000, 0.00};
// 		TorsorX[0] = BallLocalPostoBody.x() + 0.027;
// 		TorsorX[1] = BallLocalPostoBody.x() + 0.027;
// 		TorsorX[2] = BallLocalPostoBody.x() + 0.03;
// 		TorsorX[0] = BallLocalPos.x() + 0.01;
// 		TorsorX[1] = BallLocalPos.x() + 0.01;
// 		TorsorX[2] = BallLocalPos.x() + 0.006;
// 		TorsorX[3] = BallLocalPostoBody.x() + 0.0253;
// 		TorsorX[4] = BallLocalPostoBody.x() + 0.0244;
// 		TorsorX[5] = BallLocalPostoBody.x() + 0.021;
		float TorsorY[6] = 		{-0.025, -0.025, -0.0222, -0.0216, -0.0275, 0.0};
// 		TorsorY[0] = BallLocalPostoBody.y() - 0.16;
// 		TorsorY[1] = BallLocalPostoBody.y() - 0.16;
// 		TorsorY[2] = BallLocalPostoBody.y() - 0.1572;
// 		TorsorY[3] = BallLocalPostoBody.y() - 0.1566;
// 		TorsorY[4] = BallLocalPostoBody.y() - 0.1425;
// 		TorsorY[5] = BallLocalPostoBody.y() - 0.135;
		float TorsorZ[6] = 		{0.366, 0.366, 0.366, 0.366, 0.366, 0.366};
		float TorsorRx[6] = 	{0.00, 0.00, 0.00, 1.2, -1.8, 9.0};
		float TorsorRy[6] = 	{0.00, 0.00, 0.00, 3.1, 4.0, 3.0};
		float TorsorRz[6] = 	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		
		
		float KickFootX[6] =	{-0.11, -0.11, -0.1075, -0.1, -0.08, -0.05};
		//!For Spline Fixing When Direction is not 0.
// 		KickFootX[0] = BallLocalPos.x()-0.035*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[1] = BallLocalPos.x()-0.035*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[2] = BallLocalPos.x()-0.03025*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[3] = BallLocalPos.x()-0.02*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[4] = BallLocalPos.x()-0.015*(1-actionRequest.kickRequest.Direction/30.0);
		KickFootX[5] = BallLocalPos.x()+0.015*(1-actionRequest.kickRequest.Direction/30.0);
		
// 		KickFootX[0] = BallLocalPostoRightFoot.x()-0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[1] = BallLocalPostoRightFoot.x()-0.05*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[2] = BallLocalPostoRightFoot.x()-0.04025*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[3] = BallLocalPostoRightFoot.x()-0.04*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[4] = BallLocalPostoRightFoot.x()-0.01*(1-actionRequest.kickRequest.Direction/30.0);
// 		KickFootX[5] = BallLocalPostoRightFoot.x()+0.015*(1-actionRequest.kickRequest.Direction/30.0);
		float KickFootY[6] = 	{0.0072, 0.0072, 0.0079, -0.04, 0.00, 0.2};
		KickFootY[0] = BallLocalPostoRightFoot.y() - 0.14;
		KickFootY[1] = BallLocalPostoRightFoot.y() - 0.14;
		KickFootY[2] = BallLocalPostoRightFoot.y() - 0.15;
		KickFootY[3] = BallLocalPostoRightFoot.y() - 0.18;
		KickFootY[4] = BallLocalPostoRightFoot.y() - 0.135;
		KickFootY[5] = BallLocalPostoRightFoot.y() + 0.06;

		float KickFootZ[6] = 	{0.0082, 0.02, 0.04, 0.07, 0.029, 0.070};
		float KickFootRx[6] =	{0.0, 0.0, -7.0, -12.0, 7.0, 12.0};
		float KickFootRy[6] =	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		float KickFootRz[6] =	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		
		KickFootRz[4] = actionRequest.kickRequest.Direction/3.0;
		KickFootRz[5] = actionRequest.kickRequest.Direction;
		
		mSplineTorsorX.SetBreakPoints(T, TorsorX, 6);
		mSplineTorsorY.SetBreakPoints(T, TorsorY, 6);
		mSplineTorsorZ.SetBreakPoints(T, TorsorZ, 6);
		mSplineTorsorRx.SetBreakPoints(T, TorsorRx, 6);
		mSplineTorsorRy.SetBreakPoints(T, TorsorRy, 6);
		mSplineTorsorRz.SetBreakPoints(T, TorsorRz, 6);
		
		if(!mSplineTorsorX.General())
			return;
		if(!mSplineTorsorY.General())
			return;
		if(!mSplineTorsorZ.General())
			return;
		if(!mSplineTorsorRx.General())
			return;
		if(!mSplineTorsorRy.General())
			return;
		if(!mSplineTorsorRz.General())
			return;
		
		mSplineKickFootX.SetBreakPoints(T, KickFootX, 6);
		mSplineKickFootY.SetBreakPoints(T, KickFootY, 6);
		mSplineKickFootZ.SetBreakPoints(T, KickFootZ, 6);
		mSplineKickFootRx.SetBreakPoints(T, KickFootRx, 6);
		mSplineKickFootRy.SetBreakPoints(T, KickFootRy, 6);
		mSplineKickFootRz.SetBreakPoints(T, KickFootRz, 6);
		
		if(!mSplineKickFootX.General())
			return;
		if(!mSplineKickFootY.General())
			return;
		if(!mSplineKickFootZ.General())
			return;
		if(!mSplineKickFootRx.General())
			return;
		if(!mSplineKickFootRy.General())
			return;
		if(!mSplineKickFootRz.General())
			return;
		
// 		for(float t = 0.00; t < 1.0; t += 0.02)
// 		{
// 			oLOG << t << ' ' 	<< mSplineTorsorX.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorY.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorZ.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorRx.GetYGivenX(t) << ' ' 
// 								<< mSplineTorsorRy.GetYGivenX(t) << ' '
// 								<< mSplineTorsorRz.GetYGivenX(t) << ' ' 
// 								<< mSplineKickFootX.GetYGivenX(t) << ' '
// 								<< mSplineKickFootY.GetYGivenX(t) << ' '
// 								<< mSplineKickFootZ.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRx.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRy.GetYGivenX(t) << ' '
// 								<< mSplineKickFootRz.GetYGivenX(t) << ' ' << endl;
// 		}
	}
}

void NaoDynamicKickEngine::passq4()
{	
	int CycleInPhase = WM.getSelf().CurrentCycle - mPhaseStartCycle;
	float t = CycleInPhase * 0.02;
	
	//! Motions of Arms
    CalculateVel(Nao::JID_LARM_1, -120.0, 30);  CalculateVel(Nao::JID_RARM_1, -120.0, 30);
    CalculateVel(Nao::JID_LARM_2, 20.0, 30);    CalculateVel(Nao::JID_RARM_2, -10.0, 30);
    CalculateVel(Nao::JID_LARM_3, -80.0, 30);  CalculateVel(Nao::JID_RARM_3, 80.0, 30);
    CalculateVel(Nao::JID_LARM_4, -80.0, 30);  CalculateVel(Nao::JID_RARM_4, 80.0, 30);
	
	//! Get the Matrixs at Current Cycle from the Kick Spline
	Matrix torsor(Matrix::GetIdentity());
	Matrix leftfoot(Matrix::GetIdentity());
	Matrix rightfoot(Matrix::GetIdentity());
	
	torsor.Pos() = Vector3f(mSplineTorsorX.GetYGivenX(t),
							mSplineTorsorY.GetYGivenX(t),
							mSplineTorsorZ.GetYGivenX(t));
	torsor.RotateX(gDegToRad(mSplineTorsorRx.GetYGivenX(t)));
	torsor.RotateY(gDegToRad(mSplineTorsorRy.GetYGivenX(t)));
	torsor.RotateZ(gDegToRad(mSplineTorsorRz.GetYGivenX(t)));
	
	leftfoot.Pos() = Vector3f(mSplineKickFootX.GetYGivenX(t),
							mSplineKickFootY.GetYGivenX(t),
							mSplineKickFootZ.GetYGivenX(t));
	leftfoot.RotateX(gDegToRad(mSplineKickFootRx.GetYGivenX(t)));
	leftfoot.RotateY(gDegToRad(mSplineKickFootRy.GetYGivenX(t)));
	leftfoot.RotateZ(gDegToRad(mSplineKickFootRz.GetYGivenX(t)));
	
	NAO->IK_leg(torsor, leftfoot, rightfoot);

	if(t < 0.8)
	{
		ApplyJVel(true, 0.1);
		ApplyJVel(false, 0.1);
	}
	else
	{
		ApplyJVel(true, 0.04);
		ApplyJVel(false, 0.02);
	}

// 	oLOG << WM.getFieldInfo().match_time << ' ';
// 	for(JointID j = Nao::JID_HEAD_1; j <= Nao::JID_RLEG_6; j++)
// 	{
// 		oLOG << mJointAngle[j] << ' ';
// 	}
// 	oLOG << endl;

}

void NaoDynamicKickEngine::passq5()
{	
	Matrix torsor(0.998597, -0.00345847, 0.0528305, -0.0207504,
0.00259652, 0.999863, 0.0163753, 0.0024535,
-0.0528799, -0.0162151, 0.998469, 0.331,
0, 0, 0, 0);
	Matrix leftfoot(0.998316, -0.0273728, 0.0511464, -0.125051,
0.0207842, 0.991917, 0.125177, 0.0167485,
-0.0541594, -0.123903, 0.990815, -0.00254771,
0, 0, 0, 0);
	Matrix rightfoot(1, 1.04145e-09, 2.70066e-08, -0.0175075,
1.18175e-09, 1, 7.32791e-09, 0.024634,
1.0293e-08, 8.89331e-10, 1, -0.0355508,
0, 0, 0, 0);

// 	torsor.Right() = Vector3f(1, 0, 0);
// 	torsor.Up() = Vector3f(0, 1, 0);
// 	torsor.Forward() = Vector3f(0, 0, 1);
// 	torsor.Pos() = Vector3f(-0.045, 0.02, 0.37);
// 
// 	leftfoot.Right() = Vector3f(1, 0, 0);
// 	leftfoot.Up() = Vector3f(0, 1, 0);
// 	leftfoot.Forward() = Vector3f(0, 0, 1);
// 	leftfoot.Pos() = Vector3f(-0.055, 0.025, 0.015);
// 
// 	rightfoot.RotationX(0.3);
// // 	rightfoot.Right() = Vector3f(1, 0, 0);
// // 	rightfoot.Up() = Vector3f(0, 1, 0);
// // 	rightfoot.Forward() = Vector3f(0, 0, 1);
// 	rightfoot.Pos() = Vector3f(0.01, 0.2, 0.09);

	NAO->IK_leg(torsor, leftfoot, rightfoot);

	ApplyJVel(true, 0.05);
	ApplyJVel(false, 0.05);
}
