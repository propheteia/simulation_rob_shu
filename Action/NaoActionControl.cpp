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

#include "ModuleHandler.h"
#include "NaoActionControl.h"
#include "WalkingEngineSelector.h"
#include "Logger.h"
#include "geometry.h"
#include "robot.h"
#include "nao.h"

using namespace std;
using namespace salt;
using namespace boost;


NaoActionControl::NaoActionControl(ModuleHandler& moduleHandler,const ActionControlInterfaces& interfaces)
        : ActionControl(interfaces), moduleHandler(moduleHandler),
        changeOfActionControlStatePossible(true),/*headFlag1(0),headFlag2(0),*/
        lastActionType(ActionRequest::stand),
        actionControlState(NaoActionControl::stand)
{
    Init();
    /** create an uninitialised action request to set startup action -- something like getup (see ActionRequest-Constructor)*/
    //ActionRequest defaultRequest;

    WalkingEngineInterfaces walkingEngineInterfaces( walkParameterTimeStamp, receivedNewSensorData, lastActionType,actionInfo,mJointAngle,mJointVel);


    WalkingEngineSelector* tmp = new WalkingEngineSelector(moduleHandler, SolutionRequest::walkingEngine,walkingEngineInterfaces);
    pWalkingEngine[WalkRequest::normal] = tmp;


    /**now only have the normal walking,by Neil -2009-8*/
    /**pWalkingEngine[WalkRequest::turnWithBall] =
      	new ParamInvKinWalkingEngine(new TurnWithBallWalkingParameters, pInvKinWalkingEngine);

    pWalkingEngine[WalkRequest::turnKick] =
      	new ParamInvKinWalkingEngine(new TurnWithBallWalkingParameters, pInvKinWalkingEngine);

    pWalkingEngine[WalkRequest::dash] = tmp;
    pWalkingEngine[WalkRequest::debug] =
      	new ParamInvKinWalkingEngine(new ERS7EvolveWalkingParameters, pInvKinWalkingEngine);

    pWalkingEngine[WalkRequest::boost] = tmp;

    pWalkingEngine[WalkRequest::walkWithBall] = tmp;
    */

    SpecialActionInterfaces specialActionInterfaces(lastActionType,actionRequest,actionInfo,mJointAngle,mJointVel);
    pSpecialAction = new SpecialActionSelector(moduleHandler,specialActionInterfaces);

    GetupEngineInterfaces getupEngineInterfaces(lastActionType,actionRequest,actionInfo,mJointAngle,mJointVel);
    pGetupEngine = new GetupEngineSelector(moduleHandler,getupEngineInterfaces);

    KickEngineInterfaces kickEngineInerfaces(lastActionType,actionRequest,actionInfo,mJointAngle,mJointVel);
    pKickEngine = new KickEngineSelector(moduleHandler,kickEngineInerfaces);

}

NaoActionControl::~NaoActionControl()
{
    delete pWalkingEngine[WalkRequest::normal];
    /**delete pWalkingEngine[WalkRequest::turnWithBall];
    delete pWalkingEngine[WalkRequest::turnKick];
    delete pWalkingEngine[WalkRequest::dash];
    delete pWalkingEngine[WalkRequest::debug];
    delete pWalkingEngine[WalkRequest::walkWithBall]
    delete pSpecialActions;*/
    delete pGetupEngine;
    delete pKickEngine;
    delete pSpecialAction;

    moduleHandler.setModuleSelector(SolutionRequest::walkingEngine,0);
    //moduleHandler.setModuleSelector(SolutionRequest::getupEngine,0);
}

void NaoActionControl::Init()
{
    mJointAngle.reset(new float[NAO->GetJointMax() + 1]);
    mJointVel.reset(new float[NAO->GetJointMax() + 1]);
}



void NaoActionControl::execute()
{
    Update();
    DoAction();
    GeneralActionCommand();
}

void NaoActionControl::Update()
{
    const shared_array<Robot::Link> link = NAO->GetLink();
    if (link.get() != 0)
    {
        for (int i = NAO->GetJointMin(); i <= NAO->GetJointMax(); ++i)
        {
            //cerr << "ActionControl::Update----" << "i:" << i << endl;
            if (i == Robot::JID_ROOT) continue;
            /** NOTE if the link is the top one or not been
            * set up correctly, it will not be updated */
            // if (link[i].mother == 0) continue;
            mJointAngle[i] = link[i].q;
        }
    }

    for (int i = NAO->GetJointMin(); i <= NAO->GetJointMax(); ++i)
    {
        mJointVel[i] = 0.0f;
    }
}

