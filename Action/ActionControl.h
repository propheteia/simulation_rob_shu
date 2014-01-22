/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei  Qu Junjun *
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

#ifndef ACTION_H
#define ACTION_H

#include "ActionRequest.h"
#include "ActionInfo.h"
#include "HeadMotionRequest.h"
#include"Module.h"

/**
* @class ActionControlInterfaces
*
* The interfaces of the ActionControl module.
*/
class ActionControlInterfaces
{
public:
  /** Constructor */ 
  ActionControlInterfaces(const unsigned long& frameNumber,
                          const ActionRequest &actionRequest,
			  const HeadMotionRequest& headMotionRequest,
			  ActionInfo& actionInfo,
			  const unsigned long& walkParameterTimeStamp,
    			  const bool& receivedNewSensorData )
  			 :frameNumber(frameNumber),
   			  actionRequest(actionRequest), 
			  headMotionRequest(headMotionRequest),
			  actionInfo(actionInfo),
                          walkParameterTimeStamp(walkParameterTimeStamp),
			  receivedNewSensorData(receivedNewSensorData)
	{}

protected:  
  /** A reference to the frame number */
  const unsigned long& frameNumber;

  /** A request from the behavior control */  
  const ActionRequest& actionRequest;

  /** Head joint values from the head control */
  const HeadMotionRequest& headMotionRequest;

  const unsigned long& walkParameterTimeStamp;

  /** Indicates that the Action Process received a new SensorDataBuffer */
  const bool& receivedNewSensorData;

  /** The body tilt to be calculated */
  ActionInfo& actionInfo;

};



class ActionControl : public Module, public ActionControlInterfaces
{
public:
 	ActionControl(const ActionControlInterfaces& interfaces) : ActionControlInterfaces(interfaces) {}
  
  	/** Destructor */
    	~ActionControl() {}
	
};

#endif 