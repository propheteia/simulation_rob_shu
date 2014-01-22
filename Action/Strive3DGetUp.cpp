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

#include "Strive3DGetUp.h"
#include "self.h"
#include "nao.h"

Strive3DGetupEngine::Strive3DGetupEngine(const GetupEngineInterfaces& interfaces ):GetupEngine(interfaces),mInit(true),state(disable)
{	
}

void Strive3DGetupEngine::Init()
{
     	mPhase              = GETUP1;
	mPhase2             = GETUPFB1;
    	mPhaseCycle         = 0;
    	mPhaseStartCycle    = WM.getSelf().CurrentCycle;
	if(WM.getSelf().onMyback())
		state = GETUPFROMBACK;
	else if(WM.getSelf().onMyBelly())
		state = GETUP;
	else
	{	state = ONMYSIDE;
		aLOG << "On my Side ,Waitting!!!" << endl;
	}
}

void Strive3DGetupEngine::CalculateVel(JointID id, float angle, float maxVel)
{
//'	WM.getSelf().mCoMIsStable();  //by Neil Zhu
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

	vel = gAbs(minus) > maxVel ? maxVel * gSign(minus) : 1.9*minus/**sqrt(maxVel)*/;
	vel = std::min(gDegToRad(vel) * 10.0f, 100.0f);
	//aLOG << "minus: " << minus << "       vel: " << vel << endl;
	mJointVel[id] = vel;
}

bool Strive3DGetupEngine::executeParameterized()
{	if(mInit)
	{	Init();
		mInit = false;
	}
	
	if(state == GETUP)
	{
		gettingUp();
		aLOG << " GetUp Phase" << mPhase <<endl;
		switch (mPhase)
    		{
        		case GETUP1:
				getUp1();
			break;
			case GETUP2:
				getUp2();
			break;
			case GETUP3:
				getUp3();
			break;
			case GETUP4:
				getUp4();
			break;
			case GETUP5:
				getUp5();
			break;
			case GETUP6:
 				getUp6();
		}
	}
	else if (state == GETUPFROMBACK)
	{
		aLOG << " GetUpFromBack Phase" << mPhase2 <<endl;
		gettingUpFromBack();
		switch (mPhase2)
    		{
        	case GETUPFB1:
			getUpFB1();
		break;
		case GETUPFB2:
			getUpFB2();
		break;
		case GETUPFB3:
			getUpFB3();
		break;
		case GETUPFB4:
			getUpFB4();
		break;
		case GETUPFB5:
			getUpFB5();
		break;
		}
	}
	else if( state == ONMYSIDE)
	{	aLOG << "On my Side!!!!" << endl;
		adjustment();
		mInit = true;
	}
 	else 
		aLOG << "Unknowed getup state!" << endl;
	
	if (mIsDone)
	{	mInit = true;
		return false;
	}
	return true;
}

void Strive3DGetupEngine::gettingUp()
{	aLOG << "GettingUp: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
	if( mPhase == GETUP1 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LARM_1])+120.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_1])+0.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_2])+0.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_3])+0.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+0.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LARM_3])+90.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LARM_2])+0.0)<0.1))
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = GETUP2;
	}
	if( mPhase == GETUP2 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LARM_1])+90.0))<0.1 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LARM_4])+90.0))<0.1)
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = GETUP3;
	}
	if( mPhase == GETUP3 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LARM_1])+20.0))<0.1&& gAbs((gRadToDeg(mJointAngle[Nao::JID_LARM_4])+90.0))<0.1 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_1])+90.0))<0.1 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_2])+22.0))<0.1 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+130.0))<0.1 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-100.0))<0.1 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_5])-12.0))<0.1 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_6])+3.0))<0.1)
	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = GETUP4;
	}
	if( mPhase == GETUP4 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-80.0))<0.1 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_1])+80.0))<0.1 && gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_2])+20.0))<0.1)
	{	
// 		if(WM.getSelf().mCoMIsStable())
// 		{
			mPhaseStartCycle = WM.getSelf().CurrentCycle;
			mPhase = GETUP5;
// 		}
// 		else
// 		{
// 			mPhaseStartCycle = WM.getSelf().CurrentCycle;
// 			mPhase = GETUP1;
// 			aLOG<<"regetup!!"<<endl;
// 		}	
	}
	if( mPhase == GETUP5 && gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_5])-40.0)<0.1 && gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+120.0)<0.1 && gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-80.0)<0.1 && gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_2])+0.0)<0.1 && gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_1])+39.0)<0.1)
	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = GETUP6;
	}
 	if( mPhase == GETUP6 && gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-37.0)<0.1 && gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+64.0)<0.1)
 	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
 		mIsDone = true;
 	}

}

