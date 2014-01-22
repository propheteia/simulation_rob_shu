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

#ifndef BEHAVIORCONTROL_H
#define BEHAVIORCONTROL_H

#include "Module.h"
#include "ActionInfo.h"
#include "ActionRequest.h"
#include "HeadMotionRequest.h"

/** The interfaces of the BehaviorControl module.*/
class BehaviorControlInterfaces
{
public:
  /** Constructor.*/
  	BehaviorControlInterfaces( const ActionInfo& actionInfo,
				   	ActionRequest& actionRequest,
					HeadMotionRequest& headMotionRequest,
                                   const string debug_msg)
				  : actionInfo(actionInfo),
				    actionRequest(actionRequest),
					headMotionRequest(headMotionRequest),
				    debug_msg(debug_msg)
	{}

protected:
  	/** The actions that are currently executed by the ActionControl */
  	const ActionInfo& actionInfo;

   	/** The action request to be set by the behavior */
  	ActionRequest& actionRequest;

	/** The head motion request to be set by behavior */
	HeadMotionRequest& headMotionRequest;

   	/** debug message for WalkpathPlanning */
	string debug_msg;

};


class BehaviorControl : public Module, public BehaviorControlInterfaces
{
public:
/** Constructor.
* @param interfaces The paramters of the BehaviorControl module.
  */
	BehaviorControl(const BehaviorControlInterfaces& interfaces): BehaviorControlInterfaces(interfaces) {}
	~BehaviorControl(){}

};



#endif