/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei   *
 *   zhuming535984@gmail.com,zhengyonglei@gmail.com   *
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

#include "BaseAgent.h"
#include <sstream>
#include "types.h"
#include "worldmodel.h"
#include "xmlParser.h"
#include <iostream>
#include "Utilities.h"

using namespace std;
using namespace salt;
using namespace boost;

BaseAgent::BaseAgent() : mCycle(0),saycycle(0)
{

    /**For Trainer-special action debug*/
    tc = new trainer_client();
    act=new action();
    da= new control();

    /**For behaviorControl and actionControl*/
    BehaviorControlInterfaces behaviorControlInterfaces(actionInfo,actionRequest,headMotionRequest,debug_msg);
    mBehaviorControl = new BehaviorControlSelector ( moduleHandler,behaviorControlInterfaces );

    /**For headControl*/
    HeadControlInterfaces headControlInterfaces(actionRequest,actionInfo,headControlMode,false,headMotionRequest);
    mHeadConrol = new HeadControlSelector (moduleHandler,headControlInterfaces );
    ActionControlInterfaces actionControlInterfaces(0.0,actionRequest,headMotionRequest,actionInfo,0.0,true);
    mActionControl = new ActionControlSelector( moduleHandler,actionControlInterfaces);
}

int BaseAgent::GetCycle() const
{
    return mCycle;
}

const string& BaseAgent::GetCommand() const
{
    return mCommand;
}

void BaseAgent::SetCommand(const string& command)
{
    mCommand = command;
}

string BaseAgent::InitRobot() const
{
    if (! WM.getSelf().Init())
    {
        cerr << "[Behavior] Self init failed!\n";
    }
    stringstream ss;
    ss << "(scene " << WM.getSelf().GetModelFileName() << ")";
    return ss.str();
}

string BaseAgent::InitAgent() const
{
    stringstream ss;
    ss << "(init (unum " << WM.getSelf().GetUnum() << ")";
    ss << "(teamname " << WM.getFieldInfo().our_teamname << "))";
    return ss.str();
}

string BaseAgent::beam() const
{
    if ( PM_BeforeKickOff == WM.getFieldInfo().play_mode)
    {
        if (WM.getSelf().GetUnum() > 0)
        {
            stringstream ss;
			string str;
            if ( WM.getFieldInfo().team_index == TI_LEFT )//SIDE_LEFT
            {
				stringstream xmlPath;
				xmlPath << "/Strive/TeamFormation/TmmStart/Player" << WM.getSelf().GetUnum();
                str = CONF.getXMLValue(xmlPath.str().c_str()).asString();
            }
            else
            {
				stringstream xmlPath;
				xmlPath << "/Strive/TeamFormation/OppStart/Player" << WM.getSelf().GetUnum();
                str = CONF.getXMLValue(xmlPath.str().c_str()).asString();
            }
			ss << "(beam " << str << ")";
            return ss.str();
        }
    }
    else if ( PM_Goal_Left == WM.getFieldInfo().play_mode)
    {
        if (WM.getSelf().GetUnum() > 0)
        {
            stringstream ss;
			string str;
            if ( WM.getFieldInfo().team_index == TI_LEFT )//SIDE_LEFT
            {
				stringstream xmlPath;
				xmlPath << "/Strive/TeamFormation/OppStart/Player" << WM.getSelf().GetUnum();
                str = CONF.getXMLValue(xmlPath.str().c_str()).asString();
            }
            else
            {
				stringstream xmlPath;
				xmlPath << "/Strive/TeamFormation/TmmStart/Player" << WM.getSelf().GetUnum();
                str = CONF.getXMLValue(xmlPath.str().c_str()).asString();
            }
			ss << "(beam " << str << ")";
            return ss.str();
        }
    }
    else
    {
        if (WM.getSelf().GetUnum() > 0)
        {
            stringstream ss;
			string str;
            if ( WM.getFieldInfo().team_index == TI_LEFT )//SIDE_LEFT
            {
				stringstream xmlPath;
				xmlPath << "/Strive/TeamFormation/TmmStart/Player" << WM.getSelf().GetUnum();
                str = CONF.getXMLValue(xmlPath.str().c_str()).asString();
            }
            else
            {
				stringstream xmlPath;
				xmlPath << "/Strive/TeamFormation/OppStart/Player" << WM.getSelf().GetUnum();
                str = CONF.getXMLValue(xmlPath.str().c_str()).asString();
            }
			ss << "(beam " << str << ")";
            return ss.str();
        }
    }
}


void BaseAgent::run()
{
    //start from the moment when agent is run
    ++mCycle;
    mCommand.clear();
    string msg;
    if (mCycle == 1)
    {
        mCommand = InitRobot();
        NET.PutMessage( mCommand );
    }
    else if (mCycle >1 && mCycle < 10)
    {
	//get name and id
        mCommand = InitAgent();
        NET.PutMessage( mCommand ) ;
        NET.GetMessage ( msg );
        WM.Update( msg );
    }
    else
    {
        while ( 1 )
        {
            ++mCycle;
            NET.GetMessage ( msg );
            NET.PutMessage( Think( msg ) );
        }
    }
}