void Strive3DGetupEngine::gettingUpFromBack()
{	aLOG << "GettingUpFromBack: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
	if( mPhase2 == GETUPFB1 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LARM_1])+90.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_1])+0.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_2])+0.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_3])+0.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+0.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_5])+0.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_6])+0.0)<0.1))
	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase2 = GETUPFB2;
	}
	if( mPhase2 == GETUPFB2 && (gRadToDeg(mJointAngle[Nao::JID_LLEG_3])>93.0) &&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LARM_1])+10.0)<0.1))
	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase2 = GETUPFB3;
	}
	if( mPhase2 == GETUPFB3 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_1])+90.0)<0.1) && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_2])+10.0)<0.1) && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-100.0)<0.1) && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+120.0)<0.1) && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_5])+12.0)<0.1) && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_6])-22.0)<0.1) )
	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase2 = GETUPFB4;
	}
	if( mPhase2 == GETUPFB4 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LARM_1])+70.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_1])+30.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-27.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+74.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_5])-35.0)<0.1))
	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase2 = GETUPFB5;
	}
	if( mPhase2 == GETUPFB5 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-37.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+64.0)<0.1)&&(gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_5])-36.0)<0.1))
	{	
	
// 		if(!WM.getSelf().mCoMIsStable()) 
// 		{
// 			mPhaseStartCycle = WM.getSelf().CurrentCycle;
// 			state = GETUP;
// 			mPhase = GETUP2;
// 			executeParameterized();
// 		}
// 		else	
// 		{
			mPhaseStartCycle = WM.getSelf().CurrentCycle;
			mIsDone = true;
// 		}
	}	
	
//####################################### CoM apply ##############################################################
}

