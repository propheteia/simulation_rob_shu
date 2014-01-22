/***************************************************************************
 *   Copyright (C) 2008 by Neil_Zhu   *
 *   zhuming535984@gmail.com   *
 *   Walk Test                                                             *
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

#include "NaoGetUp.h"
#include "self.h"
#include "nao.h"

#include <fstream>

using namespace std;

NaoGetupEngine::NaoGetupEngine(const GetupEngineInterfaces& interfaces ):GetupEngine(interfaces),mInit(true),state(disable)
{	
}

void NaoGetupEngine::Init()
{
     	mPhase              = GETUP1;
	mPhase2             = GETUPFB1;
    	mPhaseCycle         = 0;
    	mPhaseStartCycle    = WM.getSelf().CurrentCycle;
        Vector3f acceleration = WM.getSelf().GetAcceleration();
	if(acceleration.y() > 7.0 || WM.getSelf().onMyback())
	{
		state = GETUPFROMBACK;
		aLOG << "On my Back, Getting From Back!" << endl; 
	}
	else if(acceleration.y() < -7.0 || WM.getSelf().onMyBelly())
	{
		state = GETUP;
		aLOG << "On my Belly, Getting From Belly!" << endl;
	}
	else
	{	state = ONMYSIDE;
		aLOG << "On my Side ,Waitting!!!" << endl;
	}
	
}


void NaoGetupEngine::CalculateVel1(JointID id, float angle, float maxVel)
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
	aLOG << "minus: " <<  angle << "       time: " <<  WM.getFieldInfo().match_time  << endl;
    mJointVel[id] = vel;
}

void NaoGetupEngine::CalculateVel2(JointID id, float foreangle ,float angle, float maxVel)
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
    float allminus= gNormalizeDeg(angle-foreangle);
    float w= allminus/maxVel;
    int i;
    float decAngle=allminus/50;
    float stageAngle[150];

      stageAngle[0]=foreangle;
      for(i=0;i<52;i++)
      {   stageAngle[i]+=decAngle;
       }
    if(curAngle<foreangle)
    { vel=35;}
    for(i=1;i<52;i++)
    {
         if(curAngle>stageAngle[i-1]||curAngle<stageAngle[i])
           vel=exp(i*log(allminus)/51);
		break;	
     }

    vel = gAbs(minus) > maxVel ? maxVel * gSign(minus) : minus;
    vel = std::min(gDegToRad(vel) * 10.0f, 100.0f);
	//aLOG << "minus: " << minus << "       vel: " << vel << endl;
    mJointVel[id] = vel;
}

void NaoGetupEngine::CalculateVel(JointID id, float startAngle ,float endAngle, float maxVel)
{	if (gAbs(endAngle - startAngle) < 1e-4)
	{	mJointVel[id] = 0;
		
	}
	else
	{
		//startAngle += 0.5;
		//endAngle += 0.5;
		float phi = (gRadToDeg(mJointAngle[id]) - startAngle) * M_PI / (endAngle - startAngle) + 0.018;
		float vel = gSign(endAngle - startAngle) * gSin(phi);
		mJointVel[id] = vel * gAbs(endAngle - startAngle) * 0.1;
	}
}

bool NaoGetupEngine::executeParameterized()
{
	if(mInit)
	{	
		Init();
		mInit = false;
	}

	
	if(state == GETUP)
	{
		gettingUp();
		aLOG << " GetUp Phase!: " << mPhase <<endl;
// 		adjustment();
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
			break;
			case GETUP7:
				getUp7();
			break;
		}
	}
	else if (state == GETUPFROMBACK)
	{
		aLOG << " GetUpFromBack Phase" << mPhase2 <<endl;
// 		adjustment();
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
		mIsDone = true;
	}
 	else 
		aLOG << "Unknowed getup state!" << endl;
	
	if (mIsDone || (WM.getSelf().CurrentCycle -mPhaseStartCycle )>168)
	{	mInit = true;
		return false;
	}
	aLOG << "mInit: " << mInit << " mPhaseStartCycle: " << mPhaseStartCycle << " WM.getSelf().CurrentCycle: " << WM.getSelf().CurrentCycle << endl;
	return true;
}

void NaoGetupEngine::gettingUp()
{	aLOG << "GettingUp: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle  << endl;
	if( mPhase == GETUP1 && (gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+70.0))<0.1))// &&
		 //(WM.getSelf().CurrentCycle -mPhaseStartCycle )>30 ))
	{	aLOG<<"mPhase1  GETUP1 "<<endl;
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = GETUP2;
	}
	else if( mPhase == GETUP2 &&( gAbs((gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+25.0))<0.1))//||
		// (WM.getSelf().CurrentCycle -mPhaseStartCycle )>25))
	{	aLOG<<"mPhase2 GETUP2"<<endl;
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = GETUP4;
	}
// 	else if( mPhase == GETUP3 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+88.0)<0.1))//||
// 		//(WM.getSelf().CurrentCycle -mPhaseStartCycle )>25))
// 	{	aLOG<<"mPhase3 GETUP3"<<endl;
// 		mPhaseStartCycle = WM.getSelf().CurrentCycle;
// 		mPhase = GETUP4;
// 	}
	else if( mPhase == GETUP4 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_5])-35.0)<0.1))//||
		//(WM.getSelf().CurrentCycle -mPhaseStartCycle )>21))
	{	aLOG<<"mPhase4 GETUP4"<<endl;
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = GETUP6;
	}
// 	else if( mPhase == GETUP5 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_5])-18.0)<0.1))//||
// 		//(WM.getSelf().CurrentCycle -mPhaseStartCycle )>19))
// 	{	aLOG<<"mPhase5 GETUP5"<<endl;
// 		mPhaseStartCycle = WM.getSelf().CurrentCycle;
// 		mPhase = GETUP6;
// 	}
	else if( mPhase == GETUP6 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_5]))<0.1))//||
		//(WM.getSelf().CurrentCycle -mPhaseStartCycle )>19))
	{	aLOG<<"mPhase6 GETUP6"<<endl;
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase = GETUP7;
	}
	else if( mPhase == GETUP7 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+70.0)<1.0 &&
		(WM.getSelf().CurrentCycle -mPhaseStartCycle )>15))
	{	aLOG<<"mPhase7 GETUP7"<<endl;
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mIsDone = true;
	}
	else
	{
		aLOG << "Error Phase!" << endl;
	}
}

void NaoGetupEngine::gettingUpFromBack()
{	aLOG << "GettingUpFromBack: " << WM.getSelf().CurrentCycle<<"  "<< mPhaseStartCycle << endl;
	if( mPhase2 == GETUPFB1 && ( ( gAbs(gRadToDeg(mJointAngle[Nao::JID_LARM_3])-90.0)<0.1 ) || 
//     			(WM.getSelf().CurrentCycle -mPhaseStartCycle >42) ) )
	      		(WM.getSelf().CurrentCycle -mPhaseStartCycle >15) ) )
	{	
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		aLOG<<"mPhase1"<<endl;
		
		mPhase2 = GETUPFB2;
	}
	if( mPhase2 == GETUPFB2 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_3])-90.0)<1.0||
// 		(WM.getSelf().CurrentCycle -mPhaseStartCycle )>30))
		(WM.getSelf().CurrentCycle -mPhaseStartCycle )>10))
	{	aLOG<<"mPhase2"<<endl;
		
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase2 = GETUPFB3;
	}
	if( mPhase2 == GETUPFB3 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+116.0)<1.0||
// 		(WM.getSelf().CurrentCycle -mPhaseStartCycle )>30))
		(WM.getSelf().CurrentCycle -mPhaseStartCycle )>20))
	{	aLOG<<"mPhase3"<<endl;
		
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase2 = GETUPFB4;
	}
	if( mPhase2 == GETUPFB4 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_RLEG_4])+100.0)<1.0||
// 		(WM.getSelf().CurrentCycle -mPhaseStartCycle )>30))
		(WM.getSelf().CurrentCycle -mPhaseStartCycle )>20))
	{	aLOG<<"mPhase4"<<endl;
		
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mPhase2 = GETUPFB5;
	}
	if( mPhase2 == GETUPFB5 && (gAbs(gRadToDeg(mJointAngle[Nao::JID_LLEG_4])+64.0)<1.0 ||
// 		(WM.getSelf().CurrentCycle -mPhaseStartCycle )>35))
		(WM.getSelf().CurrentCycle -mPhaseStartCycle )>25))
	{	aLOG<<"mPhase5"<<endl;
		mPhaseStartCycle = WM.getSelf().CurrentCycle;
		mIsDone = true;
	}

}



void NaoGetupEngine::getUp1()
{	mIsDone = false;
	/**&head1;=-2,&head2;=-45,&larm1;=-120,&larm2;= 	20,&larm3;=-80,&larm4;=-80,&rarm1;=-120,&rarm2;=-10,&rarm3;= 80,&rarm4;= 80,&lleg1;= 0,&lleg2;= 0,&lleg3;= 35,&lleg4;=-84,&lleg5;= 70,&lleg6;= 0,&rleg1;= 0,&rleg2;= 0,&rleg3;= 35,&rleg4;=-84,&rleg5;= 70,&rleg6;= 0;*/
	CalculateVel1(Nao::JID_LARM_1, -120.0, 40);  CalculateVel1(Nao::JID_RARM_1, -120.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 20.0, 40);    CalculateVel1(Nao::JID_RARM_2, -10.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, -80.0, 40);  CalculateVel1(Nao::JID_RARM_3, 80.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, -80.0, 40);  CalculateVel1(Nao::JID_RARM_4, 80.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_1, -0.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, 0.0, 40);    CalculateVel1(Nao::JID_RLEG_2, -0.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 39.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 39.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, -70.0, 40);  CalculateVel1(Nao::JID_RLEG_4, -70.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 38.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 38.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 0.0, 40);
}
void NaoGetupEngine::getUp2()
{	/**&head1;=-2,&head2;=-45,&larm1;=-56,&larm2;= 20,&larm3;=-80,&larm4;=-80,&rarm1;=-50,&rarm2;=-10,&rarm3;= 80,&rarm4;= 80,&lleg1;= -29,&lleg2;= 0,&lleg3;= 35,&lleg4;=-25,&lleg5;= 60,&lleg6;=-20,&rleg1;= -29,&rleg2;= 0,&rleg3;= 35,&rleg4;=-24,&rleg5;= 59,&rleg6;=22;*/
	CalculateVel1(Nao::JID_LARM_1, -56.0, 40);  CalculateVel1(Nao::JID_RARM_1, -50.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 20.0, 40);    CalculateVel1(Nao::JID_RARM_2, -10.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, -80.0, 40);  CalculateVel1(Nao::JID_RARM_3, 80.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, -80.0, 40);  CalculateVel1(Nao::JID_RARM_4, 80.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, -29.0, 40);  CalculateVel1(Nao::JID_RLEG_1, -29.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, 0.0, 40);    CalculateVel1(Nao::JID_RLEG_2, -0.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 35.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 35.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4,-25.0, 40);  CalculateVel1(Nao::JID_RLEG_4, -24.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 60.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 59.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, -20.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 22.0, 40);

}
void NaoGetupEngine::getUp3()
{	/**&head1;=-2,&head2;=-45,&larm1;=-12,&larm2;= 20,&larm3;=-71,&larm4;=-16,&rarm1;=-12,&rarm2;=-10,&rarm3;= 78,&rarm4;= 12,&lleg1;= -15,&lleg2;=-2,&lleg3;= 90,&lleg4;=-88,&lleg5;= 70,&lleg6;=-12,&rleg1;= -15,&rleg2;= 0,&rleg3;= 86,&rleg4;=-86,&rleg5;= 70,&rleg6;=12;*/
	CalculateVel1(Nao::JID_LARM_1, 0.0, 40);  CalculateVel1(Nao::JID_RARM_1, 0.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 20.0, 40);    CalculateVel1(Nao::JID_RARM_2, -10.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, -71.0, 40);  CalculateVel1(Nao::JID_RARM_3, 78.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, -16.0, 40);  CalculateVel1(Nao::JID_RARM_4, 12.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, -15.0, 20);  CalculateVel1(Nao::JID_RLEG_1, -15.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, -2.0, 40);    CalculateVel1(Nao::JID_RLEG_2, -0.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 90.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 86.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, -88.0, 40);  CalculateVel1(Nao::JID_RLEG_4, -86.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 70.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 70.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, -12.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 12.0, 40);

}