void BaseAgent::beamAndInit()
{	
	mCommand = beam();
	actionRequest.actionType = ActionRequest::specialAction;
	actionRequest.specialActionRequest.specialActionType = SpecialActionRequest::init;
	mActionControl->execute();
	mCommand += mActionControl->command;
}

const string& BaseAgent::Think(const string& message)
{
 
    mCommand.clear();
    WM.Update(message);

    LOG << "SimulatorTime:" << WM.getMySimTime() << endl;
    LOG << "PlayModeNames:" << WM.getFieldInfo().play_mode << endl;

    switch (WM.getFieldInfo().play_mode)
    {
	    case PM_BeforeKickOff :
		LOG << "PlayModeNames: BEFORE_KICK_OFF"<< endl;
		beamAndInit();
		break;

	    case PM_KickOff_Left :
		LOG << "PlayModeNames: KICK_OFF_LEFT"<< endl;

	    case PM_KickOff_Right :
		LOG << "PlayModeNames: KICK_OFF_RIGHT"<< endl;
		kickOff();
		break;

	    case PM_PlayOn :
		LOG << "PlayModeNames: PLAY_ON"<< endl;
		//kickCycles start to count from PlayOn mode
		kickCycles = 0;
		playOn();
		break;

	    case PM_CORNER_KICK_LEFT:       
	    case PM_CORNER_KICK_RIGHT:
		cornerKick();
		break;
	    case PM_KickIn_Left :
	    case PM_KickIn_Right :
		kickIn();
		break;

	    case PM_GOAL_KICK_LEFT :
	    case PM_GOAL_KICK_RIGHT:
	      	goalKick();
	      	break;

	    case PM_Goal_Left :
	    case PM_Goal_Right :
		beamAndInit();
		break;

	    default:
		break;
    }

    CommunicateWithTeammate();
    return mCommand;
}

void BaseAgent::CommunicateWithTeammate()
{
    if ( 1 == WM.getSelf().GetUnum() )
    {
	// saycycle is 0 when agent is first created
	if(saycycle <=1)
	{
		saycycle++;
		mCommand += ReportTeamMatePosition(2);
	}
	else if(saycycle <= 3)
	{
		saycycle++;
		mCommand += ReportTeamMatePosition(3);
	}
	else if(saycycle <= 5)
	{
		saycycle++;
		mCommand += ReportTeamMatePosition(4);
	}
	else if(saycycle <= 7)
	{
		saycycle++;
		mCommand += ReportTeamMatePosition(5);
	}
	else if(saycycle <= 9)
	{
		saycycle++;
		mCommand += ReportTeamMatePosition(6);
	}
	else if(saycycle <= 11)
	{
		saycycle++;
		mCommand += ReportTeamMatePosition(7);
	}
	else if(saycycle <= 13)
	{
		saycycle++;
		mCommand += ReportTeamMatePosition(8);
	}
    else if(saycycle <= 15)
	{
		saycycle++;
		mCommand += ReportTeamMatePosition(9);
	}
    else if(saycycle <= 17)
	{
        aLOG<<"report 10"<<endl;
		saycycle++;
		mCommand += ReportTeamMatePosition(10);
	}
	else
	{
		saycycle++;
		mCommand += ReportTeamMatePosition(11);
		if(saycycle >= 20)
		saycycle = 0;
	}
    }

}

void BaseAgent::kickOff()
{
    if ( WM.getFieldInfo().team_index == TI_NONE )
        return;
    if (WM.getFieldInfo().play_mode == PM_KickOff_Left)
    {
        if ( WM.getFieldInfo().team_index == TI_LEFT )
        {
      	    if(WM.getSelf().pos.to2D().Length() < 0.18 && kickCycles < 10)
            {
		actionRequest.kickRequest.cout = 0 ;
		actionRequest.kickRequest.type = 2;
		actionRequest.kickRequest.Force=1.0;
		actionRequest.kickRequest.Direction = 0.0;
		actionRequest.actionType = ActionRequest::kick;	
		mActionControl->execute();
		mCommand = mActionControl->command;
	    }
	    else
	    {
		playOn();
		kickCycles++;
	    }
	}
        else
	{
	    actionRequest.actionType = ActionRequest::stand;
  	    mActionControl->execute();
	    mCommand = mActionControl->command;
	}
        return;
    }
    else if ( WM.getFieldInfo().play_mode == PM_KickOff_Right)
    {
        if (  WM.getFieldInfo().team_index == TI_RIGHT )
        {
	    if(WM.getSelf().pos.to2D().Length() < 0.18 && kickCycles < 10)
	    {
		actionRequest.kickRequest.cout = 0;
		actionRequest.kickRequest.type = 2;
		actionRequest.kickRequest.Force=1.0;
		actionRequest.kickRequest.Direction = 0.0;
		actionRequest.actionType = ActionRequest::kick;
		mActionControl->execute();
		mCommand = mActionControl->command;
	    }
	    else
	    {
  	    playOn();
	    kickCycles++;
   	    }
	}
        else
	{
	    actionRequest.actionType = ActionRequest::stand;
	    mActionControl->execute();
	    mCommand = mActionControl->command;
	}
        return;
    }
}

