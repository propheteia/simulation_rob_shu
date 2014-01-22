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


#include "NaoKick.h"
#include "self.h"
#include "nao.h"
#include "worldmodel.h"
#include "ball.h"


NaoKickEngine::NaoKickEngine(const KickEngineInterfaces& interfaces ):KickEngine(interfaces),mInit(true),state(disable)
{	
}

void NaoKickEngine::Init()
{
     	mPhase              = KICKL1;
	mPhase2             = KICKR1;
    	mPhaseCycle         = 0;
    	mPhaseStartCycle    = WM.getSelf().CurrentCycle;
	if(1)
		state = KICKLEFT;

	else
	{	state = KICKRIGHT;
	}
}

void NaoKickEngine::CalculateVel(JointID id, float angle, float maxVel)
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

	vel = maxVel * minus / 10.0;
	vel = std::min(gDegToRad(vel) * 10.0f, 100.0f);
	//aLOG << "minus: " << minus << "       vel: " << vel << endl;
	mJointVel[id] = vel;
}

void NaoKickEngine::CalculateVel2(JointID id, float angle, float maxVel)
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
void NaoKickEngine::CalculateVel3(JointID id, float angle, float maxVel)
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

		
bool NaoKickEngine::executeParameterized()
{	if(mInit)
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
			case KICKL5:
				kickLeft5();
			break;
			
		
		
		}
	}
/*	else if (state == KICKRIGHT)
	{
		aLOG << " KickRight Phase" << mPhase2 <<endl;
		KickingRight();
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
		case KICKR5:
			kickRight5();
		break;
		}
	}
*/

	
	if (mIsDone)
	{	mInit = true;
		return false;
	} 
	return true;
}

void NaoKickEngine::KickingLeft()
{	aLOG << "KickLeft: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
	
	if( mPhase == KICKL1 && 
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_1])+0.0))<0.1 &&
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_2])+0.0))<0.1 &&
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_2])+0.0))<0.1 &&
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_3])-30.0))<0.1 &&
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_4])+60.0))<0.1 &&
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_5])-30.0))<0.1 //&&
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_6])-0.0))<0.1 
	(WM.getSelf().CurrentCycle -mPhaseStartCycle )>=15
	)
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = KICKL2;
	}
	if( mPhase == KICKL2 && /***gAbs((gRadToDeg(mJointAngle[Nao::JID_LARM_1])+20.0))<0.1&& gAbs((gRadToDeg(mJointAngle[Nao::JID_LARM_4])+90.0))<0.1 &&**/ //gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_1])+0.0))<0.1 && 
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_2])-9.0))<0.1 && 
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-0.0))<0.1 && 
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+1.0))<0.1 && 
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_5])-1.0))<0.1 && 
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_6])+9.0))<0.1
	(WM.getSelf().CurrentCycle -mPhaseStartCycle )>=15
	)
	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = KICKL3;
	}
	if( mPhase == KICKL3&& 
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_1])-1.0))<0.1 && 
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_2])-6.0))<0.1 && 
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-32.0))<0.1// &&	
 	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_4])+95.0))<0.1 &&
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_5])-70.0))<0.1 &&
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_6])-11.0))<0.1 
	(WM.getSelf().CurrentCycle -mPhaseStartCycle )>=15
	)
	{	
			mPhaseStartCycle = WM.getSelf().CurrentCycle;
			mPhase = KICKL4;
	}

	if( mPhase == KICKL4 && 
	//gAbs(gRadToDeg(mJointAngle[Nao::JID_RLEG_1])-1.0)<0.1 && 
	//gAbs(gRadToDeg(mJointAngle[Nao::JID_RLEG_2])-6.0)<0.1 && 
	//gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_3])+28.0)<0.1 && 
	//gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+69.0)<0.1 && 
	//gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_5])-34.0)<0.1 //&&
	(WM.getSelf().CurrentCycle -mPhaseStartCycle )>=10
	)
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_RLEG_6])-11.0))<0.1 
	
	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = KICKL5;
	}
 	if( mPhase == KICKL5 && 
	//gAbs(gRadToDeg(mJointAngle[Nao::JID_RLEG_3])-50.0)<0.1 && 
	//gAbs(gRadToDeg(mJointAngle[Nao::JID_RLEG_4])+34.0)<0.1// &&
	//gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_5])-0.0))<0.1 
	(WM.getSelf().CurrentCycle -mPhaseStartCycle )>=3
	)
 	{	mPhaseStartCycle = WM.getSelf().CurrentCycle;
 		mIsDone = true;
 	}

}