void NaoGetupEngine::getUp4()
{	/**&wait;= 0.5;
	&head1;=-0,&head2;=-0,&larm1;=-14,&larm2;= 0,&larm3;=-71,&larm4;= 0,&rarm1;=-21,&rarm2;=-1,&rarm3;= 78,&rarm4;= 0,&lleg1;= -70,&lleg2;= 23,&lleg3;= 90,&lleg4;=-130,&lleg5;= 35,&lleg6;=-40,&rleg1;=-70,&rleg2;=-30,&rleg3;= 90,&rleg4;=-130,&rleg5;= 34,&rleg6;=45;*/
	CalculateVel1(Nao::JID_LARM_1, -14.0, 40);  CalculateVel1(Nao::JID_RARM_1, -21.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 0.0, 40);    CalculateVel1(Nao::JID_RARM_2, -1.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, -71.0, 40);  CalculateVel1(Nao::JID_RARM_3, 78.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, 0.0, 40);  CalculateVel1(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, -70.0, 40);  CalculateVel1(Nao::JID_RLEG_1, -70.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, 23.0, 40);    CalculateVel1(Nao::JID_RLEG_2, -30.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 90.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 90.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, -130.0, 40);  CalculateVel1(Nao::JID_RLEG_4, -130.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 35.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 34.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, -40.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 45.0, 40);

}