void Strive3DGetupEngine::getUp1()
{	mIsDone = false;
	CalculateVel(Nao::JID_LARM_1, -120.0, 90);  CalculateVel(Nao::JID_RARM_1, -120.0, 90);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 30);    CalculateVel(Nao::JID_RARM_2, 0.0, 30);
   	CalculateVel(Nao::JID_LARM_3, -90.0, 30);  CalculateVel(Nao::JID_RARM_3, 90.0, 30);
    	CalculateVel(Nao::JID_LARM_4, -90.0, 90);  CalculateVel(Nao::JID_RARM_4, 90.0, 90);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 30);  CalculateVel(Nao::JID_RLEG_1, 0.0, 30);
    	CalculateVel(Nao::JID_LLEG_2, 0.0, 30);    CalculateVel(Nao::JID_RLEG_2, 0.0, 30);
   	CalculateVel(Nao::JID_LLEG_3, 0.0, 30);  CalculateVel(Nao::JID_RLEG_3, 0.0, 30);
    	CalculateVel(Nao::JID_LLEG_4, 0.0, 30);  CalculateVel(Nao::JID_RLEG_4, 0.0, 30);
	CalculateVel(Nao::JID_LLEG_5, 0.0, 30);  CalculateVel(Nao::JID_RLEG_5, 0.0, 30);
    	CalculateVel(Nao::JID_LLEG_6, 0.0, 30);  CalculateVel(Nao::JID_RLEG_6, 0.0, 30);
}
void Strive3DGetupEngine::getUp2()
{	
	CalculateVel(Nao::JID_LARM_1, -90.0, 90);  CalculateVel(Nao::JID_RARM_1, -90.0, 90);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 30);    CalculateVel(Nao::JID_RARM_2, 0.0, 30);
   	CalculateVel(Nao::JID_LARM_3, -90.0, 30);  CalculateVel(Nao::JID_RARM_3, 90.0, 30);
    	CalculateVel(Nao::JID_LARM_4, -90.0, 90);  CalculateVel(Nao::JID_RARM_4, 90.0, 90);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 30);  CalculateVel(Nao::JID_RLEG_1, 0.0, 30);
    	CalculateVel(Nao::JID_LLEG_2, 0.0, 30);    CalculateVel(Nao::JID_RLEG_2, 0.0, 30);
   	CalculateVel(Nao::JID_LLEG_3, 0.0, 30);  CalculateVel(Nao::JID_RLEG_3, 0.0, 30);
    	CalculateVel(Nao::JID_LLEG_4, 0.0, 30);  CalculateVel(Nao::JID_RLEG_4, 0.0, 30);
	CalculateVel(Nao::JID_LLEG_5, 0.0, 30);  CalculateVel(Nao::JID_RLEG_5, 0.0, 30);
    	CalculateVel(Nao::JID_LLEG_6, 0.0, 30);  CalculateVel(Nao::JID_RLEG_6, 0.0, 30);
}
void Strive3DGetupEngine::getUp3()
{	
	CalculateVel(Nao::JID_LARM_1, -20.0, 30);  CalculateVel(Nao::JID_RARM_1, -20.0, 30);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 40);    CalculateVel(Nao::JID_RARM_2, -0.0, 40);
   	CalculateVel(Nao::JID_LARM_3, -90.0, 40);  CalculateVel(Nao::JID_RARM_3, 90.0, 40);
    	CalculateVel(Nao::JID_LARM_4, -90.0, 40);  CalculateVel(Nao::JID_RARM_4, 90.0, 40);
	CalculateVel(Nao::JID_LLEG_1, -90.0, 40);  CalculateVel(Nao::JID_RLEG_1, -90.0, 40);
    	CalculateVel(Nao::JID_LLEG_2, -22.0, 40);    CalculateVel(Nao::JID_RLEG_2, 22.0, 40);
   	CalculateVel(Nao::JID_LLEG_3, 100.0, 50);  CalculateVel(Nao::JID_RLEG_3, 100.0, 50);
    	CalculateVel(Nao::JID_LLEG_4, -130.0, 50);  CalculateVel(Nao::JID_RLEG_4, -130.0, 50);
	CalculateVel(Nao::JID_LLEG_5, 12.0, 40);  CalculateVel(Nao::JID_RLEG_5, 12.0, 40);
    	CalculateVel(Nao::JID_LLEG_6, -3.0, 40);  CalculateVel(Nao::JID_RLEG_6, 3.0, 40);

}

void Strive3DGetupEngine::getUp4()
{	
	CalculateVel(Nao::JID_LARM_1, -20.0, 33);  CalculateVel(Nao::JID_RARM_1, -20.0, 33);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 44);    CalculateVel(Nao::JID_RARM_2, -0.0, 44);
   	CalculateVel(Nao::JID_LARM_3, -90.0, 44);  CalculateVel(Nao::JID_RARM_3, 90.0, 44);
    	CalculateVel(Nao::JID_LARM_4, -90.0, 44);  CalculateVel(Nao::JID_RARM_4, 90.0, 44);
	CalculateVel(Nao::JID_LLEG_1, -80.0, 44);  CalculateVel(Nao::JID_RLEG_1, -80.0, 44);
    	CalculateVel(Nao::JID_LLEG_2, -20.0, 44);    CalculateVel(Nao::JID_RLEG_2, 20.0, 44);
   	CalculateVel(Nao::JID_LLEG_3, 80.0, 11);  CalculateVel(Nao::JID_RLEG_3, 80.0, 11);
    	CalculateVel(Nao::JID_LLEG_4, -130.0, 54);  CalculateVel(Nao::JID_RLEG_4, -130.0, 55);
	CalculateVel(Nao::JID_LLEG_5, 16.0, 44);  CalculateVel(Nao::JID_RLEG_5, 16.0, 44);
    	CalculateVel(Nao::JID_LLEG_6, 5.0, 44);  CalculateVel(Nao::JID_RLEG_6, -5.0, 44);

}
void Strive3DGetupEngine::getUp5()
{	
	CalculateVel(Nao::JID_LARM_1, -20.0, 80);  CalculateVel(Nao::JID_RARM_1, -20.0, 80);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 40);    CalculateVel(Nao::JID_RARM_2, -0.0, 40);
   	CalculateVel(Nao::JID_LARM_3, -90.0, 40);  CalculateVel(Nao::JID_RARM_3, 90.0, 40);
    	CalculateVel(Nao::JID_LARM_4, -0.0, 60);  CalculateVel(Nao::JID_RARM_4, 0.0, 60);
	CalculateVel(Nao::JID_LLEG_1, -39.0, 50);  CalculateVel(Nao::JID_RLEG_1, -39.0, 50);
    	CalculateVel(Nao::JID_LLEG_2, 0.0, 60);    CalculateVel(Nao::JID_RLEG_2, -0.0, 60);
   	CalculateVel(Nao::JID_LLEG_3, 80.0, 60);  CalculateVel(Nao::JID_RLEG_3, 80.0, 60);
    	CalculateVel(Nao::JID_LLEG_4, -120.0, 40);  CalculateVel(Nao::JID_RLEG_4, -120.0, 40);
	CalculateVel(Nao::JID_LLEG_5, 40.0, 30);  CalculateVel(Nao::JID_RLEG_5, 40.0, 30);
    	CalculateVel(Nao::JID_LLEG_6, -8.0, 10);  CalculateVel(Nao::JID_RLEG_6, 8.0, 10);


}