void NaoActionControl::GeneralActionCommand()
{
    stringstream ss;

    const shared_array<Robot::Link> link = NAO->GetLink();
    if (link.get() != 0)
    {
        for (int i = NAO->GetJointMin(); i <= NAO->GetJointMax(); ++i)
        {
            if (i == Robot::JID_ROOT) continue;
            /** NOTE if the link is the top one or not been set
             * up correctly, its joint velocity will not be sent */
            // if (link[i].mother == 0) continue;
            int twin = link[i].twin;
            if (twin == 0) // hinge joint effector
            {
                ss << "(" << link[i].eff_name;
                ss << " " << Precision(mJointVel[i]) << ")";
            }
            else // universal joint effector
            {
                int child = link[i].child;
                if (twin == child) // first twin
                {
                    ss << "(" << link[i].eff_name;
                    ss << " " << Precision(mJointVel[i]);
                    ss << " " << Precision(mJointVel[twin]) << ")";
                }
            }
        }
    }
    mActionCommand = ss.str();
    aLOG << mActionCommand << endl;
    aLOG << "****************************************the end of send message****************************************" << endl;
}

void NaoActionControl::determineActionControlState()
{
    aLOG << "Current Action State :" << (int)actionControlState << endl;
    switch (actionControlState)
    {
    case NaoActionControl::getup:
        if (changeOfActionControlStatePossible)
        {
            setActionControlState();

        }
        return;
    case NaoActionControl::stand:
        if (changeOfActionControlStatePossible)
        {
            setActionControlState();
        }
        return;
    case NaoActionControl::specialAction:
        if (changeOfActionControlStatePossible)
        {
            setActionControlState();
        }
        return;
    case NaoActionControl::walk:
        if (changeOfActionControlStatePossible /**&&
      		     ((actionRequest.actionType != ActionRequest::specialAction || pSpecialActions->specialActionIsExecutableInWalkingCycle(actionRequest.specialActionRequest.specialActionType, positionInWalkCycle)))*/)
        {
            setActionControlState();
        }
        return;
    case NaoActionControl::stop:
        if ( changeOfActionControlStatePossible )
        {
            setActionControlState();
        }
        return;
    case NaoActionControl::kick:
        if ( changeOfActionControlStatePossible )
        {
            setActionControlState();
        }
        return;
    }
}

