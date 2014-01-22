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

#ifndef WALKINGENGINE_H
#define WALKINGENGINE_H

#include "Module.h"
#include "ActionRequest.h"
#include <boost/shared_array.hpp>
#include "worldmodel.h"
#include "ActionInfo.h"



/**
* @class WalkingEngineInterfaces
*
* The interfaces of the module WalkingEngine
*/
class WalkingEngineInterfaces
{
public:
  /** Constructor */
  WalkingEngineInterfaces(
    	const unsigned long& walkParameterTimeStamp,
    	const bool& receivedNewSensorData,
    	const ActionRequest::ActionID& lastActionType,
    	ActionInfo& actionInfo,
	boost::shared_array<float> JointAngle,
	boost::shared_array<float> JointVel)
    	:walkParameterTimeStamp(walkParameterTimeStamp),
    	receivedNewSensorData(receivedNewSensorData),
        lastActionType(lastActionType),
        actionInfo(actionInfo),
	mJointAngle(JointAngle),
	mJointVel(JointVel)
  {}

protected:
  /** The sensor data buffer containing all joint angles and others */
  
  const unsigned long& walkParameterTimeStamp;

  /** Indicates that the Motion Process received a new SensorDataBuffer */
  const bool& receivedNewSensorData;

  /** The action type of the previous frame */
  const ActionRequest::ActionID& lastActionType;

  /** The body tilt to be estimated by the walking engine */
  ActionInfo& actionInfo;

    boost::shared_array<float> mJointAngle;

    boost::shared_array<float> mJointVel;

};

/**
* A generic class for walking engine modules.
*
* It generates JointData and OdometryData according to current MotionRequest
* executing walking type motions.
*/
class WalkingEngine : public Module, protected WalkingEngineInterfaces
{
public:
/*
* Constructor.
* @param interfaces The paramters of the WalkingEngine module.
  */
  WalkingEngine(const WalkingEngineInterfaces& interfaces) : WalkingEngineInterfaces(interfaces){}
  
  /**
  * Calculates the next joint data set
  * @param jointData stores calculated frame
  * @param walkRequest The current motion request 
  * @param positionInWalkingCycle A request position for the walkcycle.
  * @return true if next set should be calculated by WalkingEngine
  *         false if change to other module can occur
  */
  	virtual bool executeParameterized( const WalkRequest& walkRequest, double positionInWalkingCycle) = 0;
  
  /** Destructor */
  	virtual ~WalkingEngine() {}
  
private:
  /** That function is not used but has to be implemented */
  	virtual void execute() {};
	virtual std::string GetActionCommand() {return "";};
};

#endif //__WalkingEngine_h_