void NaoGetupEngine::getUp5()
{	/**&wait;= 0.1;
	&head1;=0,&head2;=0,&larm1;=-37,&larm2;= 0,&larm3;=-71,&larm4;= 0,&rarm1;=-37,&rarm2;=-1,&rarm3;= 78,&rarm4;= 0,&lleg1;= -56,&lleg2;=-25,&lleg3;= 90,&lleg4;=-120,&lleg5;= 18,&lleg6;= 7,&rleg1;= -56,&rleg2;=-45,&rleg3;= 90,&rleg4;=-130,&rleg5;= 41,&rleg6;=45;*/
	CalculateVel1(Nao::JID_LARM_1, -37.0, 40);  CalculateVel1(Nao::JID_RARM_1, -37.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 0.0, 40);    CalculateVel1(Nao::JID_RARM_2, -1.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, -71.0, 40);  CalculateVel1(Nao::JID_RARM_3, 78.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, 0.0, 40);  CalculateVel1(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, -55.0, 40);  CalculateVel1(Nao::JID_RLEG_1, -55.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, -25.0, 40);    CalculateVel1(Nao::JID_RLEG_2, -45.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 90.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 90.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, -120.0, 40);  CalculateVel1(Nao::JID_RLEG_4, -130.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 18.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 41.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, 7.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 45.0, 40);

}