void Strive3DGetupEngine::getUp6()
{	
	CalculateVel(Nao::JID_LARM_1, -90.0, 15);  CalculateVel(Nao::JID_RARM_1, -90.0, 15);
	CalculateVel(Nao::JID_LARM_2, 7.0, 30);  CalculateVel(Nao::JID_RARM_2, -7.0, 30);
	CalculateVel(Nao::JID_LARM_3, -90.0, 30);  CalculateVel(Nao::JID_RARM_3, 90.0, 30);
	CalculateVel(Nao::JID_LARM_4, -35.0, 30);  CalculateVel(Nao::JID_RARM_4, 35.0, 30);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 20);  CalculateVel(Nao::JID_RLEG_1, 0.0, 20);
	CalculateVel(Nao::JID_LLEG_2, 0.0, 40);  CalculateVel(Nao::JID_RLEG_2, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_3, 37.0, 50);  CalculateVel(Nao::JID_RLEG_3, 37.0, 50);
	CalculateVel(Nao::JID_LLEG_4, -64.0, 40);  CalculateVel(Nao::JID_RLEG_4, -64.0, 40);
	CalculateVel(Nao::JID_LLEG_5, 36.0, 40);  CalculateVel(Nao::JID_RLEG_5, 36.0, 40);
	CalculateVel(Nao::JID_LLEG_6, 0.0, 30);  CalculateVel(Nao::JID_RLEG_6, 0.0, 30);


}


















void Strive3DGetupEngine::getUpFB1()
{	mIsDone = false;
	
	CalculateVel(Nao::JID_LARM_1, -90.0, 120);  CalculateVel(Nao::JID_RARM_1, -90.0, 120);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 40);    CalculateVel(Nao::JID_RARM_2, -0.0, 40);
   	CalculateVel(Nao::JID_LARM_3, -90.0, 120);  CalculateVel(Nao::JID_RARM_3, 90.0, 120);
    	CalculateVel(Nao::JID_LARM_4, 0.0, 10);  CalculateVel(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 40);  CalculateVel(Nao::JID_RLEG_1, 0.0, 40);
    	CalculateVel(Nao::JID_LLEG_2, 0.0, 40);    CalculateVel(Nao::JID_RLEG_2, 0.0, 40);
   	CalculateVel(Nao::JID_LLEG_3, 0.0, 40);  CalculateVel(Nao::JID_RLEG_3, 0.0, 40);
    	CalculateVel(Nao::JID_LLEG_4, 0.0, 40);  CalculateVel(Nao::JID_RLEG_4, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_5, 0.0, 40);  CalculateVel(Nao::JID_RLEG_5, 0.0, 40);
    	CalculateVel(Nao::JID_LLEG_6, 0.0, 40);  CalculateVel(Nao::JID_RLEG_6, 0.0, 40);
}
void Strive3DGetupEngine::getUpFB2()
{	
	CalculateVel(Nao::JID_LARM_1, -10.0, 40);  CalculateVel(Nao::JID_RARM_1, -10.0, 40);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 40);    CalculateVel(Nao::JID_RARM_2, 0.0, 40);
   	CalculateVel(Nao::JID_LARM_3, 0.0, 0);  CalculateVel(Nao::JID_RARM_3, 0.0, 0);
    	CalculateVel(Nao::JID_LARM_4, 0.0, 40);  CalculateVel(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 40);  CalculateVel(Nao::JID_RLEG_1, 0.0, 40);
    	CalculateVel(Nao::JID_LLEG_2, 0.0, 40);    CalculateVel(Nao::JID_RLEG_2, 0.0, 40);
   	CalculateVel(Nao::JID_LLEG_3, 100.0, 80);  CalculateVel(Nao::JID_RLEG_3, 100.0, 80);     
    	CalculateVel(Nao::JID_LLEG_4, 0.0, 40);  CalculateVel(Nao::JID_RLEG_4, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_5, 0.0, 40);  CalculateVel(Nao::JID_RLEG_5, 0.0, 40);
    	CalculateVel(Nao::JID_LLEG_6, 9.0, 40);  CalculateVel(Nao::JID_RLEG_6, -9.0, 40);

}