void BaseAgent::cornerKick()
{
    if ( WM.getFieldInfo().team_index == TI_NONE )
        return;
    if (WM.getFieldInfo().play_mode == PM_CORNER_KICK_LEFT)
    {
        if ( WM.getFieldInfo().team_index == TI_LEFT )//SIDE_LEFT
            playOn();
        else
            prepare();
        return;
    }
    else if ( WM.getFieldInfo().play_mode == PM_CORNER_KICK_RIGHT)
    {
        if (  WM.getFieldInfo().team_index == TI_RIGHT )
            playOn();
        else
            prepare();
        return;
    }
}

void BaseAgent::kickIn()
{
	if ( WM.getFieldInfo().team_index == TI_NONE )
        return;
    if (WM.getFieldInfo().play_mode == PM_KickIn_Left)
    {
        if ( WM.getFieldInfo().team_index == TI_LEFT )//SIDE_LEFT
            playOn();
        else
            prepare();
        return;
    }
    else if ( WM.getFieldInfo().play_mode == PM_KickIn_Right)
    {
        if (  WM.getFieldInfo().team_index == TI_RIGHT )
            playOn();
        else
            prepare();
        return;
    }
}

void BaseAgent::playOn()
{
    aLOG << "Self Pos: " << WM.getSelf().pos << endl;
    WM.Prepare = false;
    mBehaviorControl->execute();
    mHeadConrol->execute();
    mActionControl->execute();
    mCommand = mActionControl->command;
}

void BaseAgent::prepare()
{
    aLOG << "Self Pos: " << WM.getSelf().pos << endl;
    WM.Prepare = true;
    mBehaviorControl->execute();
    mHeadConrol->execute();
    mActionControl->execute();
    mCommand = mActionControl->command;
}

void BaseAgent::goalKick()
{
    if ( WM.getFieldInfo().team_index == TI_NONE )
        return;
    if (WM.getFieldInfo().play_mode == PM_GOAL_KICK_LEFT)
    {
        if ( WM.getFieldInfo().team_index == TI_LEFT )//SIDE_LEFT
            playOn();
        else
            prepare();
        return;
    }
    else if ( WM.getFieldInfo().play_mode == PM_GOAL_KICK_RIGHT)
    {
        if (  WM.getFieldInfo().team_index == TI_RIGHT )
            playOn();
        else
            prepare();
        return;
    }
}

const string BaseAgent::Say(const string& message)
{
    stringstream ss;
    ss << "(say " << message << ")" << endl;
    aLOG << ss.str() << endl;
    return ss.str();
}

const string BaseAgent::ReportTeamMatePosition(int number)
{
    stringstream ss;
    ss.clear();
    int index = number - 1;
    int forward1  = WM.getOurForward1().GetUnum();
    int forward2 = WM.getOurForward2().GetUnum();
    number += '0';
    forward1 += '0'; 
    forward2 += '0';
    if (WM.getTmm(index).isValid() && WM.SeeBall())
    {
		if(WM.getFieldInfo().team_index == TI_LEFT)
        {
        		ss << 'l' << static_cast<char>(number) << static_cast<char>(forward1) <<static_cast<char>(forward2);
        }
		else if(WM.getFieldInfo().team_index == TI_RIGHT)
        {
        		ss << 'r' << static_cast<char>(number) << static_cast<char>(forward1) <<static_cast<char>(forward2);
        }
      aLOG<<"Number"<<number<<endl;
        char bufX[5];
        char bufY[5];
		char ballX[5];
		char ballY[5];
        unsigned long ul;
		aLOG << "pos: " << WM.getTmm(index).pos << " ballpos: " << WM.getBall().pos << endl;
		ul = float2ul(WM.getTmm(index).pos.x());
        memcpy(bufX, &ul, sizeof(float));
        bufX[4] = '\0';
        ul = float2ul(WM.getTmm(index).pos.y());
        memcpy(bufY, &ul, sizeof(float));
        bufY[4] = '\0';
		ul = float2ul(WM.getBall().pos.x());
		memcpy(ballX, &ul, sizeof(float));
		ballX[4] = '\0';
		ul = float2ul(WM.getBall().pos.y());
		memcpy(ballY, &ul, sizeof(float));
		ballY[4] = '\0';
        ss << bufX << bufY << ballX << ballY;
	aLOG<<"Say Successful."<<endl;
        return Say(ss.str());
    }
    else
    {
        aLOG<<"Say Failed"<<endl;
        ss << "";
        return ss.str();
    }
}