void NaoGetupEngine::getUp6()
{	/**&head1;= 0,&head2;= 0,&larm1;= 0,&larm2;= 0,&larm3;= 0,&larm4;= 0,&rarm1;= 0,&rarm2;= 0,&rarm3;= 0,&rarm4;= 0,&lleg1;=-70,&lleg2;= 3,&lleg3;= 90,&lleg4;=-110,&lleg5;= 0,&lleg6;= 0,&rleg1;=-70,&rleg2;= 0,&rleg3;= 90,&rleg4;=-100,&rleg5;= 0,&rleg6;= 0;*/
	CalculateVel1(Nao::JID_LARM_1, 0.0, 40);  CalculateVel1(Nao::JID_RARM_1, 0.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 0.0, 40);    CalculateVel1(Nao::JID_RARM_2, 0.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, 0.0, 40);  CalculateVel1(Nao::JID_RARM_3, 0.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, 0.0, 40);  CalculateVel1(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, -70.0, 40); CalculateVel1(Nao::JID_RLEG_1, -70.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, -3.0, 40);   CalculateVel1(Nao::JID_RLEG_2, -0.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 90.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 90.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, -110.0, 40);  CalculateVel1(Nao::JID_RLEG_4, -100.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 0.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 0.0, 40);

}



void NaoGetupEngine::getUp7()
{	
	CalculateVel(Nao::JID_LARM_1, 0.0,-90.0, 40);  CalculateVel(Nao::JID_RARM_1, 0.0,-90.0, 40);
    	CalculateVel(Nao::JID_LARM_2, 0.0,7.0, 40);  CalculateVel(Nao::JID_RARM_2, 0.0,-7.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, -90.0, 30);  CalculateVel1(Nao::JID_RARM_3, 90.0, 30);
    	CalculateVel(Nao::JID_LARM_4, 0.0,-35.0, 40);  CalculateVel(Nao::JID_RARM_4, 0.0,35.0, 40);
	CalculateVel(Nao::JID_LLEG_1, -70.0,0.5, 40);  CalculateVel(Nao::JID_RLEG_1, -70.0,0.5, 40);
    	CalculateVel(Nao::JID_LLEG_2, 3.0,0.0, 40);  CalculateVel(Nao::JID_RLEG_2, 0.0,0.0, 40);
   	CalculateVel(Nao::JID_LLEG_3, 90.0,39.0, 40);  CalculateVel(Nao::JID_RLEG_3, 90.0,39.0, 40);
    	CalculateVel(Nao::JID_LLEG_4, -110.0,-70.0, 40);  CalculateVel(Nao::JID_RLEG_4, -100.0,-70.0, 40);
	CalculateVel(Nao::JID_LLEG_5, 0.0,38.0, 40);  CalculateVel(Nao::JID_RLEG_5, 0.0,38.0, 40);
    	CalculateVel(Nao::JID_LLEG_6, 0.0,0.0, 40);  CalculateVel(Nao::JID_RLEG_6, 0.0,0.0, 40);
}

