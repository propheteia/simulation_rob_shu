/*

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef NAOACTIONCONTROL_H
#define NAOACTIONCONTROL_H

#include "Module.h"
#include <boost/shared_array.hpp>
#include "types.h"
#include "Logger.h"
#include "ball.h"
#include "worldmodel.h"
#include "ActionControl.h"
#include "WalkingEngine.h"
#include "GetUpEngineSelector.h"
#include "KickEngineSelector.h"
#include "SpecialActionSelector.h"


class Robot;


class NaoActionControl : public ActionControl
{
public:
    NaoActionControl( ModuleHandler& moduleHandler, const ActionControlInterfaces& interfaces);
    ~NaoActionControl();

    void Init();

    virtual void execute();

    virtual std::string GetActionCommand()
    {
        return mActionCommand;
    }

protected:


    void Update();

    void DoAction();

    void GeneralActionCommand();

    /** determines the state of the ActionControl */
    void determineActionControlState();

    /** sets the ActionControlState accordingly */
    void setActionControlState();

    /** which module is currently used for calculation */
    // ActionRequest::ActionID currentActionType;

    /** which module was used before */
    ActionRequest::ActionID lastActionType;

    /** pointer to the WalkingEngine modules one for each walking type*/
    WalkingEngine *pWalkingEngine[WalkRequest::numOfWalkType];

    /** a pointer to the SpecialActions module */
    SpecialActionSelector *pSpecialAction;

    /** a pointer to the GetupEngine module */
    GetupEngineSelector *pGetupEngine;

    /** a pointer to the KickEngine module  */
	KickEngineSelector *pKickEngine;


    /**
    * A reference to the ModuleHandler of the Process.
    * Needed to create new solutions.
    */
    ModuleHandler& moduleHandler;

    /** the last executed Action indicated, that it�s possible to execute the motionRequest from Cognition */
    bool changeOfActionControlStatePossible;

    /** the last cycle we were in */
    double positionInWalkCycle;

    /** The latest WalkRequest */
    WalkRequest latestWalkRequest;

    /** The latest SpecialActionRequest */
    SpecialActionRequest latestSpecialActionRequest;

    /** The state of the ActionControl */
    enum
    {
		init,
        stand ,
        walk,
	    kick,
        specialAction,
        getup,
        stop
    } actionControlState;
    /** calculate velocity need achieve the desired angle
     *  for each Joint Angle according to JID
     */
    void CalculateVel(JointID id, float angle, float maxVel);


    boost::shared_array<float>  mJointAngle;

    boost::shared_array<float>  mJointVel;

    std::string mActionCommand;

    //int headFlag1;//Just test for headJoint//by Neil
    //int headFlag2;

};


#endif
