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

#ifndef ACTIONREQUEST_H
#define ACTIONREQUEST_H

#include "Logger.h"
#include "geometry.h"
#include "vector.h"


/**@name walking max speeds
*@{
* @attention These values are implementation independent maximum speeds.
* They dont reflect speeds reached with current walking engine implementations.
*
* They should be used e.g. for the Action Tester and for the behavior to walk with
* maximum speed.
*/
/** the maximum forward speed for walk in m/s ,not the final and best result,by Neil*/
static const double walkMaxForwardSpeed = 0.6;
/** the maximum backward speed for walk in m/s ,not the final and best result,by Neil*/
static const double walkMaxBackwardSpeed = 0.6;
/** the maximum sideward left/right speed for walk in m/s ,not the final and best result,by Neil*/
static const double walkMaxLeftRightSpeed = 0.1;
/** the maximum rotation speed for walk in degree/s ,not the final and best result,by Neil*/
static const double walkMaxRotationSpeed = 10.0;
/**
*@} */

class SpecialActionRequest
{
public:
    /** ids for all special actions */
    enum SpecialActionID
    {
        kickWithLeft,
        kickWithRight,
        getUp,
        getUpFromBack,
		divingSaveLeft,
		divingSaveRight,
		init,
        numOfSpecialAction
    }specialActionType;

    SpecialActionRequest():specialActionType(kickWithLeft){}

    /** returns names for special action ids */
    static const char* getSpecialActionIDName(SpecialActionID id)
    {
        switch (id)
        {
        case kickWithLeft:
            return "kickWithLeft";
        case kickWithRight:
            return "kickWithRight";
        case getUp:
            return "getUp";
        case getUpFromBack:
            return "getUpFromBack";
		case divingSaveLeft:
			return "divingSaveLeft";
		case divingSaveRight:
			return "divingSaveRight";
		case init:
			return "init";
        default:
            return "Not have this Action,please edit ActionRequest::getSpecialActionName";
        }
    }

    /**
    * Returns the special action id for a name
    */
    static SpecialActionRequest::SpecialActionID getSpecialActionIDFromName(const char* name);
};

class WalkRequest
{
public:
    /** ids for all walking types */
    enum WalkType
    {
        normal,
        turnWithBall,
        turnKick,
        dash,
        debug,
        walkWithBall,
        numOfWalkType
    };

    WalkRequest():walkType(normal) {}

    /** walking type */
    WalkType walkType;

    /** walk parameters, in m/s / degree/s */
    //Pose2D walkParams;
    salt::Vector2f walkParams;
    double rotation;//by Neil,need another parameter for rotation

    /** returns names for walking types */
    static const char* getWalkTypeName(WalkType id)
    {
        switch (id)
        {
        case normal:
            return "normal";
        case turnWithBall:
            return "turnWithBall";
        case turnKick:
            return "turnKick";
        case dash:
            return "dash";
        case debug:
            return "debug";
        case walkWithBall:
            return "walkWithBall";
        default:
            return "Unknown action name, please edit ActionRequest::getWalkTypeName";
        }
    }
};

class KickRequest
{
public:
    /** ids for all walking types */
    enum KickType
    {	
        kickright,
        kickleft
    };

    KickRequest():kickType(kickright) {}

    /** kicking type */
    KickType kickType;

    /** kick force and direction */
    float Direction;
    float Force;
    int type;
    int cout;
    
    /** returns names for kicking types */
    static const char* getKickTypeName(KickType id)
    {
        switch (id)
        {
        case kickright:
            return "kickright";
        case kickleft:
            return "kickleft";
        default:
            return "Unknown action name, please edit ActionRequest::getWalkTypeName";
        }
    }
};

/**
* This describes the ActionRequest
*/
class ActionRequest
{
public:

    unsigned long frameNumber;

    void setFrameNumber(unsigned long frameNumber)
    {
        this->frameNumber = frameNumber;
    }


    /** ids for all action types */
    enum ActionID
    {
        stand,
        walk,
        kick,
        specialAction,
        getup,
        stop,
        numOfAction
    };

    /** returns names for action ids */
    static const char* getActionName(ActionID id)
    {
        switch (id)
        {
        case stand:
            return "stand";
        case walk:
            return "walk";
        case specialAction:
            return "specialAction";
        case getup:
            return "getup";
        case stop:
            return "stop";
	case kick:
		return "kick";

        default:
            return "Unknown action name, please edit ActionRequest::getActionName";
        }
    }

    /** action type */
    ActionID actionType;

    /** SpecialActionRequest */
    SpecialActionRequest specialActionRequest;

    WalkRequest walkRequest;
    
    KickRequest kickRequest;

    /** constructor, startup action defined here */
    ActionRequest():actionType(stand),stabilize(false),frameNumber(0) {}

    /** == operator */
    bool operator==(const ActionRequest& other) const
    {
        switch (actionType)
        {
        case stand:
        case stop:
        case getup:
	case kick:
	    return(   /*kickRequest.kick == other.kickRequest.kick &&*/
		      stabilize == other.stabilize &&
                      actionType == other.actionType &&
                      kickRequest.kickType == other.kickRequest.kickType);
        default:
            return (
                       actionType == other.actionType &&
                       stabilize == other.stabilize);
        case walk:
            return (walkRequest.walkParams == other.walkRequest.walkParams &&
                    stabilize == other.stabilize &&
                    actionType == other.actionType &&
                    walkRequest.walkType == other.walkRequest.walkType);
        case specialAction:
            return (
                       stabilize == other.stabilize &&
                       actionType == other.actionType &&
                       specialActionRequest.specialActionType == other.specialActionRequest.specialActionType);
        }
    }

    /** = operator */
    void operator=(const ActionRequest& other);

    /** determines if the action is being stabilized */
    bool stabilize;


    /** returns name of current action */
    const char* getActionName() const
    {
        return getActionName(actionType);
    };

    /** returns name of current special action if any */
    const char* getSpecialActionName() const
    {
        if (actionType == specialAction)
            return specialActionRequest.getSpecialActionIDName(specialActionRequest.specialActionType);
        else
            return "";
    }

    /** return name of current walking type if any */
    const char* getWalkTypeName() const
    {
        if (actionType == walk)
            return walkRequest.getWalkTypeName(walkRequest.walkType);
        else
            return "";
    }
    
    /** return name of current kicking type if any */
    const char* getKickTypeName() const
    {
        if (actionType == kick)
            return kickRequest.getKickTypeName(kickRequest.kickType);
        else
            return "";
    }

    /**
    * Prints the action request to a readable string. (E.g. "NormalWalk 100,0,0")
    * @param destination The string to fill
    */
    void printOut(char* destination) const;

};

#endif