void NaoActionControl::setActionControlState()
{
    aLOG << "Action Type :" << actionRequest.getActionName() << endl;
    switch (actionRequest.actionType)
    {
    case ActionRequest::getup:
        actionControlState = NaoActionControl::getup;
        return;
    case ActionRequest::stand:
        actionControlState = NaoActionControl::stand;
        return;
    case ActionRequest::specialAction:
        actionControlState = NaoActionControl::specialAction;
        return;
    case ActionRequest::walk:
        actionControlState = NaoActionControl::walk;
        return;
    case ActionRequest::stop:
        actionControlState = NaoActionControl::stop;
        return;
    case ActionRequest::kick:
        actionControlState = NaoActionControl::kick;
        return;
    }
}
void NaoActionControl::DoAction()
{
    // remember some variables
    if (actionRequest.actionType == ActionRequest::walk)
        latestWalkRequest = actionRequest.walkRequest;
    /**else if(actionRequest.actionType == ActionRequest::specialAction)
      	latestSpecialActionRequest = actionRequest.specialActionRequest;*/

    // determine current state for Action Control
    determineActionControlState();

    /* state machine for current action*/
    switch (actionControlState)
    {
    case NaoActionControl::getup:
    {
        actionInfo.stopWalking = false;
        changeOfActionControlStatePossible = !pGetupEngine->executeParameterized();
        lastActionType = ActionRequest::getup;
        break;
    }
    case NaoActionControl::kick:
    {
        actionInfo.stopWalking = false;
        changeOfActionControlStatePossible = !pKickEngine->executeParameterized();
        lastActionType = ActionRequest::kick;
        break;
    }
    case NaoActionControl::stand:
    {
        actionInfo.stopWalking = false;
        WalkRequest standRequest;
        standRequest.walkType = WalkRequest::normal;
        standRequest.walkParams = Vector2f(0.01f,0.0f);
        changeOfActionControlStatePossible
        = !pWalkingEngine[WalkRequest::normal]->executeParameterized( standRequest,0.0 );
        actionInfo.executedActionRequest.actionType = ActionRequest::stand;
        positionInWalkCycle = actionInfo.positionInWalkCycle;
        lastActionType = ActionRequest::stand;
        break;
    }

    case NaoActionControl::specialAction:
    {	
        actionInfo.stopWalking = false;
        changeOfActionControlStatePossible = !pSpecialAction->executeParameterized();
        lastActionType = ActionRequest::specialAction;
        break;
    }
    case NaoActionControl::walk:
    {
        actionInfo.stopWalking = false;
        changeOfActionControlStatePossible
        = !pWalkingEngine[WalkRequest::normal]->executeParameterized( latestWalkRequest, positionInWalkCycle);
        positionInWalkCycle = actionInfo.positionInWalkCycle;
        lastActionType = ActionRequest::walk;
        break;
    }
    case NaoActionControl::stop:
    {
        actionInfo.stopWalking = true;
        changeOfActionControlStatePossible
        = !pWalkingEngine[WalkRequest::normal]->executeParameterized( latestWalkRequest, positionInWalkCycle);
        positionInWalkCycle = actionInfo.positionInWalkCycle;
        lastActionType = ActionRequest::stop;
    }
    }

    if (actionControlState != NaoActionControl::walk)
    {
        positionInWalkCycle = 0;
    }
    /* end of state machine*/

    /** execute HeadMotionRequest*/


    CalculateVel(Nao::JID_HEAD_2,headMotionRequest.yaw,40);

    aLOG << "Nao::NECK: " << gRadToDeg(mJointAngle[Nao::JID_HEAD_2]) << endl;

    //if( gAbs(gAbs(gRadToDeg(mJointAngle[Nao::JID_HEAD_1]))-45.0) < 0.001)
    //	CalculateVel(Nao::JID_HEAD_1,0.0,40);
    //else
    CalculateVel(Nao::JID_HEAD_1,headMotionRequest.pitch,40);

    aLOG << "Nao::HEAD: " << gRadToDeg(mJointAngle[Nao::JID_HEAD_1]) << endl;



    /*const long maxdiff=50000; //(~2pi/s)
    long diff;

    if (currentFrame.data[JointData::neckTilt] == jointDataInvalidValue)
    {
        diff=headMotionRequest.tilt-lastHeadTilt;
        if (diff<-maxdiff)
        {
            currentFrame.data[JointData::neckTilt] = lastHeadTilt-maxdiff;
        }
        else if (diff<maxdiff)
        {
            currentFrame.data[JointData::neckTilt] = headMotionRequest.tilt;
        }
        else
        {
            currentFrame.data[JointData::neckTilt] = lastHeadTilt+maxdiff;
        }
    }
    if (currentFrame.data[JointData::headPan] == jointDataInvalidValue)
    {
        diff=headMotionRequest.pan-lastHeadPan;
        if (diff<-maxdiff)
        {
            currentFrame.data[JointData::headPan] = lastHeadPan-maxdiff;
        }
        else if (diff<maxdiff)
        {
            currentFrame.data[JointData::headPan] = headMotionRequest.pan;
        }
        else
        {
            currentFrame.data[JointData::headPan] = lastHeadPan+maxdiff;
        }
    }



    lastHeadTilt=currentFrame.data[JointData::neckTilt];
    lastHeadPan =currentFrame.data[JointData::headPan];
    lastHeadRoll=currentFrame.data[JointData::headTilt];*/

    /** whether or not stabilize does something is determined in the method.
     it is called because of averaging that needs to be done continously!!*/
    //stabilize(lastActionType, actionRequest, currentFrame, odometryData, sensorDataBuffer);
}
void NaoActionControl::CalculateVel(JointID id, float angle, float maxVel)
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
    //aLOG << "minus: " << minus << "       vel: " << vel << endl;
    mJointVel[id] = vel;
}