void NaoKickEngine::kickLeft1()
{	
	mIsDone = false;
	cout<<"Kick2"<<endl;
	CalculateVel(Nao::JID_LARM_1, -120.0, 3);  CalculateVel(Nao::JID_RARM_1, -120.0, 3);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 3);    CalculateVel(Nao::JID_RARM_2, -10.0, 3);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 3);  CalculateVel(Nao::JID_RARM_3, 80.0, 3);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 3);  CalculateVel(Nao::JID_RARM_4, 80.0, 3);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 3);  CalculateVel(Nao::JID_RLEG_1, 0.0, 3);
    	CalculateVel(Nao::JID_LLEG_2, 0.0, 3);    CalculateVel(Nao::JID_RLEG_2, 0.0, 3);
   	CalculateVel(Nao::JID_LLEG_3, 32.0, 3);  CalculateVel(Nao::JID_RLEG_3, 32.0, 3);
    	CalculateVel(Nao::JID_LLEG_4, -61.0, 3);  CalculateVel(Nao::JID_RLEG_4, -61.0, 3);
	CalculateVel(Nao::JID_LLEG_5, 32.0, 3);  CalculateVel(Nao::JID_RLEG_5, 32.0, 3);
    	CalculateVel(Nao::JID_LLEG_6, 0.0, 3);  CalculateVel(Nao::JID_RLEG_6, 0.0, 3);
}
void NaoKickEngine::kickLeft2()
{	
	cout<<"Kick3"<<endl;
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

}

void NaoKickEngine::kickLeft3()
{	
	cout<<"Kick4"<<endl;
	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);    CalculateVel(Nao::JID_RLEG_1, 0.0, 2);
    	CalculateVel(Nao::JID_LLEG_2, 10.0, 0);    CalculateVel(Nao::JID_RLEG_2, 9.0, 0);
   	CalculateVel(Nao::JID_LLEG_3, 35.0, 4);  CalculateVel(Nao::JID_RLEG_3, 5.0, 6);
    	CalculateVel(Nao::JID_LLEG_4, -80.0, 4);  CalculateVel(Nao::JID_RLEG_4, -9.0, 5);
	CalculateVel(Nao::JID_LLEG_5, 35.0, 4);  CalculateVel(Nao::JID_RLEG_5, 5.0, 4);
    	CalculateVel(Nao::JID_LLEG_6, -10.0, 0);  CalculateVel(Nao::JID_RLEG_6, -10.0, 0);

}

void NaoKickEngine::kickLeft4()
{	cout<<"Kick5"<<endl;
	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 6);    CalculateVel(Nao::JID_RLEG_1, 0.0, 4);
    	CalculateVel(Nao::JID_LLEG_2, -9.0, 4);    CalculateVel(Nao::JID_RLEG_2, 9.0, 0);
   	CalculateVel(Nao::JID_LLEG_3, 5.0, 6);  CalculateVel(Nao::JID_RLEG_3, 5.0, 9);
    	CalculateVel(Nao::JID_LLEG_4, -70.0, 4);  CalculateVel(Nao::JID_RLEG_4, -9.0, 8);
	CalculateVel(Nao::JID_LLEG_5, 45.0, 7);  CalculateVel(Nao::JID_RLEG_5, 5.0, 4);
    	CalculateVel(Nao::JID_LLEG_6, 5.0, 6);  CalculateVel(Nao::JID_RLEG_6, -10.0, 0);


}

void NaoKickEngine::kickLeft5()
{	cout<<"Kick6"<<endl;
	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
    	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
   	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
    	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 4);
	CalculateVel(Nao::JID_LLEG_2, -9.0, 0);  CalculateVel(Nao::JID_RLEG_2, 9.0, 0);
	CalculateVel(Nao::JID_LLEG_3, 80.0, 8);  CalculateVel(Nao::JID_RLEG_3, -30.0, 6);
	CalculateVel(Nao::JID_LLEG_4, 10.0, 8);  CalculateVel(Nao::JID_RLEG_4, -10.0, 0);
	CalculateVel(Nao::JID_LLEG_5, -20, 9);  CalculateVel(Nao::JID_RLEG_5, 5.0, 0);
	CalculateVel(Nao::JID_LLEG_6, 1.0, 0);  CalculateVel(Nao::JID_RLEG_6, -10.0, 0);


}
// void NaoKickEngine::kickLeft7()
// {	cout<<"Kick6"<<endl;
// 	CalculateVel(Nao::JID_LARM_1, -120.0, 1);  CalculateVel(Nao::JID_RARM_1, -120.0, 1);
//     	CalculateVel(Nao::JID_LARM_2, 20.0, 1);    CalculateVel(Nao::JID_RARM_2, -10.0, 1);
//    	CalculateVel(Nao::JID_LARM_3, -80.0, 1);  CalculateVel(Nao::JID_RARM_3, 80.0, 1);
//     	CalculateVel(Nao::JID_LARM_4, -80.0, 1);  CalculateVel(Nao::JID_RARM_4, 80.0, 1);
// 	CalculateVel(Nao::JID_LLEG_1, 0.0, 2);  CalculateVel(Nao::JID_RLEG_1, 0.0, 4);
// 	CalculateVel(Nao::JID_LLEG_2, -2.0, 0);  CalculateVel(Nao::JID_RLEG_2, 9.0, 0);
// 	CalculateVel(Nao::JID_LLEG_3, 40.0, 8);  CalculateVel(Nao::JID_RLEG_3, 25.0, 6);
// 	CalculateVel(Nao::JID_LLEG_4, -70.0, 8);  CalculateVel(Nao::JID_RLEG_4, -65.0, 0);
// 	CalculateVel(Nao::JID_LLEG_5, 35.0, 9);  CalculateVel(Nao::JID_RLEG_5, 36.0, 0);
// 	CalculateVel(Nao::JID_LLEG_6, 2.0, 0);  CalculateVel(Nao::JID_RLEG_6, -10.0, 0);
// 
// 
// }