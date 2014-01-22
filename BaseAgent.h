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

#ifndef BASEAGENT_H
#define BASEAGENT_H

#include <string>
#include "AdvancedAgent.h"
#include "StaticModule.h"
#include "ActionSelector.h"
#include "BehaviorControlSelector.h"
#include "HeadControlSelector.h"
#include "ActionRequest.h"
#include "HeadMotionRequest.h"
#include "HeadControlMode.h"


#include "ModuleHandler.h"
#include "trainer_client.h"
#include "common.h"
#include "control.h"

class BaseAgent
{
public:
	BaseAgent();
	~BaseAgent(){}

	int GetCycle() const;
	std::string InitRobot() const;
	std::string InitAgent() const;
	std::string beam() const;
	void run();
	void SetCommand(const std::string& command);
	const std::string& GetCommand() const;
	const std::string& Think(const std::string& message);
protected:
	void beamAndInit();
	void CommunicateWithTeammate();
	void kickOff();
	void cornerKick();
	void kickIn();
	void playOn();
	void prepare();
	void goalKick();
	const std::string Say(const std::string& message);
	const std::string ReportTeamMatePosition(int index);
protected:
	/** a pointer to the MotionControl module */
  	ActionControlSelector *mActionControl;
	BehaviorControlSelector *mBehaviorControl;
	HeadControlSelector *mHeadConrol;
  	/** A module handler for managing runtime exchangeable modules */
 	ModuleHandler moduleHandler;
private:
	int		kickCycles;
	int		 mCycle;
	int		saycycle;
	std::string     mCommand;

	ActionRequest actionRequest;
	HeadControlMode headControlMode;
	HeadMotionRequest headMotionRequest ;
	ActionInfo actionInfo;
	
	string debug_msg;

	trainer_client *tc;
 	action *act;
	control *da;
};

#endif