void NaoGetupEngine::getUpFB1()
{	mIsDone = false;
	/**&head1;= 0,&head2;= 0,&larm1;=-120,&larm2;= 0,&larm3;= 90,&larm4;=-72,&lleg1;= 0,&lleg2;= 0,&lleg3;= 0,&lleg4;= 0,&lleg5;= 0,&lleg6;= 0,&rleg1;= 0,&rleg2;= 0,&rleg3;= 0,&rleg4;= 0,&rleg5;= 0,&rleg6;= 0,&rarm1;=-120,&rarm2;= 0,&rarm3;=-90,&rarm4;= 61;	*/
	CalculateVel1(Nao::JID_LARM_1, -120.0, 40);  CalculateVel1(Nao::JID_RARM_1, -120.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 0.0, 40);    CalculateVel1(Nao::JID_RARM_2, -0.0, 40);
   	CalculateVel2(Nao::JID_LARM_3, 90.0,90.0, 40);  CalculateVel2(Nao::JID_RARM_3, -90.0,-90.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, -65.0, 40);  CalculateVel1(Nao::JID_RARM_4, 61.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_1, 0.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, 0.0, 40);    CalculateVel1(Nao::JID_RLEG_2, 0.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 0.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_4, 0.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 0.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 0.0, 40);;
}
void NaoGetupEngine::getUpFB2()
{	/**&head1;= 0,&head2;= 0,&larm1;= 0,&larm2;= 0,&larm3;= 0,&larm4;= 0,&lleg1;=-70,&lleg2;=-4,&lleg3;= 90,&lleg4;= 0,&lleg5;= 0,&lleg6;= 0,&rleg1;=-70,&rleg2;= 0,&rleg3;= 90,&rleg4;= 0,&rleg5;= 0,&rleg6;= 0,&rarm1;= 0,&rarm2;= 0,&rarm3;= 0,&rarm4;= 0;*/
	CalculateVel1(Nao::JID_LARM_1, 0.0, 40);  CalculateVel1(Nao::JID_RARM_1, 0.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 0.0, 40);    CalculateVel1(Nao::JID_RARM_2, 0.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, 0.0, 40);  CalculateVel1(Nao::JID_RARM_3, 0.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, 0.0, 40);  CalculateVel1(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, -70.0, 40);  CalculateVel1(Nao::JID_RLEG_1, -70.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, -4.0, 40);    CalculateVel1(Nao::JID_RLEG_2, -0.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 90.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 90.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_4, 0.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 0.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 0.0, 40);

}

void NaoGetupEngine::getUpFB3()
{	/**&head1;= 0,&head2;= 0,&larm1;= 0,&larm2;= 0,&larm3;= 0,&larm4;= 0,&lleg1;=-70,&lleg2;=-4,&lleg3;= 90,&lleg4;=-116,&lleg5;= 0,&lleg6;= 0,&rleg1;=-70,&rleg2;= 0,&rleg3;= 90,&rleg4;= 0,&rleg5;= 0,&rleg6;= 0,&rarm1;= 0,&rarm2;= 0,&rarm3;= 0,&rarm4;= 0;
	&wait;= 0;*/
	CalculateVel1(Nao::JID_LARM_1, 0.0, 40);  CalculateVel1(Nao::JID_RARM_1, 0.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 0.0, 40);    CalculateVel1(Nao::JID_RARM_2, 0.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, 0.0, 40);  CalculateVel1(Nao::JID_RARM_3, 0.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, 0.0, 40);  CalculateVel1(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, -70.0, 40);  CalculateVel1(Nao::JID_RLEG_1, -70.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, -4.0, 40);    CalculateVel1(Nao::JID_RLEG_2, -0.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 90.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 90.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, -116.0, 40);  CalculateVel1(Nao::JID_RLEG_4, 0.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 0.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 0.0, 40);
}
void NaoGetupEngine::getUpFB4()
{	/**&head1;= 0,&head2;= 0,&larm1;= 0,&larm2;= 0,&larm3;= 0,&larm4;= 0,&lleg1;=-70,&lleg2;= 3,&lleg3;= 90,&lleg4;=-110,&lleg5;= 0,&lleg6;= 0,&rleg1;=-70,&rleg2;= 0,&rleg3;= 90,&rleg4;=-100,&rleg5;= 0,&rleg6;= 0,&rarm1;= 0,&rarm2;= 0,&rarm3;= 0,&rarm4;= 0;*/
	CalculateVel1(Nao::JID_LARM_1, 0.0, 40);  CalculateVel1(Nao::JID_RARM_1, 0.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 0.0, 40);    CalculateVel1(Nao::JID_RARM_2, 0.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, 0.0, 40);  CalculateVel1(Nao::JID_RARM_3, 0.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, 0.0, 40);  CalculateVel1(Nao::JID_RARM_4, 0.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, -71.0, 40);  CalculateVel1(Nao::JID_RLEG_1, -71.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, 3.0, 40);    CalculateVel1(Nao::JID_RLEG_2, -0.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 90.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 90.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, -110.0, 40);  CalculateVel1(Nao::JID_RLEG_4, -100.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 0.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 0.0, 40);

}