void Strive3DGetupEngine::getUpFB3()
{	
	CalculateVel(Nao::JID_LARM_1, 0.0, 120);  CalculateVel(Nao::JID_RARM_1, 0.0, 120);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 40);    CalculateVel(Nao::JID_RARM_2, 0.0, 40);
   	CalculateVel(Nao::JID_LARM_3, 0.0, 0);  CalculateVel(Nao::JID_RARM_3, 0.0, 0);
    	CalculateVel(Nao::JID_LARM_4, 0.0, 40);  CalculateVel(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_1, -90.0, 40);  CalculateVel(Nao::JID_RLEG_1, -90.0, 40);         
    	CalculateVel(Nao::JID_LLEG_2, -10.0, 20);    CalculateVel(Nao::JID_RLEG_2, 10.0, 20);
   	CalculateVel(Nao::JID_LLEG_3, 100.0,80);  CalculateVel(Nao::JID_RLEG_3, 100.0, 80);
    	CalculateVel(Nao::JID_LLEG_4, -120.0, 40);  CalculateVel(Nao::JID_RLEG_4, -120.0, 40);
	CalculateVel(Nao::JID_LLEG_5, -12.0, 20);  CalculateVel(Nao::JID_RLEG_5, -12.0, 20);
    	CalculateVel(Nao::JID_LLEG_6, 22.0, 44);  CalculateVel(Nao::JID_RLEG_6, -22.0, 44);
}
void Strive3DGetupEngine::getUpFB4()
{	
	CalculateVel(Nao::JID_LARM_1, -70.0, 10);  CalculateVel(Nao::JID_RARM_1, -70.0, 10);
    	CalculateVel(Nao::JID_LARM_2, 0.0, 40);    CalculateVel(Nao::JID_RARM_2, 0.0, 40);
   	CalculateVel(Nao::JID_LARM_3, 0.0, 0);  CalculateVel(Nao::JID_RARM_3, 0.0, 0);
    	CalculateVel(Nao::JID_LARM_4, -35.0, 40);  CalculateVel(Nao::JID_RARM_4, 35.0, 40);
	CalculateVel(Nao::JID_LLEG_1, -30.0, 40);  CalculateVel(Nao::JID_RLEG_1, -30.0, 40);    
    	CalculateVel(Nao::JID_LLEG_2, 0.0, 10);    CalculateVel(Nao::JID_RLEG_2, -0.0, 10);
   	CalculateVel(Nao::JID_LLEG_3, 27.0, 30);  CalculateVel(Nao::JID_RLEG_3, 27.0, 30);    
    	CalculateVel(Nao::JID_LLEG_4, -74.0, 50);  CalculateVel(Nao::JID_RLEG_4, -74.0, 50);
	CalculateVel(Nao::JID_LLEG_5, 35.0, 10);  CalculateVel(Nao::JID_RLEG_5, 35.0, 10);
    	CalculateVel(Nao::JID_LLEG_6, 2.0, 40);  CalculateVel(Nao::JID_RLEG_6, -2.0, 40);
}
void Strive3DGetupEngine::getUpFB5()
{	
	CalculateVel(Nao::JID_LARM_1, -90.0, 15);  CalculateVel(Nao::JID_RARM_1, -90.0, 15);
	CalculateVel(Nao::JID_LARM_2, 7.0, 30);  CalculateVel(Nao::JID_RARM_2, -7.0, 30);
	CalculateVel(Nao::JID_LARM_3, -90.0, 30);  CalculateVel(Nao::JID_RARM_3, 90.0, 30);
	CalculateVel(Nao::JID_LARM_4, -35.0, 30);  CalculateVel(Nao::JID_RARM_4, 35.0, 30);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 20);  CalculateVel(Nao::JID_RLEG_1, 0.0, 20);
	CalculateVel(Nao::JID_LLEG_2, 0.0, 40);  CalculateVel(Nao::JID_RLEG_2, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_3, 37.0, 50);  CalculateVel(Nao::JID_RLEG_3, 37.0, 50);
	CalculateVel(Nao::JID_LLEG_4, -64.0, 40);  CalculateVel(Nao::JID_RLEG_4, -64.0, 40);
	CalculateVel(Nao::JID_LLEG_5, 36.0, 10);  CalculateVel(Nao::JID_RLEG_5, 36.0, 10);
	CalculateVel(Nao::JID_LLEG_6, 0.0, 30);  CalculateVel(Nao::JID_RLEG_6, 0.0, 30);	
}
/*void Strive3DGetupEngine::getUpFB6()
{	
        CalculateVel(Nao::JID_LARM_1, -90.0, 15);  CalculateVel(Nao::JID_RARM_1, -90.0, 15);
	CalculateVel(Nao::JID_LARM_2, 7.0, 30);  CalculateVel(Nao::JID_RARM_2, -7.0, 30);
	CalculateVel(Nao::JID_LARM_3, -90.0, 30);  CalculateVel(Nao::JID_RARM_3, 90.0, 30);
	CalculateVel(Nao::JID_LARM_4, -35.0, 30);  CalculateVel(Nao::JID_RARM_4, 35.0, 30);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 20);  CalculateVel(Nao::JID_RLEG_1, 0.0, 20);
	CalculateVel(Nao::JID_LLEG_2, 0.0, 40);  CalculateVel(Nao::JID_RLEG_2, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_3, 37.0, 50);  CalculateVel(Nao::JID_RLEG_3, 37.0, 50);
	CalculateVel(Nao::JID_LLEG_4, -64.0, 40);  CalculateVel(Nao::JID_RLEG_4, -64.0, 40);
	CalculateVel(Nao::JID_LLEG_5, 36.0, 40);  CalculateVel(Nao::JID_RLEG_5, 36.0, 40);
	CalculateVel(Nao::JID_LLEG_6, 0.0, 30);  CalculateVel(Nao::JID_RLEG_6, 0.0, 30);
}	*/