void NaoGetupEngine::getUpFB5()
{	
	CalculateVel1(Nao::JID_LARM_1, -90.0, 40);  CalculateVel1(Nao::JID_RARM_1, -90.0, 40);
    	CalculateVel1(Nao::JID_LARM_2, 7.0, 40);  CalculateVel1(Nao::JID_RARM_2, -7.0, 40);
   	CalculateVel1(Nao::JID_LARM_3, -90.0, 40);  CalculateVel1(Nao::JID_RARM_3, 90.0, 40);
    	CalculateVel1(Nao::JID_LARM_4, -35.0, 40);  CalculateVel1(Nao::JID_RARM_4, 35.0, 40);
	CalculateVel1(Nao::JID_LLEG_1, 1.0, 40);  CalculateVel1(Nao::JID_RLEG_1, 1.0, 40);
    	CalculateVel1(Nao::JID_LLEG_2, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_2, 0.0, 40);
   	CalculateVel1(Nao::JID_LLEG_3, 37.0, 40);  CalculateVel1(Nao::JID_RLEG_3, 37.0, 40);
    	CalculateVel1(Nao::JID_LLEG_4, -64.0, 40);  CalculateVel1(Nao::JID_RLEG_4, -64.0, 40);
	CalculateVel1(Nao::JID_LLEG_5, 36.0, 40);  CalculateVel1(Nao::JID_RLEG_5, 36.0, 40);
    	CalculateVel1(Nao::JID_LLEG_6, 0.0, 40);  CalculateVel1(Nao::JID_RLEG_6, 0.0, 40);
}

void NaoGetupEngine::adjustment()
{
	CalculateVel2(Nao::JID_LARM_1, -120.0,-120.0, 40);  CalculateVel2(Nao::JID_RARM_1, 120.0,-120.0, 40);
    	CalculateVel2(Nao::JID_LARM_2, 35.0,35.0, 40);  CalculateVel2(Nao::JID_RARM_2, -35.0,-35.0, 40);
   	CalculateVel2(Nao::JID_LARM_3, 0.0,0.0, 40);  CalculateVel2(Nao::JID_RARM_3, 0.0,0.0, 40);
    	CalculateVel2(Nao::JID_LARM_4, 0.0,0.0, 40);  CalculateVel2(Nao::JID_RARM_4, 0.0,0.0, 40);
	CalculateVel2(Nao::JID_LLEG_1, 1.0,0.0, 40);  CalculateVel2(Nao::JID_RLEG_1, 1.0,0.0, 40);
    	CalculateVel2(Nao::JID_LLEG_2, 0.0,0.0, 40);  CalculateVel2(Nao::JID_RLEG_2, 0.0,0.0, 40);
   	CalculateVel2(Nao::JID_LLEG_3, 0.0,0.0, 40);  CalculateVel2(Nao::JID_RLEG_3, 0.0,0.0, 40);
    	CalculateVel2(Nao::JID_LLEG_4, 0.0,0.0, 40);  CalculateVel2(Nao::JID_RLEG_4, 0.0,0.0, 40);
	CalculateVel2(Nao::JID_LLEG_5, 0.0,0.0, 40);  CalculateVel2(Nao::JID_RLEG_5, 0.0,0.0, 40);
    	CalculateVel2(Nao::JID_LLEG_6, 0.0,0.0, 40);  CalculateVel2(Nao::JID_RLEG_6, 0.0,0.0, 40);
}