void Strive3DGetupEngine::adjustment()
{
	CalculateVel(Nao::JID_LARM_1, -120.0, 40);  CalculateVel(Nao::JID_RARM_1, -120.0, 40);
    	CalculateVel(Nao::JID_LARM_2, 30.0, 40);  CalculateVel(Nao::JID_RARM_2, -30.0, 40);
   	CalculateVel(Nao::JID_LARM_3, 0.0, 40);  CalculateVel(Nao::JID_RARM_3, 0.0, 40);
    	CalculateVel(Nao::JID_LARM_4, 0.0, 40);  CalculateVel(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 40);  CalculateVel(Nao::JID_RLEG_1, 0.0, 40);
    	CalculateVel(Nao::JID_LLEG_2, 0.0, 40);  CalculateVel(Nao::JID_RLEG_2, 0.0, 40);
   	CalculateVel(Nao::JID_LLEG_3, 0.0, 40);  CalculateVel(Nao::JID_RLEG_3, 0.0, 40);
    	CalculateVel(Nao::JID_LLEG_4, 0.0, 40);  CalculateVel(Nao::JID_RLEG_4, 0.0, 40);
	CalculateVel(Nao::JID_LLEG_5, 0.0, 40);  CalculateVel(Nao::JID_RLEG_5, 0.0, 40);
    	CalculateVel(Nao::JID_LLEG_6, 0.0, 40);  CalculateVel(Nao::JID_RLEG_6, 0.0, 40);
}
