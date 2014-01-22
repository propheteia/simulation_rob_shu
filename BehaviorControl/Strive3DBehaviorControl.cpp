/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei,Qu Junjun   *
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

#include "Strive3DBehaviorControl.h"
#include "gmath.h"
#include "ball.h"
#include "fieldinfo.h"
#include "worldmodel.h"
#include "Logger.h"
Strive3DBehaviorControl::Strive3DBehaviorControl(ModuleHandler& moduleHandler,const BehaviorControlInterfaces& interfaces)
        : BehaviorControl(interfaces), moduleHandler(moduleHandler)
{
}

// Move according to walking parameters
void Strive3DBehaviorControl::Debug(double walkparams_x,double walkparams_y,double rotation)
{
    actionRequest.actionType = ActionRequest::walk;
    actionRequest.walkRequest.walkParams.x() = walkparams_x;
    actionRequest.walkRequest.walkParams.y() = walkparams_y;
    actionRequest.walkRequest.rotation = rotation;
}

void Strive3DBehaviorControl::MoveToDest(Vector2f dest,double speedfactor)
{
    //If I'm close to destination, move to the destination without turn.
    
}

void Strive3DBehaviorControl::TurnBeforeMoveToDest(Vector2f dest,double speedfactor)
{
    //If I'm far away from destination, turn to destination and then move forward.
}

void Strive3DBehaviorControl::TurnAndMoveToDest(Vector2f dest,double speedfactor)
{
    //If I'm close to destination, move to the destination directly.If I'm far away from it,turn to destination and then move forward.
}

void Strive3DBehaviorControl::FineTune(Vector2f dest,double angle)
{
    //Get to destination and turn to demanding angle Precisely.
}


//
void Strive3DBehaviorControl::GoToPoint(Vector2f dest, double speedfactor)
{
    double maxSpeed = 0.8f;
    double maxSpeedY = 0.2f;
    double maxTurnSpeed = 6.0;
    Vector3f agentWorldPos = WM.getSelf().pos;
    Vector2f destinationToAgent = dest - agentWorldPos.to2D();
    double distanceToDestination = destinationToAgent.Length();
    bLOG << "destinationToAgent: " << destinationToAgent << "\tdistanceToDestination: " << distanceToDestination << endl;
    bLOG << "agentWorldPos: " << agentWorldPos << "\tdest: " << dest << endl;
    
    double angleToDestination = gNormalizeDeg( destinationToAgent.GetAngleDeg()-WM.getSelf().GetTorsoYawAngle());
    Vector2f destination( distanceToDestination * gCos(gDegToRad(angleToDestination)),distanceToDestination * gSin(gDegToRad(angleToDestination)) );
    bLOG << "destination: " << destination << endl;
    float factorClipping = 1.0; // full speed

    if (distanceToDestination < 0.5)
    {
        factorClipping = 0.9; // smoother approach
    }

    if (angleToDestination >90.0)
    {
        angleToDestination -= 180.0;
    }
    else if (angleToDestination < -90.0)
    {
        angleToDestination += 180.0;
    }

    double factor = (90-fabs(angleToDestination))/90;
    if (factor > factorClipping) factor = factorClipping;
    if (factor < 0) factor = 0;

    destination.Normalized();
	 destination *= (speedfactor * maxSpeed*factor);

    actionRequest.actionType = ActionRequest::walk;
    actionRequest.walkRequest.walkParams = destination;
    bLOG << "walkParams1: " << actionRequest.walkRequest.walkParams << endl;
    actionRequest.walkRequest.walkParams.x() *= 0.2;
    actionRequest.walkRequest.walkParams.y() *=-0.2;
    bLOG << "TorsoYawAngle: " << WM.getSelf().GetTorsoYawAngle() << endl;
    bLOG << "angleToDestination : " << angleToDestination << endl;
    bLOG << "walkParams: " << actionRequest.walkRequest.walkParams << endl;
    actionRequest.walkRequest.walkParams.x() = gMin(actionRequest.walkRequest.walkParams.x(),0.10f);
    actionRequest.walkRequest.walkParams.x() = gMax(actionRequest.walkRequest.walkParams.x(),-0.10f);

    actionRequest.walkRequest.walkParams.y() = gMin(actionRequest.walkRequest.walkParams.y(),0.05f);
    actionRequest.walkRequest.walkParams.y() = gMax(actionRequest.walkRequest.walkParams.y(),-0.05f);
    bLOG << "walkParams: " << actionRequest.walkRequest.walkParams << endl;
    actionRequest.walkRequest.rotation = angleToDestination;
    bLOG << "rotation: " << actionRequest.walkRequest.rotation << endl;
    // clip rotation speed
    actionRequest.walkRequest.rotation = gMin(actionRequest.walkRequest.rotation, maxTurnSpeed);
    actionRequest.walkRequest.rotation = gMax(actionRequest.walkRequest.rotation, -maxTurnSpeed);
    bLOG << "rotation: " << actionRequest.walkRequest.rotation << endl;
//     }
}

void Strive3DBehaviorControl::GoToRelativePoint(Vector2f Destination)
{
    double maxSpeed = 0.8f;
    double maxSpeedY = 0.2f;
    double maxTurnSpeed = 6;

    Vector2f Destination2D(Destination.x(), Destination.y());
    double angleToDestination = gRadToDeg(gArcTan2(Destination.y(),Destination.x()));
    double distanceToDestination = Destination2D.Length();

    bLOG <<"distanceToDestination : "<< distanceToDestination <<endl
    << "angleToDestination : " << angleToDestination << endl;

    float factorClipping = 1.0; // full speed
    if (distanceToDestination < 0.5)
    {
        factorClipping = 0.9; // smoother approach
    }
    double factor = (180-fabs(angleToDestination))/180;
    if (factor > factorClipping) factor = factorClipping;
    if (factor < 0) factor = 0;

    Destination2D.Normalized();
    Destination2D *= (1.3 * maxSpeed*factor);
    actionRequest.actionType = ActionRequest::walk;
    actionRequest.walkRequest.walkParams = Destination2D;

    actionRequest.walkRequest.walkParams.x() *= 0.2;
    actionRequest.walkRequest.walkParams.y() *=-0.2;

    bLOG << "angleToDestination : " << angleToDestination << endl;
    if ( angleToDestination > 40.0 || angleToDestination < -40.0)
    {
        actionRequest.walkRequest.walkParams.x() = gMin(actionRequest.walkRequest.walkParams.x(),0.015f);
        actionRequest.walkRequest.walkParams.x() = gMax(actionRequest.walkRequest.walkParams.x(),-0.015f);

        actionRequest.walkRequest.walkParams.y() = gMin(actionRequest.walkRequest.walkParams.y(),0.05f);
        actionRequest.walkRequest.walkParams.y() = gMax(actionRequest.walkRequest.walkParams.y(),-0.05f);

        actionRequest.walkRequest.rotation = 2*angleToDestination;
        // clip rotation speed
        actionRequest.walkRequest.rotation = gMin(actionRequest.walkRequest.rotation, maxTurnSpeed*2);
        actionRequest.walkRequest.rotation = gMax(actionRequest.walkRequest.rotation, -maxTurnSpeed*2);
    }
    else
    {
        //test not need turn so much,just walk formward or backward

        actionRequest.walkRequest.walkParams.x() = gMin(actionRequest.walkRequest.walkParams.x(),0.10f);
        actionRequest.walkRequest.walkParams.x() = gMax(actionRequest.walkRequest.walkParams.x(),-0.10f);

        actionRequest.walkRequest.walkParams.y() = gMin(actionRequest.walkRequest.walkParams.y(),0.05f);
        actionRequest.walkRequest.walkParams.y() = gMax(actionRequest.walkRequest.walkParams.y(),-0.05f);

        actionRequest.walkRequest.rotation = angleToDestination*1.3;
        // clip rotation speed
        actionRequest.walkRequest.rotation = gMin(actionRequest.walkRequest.rotation, maxTurnSpeed);
        actionRequest.walkRequest.rotation = gMax(actionRequest.walkRequest.rotation, -maxTurnSpeed);
    }
	
	if(actionRequest.walkRequest.walkParams.Length() <  0.05)
		actionRequest.walkRequest.walkParams = actionRequest.walkRequest.walkParams.Normalized() * 0.05;

	bLOG << "walkParams: " << actionRequest.walkRequest.walkParams << endl;
    bLOG << "Rotation :" << actionRequest.walkRequest.rotation << endl;
}

void Strive3DBehaviorControl::Stand()
{
	bLOG << "Stand State!" << endl;
    actionRequest.walkRequest.walkParams.x() = 0.0;
    actionRequest.walkRequest.walkParams.y() = 0.0;
    actionRequest.walkRequest.rotation = 0.0;
    actionRequest.actionType = ActionRequest::stand;
}

void Strive3DBehaviorControl::TurnTo(Vector2f dest, double angle)
{
    double maxSpeed = 0.8f;
    double maxSpeedY = 0.8f;
    double maxTurnSpeed = 10;

    Vector3f agentWorldPos = WM.getSelf().pos;
    Vector2f destinationToAgent = dest - agentWorldPos.to2D();
    double distanceToDestination = destinationToAgent.Length();
    bLOG << "destinationToAgent: " << destinationToAgent << "\tdistanceToDestination: " << distanceToDestination << endl;
    bLOG << "agentWorldPos: " << agentWorldPos << "\tdest: " << dest << endl;
    double angleToDestination = gNormalizeDeg( destinationToAgent.GetAngleDeg()-WM.getSelf().GetTorsoYawAngle());
    if (distanceToDestination < 0.16)
        distanceToDestination = 0.16;
    Vector2f destination( distanceToDestination * gCos(gDegToRad(angleToDestination)),distanceToDestination * gSin(gDegToRad(angleToDestination)) );
    bLOG << "destination: " << destination << endl;
    float factorClipping = 1.0; // full speed
    if (distanceToDestination < 0.5)
    {
        factorClipping = 0.9; // smoother approach
    }

    if (angleToDestination >90.0)
    {
        angleToDestination -= 180.0;
    }
    else if (angleToDestination < -90.0)
    {
        angleToDestination += 180.0;
    }

    double factor = (90-fabs(angleToDestination))/90;
    if (factor > factorClipping) factor = factorClipping;
    if (factor < 0) factor = 0;
	if(fabs(angleToDestination) > 60 && fabs(angleToDestination) < 120)
		factor = 0.5;
    destination.Normalized();
    destination *= (0.4 * maxSpeed*factor);
//        destination *= (0.5 * maxSpeed*factor);

    actionRequest.actionType = ActionRequest::walk;
    //motionRequest.motionType = MotionRequest::walk;
    //motionRequest.walkRequest.walkType = static_cast<WalkRequest::WalkType>(static_cast<int>(walkType));
    actionRequest.walkRequest.walkParams = destination;
    bLOG << "walkParams1: " << actionRequest.walkRequest.walkParams << endl;
    actionRequest.walkRequest.walkParams.x() *= 1.2;
    actionRequest.walkRequest.walkParams.y() *=-1.2;
    bLOG << "TorsoYawAngle: " << WM.getSelf().GetTorsoYawAngle() << endl;
    bLOG << "angleToDestination : " << angleToDestination << endl;
    /**if ( angleToDestination > 40.0 || angleToDestination < -40.0)
    {
        actionRequest.walkRequest.walkParams.x() = gMin(actionRequest.walkRequest.walkParams.x(),0.01f);
        actionRequest.walkRequest.walkParams.x() = gMax(actionRequest.walkRequest.walkParams.x(),-0.01f);

        actionRequest.walkRequest.walkParams.y() = gMin(actionRequest.walkRequest.walkParams.y(),0.01f);
        actionRequest.walkRequest.walkParams.y() = gMax(actionRequest.walkRequest.walkParams.y(),-0.01f);

    actionRequest.walkRequest.rotation = 2*angleToDestination;
        // clip rotation speed
        actionRequest.walkRequest.rotation = gMin(actionRequest.walkRequest.rotation, maxTurnSpeed);
        actionRequest.walkRequest.rotation = gMax(actionRequest.walkRequest.rotation, -maxTurnSpeed);
    }
    else
    {*/
    //test not need turn so much,just walk formward or backward
    bLOG << "walkParams: " << actionRequest.walkRequest.walkParams << endl;
    actionRequest.walkRequest.walkParams.x() = gMin(actionRequest.walkRequest.walkParams.x(),0.10f);
    actionRequest.walkRequest.walkParams.x() = gMax(actionRequest.walkRequest.walkParams.x(),-0.10f);

    actionRequest.walkRequest.walkParams.y() = gMin(actionRequest.walkRequest.walkParams.y(),0.05f);
    actionRequest.walkRequest.walkParams.y() = gMax(actionRequest.walkRequest.walkParams.y(),-0.05f);
    bLOG << "walkParams: " << actionRequest.walkRequest.walkParams << endl;
    actionRequest.walkRequest.rotation = 0.3 * gNormalizeDeg(angle - WM.getSelf().GetTorsoYawAngle());
    bLOG << "rotation: " << actionRequest.walkRequest.rotation << endl;
    // clip rotation speed
    actionRequest.walkRequest.rotation = gMin(actionRequest.walkRequest.rotation, maxTurnSpeed);
    actionRequest.walkRequest.rotation = gMax(actionRequest.walkRequest.rotation, -maxTurnSpeed);
    bLOG << "rotation: " << actionRequest.walkRequest.rotation << endl;
    //}
}

bool Strive3DBehaviorControl::GoToBallToTargetLine()
{
    bLOG << "In GoToBallToTargetLine " << endl;
    Vector2f ball(WM.getBall().pos.x(), WM.getBall().pos.y());
    Vector2f tmpvect = WM.getSelf().pos.y() > ball.y() ? Vector2f(-0.4, 0.4) : Vector2f(-0.4, -0.4);
//     Vector2f ball(WM.getBall().GetBallLocalPos().x(), WM.getBall().GetBallLocalPos().y());
//    if (WM.SeeBall())
// 	Vector2f tmpvect = WM.getSelf().pos.y() > ball.y() ? Vector2f(-0.2, 0.01) : Vector2f(-0.2, -0.01);
//    else
// 	Vector2f tmpvect = WM.getSelf().pos.y() > ball.y() ? Vector2f(-0.4, 0.4) : Vector2f(-0.4, -0.4);
    tmpvect = ball + tmpvect;
    Vector2f tmptoself = WM.getSelf().pos.to2D() - tmpvect;
    if (tmptoself.Length() > 0.1)
    {
	    GoToPoint(tmpvect, 1.4);
        mTmpPosVect.clear();
        mTmpPosVect.push_back(tmpvect);
        return false;
    }
    else
    {
// 		Stop();
        return true;
    }
}

bool Strive3DBehaviorControl::GoToPointAndTurnTo(salt::Vector2f dest, double angle)
{
    Vector2f toDestination = dest - WM.getSelf().pos.to2D();
    Vector2f self = WM.getSelf().pos.to2D();
    Vector3f ball = WM.getBall().GetLocalPos();

    double distanceToDestination = toDestination.Length();
    double angleToDestination = gNormalizeDeg(angle-WM.getSelf().GetTorsoYawAngle());
    bLOG << "dest: " << dest << "\tangle: " << angle << endl;
    bLOG << "AgentPos: " << WM.getSelf().pos.to2D() << "\tAgentAngle: " << WM.getSelf().GetTorsoYawAngle() << endl;
    bLOG << "toDestination: " << toDestination
    << "\tdistanceToDestination: " << distanceToDestination
    << "\tangleToDestination: " << angleToDestination << endl;
    static int phase = 0;
    switch (phase)
    {
    case 0:
        if (distanceToDestination > 0.5)
        {
            if (distanceToDestination > 0.8)
                ObstacleAvoidanceTo(dest);
            else
                GoToPoint(dest, 1.4);
            phase = 0;
            bLOG << "Phase GoToPoint: " << distanceToDestination << endl;
        }
        else
            phase = 1;
        break;
    case 1:
        if (distanceToDestination <= 0.5)
        {
             if ((gAbs(angleToDestination) > 20) || (distanceToDestination > 0.035))
            //if ((gAbs(angleToDestination) > 3) || (self.x() > ball.x() - 0.01 && WM.SeeBall() == false ) || ( (self.y() > ball.y() - 0.01 ||self.y() < ball.y() + 0.01) && WM.SeeBall() == false))
            {
                bLOG << "Phase TurnTo!" << endl;
                TurnTo(dest, angle);
                phase = 1;
            }
            else
                phase = 2;
        }
        else
            phase = 0;
        break;
    case 2:
        if (distanceToDestination < 0.1)
        {
            bLOG << "Phase Stop!" << endl;	
// 			Stand();
            phase = 2;
        }
        else
            phase = 0;
        break;
    }
    if (phase < 2)
        return false;
    else
        return true;

}

void Strive3DBehaviorControl::DribbleTo(salt::Vector2f dest)
{

    Vector2f ball = WM.getBall().pos.to2D();
    Vector2f agent = WM.getSelf().pos.to2D();
    Vector2f toball = ball - agent;
    Vector3f localball = WM.getBall().GetLocalPos();
    Vector2f balltogoalin = localball.to2D() - WM.getFieldInfo().oppgoalcenterlocal.to2D();


     // double distancetoball = WM.getBall().GetBallDistance()*gCos(gDegToRad(WM.getBall().GetBallPhi()));
      double distancetoball = (WM.getBall().pos.to2D() - WM.getSelf().pos.to2D()).Length();
      //cout <<"#######################BALL"<<endl;
      //cout<<distancetoball<<endl<<distancetoball2<<endl;
//       double distancetoball = gSqrt(toball.x() * toball.x() + toball.y() * toball.y());

    static bool phase = 0;
    bLOG << "*****************************************DribbleTo****************************************" << endl;
    bLOG << "ball: " << ball << "\t agent: " << agent << "\t toball: " << toball << endl;
    bLOG << "distancetoball: " << distancetoball << endl;

    switch (phase)
    {
    case 0:
        if (GoToBallForShoot(dest) == false)
            phase = 0;
        else
        {
			phase = 1;
	}
        break;
    case 1:
        if (WM.getSelf().pos.x() - WM.getBall().pos.x() < 0.02 && distancetoball < 0.2)
        {
//             GoToPoint(dest, 1.8);
            GoToPoint(dest, 1.4);
            phase = 1;
        }
        else
            phase = 0;
        break;
    }
}

bool Strive3DBehaviorControl::AdjustmentForKick(Vector2f dest)
{
// 	double dist = 0.12;
	double dist = 0.145;
	int cout = 0 ;
    Vector3f LeftFoot = WM.getSelf().GetLeftFootGloblePosition();
    Vector3f RightFoot = WM.getSelf().GetRightFootGloblePosition();
    Vector3f Foot = WM.getSelf().pos;
    Vector2f ball = WM.getBall().pos.to2D();
    Vector2f desttoball = ball - dest;
    Vector3f LeftFootloc = WM.getSelf().GetLeftFootLocalPosition();
    Vector3f RightFootloc = WM.getSelf().GetRightFootLocalPosition();
    Vector3f ballloc = WM.getBall().GetLocalPos();
	Vector2f tmp(dist * gCos(desttoball.GetAngleRad()), dist * gSin(desttoball.GetAngleRad()));
	double angle = desttoball.GetAngleDeg() - 180.0;
	tmp = ball + tmp;

//	Vector2f agenttodesttoleft = tmp - LeftFoot.to2D();
//	Vector2f agenttodesttoright = tmp - RightFoot.to2D();
	Vector2f agenttodest = tmp - Foot.to2D();

	
    double angleToDestination = gNormalizeDeg( agenttodest.GetAngleDeg()-WM.getSelf().GetTorsoYawAngle());
    double factor = 0.08;
    Vector2f destination( factor * gCos(gDegToRad(angleToDestination)),factor * gSin(gDegToRad(angleToDestination)) );
	actionRequest.walkRequest.walkParams = destination * 0.3;
	actionRequest.walkRequest.walkParams.y() *= -1;
	actionRequest.walkRequest.rotation = gMax(0.3 * gNormalizeDeg(angle - WM.getSelf().GetTorsoYawAngle()), -7.0);
	actionRequest.walkRequest.rotation = gMin(actionRequest.walkRequest.rotation, 7.0);
	actionRequest.actionType = ActionRequest::walk;
	bLOG << "**********************AdjustmentForKick***************************" << endl;
	bLOG << "LeftFoot: " << LeftFoot << "\tball: " << ball << "\tdesttoball: " << desttoball << endl;
	bLOG << "RightFoot: " << RightFoot << "\tball: " << ball << "\tdesttoball: " << desttoball << endl;
	bLOG << "walkParams: " << actionRequest.walkRequest.walkParams << "\trotation: " << actionRequest.walkRequest.rotation << endl;
	bLOG << "**********************AdjustmentForKick End***********************" << endl;

	Vector2f self = WM.getSelf().pos.to2D();
	Vector2f selftoball = ball -self;
	Vector2f balltodest = dest - ball;
//	double deltangle = gNormalizeDeg( balltodest.GetAngleDeg() - WM.getSelf().GetTorsoYawAngle() );
	double deltangle = gNormalizeDeg( selftoball.GetAngleDeg() - WM.getSelf().GetTorsoYawAngle() );
//	cout<<"a      "<<deltangle<<endl;
//	cout<<"L          "<<selftoball.Length()<<endl;
	

	    if (selftoball.Length() < 0.12  )
	    {
	      return true;
	    }
	    else
	    {
	      return false;
	    }



}

void Strive3DBehaviorControl::KickTo(Vector2f dest)
{

	Vector2f ball = WM.getBall().pos.to2D();
    Vector2f selftoball = WM.getSelf().pos.to2D() - ball;
    static int count = 0;
    if (selftoball.Length() > 0.25 || selftoball.x() > -0.02)
    {
	   if (WM.CanLocalize())
	       LocalViewGoToBallForShoot(dest);
	   else
	       GoToBallForShoot(dest);
     }
     else if (AdjustmentForKick(dest) == true)
     {
	   bLOG << "AdjustmentForKick Finish!" << endl;
	   headMotionRequest.headMotionType = HeadMotionRequest::lookatball;
	   actionRequest.kickRequest.Force=1.0;	
	   actionRequest.actionType = ActionRequest::kick;
	   bLOG << "In Shooting Phase" << endl;
	     count = 0;
     }
}

void Strive3DBehaviorControl::LocalKickTo(Vector2f dest)
{
	Vector3f ball3f = WM.getBall().GetLocalPos();
	Vector2f standpoint = ball3f.to2D() - dest;
	standpoint = standpoint.Normalized() * 0.18 + ball3f.to2D();
	Vector2f balltodest = dest -ball3f.to2D();
	double angletodest = gNormalizeDeg(balltodest.GetAngleDeg());
	if (actionRequest.kickRequest.type == 0)
	{
	  if (actionRequest.kickRequest.Direction < 15 && actionRequest.kickRequest.Direction > 0 )
	    angletodest = angletodest - actionRequest.kickRequest.Direction  ;
	  else
	  {
	    actionRequest.kickRequest.Direction = 0 ;
	    angletodest = angletodest - actionRequest.kickRequest.Direction  ;
	  }
	}
	else if (actionRequest.kickRequest.type == 1)
	{
	  if (actionRequest.kickRequest.Direction < 15 && actionRequest.kickRequest.Direction > 0 )
	    angletodest = angletodest - actionRequest.kickRequest.Direction  ;
	  else
	  {
	    actionRequest.kickRequest.Direction = 0 ;
	    angletodest = angletodest - actionRequest.kickRequest.Direction  ;
	  }
	}
	else if (actionRequest.kickRequest.type == 3)
	{
	  if (actionRequest.kickRequest.Direction < 15 && actionRequest.kickRequest.Direction > 0 )
	    angletodest = angletodest - actionRequest.kickRequest.Direction  ;
	  else
	  {
	    actionRequest.kickRequest.Direction = 0 ;
	    angletodest = angletodest - actionRequest.kickRequest.Direction  ;
	  }
	}	
	bLOG << "dest:" << dest << " ball3f: " << ball3f << " standpoint: " << standpoint << endl;
	bLOG << "AngleToBall: " << ball3f.to2D().GetAngleDeg() << endl;
	bLOG << "balltodest: " << balltodest << " angletodest: " << angletodest << endl;
	static int phase = 0;	
	static int count = 0;
	switch(phase)
	{
		case 0:
			bLOG << "In phase 0" << endl;
			if(standpoint.Length() > 0.05)
			{
				GoToRelativePoint(standpoint);
				phase = 0;
			}
			else
				phase = 1;
			break;
		case 1:
			bLOG << "In phase 1" << endl;
			if(standpoint.Length() < 0.05)
			{
				if(angletodest > 2.0 || standpoint.Length() > 0.025)
				{	
					float disttostdpoint = standpoint.Length();
					actionRequest.walkRequest.walkParams = standpoint.Normalized() * disttostdpoint * 0.5;
					actionRequest.walkRequest.walkParams.y() *= -1.0;
					if(gAbs(angletodest) < 0.5)
						angletodest = 0;
					actionRequest.walkRequest.rotation = gMax(angletodest, -5.0);
					actionRequest.walkRequest.rotation = gMin(actionRequest.walkRequest.rotation, 5.0);
					actionRequest.actionType = ActionRequest::walk;
					bLOG << "balltodest: " << balltodest << endl;
					bLOG << "angletodest: " << angletodest << " angletodest: " << angletodest << " disttostdpoint: " << disttostdpoint << endl;
					bLOG << "rotation: " << actionRequest.walkRequest.rotation << endl;
					phase = 1;
				}
				else
					phase = 2;
			}
			else
				phase = 0;
			break;
		case 2:
			bLOG << "In phase 2" << endl;
			if(standpoint.Length() < 0.06 && angletodest < 5.0)
			{
// 				if(count < 3)
// 				{
// 					Stand();
// 					phase = 2;
// 					count++;
// 				}
// 				else
// 				{
					phase = 3;
// 				}
			}
			else 
				phase = 0;
			break;
		case 3:	
			bLOG << "In phase 3" << endl;
			headMotionRequest.headMotionType = HeadMotionRequest::lookatball;
			actionRequest.kickRequest.Force = 1.0;
			actionRequest.actionType = ActionRequest::kick;
			count = 0;
			phase = 0;
			break;
	}
}

void Strive3DBehaviorControl::KickDecision()
{
	bLOG << "Time: " << WM.getFieldInfo().match_time << endl;
	Player& oppnearesttogoalie = WM.getmOppTeamNearestToGoalie();
	Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
	if(WM.CanLocalize())
	{
		bLOG << "I Can Localize" << endl;
		if(&oppnearesttogoalie == NULL)
			{
			  actionRequest.kickRequest.type = 2;
			  actionRequest.kickRequest.Direction= 0.0;
			  KickTo(goalin);
			}
		else
		{

			Vector2f opptogoalie = oppnearesttogoalie.pos.to2D() - WM.getFieldInfo().oppgoalcenter.to2D();

			if(opptogoalie.Length() > 2.5)
				{
				  actionRequest.kickRequest.type = 2;
				  actionRequest.kickRequest.Direction= 0.0;
				  KickTo(goalin);
				}
			else
			{
				bLOG << "OppPos: " << oppnearesttogoalie.pos << endl;
				
				Vector2f RightGoalie = Vector2f(WM.getFieldInfo().fieldlength/2.0,
												-WM.getFieldInfo().goalwidth/2.0);//From Our View
				Vector2f LeftGoalie = Vector2f(WM.getFieldInfo().fieldlength/2.0,
											WM.getFieldInfo().goalwidth/2.0);  //From Our View
				Vector2f KickTarget;
				
				Vector2f KickTarget_1;
				
				Vector2f KickTarget_2;
				

				 double dist= 0.145;
				Vector2f ball = WM.getBall().pos.to2D();
				Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
				Vector2f balltogoalin = ball - goalin;
				Vector2f Targettoball = ball - KickTarget_1;
				Vector2f tmp(dist * gCos(Targettoball.GetAngleRad()), dist * gSin(Targettoball.GetAngleRad()));
				tmp = ball + tmp;
				Vector2f agenttodest = tmp - KickTarget_1;
				if(oppnearesttogoalie.pos.y() + 0.3 >= 0)
					{
					  KickTarget = (oppnearesttogoalie.pos.to2D() + RightGoalie)/2.0;
					  KickTarget_1 = (oppnearesttogoalie.pos.to2D() + RightGoalie)/2.0;
					  KickTarget_2 = RightGoalie ;
					  double angleToDestination = gRadToDeg(gArcTan2(KickTarget_1.y(),KickTarget_1.x()));
					  double angleToRightGoalie = gRadToDeg(gArcTan2(KickTarget_2.y(),KickTarget_2.x()));
// 					  actionRequest.kickRequest.type = 0;
					  actionRequest.kickRequest.Direction= angleToDestination;
//					  if (actionRequest.kickRequest.cout >= 2)
//					    KickTarget = (RightGoalie + LeftGoalie)/2.0;
					  if (ball.y() > 0)
					   KickTarget = KickTarget - Vector2f(0,0.12*balltogoalin.Length());
					}
				else if(oppnearesttogoalie.pos.y() - 0.3 < 0)
					{
					  KickTarget = (oppnearesttogoalie.pos.to2D() + LeftGoalie)/2.0;
					  KickTarget_1 = (oppnearesttogoalie.pos.to2D() + LeftGoalie)/2.0;
					  KickTarget_2 = LeftGoalie;
					  double angleToDestination = gRadToDeg(gArcTan2(KickTarget_1.y(),KickTarget_1.x()));
					  double angleToLeftGoalie = gRadToDeg(gArcTan2(KickTarget_2.y(),KickTarget_2.x()));
// 					  KickTarget = ( RightGoalie + LeftGoalie)/2.0;
// 					  actionRequest.kickRequest.type = 2;
					  actionRequest.kickRequest.Direction= angleToDestination;
//					  if (actionRequest.kickRequest.cout >= 2)
//					    KickTarget = (RightGoalie + LeftGoalie)/2.0;
					 if (ball.y() < 0)
					   KickTarget = (RightGoalie + LeftGoalie)/2.0 + Vector2f(0,1.0 -0.18*balltogoalin.Length());
					}
				  KickTo(KickTarget);
			}
		}
	}
	else
	{
		bLOG << "I Can't Localize" << endl;
		if(&oppnearesttogoalie == NULL)
			{
			  actionRequest.kickRequest.type = 2;
			  actionRequest.kickRequest.Direction= 0.0;
			  LocalKickTo(WM.getFieldInfo().oppgoalcenterlocal.to2D());
			 }
		else
		{
			Vector3f opp = oppnearesttogoalie.GetHeadLocalPosition();
			Vector2f opptogoalie = opp.to2D() - WM.getFieldInfo().oppgoalcenterlocal.to2D();
			if(opptogoalie.Length() > 2.0)
			{
				actionRequest.kickRequest.type = 2;
				actionRequest.kickRequest.Direction = 0.0;
				KickTo(WM.getFieldInfo().oppgoalcenterlocal.to2D());
			}
			else
			{
				Vector2f RightGoalie = WM.getFieldInfo().opprightgoallocal.to2D(); //From Our View
				Vector2f LeftGoalie = WM.getFieldInfo().oppleftgoallocal.to2D(); //From Our View
				Vector2f RGtoOpp = opp.to2D() - RightGoalie;
				Vector2f LGtoOpp = opp.to2D() - LeftGoalie;
				Vector2f KickTarget;
				Vector2f KickTarget_1;
				Vector2f KickTarget_2;
				 double dist= 0.145;
				Vector2f ball = WM.getBall().pos.to2D();
				Vector2f Targettoball = ball - KickTarget_1;
				Vector2f tmp(dist * gCos(Targettoball.GetAngleRad()), dist * gSin(Targettoball.GetAngleRad()));
				tmp = ball + tmp;
				Vector2f agenttodest = tmp - KickTarget_1;
// 				double angleToDestination = gNormalizeDeg( agenttodest.GetAngleDeg()-WM.getSelf().GetTorsoYawAngle());
				
				if(RGtoOpp.Length() + 0.01 >= LGtoOpp.Length())
					{
					  KickTarget = (RightGoalie + opp.to2D()) / 2.0;
					  KickTarget_1 = (RightGoalie + opp.to2D()) / 2.0;
					  KickTarget_2 = RightGoalie ;
					  double angleToDestination = gRadToDeg(gArcTan2(KickTarget_1.y(),KickTarget_1.x()));
					  double angleToRightGoalie = gRadToDeg(gArcTan2(KickTarget_2.y(),KickTarget_2.x()));
// 					  actionRequest.kickRequest.type = 0;
					  actionRequest.kickRequest.Direction= angleToDestination;
//					  if (actionRequest.kickRequest.cout >= 2)
//					    KickTarget = (RightGoalie + LeftGoalie)/2.0;
					  if (ball.y() > 0)
					    KickTarget = (RightGoalie + LeftGoalie)/2.0;
// 					  if (angleToRightGoalie < angleToDestination)
// 					    actionRequest.kickRequest.Direction = 0 ;
// 					   if (actionRequest.kickRequest.Direction < 0 )
// 					  {
// 					    actionRequest.kickRequest.type = 0;
// 					    actionRequest.kickRequest.Direction = -actionRequest.kickRequest.Direction;
// 					  }
					}
				else if (RGtoOpp.Length() -0.01 < LGtoOpp.Length())
					{
					   KickTarget = ( LeftGoalie + opp.to2D())/2.0;
					  KickTarget_1 = (LeftGoalie + opp.to2D()) / 2.0;
					  KickTarget_2 = LeftGoalie;
					  double angleToDestination = gRadToDeg(gArcTan2(KickTarget_1.y(),KickTarget_1.x()));
					  double angleToLeftGoalie = gRadToDeg(gArcTan2(KickTarget_2.y(),KickTarget_2.x()));					  
// 					  actionRequest.kickRequest.type = 2;
					  actionRequest.kickRequest.Direction=  angleToDestination; 
//					  if (actionRequest.kickRequest.cout >= 2)
//					    KickTarget = (RightGoalie + LeftGoalie)/2.0;
					 if (ball.y() < 0)
					   KickTarget = (RightGoalie + LeftGoalie)/2.0;
					}
				  LocalKickTo(KickTarget);
				bLOG << "Time: " << WM.getFieldInfo().match_time << endl;
				bLOG << " KickTarget: " << KickTarget << endl;
				bLOG << "RightGoalie: " << RightGoalie << " LeftGoalie: " << LeftGoalie << endl;
				bLOG << "opp: " << opp.to2D() << endl;
			}
		}
	}
}


void Strive3DBehaviorControl::execute()
{
	bLOG << "time: " << WM.getFieldInfo().match_time << endl;
	float k = 0.9;
	static Vector3f acceleration;
	Vector3f rawacceleration = WM.getSelf().GetAcceleration();
	acceleration = k * acceleration + (1-k) * rawacceleration;
	if ( acceleration.z()< 2.0/* || (WM.CanLocalize() && (WM.getSelf().onMyback()||WM.getSelf().onMyBelly()||WM.getSelf().onMySide())*/)
    {
        bLOG << "Getting Up!" << endl;
// 	actionRequest.kickRequest.cout = 0 ;
        actionRequest.actionType = ActionRequest::getup;
    }
	else
	{
		if ( WM.getSelf().GetUnum() != 1 )
        {
			headMotionRequest.headMotionType = HeadMotionRequest::automode;
            Decision();
            //KickTo(Vector2f(10,0));
			//GoToPoint(Vector2f(12,12),1);		
            //Debug(0.12,0.04,0.0);
            //GoToRelativePoint(Vector2f(0,1));
            //LocalViewGoToBallForShoot(Vector2f(6,0));
            //DribbleTo(Vector2f(15,0));
    	}
		else
        {
			headMotionRequest.headMotionType = HeadMotionRequest::lookatball;
			GoalieDecision();
		}    	
	}
	//sendMsg2GuiDebugger();
}

bool	Strive3DBehaviorControl::Decision()
{
// 	if (WM.getBall().pos.x() > 0.5)
// 		ballState = OnOurField;
// 	else
// 		ballState = OnOppField;
// 
	if(!WM.Prepare)
	{
		Player & nearestplayertoball = WM.getPlayerNearestToBall();
		if(&nearestplayertoball == NULL)
			bLOG << "nearestplayertoball = NULL" << endl;
		
		if(nearestplayertoball.GetName() == WM.getSelf().GetName())
			OffensiveDecision();
		else
			DefensiveDecision();
	}
	else
	{
		PrepareDecision();
	}
// 	Vector2f ball = WM.getBall().pos.to2D();
// 	Vector2f self = WM.getSelf().pos.to2D();
// 	Vector2f dest = ball - self;
// 	dest += ball;
// 	dest = dest.Normalized();
// 	DribbleTo(dest);
// 	KickDecision();
}

bool Strive3DBehaviorControl::PrepareDecision()
{
	bLOG << "************************In PrepareDecision***********************" << endl;
	switch(WM.getSelf().roleinteam)
	{
		case Self::forward1:
			{	
				CoverTeammate(2.3, Vector2f(0.35, 0.35));
			}
			break;
		case Self::forward2:
			{
				Player& self = WM.getSelf();
				Vector2f selfpos = self.pos.to2D();
				Vector2f destpos;
				Vector2f ballpos = WM.getBall().pos.to2D();
				float penaltylength = WM.getFieldInfo().penaltylength;
				float penaltywidth =  WM.getFieldInfo().penaltywidth;
				if (WM.getFieldInfo().play_mode == PM_GOAL_KICK_LEFT)
				{	
					if ( WM.getFieldInfo().team_index == TI_LEFT)
						Forward2OffDecision();
					else
					{
						destpos = Vector2f(7.0, penaltywidth - 1.5);
						GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					}
				}
				else if ( WM.getFieldInfo().play_mode == PM_GOAL_KICK_RIGHT)
				{
					if (  WM.getFieldInfo().team_index == TI_RIGHT)
						Forward2OffDecision();
					else
					{
						destpos = Vector2f(7.0, penaltywidth - 1.5);
						GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					}
				}
				else
					CoverTeammate(3.0);
				break;
			}
		case Self::center:
			{
				Player& self = WM.getSelf();
				Vector2f selfpos = self.pos.to2D();
				Vector2f destpos;
				Vector2f ballpos = WM.getBall().pos.to2D();
				float penaltylength = WM.getFieldInfo().penaltylength;
				float penaltywidth =  WM.getFieldInfo().penaltywidth;
				if (WM.getFieldInfo().play_mode == PM_GOAL_KICK_LEFT)
				{		
					if ( WM.getFieldInfo().team_index == TI_LEFT)
						CenterDefDecision();
					else
					{
						destpos = Vector2f(7.0, -penaltywidth + 1.5);
						GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					}
				}
				else if ( WM.getFieldInfo().play_mode == PM_GOAL_KICK_RIGHT)
				{
					if (  WM.getFieldInfo().team_index == TI_RIGHT)
						CenterDefDecision();
					else
					{
						destpos = Vector2f(7.0, -penaltywidth + 1.5);
						GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					}
				}
				else
					CoverTeammate(3.5, Vector2f(-0.35, -0.35));
				break;	
			}
		case Self::guard1:
			Guard1DefDecision();//Guard1OffDecision();
			break;
		case Self::guard2:
			Guard2DefDecision();//Guard2OffDecision();
			break;
		case Self::guard3:
			Guard3DefDecision();
			break;
		case Self::guard4:
			Guard4DefDecision();
			break;
		case Self::guard5:
			Guard5DefDecision();
			break;
		default:
			bLOG << "Unknow Role In Team!" << endl;
	}
	bLOG << "************************PrepareDecision End***********************" << endl;
	
}

bool Strive3DBehaviorControl::OffensiveDecision()
{
	bLOG << "************************In OffensiveDecision***********************" << endl;
	switch(WM.getSelf().roleinteam)
	{
		case Self::forward1:
		{
			Player& oppteamnearesttoball = WM.getOppTeamNearestToBall();
			Player& ourteamnearesttoball = WM. getOurTeamNearestToBall();
			if(&oppteamnearesttoball == NULL)
			{
				bLOG << "Point Is NULL!" << endl;
				bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
				bLOG << "oppteamnearesttoball: " << &oppteamnearesttoball << endl;
				Forward1OffDecision();
			}
			else
			{
				Player& self = WM.getSelf();
				Vector2f opppos = oppteamnearesttoball.pos.to2D();
				Vector2f KickTarget;
				Vector2f ballpos = WM.getBall().pos.to2D();
				float fieldlength = WM.getFieldInfo().fieldlength;
				float fieldwidth =  WM.getFieldInfo().fieldwidth;
				if (WM.getFieldInfo().play_mode == PM_GOAL_KICK_LEFT)
				{
					if(opppos.y() > 0)
						KickTarget = Vector2f(0,  -fieldwidth/2);
					else
						KickTarget =Vector2f(0,  fieldwidth/2);
					if ( WM.getFieldInfo().team_index == TI_LEFT && ourteamnearesttoball.GetUnum() == self.GetUnum())
						KickTo(KickTarget);
					else
						Forward1OffDecision();
				}
				else if ( WM.getFieldInfo().play_mode == PM_GOAL_KICK_RIGHT)
				{
					if(opppos.y() > 0)
						KickTarget = Vector2f(0,  -fieldwidth/2);
					else
						KickTarget =Vector2f(0,  fieldwidth/2);
					if (  WM.getFieldInfo().team_index == TI_RIGHT && ourteamnearesttoball.GetUnum() == self.GetUnum())
						KickTo(KickTarget);
					else
						Forward1OffDecision();
				}
				else if (WM.getFieldInfo().play_mode == PM_KickIn_Left)
				{
					if(ballpos.y() > 0)
						KickTarget = Vector2f(fieldlength/2,  5.0);
					else
						KickTarget =Vector2f(fieldlength/2,  -5.0);
					if ( WM.getFieldInfo().team_index == TI_LEFT && ourteamnearesttoball.GetUnum() == self.GetUnum() && ballpos.x() <= 6)
						KickTo(KickTarget);
					else
						Forward1OffDecision();
				}
				else if ( WM.getFieldInfo().play_mode == PM_KickIn_Right)
				{
					if(opppos.y() > 0)
						KickTarget = Vector2f(fieldlength/2,  5.0);
					else
						KickTarget =Vector2f(fieldlength/2,  -5.0);
					if (  WM.getFieldInfo().team_index == TI_RIGHT && ourteamnearesttoball.GetUnum() == self.GetUnum() && ballpos.x() <= 6)
						KickTo(KickTarget);
					else
						Forward1OffDecision();
				}
				else
					Forward1OffDecision();
			}
			break;
		}
		case Self::forward2:
			Forward2OffDecision();
			break;
		case Self::center:
		{
		  	Player& self = WM.getSelf();
			Vector2f selfpos = self.pos.to2D();
			Vector2f destpos;
			Vector2f ballpos = WM.getBall().pos.to2D();
			float penaltylength = WM.getFieldInfo().penaltylength;
			float penaltywidth =  WM.getFieldInfo().penaltywidth;
			if (WM.getFieldInfo().play_mode == PM_CORNER_KICK_LEFT)
			{		
				if ( WM.getFieldInfo().team_index == TI_RIGHT)
					CenterDefDecision();
				else
				{
					if(ballpos.y() >= 0)
					{
					    destpos = Vector2f(7.0, penaltywidth - 1.5);
					    GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					} 
					else
					{
					    destpos = Vector2f(7.0, -penaltywidth + 1.5);
					    GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					} 
				}
			}
			else if ( WM.getFieldInfo().play_mode == PM_CORNER_KICK_RIGHT)
			{
				if ( WM.getFieldInfo().team_index == TI_LEFT)
					CenterDefDecision();
				else
				{
					if(ballpos.y() >= 0)
					{
					    destpos = Vector2f(7.0, penaltywidth - 1.5);
					    GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					} 
					else
					{
					    destpos = Vector2f(7.0, -penaltywidth + 1.5);
					    GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					} 
				}
			}
			else
				CenterDefDecision();
			break;	
		}
		case Self::guard1:
			Guard1DefDecision();//Guard1OffDecision();
			break;
		case Self::guard2:
			KeepGuard2DefDecision();//Guard2OffDecision();
			break;
		case Self::guard3:
		{
		  	Player& self = WM.getSelf();
			Vector2f selfpos = self.pos.to2D();
			Vector2f destpos;
			Vector2f ballpos = WM.getBall().pos.to2D();
			float penaltylength = WM.getFieldInfo().penaltylength;
			float penaltywidth =  WM.getFieldInfo().penaltywidth;
			if (WM.getFieldInfo().play_mode == PM_CORNER_KICK_LEFT)
			{		
				if ( WM.getFieldInfo().team_index == TI_RIGHT)
					Guard3DefDecision();
				else
				{
					destpos = Vector2f(7.5, 0);
					GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
				}
			}
			else if ( WM.getFieldInfo().play_mode == PM_CORNER_KICK_RIGHT)
			{
				if ( WM.getFieldInfo().team_index == TI_LEFT)
					Guard3DefDecision();
				else
				{
					destpos = Vector2f(7.5, 0);
					GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
				}
			}
			else	
				Guard3DefDecision();
			break;
		}
		case Self::guard4:
		{
		  	Player& self = WM.getSelf();
			Vector2f selfpos = self.pos.to2D();
			Vector2f destpos;
			Vector2f ballpos = WM.getBall().pos.to2D();
			float penaltylength = WM.getFieldInfo().penaltylength;
			float penaltywidth =  WM.getFieldInfo().penaltywidth;
			if (WM.getFieldInfo().play_mode == PM_CORNER_KICK_LEFT)
			{		
				if ( WM.getFieldInfo().team_index == TI_RIGHT)
					Guard4DefDecision();
				else
				{
					if(ballpos.y() >= 0)
					{
					    destpos = Vector2f(7.0, -penaltywidth + 1.5);
					    GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					} 
					else
					{
					    destpos = Vector2f(7.0, penaltywidth - 1.5);
					    GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					} 
				}
			}
			else if ( WM.getFieldInfo().play_mode == PM_CORNER_KICK_RIGHT)
			{
				if ( WM.getFieldInfo().team_index == TI_LEFT)
					Guard4DefDecision();
				else
				{
					if(ballpos.y() >= 0)
					{
					    destpos = Vector2f(7.0, -penaltywidth + 1.5);
					    GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					} 
					else
					{
					    destpos = Vector2f(7.0, penaltywidth - 1.5);
					    GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
					} 
				}
			}
			else	  
				Guard4DefDecision();
			break;
		}
		case Self::guard5:
		{
		  	Player& self = WM.getSelf();
			Vector2f selfpos = self.pos.to2D();
			Vector2f destpos;
			Vector2f ballpos = WM.getBall().pos.to2D();
			float penaltylength = WM.getFieldInfo().penaltylength;
			float penaltywidth =  WM.getFieldInfo().penaltywidth;
			if (WM.getFieldInfo().play_mode == PM_CORNER_KICK_LEFT)
			{		
				if ( WM.getFieldInfo().team_index == TI_RIGHT)
					Guard5DefDecision();
				else
				{
					destpos = Vector2f(4.5, 0);
					GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
				}
			}
			else if ( WM.getFieldInfo().play_mode == PM_CORNER_KICK_RIGHT)
			{
				if ( WM.getFieldInfo().team_index == TI_LEFT)
					Guard5DefDecision();
				else
				{
					destpos = Vector2f(4.5, 0);
					GoToPointAndTurnTo(destpos,(ballpos-selfpos).GetAngleDeg());
				}
			}
			else	
				Guard5DefDecision();
			break;
		}
        case Self::guard6:
            Guard6DefDecision();
			break;
		case Self::guard7:
            Guard7DefDecision();
			break;
		default:
			bLOG << "Unknow Role In Team!" << endl;
	}
	bLOG << "************************OffensiveDecision End***********************" << endl;
}

void	Strive3DBehaviorControl::Forward1OffDecision()
{
	headMotionRequest.headMotionType = HeadMotionRequest::intelligencemode;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	cout << "In Forward1OffDecision" << endl;
	float fieldlength = WM.getFieldInfo().fieldlength;
	Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
	Vector2f ballpos = WM.getBall().pos.to2D();
	if (WM.CanLocalize())
	{
	  Vector3f ball3f = WM.getBall().GetLocalPos();
	  Vector2f ballpos = ball3f.to2D();
	}
	else
	  Vector2f ballpos = WM.getBall().pos.to2D();
	/*if (ballpos.x() < 6 && WM.getTmm(10)->isValid)
	 {
		 kickTo();
	 }	 */

	Vector2f agentpos = WM.getSelf().pos.to2D();
	Vector2f agenttoball = ballpos - agentpos;
	Vector3f localball = WM.getBall().GetLocalPos();
	Vector2f balltogoalin = localball.to2D() - WM.getFieldInfo().oppgoalcenterlocal.to2D();
//	Vector2f dest = ballpos + agenttoball.Normalized() * 20;
	float agenttoball_angle = gNormalizeDeg(agenttoball.GetAngleDeg());
//	bLOG << "dest: " << dest 
		bLOG<< " goalin: " << goalin 
		<< " ballpos: " << ballpos 
		<< " agentpos: " << agentpos
		<< " goalwidth: " << WM.getFieldInfo().goalwidth
		<< " balltogoalin.Length: " << balltogoalin.Length() << endl;
	if(balltogoalin.Length() < WM.getFieldInfo().goalwidth * 0.5)
	{
		bLOG << "DribbleForward!" << endl;
		Vector2f forward = Vector2f(0.5, 0);
		forward += ballpos;
		DribbleTo(forward);
	}
	else if(balltogoalin.Length() > 5.0 || balltogoalin.Length() < 2.0 || ballpos.x() > WM.getFieldInfo().fieldlength/2 - 1.8 /*|| (ballpos.y() > 2.6 && ballpos.x() > WM.getFieldInfo().fieldlength/2 - 2.0)*/)
	{
		Player& nearestplayertoball = WM.getOppTeamNearestToBall();
		Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
		if(ourteamnearesttoball.GetUnum() == 1)
		{
		        Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
			Vector2f agenttoball = ballpos - agentpos;
			if(tmmpos.y() >=0)
			      GoToPointAndTurnTo(Vector2f(tmmpos.x() - 0.3,tmmpos.y() - 1),agenttoball.GetAngleDeg());
			else
			      GoToPointAndTurnTo(Vector2f(tmmpos.x() - 0.3,tmmpos.y() + 1),agenttoball.GetAngleDeg());
		}
		else if(ourteamnearesttoball.GetUnum() == 8 || ourteamnearesttoball.GetUnum() == 9)
			Forward2OffDecision();
		else
		{
			if(balltogoalin.Length() > 5.0 && &nearestplayertoball != NULL)
			{
				if(ballpos.x() < 4.5)
				{
					if(ballpos.y() <= 4.5 && ballpos.y() >= -4.5)
					{
						bLOG << "MMMMMMM" << endl;
						Vector2f opppos = nearestplayertoball.pos.to2D();
						Vector2f selfpos = ourteamnearesttoball.pos.to2D();
						Vector2f balltoopp = opppos - ballpos;
						Vector2f balltoself = selfpos - ballpos;
						Vector2f selftoball = -balltoself;
						float fieldlength = WM.getFieldInfo().fieldlength;
						float distballtoopp = balltoopp.Length();
						float distballtoself = balltoself.Length();
						float diffangle = gNormalizeDeg(agenttoball.GetAngleDeg() - balltoopp.GetAngleDeg());
						float difftorso = gAbs(agenttoball.GetAngleDeg()-WM.getSelf().GetTorsoYawAngle());
						bLOG << "distballtoopp: " << distballtoopp <<"	distballtoself:"<<distballtoself<<"   diffangle:" << diffangle <<"	agenttoball_angle:" <<agenttoball_angle << endl;
						if(agenttoball_angle > 45.0)
						{
							static bool flag1 = false;
							float k = gTan(gDegToRad( 30.0));
							float b = ballpos.y() - k*ballpos.x();
							static Vector2f bottom1 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
							switch(flag1)
							{
							      case false:
								    if(distballtoself <= 0.5 && difftorso <= 60)
								    {
									bLOG << "kkk111" << endl; 
									flag1 = true;
									bottom1 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
									break;
								    }
								    else
								    {
								      bLOG << "kkk222" << endl; 
									flag1 = false;
									bottom1 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
									DribbleTo(bottom1);
									break;
								    }
							      case true:
								    if(gAbs(agenttoball_angle) >= 60) 
								    {
								      bLOG << "kkk333" << endl; 
									flag1 = false;
									break;
								    }
								    else
								    {
								      bLOG << "kkk444" << endl; 
									flag1 = true;
									DribbleTo(bottom1);
									break;
								    }
							}
						}
						else if (agenttoball_angle < -45.0)
						{
							static bool flag2 = false;
							float k = gTan(gDegToRad( -30.0));
							float b = ballpos.y() - k*ballpos.x();
							static Vector2f bottom2 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
							switch(flag2)
							{
							      case false:
								    if(distballtoself <= 0.5 && difftorso <= 60)
								    {
								      bLOG << "kkk555" << endl; 
									flag2 = true;
									bottom2 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
									break;
								    }
								    else
								    {
								      bLOG << "kkk666" << endl; 
									flag2 = false;
									bottom2 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
									DribbleTo(bottom2);
									break;
								    }
							      case true:
								    if(gAbs(agenttoball_angle) >= 60) 
								    {
								      bLOG << "kkk777" << endl; 
									flag2 = false;
									break;
								    }
								    else
								    {
								      bLOG << "kkk888" << endl; 
									flag2 = true;
									DribbleTo(bottom2);
									break;
								    }
							}
						}
						else
						{
						  	static bool flag3 = false;
							static Vector2f dest3 = ballpos + agenttoball.Normalized() * 20;
							switch(flag3)
							{
							      case false:
								    if(distballtoself <= 0.5 && difftorso <= 60)
								    {
									bLOG << "kkk999" << endl; 
									flag3 = true;
									dest3 = ballpos + agenttoball.Normalized() * 20;
									 bLOG << "dest3.x:" << dest3.x() <<"dest3.y:" <<dest3.y() << endl;
									break;
								    }
								    else
								    {
								      bLOG << "kkk101010" << endl; 
									flag3 = false;
									dest3 = ballpos + agenttoball.Normalized() * 20;
									DribbleTo(dest3);
									 bLOG << "dest3.x:" << dest3.x() <<"dest3.y:" <<dest3.y() << endl;
									break;
								    }
							      case true:
								    if(gAbs(agenttoball_angle) >= 60) 
								    {
								      bLOG << "kkk111111" << endl; 
									flag3 = false;
									 bLOG << "dest3.x:" << dest3.x() <<"dest3.y:" <<dest3.y() << endl;
									break;
								    }
								    else
								    {
								      bLOG << "kkk121212" << endl; 
									flag3 = true;
									DribbleTo(dest3);
								      bLOG << "dest3.x:" << dest3.x() <<"dest3.y:" <<dest3.y() << endl;
									break;
								    }
							}
						}
					}
					else if(ballpos.y() > 4.5)
					{
						bLOG << "LLLLLL" <<endl;
						Vector2f opppos = nearestplayertoball.pos.to2D();
						Vector2f selfpos = ourteamnearesttoball.pos.to2D();
						Vector2f balltoopp = opppos - ballpos;
						Vector2f balltoself = selfpos - ballpos;
						float fieldlength = WM.getFieldInfo().fieldlength;
						float distballtoopp = balltoopp.Length();
						float distballtoself = balltoself.Length();
						float diffangle = gNormalizeDeg(agenttoball.GetAngleDeg() - balltoopp.GetAngleDeg());
						float difftorso = gAbs(agenttoball.GetAngleDeg()-WM.getSelf().GetTorsoYawAngle());
						if(agenttoball_angle >= 0.0)
							DribbleTo(Vector2f(fieldlength/2, 4.5));
						else if (agenttoball_angle < -45.0)
						{
							static bool flag4 = false;
							float k = gTan(gDegToRad( -30.0));
							float b = ballpos.y() - k*ballpos.x();
							static Vector2f bottom4 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
							switch(flag4)
							{
							      case false:
								    if(distballtoself <= 0.5 && difftorso <= 60)
								    {
									flag4 = true;
									bottom4 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
									break;
								    }
								    else
								    {
									flag4 = false;
									bottom4 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
									DribbleTo(bottom4);
									break;
								    }
							      case true:
								    if(gAbs(agenttoball_angle) >= 60) 
								    {
									flag4 = false;
									break;
								    }
								    else
								    {
									flag4 = true;
									DribbleTo(bottom4);
									break;
								    }
							}
						}
						else
						{
							static bool flag5 = false;
							static Vector2f dest5 = ballpos + agenttoball.Normalized() * 20;
							switch(flag5)
							{
							      case false:
								    if(distballtoself <= 0.5 && difftorso <= 60)
								    {
									flag5 = true;
									dest5 = ballpos + agenttoball.Normalized() * 20;
									break;
								    }
								    else
								    {
									flag5 = false;
									dest5 = ballpos + agenttoball.Normalized() * 20;
									DribbleTo(dest5);
									break;
								    }
							      case true:
								    if(gAbs(agenttoball_angle) >= 60) 
								    {
									flag5 = false;
									break;
								    }
								    else
								    {
									flag5 = true;
									DribbleTo(dest5);
									break;
								    }
							}
						}
					}
					else
					{
						bLOG << " RRRRRR " << endl;
						Vector2f opppos = nearestplayertoball.pos.to2D();
						Vector2f selfpos = ourteamnearesttoball.pos.to2D();
						Vector2f balltoopp = opppos - ballpos;
						Vector2f balltoself = selfpos - ballpos;
						float fieldlength = WM.getFieldInfo().fieldlength;
						float distballtoopp = balltoopp.Length();
						float distballtoself = balltoself.Length();
						float diffangle = gNormalizeDeg(agenttoball.GetAngleDeg() - balltoopp.GetAngleDeg());
						float difftorso = gAbs(agenttoball.GetAngleDeg()-WM.getSelf().GetTorsoYawAngle());
						bLOG << "distballtoopp: " << distballtoopp <<"	distballtoself:"<<distballtoself<<"   diffangle:" << diffangle <<"	agenttoball_angle:" <<agenttoball_angle << endl;
						if(agenttoball_angle <= 0.0)
							DribbleTo(Vector2f(fieldlength/2, -4.5));
						else if (agenttoball_angle > 45.0)
						{
							static bool flag6 = false;
							float k = gTan(gDegToRad( 30.0));
							float b = ballpos.y() - k*ballpos.x();
							static Vector2f bottom6 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
							switch(flag6)
							{
							      case false:
								    if(distballtoself <= 0.5 && difftorso <= 60)
								    {
									flag6 = true;
									bottom6 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
									break;
								    }
								    else
								    {
									flag6 = false;
									bottom6 = Vector2f(fieldlength/2, fieldlength/2 * k +b);
									DribbleTo(bottom6);
									break;
								    }
							      case true:
								    if(gAbs(agenttoball_angle) >= 60) 
								    {
									flag6 = false;
									break;
								    }
								    else
								    {
									flag6 = true;
									DribbleTo(bottom6);
									break;
								    }
							}

						}
						else
						{
							static bool flag7 = false;
							static Vector2f dest7 = ballpos + agenttoball.Normalized() * 20;
							switch(flag7)
							{
							      case false:
								    if(distballtoself <= 0.5 && difftorso <= 60)
								    {
									flag7 = true;
									dest7 = ballpos + agenttoball.Normalized() * 20;
									break;
								    }
								    else
								    {
									flag7 = false;
									dest7 = ballpos + agenttoball.Normalized() * 20;
									DribbleTo(dest7);
									break;
								    }
							      case true:
								    if(gAbs(agenttoball_angle) >= 60) 
								    {
									flag7 = false;
									break;
								    }
								    else
								    {
									flag7 = true;
									DribbleTo(dest7);
									break;
								    }
							}
						}
					}
				}
				else
					DribbleTo(goalin);
			}
			else
				DribbleTo(goalin);
		}
	}
	else	
	{
		if (IsCanShoot() == false)
		      DribbleTo(goalin);
		else
		{
		      cout << "KickDecision!" << endl;
		      KickDecision();
		}
	}
}

void	Strive3DBehaviorControl::Forward2OffDecision()
{
	headMotionRequest.headMotionType = HeadMotionRequest::intelligencemode;
	cout << "In Forward2OffDecision" << endl;
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	bLOG << "ourteamnearesttoball.State: " << ourteamnearesttoball.GetState() << endl;
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(1.5);
	}
	else
	{
		if(ourteamnearesttoball.GetState() != TUMBLE)
		{
			if(ourteamnearesttoball.GetUnum() == 1 || ourteamnearesttoball.GetUnum() == 8 || ourteamnearesttoball.GetUnum() == 9)
				CenterDefDecision();
			else
			{
				Vector2f selfpos = WM.getSelf().pos.to2D();
				Vector2f ballpos = WM.getBall().pos.to2D();
				Vector2f opppos = nearestplayertoball.pos.to2D();
				Vector2f vectopptoball = ballpos - opppos;
				Vector2f destpos = ballpos + vectopptoball.Normalized() * 1.5;
				vectopptoball = -vectopptoball;
				float destangle = gNormalizeDeg(vectopptoball.GetAngleDeg());
				if(selfpos.x() > ballpos.x() && vectopptoball.Length() >=0.5 && vectopptoball.Length() <= 3.0)
				{
					GoToPoint(opppos,1.4);
				}
				else
				{
					if(selfpos.x() > ballpos.x() -0.5)
					{
						if(selfpos.y() >= ballpos.y())
						{
							GoToPoint(Vector2f(ballpos.x() - 1.5, ballpos.y() +1),1.4); 
						}
						else
						{
							GoToPoint(Vector2f(ballpos.x() - 1.5, ballpos.y() -1),1.4); 
						}
					}
					else
					{
						if(destangle > 45 || destangle < -45)
							CoverTeammate(1.5);
						else
							GoToPointAndTurnTo(destpos, destangle);
					}
				}
			}
		}
		else
		{
//			Vector2f opppos = nearestplayertoball.pos.to2D();
//			Vector2f ballpos = WM.getBall().pos.to2D();
//			Vector2f opptoball = opppos - ballpos;

			Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
			Vector2f ballpos = WM.getBall().pos.to2D();
			Vector2f tmmtoball = ballpos - tmmpos;
			
			if(tmmtoball.Length() >= 0.5)
				Forward1DefDecision();
			else
				CoverTeammate(1.5);
			bLOG << "The Nearest Tmm is TUMBLE!" << endl;
		}
	}
//       Vector2f dest = WM.getFieldInfo().oppgoalcenter.to2D();
//       GoToPoint(dest, 1.4);
//       Vector2f ball = WM.getBall().pos.to2D();
//       Vector2f self = WM.getSelf().pos.to2D();
//       Vector2f selftoball = self - ball ;
//       if (selftoball.Length() < 2.0 && self.x() > ball.x())
// 	Stand();
}

void	Strive3DBehaviorControl::CenterOffDecision()
{
	cout << "In CenterOffDecision" << endl;
	CoverTeammate(3.5);
}

void	Strive3DBehaviorControl::Guard1OffDecision()
{
	bLOG << "In Guard1OffDecision" << endl;
	actionRequest.actionType = ActionRequest::stand;
}

void	Strive3DBehaviorControl::Guard2OffDecision()
{
	bLOG << "In Guard2OffDecision" << endl;
	actionRequest.actionType = ActionRequest::stand;
}

void	Strive3DBehaviorControl::Guard3OffDecision()
{
	bLOG << "In Guard3OffDecision" << endl;
	actionRequest.actionType = ActionRequest::stand;
}

void	Strive3DBehaviorControl::Guard4OffDecision()
{
	bLOG << "In Guard4OffDecision" << endl;
	actionRequest.actionType = ActionRequest::stand;
}

void	Strive3DBehaviorControl::Guard5OffDecision()
{
	bLOG << "In Guard5OffDecision" << endl;
	actionRequest.actionType = ActionRequest::stand;
}

bool	Strive3DBehaviorControl::DefensiveDecision()
{
	bLOG << "************************In DefensiveDecision***********************" << endl;
	switch(WM.getSelf().roleinteam)
	{
		case Self::forward1:
			Forward1DefDecision();
			break;
		case Self::forward2:
			Forward2DefDecision();
			break;
		case Self::center:
			CenterDefDecision();
			break;
		case Self::guard1:
			Guard1DefDecision();
			break;
		case Self::guard2:
			KeepGuard2DefDecision();
			break;
		case Self::guard3:
			Guard3DefDecision();
			break;
		case Self::guard4:
			Guard4DefDecision();
			break;
		case Self::guard5:
			Guard5DefDecision();
			break;
        case Self::guard6:
            Guard6DefDecision();
			break;
		case Self::guard7:
			Guard7DefDecision();
			break;
		default:
			bLOG << "Unknow Role In Team!" << endl;
	}
	bLOG << "************************DefensiveDecision End***********************" << endl;
}
/********** New added players (def)**********/
void 	Strive3DBehaviorControl::Guard6DefDecision()
{
    Stand();
}
void 	Strive3DBehaviorControl::Guard7DefDecision()
{
    Stand();
}
/********** New added player (offensive)*****/
void    Strive3DBehaviorControl::Guard6OffDecision()
{
    Stand();
}

void    Strive3DBehaviorControl::Guard7OffDecision()
{
    Stand();
}
void	Strive3DBehaviorControl::Forward1DefDecision()
{
    bLOG << "In Forward1DefDecision" << endl;
    Vector2f ballpos = WM.getBall().pos.to2D();
    Vector2f goalpos = WM.getFieldInfo().oppgoalcenter.to2D();
    Vector2f agentpos = WM.getSelf().pos.to2D();
    float sideparam = 6.0/(6.0 - gAbs(ballpos.y()));
    float baseparam = 6.0/(9.0 - ballpos.x());
    Vector2f selftoball = ballpos - agentpos;
    Vector2f vectball2goal = (goalpos - ballpos);
    vectball2goal = vectball2goal/vectball2goal.Length();
    Vector2f vectagent2ball = (ballpos - agentpos);
    vectagent2ball = vectagent2ball/vectagent2ball.Length();
    if(agentpos.x() <= ballpos.x())
    {
	Vector2f dribblepos = vectagent2ball* 20;
	DribbleTo(dribblepos);
    }
    else
    {
	Vector2f dribblepos = sideparam * baseparam * vectball2goal + vectagent2ball + ballpos;
	DribbleTo(dribblepos);
    }
}

void	Strive3DBehaviorControl::Forward2DefDecision()
{
	bLOG << "In Forward2DefDecision" << endl;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(1.5);
	}
	else
	{
		if(ourteamnearesttoball.GetState() != TUMBLE)
		//CoverTeammate(2);
		{		
			Vector2f selfpos = WM.getSelf().pos.to2D();
			Vector2f ballpos = WM.getBall().pos.to2D();
			Vector2f opppos = nearestplayertoball.pos.to2D();
			Vector2f vectopptoball = ballpos - opppos;
			Vector2f destpos = ballpos + vectopptoball.Normalized() * 1.5;
			vectopptoball = -vectopptoball;
			float destangle = gNormalizeDeg(vectopptoball.GetAngleDeg());
			if(selfpos.x() > ballpos.x() -0.3)
			{
				if(selfpos.y() >= ballpos.y())
				{
					GoToPoint(Vector2f(ballpos.x() - 1, ballpos.y() +1),1.4); 
				}
				else
				{
					GoToPoint(Vector2f(ballpos.x() - 1, ballpos.y() -1),1.4); 
				}
			}
			else
			{
				if(destangle > 45 || destangle < -45)
					CoverTeammate(1.5);
				else
					GoToPointAndTurnTo(destpos, destangle);
			}
		}
		else
		{
			Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
			Vector2f opppos = nearestplayertoball.pos.to2D();
			Vector2f ballpos = WM.getBall().pos.to2D();
			Vector2f vecttmmtoball = ballpos - tmmpos;
			Vector2f vectopptoball = ballpos - opppos;
			Vector2f destpos = ballpos + vectopptoball.Normalize() * 1.5;
			vectopptoball = -vectopptoball;
			float destangle = gNormalizeDeg(vectopptoball.GetAngleDeg());
			float  disttmmtoball = vecttmmtoball.Length();	
			if(disttmmtoball < 0.5)
				CoverTeammate(1.5);
//				GoToPointAndTurnTo(destpos, destangle);
			else
				Forward1DefDecision();
		}
	}
}

void	Strive3DBehaviorControl::CenterDefDecision()
{
	bLOG << "In CenterDefDecision" << endl;
	double dist = 4.0;
	static bool flagcenter = 0;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& ourteamsecondtoball = WM.getOurTeamSecondToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(4);
	}
	else
	{
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f tmmsecondpos = ourteamsecondtoball.pos.to2D();
		Vector2f opppos = nearestplayertoball.pos.to2D();
		Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f goal;
		if(ball.y() >=0)
			goal = Vector2f(-fieldlength/2,penaltywidth/2);
		else
			goal = Vector2f(-fieldlength/2,-penaltywidth/2);
		Vector2f balltogoal = goal - ball;
		Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		Vector2f balltoself = self - ball;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltotmmsecond = tmmsecondpos - ball;
		Vector2f balltoopp = opppos - ball;
		float length = balltoself.Length();
		float lengthballtotmm = balltotmm.Length();
		float lengthballtotmmsecond = balltotmmsecond.Length();
		float lengthballtoopp = balltoopp.Length();
		if(self.x() > ball.x() && lengthballtoopp >=0.5 && lengthballtoopp <= 3.0)
		{
			GoToPoint(opppos,1.4);
		}
		else
		{
			if((self.x() <= 5.0 - fieldlength/2))
			{
				Vector2f dest2 = Vector2f(4.5 - fieldlength/2 , (4.5 - fieldlength/2) * gTan(dest.GetAngleRad()));
				if(dest.x() > self.x())
				{
					GoToPointAndTurnTo(dest , (ball -goal).GetAngleDeg());
				}
				else
				{
					GoToPointAndTurnTo(dest2, (ball - goal).GetAngleDeg());
				}
			}
			else
			{
				if(self.x() > ball.x() -1.0)
				{
					if(self.y() >= ball.y())
					{
						GoToPoint(Vector2f(ball.x() - 2, ball.y() +2),1.4); 
					}
					else
					{
						GoToPoint(Vector2f(ball.x() - 2, ball.y() -2),1.4); 
					}
				}
				else
					GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg());
			}
		}
	}

}
/*
void	Strive3DBehaviorControl::Guard1DefDecision()
{
	bLOG << "In Guard1DefDecision" << endl;
	double dist = 5.0;
	static bool flagguard1 = 0;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& ourteamsecondtoball = WM.getOurTeamSecondToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(4);
	}
	else
	{
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f tmmsecondpos = ourteamsecondtoball.pos.to2D();
		Vector2f opppos = nearestplayertoball.pos.to2D();
		Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f goal = WM.getFieldInfo().ourgoalcenter.to2D()+Vector2f(0,0.3);
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f balltogoal = goal - ball;
		//Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), 0);
		Vector2f balltoself = self - ball;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltotmmsecond = tmmsecondpos - ball;
		Vector2f balltoopp = opppos - ball;
		float length = balltoself.Length();
		float lengthballtotmm = balltotmm.Length();
		float lengthballtotmmsecond = balltotmmsecond.Length();
		float lengthballtoopp = balltoopp.Length();
		if((self.x()<= 4.5 - fieldlength/2))
		{
			Vector2f dest2 = Vector2f(4.0 - fieldlength/2 , ball.y());
			if(dest.x() > self.x())
			{
				if(self.y() <= fieldwidth/2 + 0.3 && self.y() >= -fieldwidth/2-0.3)
					GoToPointAndTurnTo(dest , (ball -goal).GetAngleDeg());
				else if(self.y() > fieldwidth/2 + 0.3)
				{
					Vector2f dest3 = Vector2f(ball.x() + dist * gCos(balltogoal.GetAngleRad()),fieldwidth/2);
					GoToPointAndTurnTo(dest3 , (ball-goal).GetAngleDeg());
				}
				else
				{
					Vector2f dest4 = Vector2f(ball.x()+dist * gCos(balltogoal.GetAngleRad()),-fieldwidth/2);
					GoToPointAndTurnTo(dest4 , (ball-goal).GetAngleDeg());
				}
			}
			else
			{
				if(self.y() <= fieldwidth/2 + 0.3 && self.y() >= -fieldwidth/2-0.3)
					GoToPointAndTurnTo(dest2, (ball - goal).GetAngleDeg());
				else if(self.y() > fieldwidth/2 + 0.3)
				{
					Vector2f dest3 = Vector2f(4.0 - fieldlength/2,fieldwidth/2);
					GoToPointAndTurnTo(dest3 , (ball-goal).GetAngleDeg());
				}
				else
				{
					Vector2f dest4 = Vector2f(4.0 - fieldlength/2,-fieldwidth/2);
					GoToPointAndTurnTo(dest4 , (ball-goal).GetAngleDeg());
				}
			}
		}
		else
		{
			if(self.y() <= fieldwidth/2 + 0.3 && self.y() >= -fieldwidth/2-0.3)
				GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg());
			else if(self.y() > fieldwidth/2 + 0.3)
			{
				Vector2f dest3 = Vector2f(ball.x() + dist * gCos(balltogoal.GetAngleRad()),fieldwidth/2);
				GoToPointAndTurnTo(dest3 , (ball-goal).GetAngleDeg());
			}
			else
			{
				Vector2f dest4 = Vector2f(ball.x()+dist * gCos(balltogoal.GetAngleRad()),-fieldwidth/2);
				GoToPointAndTurnTo(dest4 , (ball-goal).GetAngleDeg());
			}
		}
	}
}
*/
void	Strive3DBehaviorControl::Guard1DefDecision()
{
	bLOG << "In Guard1DefDecision" << endl;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& ourteamsecondtoball = WM.getOurTeamSecondToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(4);
	}
	else
	{
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		float goalwidth = WM.getFieldInfo().goalwidth;
		float goaldepth = WM.getFieldInfo().goaldepth;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f tmmsecondpos = ourteamsecondtoball.pos.to2D();
		Vector2f opppos = nearestplayertoball.pos.to2D();
		Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f goal = WM.getFieldInfo().ourgoalcenter.to2D() + Vector2f(0,0.3);
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f balltogoal = goal - ball;
		Vector2f goaltoball = ball - goal;
		Vector2f balltoself = self - ball;
		Vector2f selftoball = ball - self;
		Vector2f balltogoalin = goalin - ball;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltotmmsecond = tmmsecondpos - ball;
		Vector2f balltoopp = opppos - ball;
		float length = balltoself.Length();
		float lengthballtotmm = balltotmm.Length();
		float lengthballtotmmsecond = balltotmmsecond.Length();
		float lengthballtoopp = balltoopp.Length();
//		Vector2f dest;		
		if(WM.getHitout() == true)
		{
			bLOG << "mCanHitout:" << WM.getHitout() << endl;
			Forward1DefDecision();
		}
		else
		{
//			if(ball.y() >= 0)
//				dest = Vector2f(3.1 - fieldlength/2, 0.3);
//			else
//				dest = Vector2f(3.1 - fieldlength/2, -0.3);
//			GoToPointAndTurnTo(dest, selftoball.GetAngleDeg());
			float funcgoalball_k=(goal.y() - ball.y())/(goal.x() - ball.x());
			float funcgoalball_b=goal.y() - funcgoalball_k*goal.x();
			float funcoppball_k = (opppos.y() - ball.y())/(opppos.x() - ball.x());
			float funcoppball_b = ball.y() -funcoppball_k * ball.x();
			float bottomline_y = -fieldlength/2 * funcoppball_k + funcoppball_b;
			if((bottomline_y <= goalwidth/2) && (bottomline_y >= -goalwidth/2) && (ball.x() < opppos.x()))
			{
				Vector2f dest = Vector2f(4.0 - fieldlength/2, (4.0 - fieldlength/2)*funcoppball_k + funcoppball_b);
				GoToPointAndTurnTo(dest, balltoopp.GetAngleDeg());
			}
			else
			{
				Vector2f dest = Vector2f(4.0 - fieldlength/2, (4.0 - fieldlength/2)*funcgoalball_k + funcgoalball_b);
				GoToPointAndTurnTo(dest, goaltoball.GetAngleDeg());
			}
		}
	}
}

void	Strive3DBehaviorControl::Guard2DefDecision()
{
	bLOG << "In Guard2DefDecision" << endl;
	double dist = 9.0;
	static bool flagguard2 = 0;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& ourteamsecondtoball = WM.getOurTeamSecondToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(5);
	}
	else
	{
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f tmmsecondpos = ourteamsecondtoball.pos.to2D();
		Vector2f opppos = nearestplayertoball.pos.to2D();
		Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f goal = WM.getFieldInfo().ourgoalcenter.to2D()+Vector2f(0,-0.3);
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f balltogoal = goal - ball;
		Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		Vector2f balltoself = self - ball;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltotmmsecond = tmmsecondpos - ball;
		Vector2f balltoopp = opppos - ball;
		float length = balltoself.Length();
		float lengthballtotmm = balltotmm.Length();
		float lengthballtotmmsecond = balltotmmsecond.Length();
		float lengthballtoopp = balltoopp.Length();
		if((self.x()<= 2.5 - fieldlength/2))
		{
			float k=(goal.y() - ball.y())/(goal.x() - ball.x());
			float b=goal.y() - k*goal.x();
			Vector2f dest2 = Vector2f(2.1 - fieldlength/2, (2.1 - fieldlength/2)*k + b);
			if(dest.x() > self.x())
			{
				GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg());
			}
			else
			{
				GoToPointAndTurnTo(dest2, (ball - goal).GetAngleDeg());
			}
		}
		else
		{
			GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg());
		}
	}		
}

void	Strive3DBehaviorControl::Guard3DefDecision()
{
	bLOG << "In Guard1DefDecision" << endl;
	double dist = 5.0;
	static bool flagguard1 = 0;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& ourteamsecondtoball = WM.getOurTeamSecondToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(4);
	}
	else
	{
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f tmmsecondpos = ourteamsecondtoball.pos.to2D();
		Vector2f opppos = nearestplayertoball.pos.to2D();
		Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f goal;
		if(ball.y() > -3)
			 goal = Vector2f(-fieldlength/2,fieldwidth/2 - 2);
		if(ball.y() < 3)
			 goal = Vector2f(-fieldlength/2,-fieldwidth/2 + 2);
		Vector2f balltogoal = goal - ball;
		Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		Vector2f balltoself = self - ball;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltotmmsecond = tmmsecondpos - ball;
		Vector2f balltoopp = opppos - ball;
		float length = balltoself.Length();
		float lengthballtotmm = balltotmm.Length();
		float lengthballtotmmsecond = balltotmmsecond.Length();
		float lengthballtoopp = balltoopp.Length();
		if((self.x()<= 4.0 - fieldlength/2))
		{
//			Vector2f dest2 = Vector2f(4.0 - fieldlength/2 , ball.y());
			Vector2f dest2 = Vector2f(3.5 - fieldlength/2 , (3.5 - fieldlength/2) * gTan(dest.GetAngleRad()));
			if(dest.x() > self.x())
				GoToPointAndTurnTo(dest , (ball -goal).GetAngleDeg());
			else
				GoToPointAndTurnTo(dest2, (ball - goal).GetAngleDeg());
		}
		else
			GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg());
	}
}

void	Strive3DBehaviorControl::Guard4DefDecision()
{
	bLOG << "In Cuard3DefDecision" << endl;
	double dist = 6.3;
	static bool flagcenter = 0;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& ourteamsecondtoball = WM.getOurTeamSecondToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(6.0);
	}
	else
	{
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f tmmsecondpos = ourteamsecondtoball.pos.to2D();
		Vector2f opppos = nearestplayertoball.pos.to2D();
		Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f goal;
		if(ball.y() > -3)
			 goal = Vector2f(-fieldlength/2,-fieldwidth/2 + 2);
		if(ball.y() < 3)
			 goal = Vector2f(-fieldlength/2,fieldwidth/2 - 2);
		Vector2f balltogoal = goal - ball;
		Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		Vector2f balltoself = self - ball;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltotmmsecond = tmmsecondpos - ball;
		Vector2f balltoopp = opppos - ball;
		float length = balltoself.Length();
		float lengthballtotmm = balltotmm.Length();
		float lengthballtotmmsecond = balltotmmsecond.Length();
		float lengthballtoopp = balltoopp.Length();
		if((self.x() <= 3.5 - fieldlength/2))
		{
			Vector2f dest2 = Vector2f(3.0 - fieldlength/2 , (3.0 - fieldlength/2) * gTan(dest.GetAngleRad()));
			if(dest.x() > self.x())
			{
				GoToPointAndTurnTo(dest , (ball -goal).GetAngleDeg());
			}
			else
			{
				GoToPointAndTurnTo(dest2, (ball - goal).GetAngleDeg());
			}
		}
		else
		{
			if(self.x() > ball.x() -0.7)
			{
				if(self.y() >= ball.y())
				{
					GoToPoint(Vector2f(ball.x() - 2, ball.y() +2),1.4); 
				}
				else
				{
					GoToPoint(Vector2f(ball.x() - 2, ball.y() -2),1.4); 
				}
			}
			else
				GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg());
		}
	}
}

void	Strive3DBehaviorControl::Guard5DefDecision()
{
	bLOG << "In Cuard3DefDecision" << endl;
	double dist = 8.0;
	static bool flagcenter = 0;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& ourteamsecondtoball = WM.getOurTeamSecondToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(6.0);
	}
	else
	{
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f tmmsecondpos = ourteamsecondtoball.pos.to2D();
		Vector2f opppos = nearestplayertoball.pos.to2D();
		Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f goal;
		if(ball.y() > -2)
			 goal = Vector2f(-fieldlength/2,-penaltywidth/2);
		if(ball.y() < 2)
			 goal = Vector2f(-fieldlength/2,penaltywidth/2);
		Vector2f balltogoal = goal - ball;
		Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		Vector2f balltoself = self - ball;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltotmmsecond = tmmsecondpos - ball;
		Vector2f balltoopp = opppos - ball;
		float length = balltoself.Length();
		float lengthballtotmm = balltotmm.Length();
		float lengthballtotmmsecond = balltotmmsecond.Length();
		float lengthballtoopp = balltoopp.Length();
		if((self.x() <= 1.5 - fieldlength/2))
		{
			Vector2f dest2 = Vector2f(1.0 - fieldlength/2 , (1.0 - fieldlength/2) * gTan(dest.GetAngleRad()));
			if(dest.x() > self.x())
			{
				GoToPointAndTurnTo(dest , (ball -goal).GetAngleDeg());
			}
			else
			{
				GoToPointAndTurnTo(dest2, (ball - goal).GetAngleDeg());
			}
		}
		else
		{
			if(self.x() > ball.x() -0.7)
			{
				if(self.y() >= ball.y())
				{
					GoToPoint(Vector2f(ball.x() - 2, ball.y() +2),1.4); 
				}
				else
				{
					GoToPoint(Vector2f(ball.x() - 2, ball.y() -2),1.4); 
				}
			}
			else
				GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg());
		}
	}
}
/*
void	Strive3DBehaviorControl::Guard5DefDecision()
{
	bLOG << "In Guard4DefDecision" << endl;
	double dist = 10.0;
	static bool flagcenter = 0;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& ourteamsecondtoball = WM.getOurTeamSecondToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(7.0);
	}
	else
	{
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f tmmsecondpos = ourteamsecondtoball.pos.to2D();
		Vector2f opppos = nearestplayertoball.pos.to2D();
		Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f goal = WM.getFieldInfo().ourgoalcenter.to2D();
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f balltogoal = goal - ball;
		Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		Vector2f balltoself = self - ball;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltotmmsecond = tmmsecondpos - ball;
		Vector2f balltoopp = opppos - ball;
		float length = balltoself.Length();
		float lengthballtotmm = balltotmm.Length();
		float lengthballtotmmsecond = balltotmmsecond.Length();
		float lengthballtoopp = balltoopp.Length();
		if((self.x() <= 3.5 - fieldlength/2))
		{
			Vector2f dest2 = Vector2f(3.0 - fieldlength/2 , (3.0 - fieldlength/2) * gTan(dest.GetAngleRad()));
			if(dest.x() > self.x())
			{
				GoToPointAndTurnTo(dest , (ball -goal).GetAngleDeg());
			}
			else
			{
				GoToPointAndTurnTo(dest2, (ball - goal).GetAngleDeg());
			}
		}
		else
		{
			if(self.x() > ball.x() -0.7)
			{
				if(self.y() >= ball.y())
				{
					GoToPoint(Vector2f(ball.x() - 2, ball.y() +2),1.4); 
				}
				else
				{
					GoToPoint(Vector2f(ball.x() - 2, ball.y() -2),1.4); 
				}
			}
			else
				GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg());
		}
	}
}
*/
void	Strive3DBehaviorControl::KeepGuard2DefDecision()
{
	bLOG << "In Guard2DefDecision" << endl;
	double dist = 9.0;
	static bool flagguard2 = 0;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& ourteamsecondtoball = WM.getOurTeamSecondToBall();
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	if(&nearestplayertoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "nearestplayertoball: " << &nearestplayertoball << endl;
		CoverTeammate(5);
	}
	else
	{
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		float goalwidth = WM.getFieldInfo().goalwidth;
		float goaldepth = WM.getFieldInfo().goaldepth;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f tmmsecondpos = ourteamsecondtoball.pos.to2D();
		Vector2f opppos = nearestplayertoball.pos.to2D();
		Vector2f goalin = WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f goal = WM.getFieldInfo().ourgoalcenter.to2D() -Vector2f(0,0.3);
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f balltogoal = goal - ball;
		Vector2f goaltoball = ball - goal;
//		Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		Vector2f balltoself = self - ball;
		Vector2f selftoball = ball - self;
		Vector2f balltogoalin = goalin - ball;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltotmmsecond = tmmsecondpos - ball;
		Vector2f balltoopp = opppos - ball;
		float length = balltoself.Length();
		float lengthballtotmm = balltotmm.Length();
		float lengthballtotmmsecond = balltotmmsecond.Length();
		float lengthballtoopp = balltoopp.Length();
		Vector2f dest;
/*
		if((self.x()<= 2.5 - fieldlength/2))
		{
			float funcgoalball_k=(goal.y() - ball.y())/(goal.x() - ball.x());
			float funcgoalball_b=goal.y() - funcgoalball_k*goal.x();
			float funcoppball_k = (opppos.y() - ball.y())/(opppos.x() - ball.x());
			float funcoppball_b = ball.y() -funcoppball_k * ball.x();
			float bottomline_y = -fieldlength/2 * funcoppball_k + funcoppball_b;
			if((bottomline_y <= goalwidth/2) && (bottomline_y >= -goalwidth/2) && (ball.x() < opppos.x()))
			{
				Vector2f dest = Vector2f(2.0- fieldlength/2, (2.0 - fieldlength/2)*funcoppball_k + funcoppball_b);
				GoToPointAndTurnTo(dest, balltoopp.GetAngleDeg());
			}
			else
			{
				Vector2f dest = Vector2f(2.0- fieldlength/2, (2.0 - fieldlength/2)*funcgoalball_k + funcgoalball_b);
				GoToPointAndTurnTo(dest, goaltoball.GetAngleDeg());
			}
		}
		else
		{
			float funcgoalball_k=(goal.y() - ball.y())/(goal.x() - ball.x());
			float funcgoalball_b=goal.y() - funcgoalball_k*goal.x();
			Vector2f dest = Vector2f(2.0- fieldlength/2, (2.0 - fieldlength/2)*funcgoalball_k + funcgoalball_b);
			GoToPointAndTurnTo(dest, goaltoball.GetAngleDeg());
		}
*/
		if(WM.getHitout() == true)
		{
			bLOG << "mCanHitout:" << WM.getHitout() << endl;
			Forward1DefDecision();
		}
		else
		{
//			if(ball.y() >= 0)
//				dest = Vector2f(2.3 - fieldlength/2, -0.3);
//			else
//				dest = Vector2f(2.3 - fieldlength/2, 0.3);
//			GoToPointAndTurnTo(dest,selftoball.GetAngleDeg());
			float funcgoalball_k=(goal.y() - ball.y())/(goal.x() - ball.x());
			float funcgoalball_b=goal.y() - funcgoalball_k*goal.x();
			float funcoppball_k = (opppos.y() - ball.y())/(opppos.x() - ball.x());
			float funcoppball_b = ball.y() -funcoppball_k * ball.x();
			float bottomline_y = -fieldlength/2 * funcoppball_k + funcoppball_b;
			if((bottomline_y <= goalwidth/2) && (bottomline_y >= -goalwidth/2) && (ball.x() < opppos.x()))
			{
				Vector2f dest = Vector2f(2.3- fieldlength/2, (2.3 - fieldlength/2)*funcoppball_k + funcoppball_b);
				GoToPointAndTurnTo(dest, balltoopp.GetAngleDeg());
			}
			else
			{
				Vector2f dest = Vector2f(2.3- fieldlength/2, (2.3 - fieldlength/2)*funcgoalball_k + funcgoalball_b);
				GoToPointAndTurnTo(dest, goaltoball.GetAngleDeg());
			}
		}
	}
}

bool	Strive3DBehaviorControl::GoalieDecision()
{

    bLOG << "Num: " << WM.getSelf().GetUnum() << "GoalieDecision " << goliestate << endl;
		 static bool phase = 0;
	switch (phase)
	{	case 0:
		{ 
			if (!GoBeHindBall())
			{ 
                phase = 0;
			}
		
            else
        	{
				phase = 1;
		   	}
		}
       		 break;
		case 1:
		{	 
			if(!WM.CanLocalize())
// 			if(!WM.SeeBall() || !WM.CanLocalize())
				phase = 0;
			else
			{	
				Player& oppteamnearesttoball = WM.getOppTeamNearestToBall();
				Player& ourteamnearesttoball = WM. getOurTeamNearestToBall();
				if(&oppteamnearesttoball == NULL)
				{
					bLOG << "Point Is NULL!" << endl;
					bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
					bLOG << "oppteamnearesttoball: " << &oppteamnearesttoball << endl;
					newgoliestatejudge();
				}
				else
				{
					Player& self = WM.getSelf();
					Vector2f opppos = oppteamnearesttoball.pos.to2D();
					Vector2f KickTarget;
					float fieldlength = WM.getFieldInfo().fieldlength;
					float fieldwidth =  WM.getFieldInfo().fieldwidth;
					if(opppos.y() > 0)
						KickTarget = Vector2f(0,  -fieldwidth/2);
					else
						KickTarget =Vector2f(0,  fieldwidth/2);
					if (WM.getFieldInfo().play_mode == PM_GOAL_KICK_LEFT)
					{
						if ( WM.getFieldInfo().team_index == TI_LEFT && ourteamnearesttoball.GetUnum() == self.GetUnum())
						{
							actionRequest.kickRequest.type = 1;
							actionRequest.kickRequest.Direction = 0.0 ;
							KickTo(KickTarget);
						}
						else
							newgoliestatejudge();
					}
					else if ( WM.getFieldInfo().play_mode == PM_GOAL_KICK_RIGHT)
					{
						if (  WM.getFieldInfo().team_index == TI_RIGHT && ourteamnearesttoball.GetUnum() == self.GetUnum())
						{
							actionRequest.kickRequest.type = 1;
							actionRequest.kickRequest.Direction = 0.0 ;
							KickTo(KickTarget);
						}
						else
							newgoliestatejudge();
					}
					else
						newgoliestatejudge();
				}
				phase = 1;
			}
		}
		break;
 	}
}

bool Strive3DBehaviorControl::GoToBallForShoot(Vector2f destination)
{
    Vector2f ball = WM.getBall().pos.to2D();
    Vector2f tmp = ball - destination;
    Vector2f selftoball = WM.getSelf().pos.to2D() - ball;
    double dist = 0.16;
    double rad = tmp.GetAngleRad();
    tmp.x() = dist * gCos(rad);
    tmp.y() = dist * gSin(rad);
    tmp = ball + tmp;
//     Vector2f offset(0.05 * gCos(rad + 0.5), 0.05 * gSin(rad + 0.5));
//     tmp += offset;
    Vector2f vectforangle = destination - ball;
    double angle = vectforangle.GetAngleDeg();
    bLOG << "GoToBallForShoot: " << ball << " tmp: " << tmp << " pos: " << WM.getSelf().pos << endl;

    if (WM.getSelf().pos.x() > WM.getBall().pos.x() + 0.05)
    {
        GoToBallToTargetLine();
        return false;
    }
    else
        return GoToPointAndTurnTo(tmp, angle);
}

void Strive3DBehaviorControl::GoAroundBall(bool clockwise)
{
    Vector3f ball3f = WM.getBall().GetLocalPos();
    Vector2f localball = ball3f.to2D();
    Vector2f localdest;
	Vector2f verticalvect;
	float	dist = 0.4;
	bLOG << "localball: " << localball << endl;
	if(clockwise)
	{
		verticalvect = Vector2f(dist * gCos(localball.GetAngleRad() /*+ M_PI/2*/), dist * gSin(localball.GetAngleRad() /*+ M_PI/2*/));
		bLOG << "ClockWise!" << endl;
	}
	else
	{
		verticalvect = Vector2f(dist * gCos(localball.GetAngleRad() /*- M_PI/2*/), dist * gSin(localball.GetAngleRad() /*- M_PI/2*/));
		bLOG << "UnClockWise!" << endl;
	}
	localdest = localball + verticalvect;
	bLOG << "verticalvect: " << verticalvect << "\tlocaldest: " << localdest << endl;
	GoToRelativePoint(localdest);
}

bool Strive3DBehaviorControl::GoBeHindBall()
{
	bLOG << "***********************In GoBeHindBall*********************" << endl;
	float fieldwidth = WM.getFieldInfo().fieldwidth;
	float fieldlength = WM.getFieldInfo().fieldlength;
	float penaltywidth = WM.getFieldInfo().penaltywidth;
	float penaltylength = WM.getFieldInfo().penaltylength;
	Vector2f agentpos = WM.getSelf().pos.to2D();
	Vector2f ballpos = WM.getBall().pos.to2D();
	Vector2f vectballtoagent = ballpos - agentpos;
	float angleballtoagent = gNormalizeDeg(vectballtoagent.GetAngleDeg());
	bLOG << "agentpos: " << agentpos 
		<< " ballpos: " << agentpos 
		<< " vectballtoagent: " << vectballtoagent 
		<< " vectballtoagent.GetAngleDeg: " << angleballtoagent << endl;
//	if(!WM.CanLocalize() || gAbs(angleballtoagent) > 20)
	if((!WM.CanLocalize() /*|| gAbs(angleballtoagent) > 20*/) && ((ballpos.x() < penaltylength -fieldlength/2) && ( ballpos.y() <= penaltywidth/2) && (ballpos.y() >= -penaltywidth/2 )))
	{
		if(ballpos.y() > agentpos.y())
			GoAroundBall(true);
		else
			GoAroundBall(false);
		return false;
	}
	else
	{
		return true;
	}
}

bool Strive3DBehaviorControl::LocalViewGoToBallForShoot(salt::Vector2f destination)
{
    double dist = 0.5;
    Vector3f ball3f = WM.getBall().GetLocalPos();
    Vector2f ball = ball3f.to2D();
    Vector2f tmp = ball - destination;
    double angle = tmp.GetAngleRad();
    tmp = tmp * 0.15 / tmp.Length();
    tmp = ball + tmp;

    if (tmp.Length() > 0.1)
    {
        GoToRelativePoint(tmp);
        cout<<"GoToRelativePoint"<<endl;
        return false;
    }
    else
        return true;
}

bool Strive3DBehaviorControl::IsCanShoot()
{
	static bool shootflag = false;
	Player& nearestopptoball = WM.getOppTeamNearestToBall();
	if(&nearestopptoball == NULL)
	      return true;
	else
	{  
		Vector2f ballpos = WM.getBall().pos.to2D();
		Vector2f agentpos = WM.getSelf().pos.to2D();
		Vector2f opppos = WM.getOppTeamNearestToBall().pos.to2D();
		Vector2f agenttoball = ballpos - agentpos;
		Vector2f balltoopp = opppos - ballpos;
		float torso_angle = gNormalizeDeg(WM.getSelf().GetTorsoYawAngle());
		float agenttoball_angle = gNormalizeDeg(agenttoball.GetAngleDeg());
		float balltoopp_angle = gNormalizeDeg(balltoopp.GetAngleDeg());
		float diffangle = gAbs(gNormalizeDeg(agenttoball_angle - balltoopp_angle));
		bLOG << "shootflag:" << shootflag << endl;
		switch(shootflag)
		{
		      case false:
			    if((agenttoball.Length() <= 0.25) && (agentpos.x() < ballpos.x()) && (balltoopp.Length() <= 1.0))
			    {
				shootflag = false;
				bLOG << "ppp111111" << endl;
			    }
			    else
			    {
				shootflag = true;
				bLOG << "ppp222222" << endl;
			    }
			    break;
		      case true:
			    if(balltoopp.Length() <= 0.1)
			    {
				shootflag = false;
				bLOG << "ppp33333" << endl;
			    }
			    else
			    {
				shootflag = true;
				bLOG << "ppp4444" << endl;
			    }
			    break;
		}
		return shootflag;
	}
}

bool Strive3DBehaviorControl::IsCanPassout()
{
	static bool passflag = false;
	Player& nearestopptoball = WM.getOppTeamNearestToBall();
	if(&nearestopptoball == NULL)
	      return true;
	else
	{  
		Vector2f ballpos = WM.getBall().pos.to2D();
		Vector2f agentpos = WM.getSelf().pos.to2D();
		Vector2f opppos = WM.getOppTeamNearestToBall().pos.to2D();
		Vector2f agenttoball = ballpos - agentpos;
		Vector2f balltoopp = opppos - ballpos;
		float torso_angle = gNormalizeDeg(WM.getSelf().GetTorsoYawAngle());
		float agenttoball_angle = gNormalizeDeg(agenttoball.GetAngleDeg());
		float balltoopp_angle = gNormalizeDeg(balltoopp.GetAngleDeg());
		float diffangle = gAbs(gNormalizeDeg(agenttoball_angle - balltoopp_angle));
		float difftorsoangle = gAbs(gNormalizeDeg(torso_angle - agenttoball_angle));
		bLOG << "passflag:" << passflag << endl;
		switch(passflag)
		{
		      case false:
			    if((agenttoball.Length() <= 0.25) && (agentpos.x() < ballpos.x()) && (gAbs(agenttoball_angle) <= 60) && (difftorsoangle <= 30) && (balltoopp.Length() > 3.0))
			    {
				passflag = true;
			    	bLOG << "ppp111111" << endl;
			    }
			    else
			    {
				passflag = false;
				bLOG << "ppp22222" << endl;
			    }
			    break;
		      case true:
			    if(balltoopp.Length() <= 0.1)
			    {
				passflag = false;
				bLOG << "ppp3333" << endl;
			    }
			    else
			    {
				passflag = true;
				bLOG << "ppp44444" << endl;
			    }
			    break;
		}
		return passflag;
	}
}

void Strive3DBehaviorControl::DivingSave(bool left)
{
		actionRequest.actionType = ActionRequest::specialAction;
		if(left)
			actionRequest.specialActionRequest.specialActionType = SpecialActionRequest::divingSaveLeft;
		else
			actionRequest.specialActionRequest.specialActionType = SpecialActionRequest::divingSaveRight;
		oLOG << "specialActionType in behavior: " << actionRequest.specialActionRequest.specialActionType << endl;
}

void Strive3DBehaviorControl::CoverTeammate(float dist)
{
	bLOG << "CoverTeammate" << endl;
    Vector2f goal = WM.getFieldInfo().ourgoalcenter.to2D();
    Vector2f ball = WM.getBall().pos.to2D();
    Vector2f balltogoal = goal - ball;

	if(dist > balltogoal.Length())
		dist = balltogoal.Length() * 2 / 3;

    Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		
    if(GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg()))
		Stand();
}

void Strive3DBehaviorControl::CoverTeammate(float dist, Vector2f offset)
{
	bLOG << "CoverTeammate" << endl;
    Vector2f goal = WM.getFieldInfo().ourgoalcenter.to2D();
    Vector2f ball = WM.getBall().pos.to2D();
    Vector2f balltogoal = goal - ball;
	
	if(dist > balltogoal.Length())
		dist = balltogoal.Length() * 2 / 3;

    Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
	dest += offset;

	if(dest.x() > -9.5)
	{
		if(GoToPointAndTurnTo(dest, (ball - goal).GetAngleDeg()))
			Stand();
	}
	else
	{
		Stand();
	}
}

void Strive3DBehaviorControl::ManToManDef(Player & player, float disttoball)
{
	Player& nearestplayertoball = WM.getOppTeamNearestToBall();
	Vector3f localBallPos = WM.getBall().GetLocalPos();
	Vector2f theNearestOppToBallPos = nearestplayertoball.pos.to2D();
	float distAgentToBall = localBallPos.to2D().Length();
	
}

void Strive3DBehaviorControl::ClearanceKick()
{
	Vector3f localBall3f = WM.getBall().GetLocalPos();
	Vector2f localDest = localBall3f.to2D() * 3;
	localDest.x() += -0.08;
	GoToRelativePoint(localDest);
}

void Strive3DBehaviorControl::ObstacleAvoidanceTo(const Vector2f goalPos)
{
    DEBUG<<"Strive3DBehaviorControl::ObstacleAvoidanceTo"<<endl;
    Vector2f agentPos = WM.getSelf().pos.to2D();
    Vector3f agentPos3f(agentPos.x(), agentPos.y(), 0);
// 	Vector2f goalPos = mDebugMsgParser.destination();
    const Vector3f goalPos3f(goalPos.x(), goalPos.y(), 0);

    Vector3f tmpPos = goalPos3f;
    //Vector3f distMinPos = goalPos3f;
    //Vector3f distMaxPos = goalPos3f;
    //Vector3f tmpGoal = goalPos3f;
    ConvexHullManager& chm =  WM.getSafeConvexHullManager();
    set<ConvexHull *> crossConvexHullSet;
    float distMin = 9999.9f;
    ConvexHull* nearestConvexHull = NULL;
    if (chm.IsSegmentCross(agentPos3f, goalPos3f,crossConvexHullSet))
    {
        for (	set<ConvexHull *>::iterator chit = crossConvexHullSet.begin();
                chit != crossConvexHullSet.end();
                chit++ )
        {
            ConvexHull* ch = *chit;
            Vector3f center(ch->Center().x(), ch->Center().y(), 0);
            Vector3f distVect = center - agentPos3f;
            if (distVect.Length() < distMin)
            {
                distMin = distVect.Length();
                nearestConvexHull = ch;
            }
        }

        float distMin = 9999.9f;
        float distMax = -9999.9f;
        Vector3f distMinPos = goalPos3f;
        Vector3f distMaxPos = goalPos3f;
        if (nearestConvexHull) //The nearest convex hull found
        {
            //DEBUG<<"WalkPathPlanningControl::ObstacleAvoidanceTo-->CH center is "<<nearestConvexHull->Center()<<endl;
            //Find the two possible way points
            list<Vector3f> vertexList = nearestConvexHull->GetPointList();
            for (list<Vector3f>::iterator it = vertexList.begin(); it != vertexList.end(); it++)
            {
                Vector3f vertex((*it).x(), (*it).y(), 0);
                //distMin
                Vector3f AB = vertex - agentPos3f;
                Vector3f AC = goalPos3f - agentPos3f;
                //Vector3f AC = goalPos3f - agentPos3f;
                Vector3f distVect = AB.Cross(AC);
                float dist = distVect.Length()/AC.Length();
                if (distVect.z() < 0)
                    dist = - dist;

                if (dist < distMin)
                {
                    distMin = dist;
                    distMinPos = vertex;
                }

                if (dist > distMax)
                {
                    distMax = dist;
                    distMaxPos = vertex;
                }
            }

            //select a way point
            float dist1 = Vector3f(distMinPos-agentPos3f).Length() + Vector3f(distMinPos-goalPos3f).Length();
            float dist2 = Vector3f(distMaxPos-agentPos3f).Length() + Vector3f(distMaxPos-goalPos3f).Length();
            if (dist1 < dist2)
                tmpPos = distMinPos;
            else
                tmpPos = distMaxPos;
        }

        //DEBUG<<"WalkPathPlanningControl::ObstacleAvoidanceTo-->distMin="<<distMin<<endl;
        //DEBUG<<"WalkPathPlanningControl::ObstacleAvoidanceTo-->distMax="<<distMax<<endl;
    }


    mTmpPosVect.clear();
    mTmpPosVect.push_back(agentPos);
    mTmpPosVect.push_back(tmpPos.to2D());
    mTmpPosVect.push_back(goalPos);
    GoToPoint(tmpPos.to2D(), 0.8);
    DEBUG<<"WalkPathPlanningControl::ObstacleAvoidanceTo-->GoToPoint: "<<tmpPos<<endl;
    return;
}

void Strive3DBehaviorControl::sendMsg2GuiDebugger()
{
    AA.autoSendMsg = false;
    //DEBUG<<"WalkPathPlanningControl::sendMsg2GuiDebugger"<<endl;
    //cout<<"AdvancedAgent::sendInfo2GuiDebugger-->(my_pos("<<WM.getSelf().pos<<"))"<<endl;
    //if (WM.getTmm(1).isValid())
    //	cout<<"AdvancedAgent::sendInfo2GuiDebugger-->WM.getTmm("<<1<<").pos = "<<WM.getTmm(0).pos<<endl;
    if (DEBUG_NET.isValid())
    {
        ostringstream o_msg;
        o_msg<<"<r>";
        o_msg<<"<c>"<<int(WM.getSelf().CurrentCycle)<<"</c>";
        o_msg<<"<rp>";
        o_msg<<"<dir>"<<WM.getSelf().GetTorsoYawAngle()<<"</dir>";
        o_msg<<"<pos>";
        o_msg<<"<t>Strive3D</t>";
        o_msg<<"<u>"<<WM.getSelf().GetUnum()<<"</u>";
        o_msg<<"<h>"<<WM.getSelf().pos<<"</h>";
        o_msg<<"<lla>"<<WM.getSelf().GetLeftArmGloblePosition()<<"</lla>";
        o_msg<<"<rla>"<<WM.getSelf().GetRightArmGloblePosition()<<"</rla>";
        o_msg<<"<lf>"<<WM.getSelf().GetLeftFootGloblePosition()<<"</lf>";
        o_msg<<"<rf>"<<WM.getSelf().GetRightFootGloblePosition()<<"</rf>";
        o_msg<<"</pos>";
        o_msg<<"</rp>";
        //o_msg<<"<c>"<<int(WM.getMySimTime()*100)<<"</c>";
        o_msg<<"<av>";
        //teammate position
        for (int i=0; i<MAX_TEAM_SIZE; i++)
        {
            if (WM.getTmm(i).isValid())
            {
                o_msg<<"<p>";
                o_msg<<"<t>Strive3D</t>";
                o_msg<<"<u>"<<WM.getTmm(i).GetUnum()<<"</u>";
                o_msg<<"<h>"<<WM.getTmm(i).GetHeadGloblePosition()<<"</h>";
                //o_msg<<"<h>"<<WM.getTmm(i).GetHeadGloblePosition()<<"</h>";
                o_msg<<"<lla>"<<WM.getTmm(i).GetLeftArmGloblePosition()<<"</lla>";
                o_msg<<"<rla>"<<WM.getTmm(i).GetRightArmGloblePosition()<<"</rla>";
                o_msg<<"<lf>"<<WM.getTmm(i).GetLeftFootGloblePosition()<<"</lf>";
                o_msg<<"<rf>"<<WM.getTmm(i).GetRightFootGloblePosition()<<"</rf>";
                o_msg<<"</p>";
            }
        }
        //opponent position
        for (int i=0; i<MAX_TEAM_SIZE; i++)
        {
            if (WM.getOpp(i).isValid())
            {
                o_msg<<"<p>";
                o_msg<<"<t>Oponent</t>";
                o_msg<<"<u>"<<WM.getOpp(i).GetUnum()<<"</u>";
                o_msg<<"<h>"<<WM.getOpp(i).GetHeadGloblePosition()<<"</h>";
                //o_msg<<"<h>"<<WM.getOpp(i).GetHeadGloblePosition()<<"</h>";
                o_msg<<"<lla>"<<WM.getOpp(i).GetLeftArmGloblePosition()<<"</lla>";
                o_msg<<"<rla>"<<WM.getOpp(i).GetRightArmGloblePosition()<<"</rla>";
                o_msg<<"<lf>"<<WM.getOpp(i).GetLeftFootGloblePosition()<<"</lf>";
                o_msg<<"<rf>"<<WM.getOpp(i).GetRightFootGloblePosition()<<"</rf>";
                o_msg<<"</p>";
            }
        }
        o_msg<<"<o>";
        o_msg<<"<role>ball</role>";
        o_msg<<"<id>1</id>";
        o_msg<<"<pos>"<<WM.getBall().pos<<"</pos>";
        o_msg<<"</o>";
        //convex hull
        o_msg<<"<chm>";
        ConvexHullManager& chm = WM.getConvexHullManager();
        vector< vector<Vector3f> >& chPosVect = chm.GetConvexHull();
        for (int i=0; i<chPosVect.size(); i++)
        {
            vector<Vector3f>& posVect = chPosVect[i];
            o_msg<<"<ch>";
            for (int j=0; j<posVect.size(); j++)
                o_msg<<"<pos>"<<posVect[j]<<"</pos>";
            o_msg<<"</ch>";
        }

        ConvexHullManager& safechm = WM.getSafeConvexHullManager();
        vector< vector<Vector3f> >& safechPosVect = safechm.GetConvexHull();
        for (int i=0; i<safechPosVect.size(); i++)
        {
            vector<Vector3f>& posVect = safechPosVect[i];
            o_msg<<"<ch>";
            for (int j=0; j<posVect.size(); j++)
                o_msg<<"<pos>"<<posVect[j]<<"</pos>";
            o_msg<<"</ch>";
        }
        o_msg<<"</chm>";

        o_msg<<"</av>";
        o_msg<<"<ad>";
        o_msg<<"<x>"<<actionRequest.walkRequest.walkParams.x()<<"</x>";
        o_msg<<"<y>"<<actionRequest.walkRequest.walkParams.y()<<"</y>";
        o_msg<<"<r>"<<actionRequest.walkRequest.rotation<<"</r>";
        //if (mDebugMsgParser.cmdType() == CT_Destination)// && mPathPlanningTest.finalWayPoints().size() > 0)
        //{
        o_msg<<"<wp>";
        //vector<Vector2f>& finalWayPoints = mPathPlanningTest.finalWayPoints();
        //for (int i=0; i<finalWayPoints.size(); i++)
        //{
        //o_msg<<"<pos>"<<finalWayPoints[i]<<"</pos>";
        //}
        for (int i=0; i<mTmpPosVect.size(); i++)
            o_msg<<"<pos>"<<mTmpPosVect[i]<<"</pos>";
        o_msg<<"</wp>";
        //}
        o_msg<<"</ad>";
        o_msg<<"</r>";
        //cout<<"AdvancedAgent::sendInfo2GuiDebugger-->o_msg="<<o_msg.str()<<endl;
        DEBUG<<"WalkPathPlanningControl::sendMsg2GuiDebugger-->o_msg="<<o_msg.str()<<endl;
        DEBUG_NET.PutMessage(o_msg.str());
    }
}

//2009,12,23 by ian
void Strive3DBehaviorControl:: defencestatejudge()
{
    aLOG<<"OPPREADYFORSHOOT :"<< WM.Oppreadyforshoot()<<endl;


    if ( WM.Oppshooting() )
        defencestate = oppshooting;
    else if ( WM.Oppreadyforshoot() )
        defencestate = oppoppreadyforshoot;
//     else if(WM.getBall().GetBallLocalPos().x() > 13.0)
//	defencestate = nobehavior;
    else
        defencestate = oppdribbling;
}

void Strive3DBehaviorControl::goliestatejudge()
{
    aLOG << "111111111111111111111111 GOALIESTATEJUDGE11111111111111111111111 " << endl;

    defencestatejudge();
    switch ( defencestate )
    {
    case oppshooting :
    {
        goliestate = divingsave;
        aLOG << "goaliestate = divingsave!"<<endl;
    }
    break;
    case oppoppreadyforshoot :
    {
        goliestate = getreadyfordivingsave;
        aLOG << "goaliestate = getreadyfordivingsave!"<<endl;
    }
//    case(nobehavior):
//    {
//	 goliestate = stand;
 //    }
	break;
    case oppdribbling :
    {
        float dist = Vector2f(WM.getBall().pos.to2D() - Vector2f(- WM.getFieldInfo().fieldlength/2,0)).Length();
        //float phi = (WM.getBall().pos.to2D()-Vector2f(- WM.getFieldInfo().fieldlength/2,0)).GetAngleDeg();
        if ( dist <=2.5 || (WM.getBall().pos.x()<-7.8 && dist < 5.0  ))
        {
        //    goliestate = returntocenter;
       //     aLOG<< "goaliestate = returntocenter"<<endl;
       // }
      //  else if ( dist >1.2 && dist <2.5 && gAbs( phi ) <=80 && WM.getPlayerNearestToBall().GetName() != WM.getFieldInfo().our_teamname )
       // {
            goliestate = debarrass;
            aLOG<< "goaliestate = debarrass"<<endl;
        }
        else
        {
            goliestate = goaround;
            aLOG<< "goaliestate = goaround"<<endl;
        }
    }
    break;

    }

	switch ( goliestate )
    {
    case divingsave :
    {	if((gAbs(Vector2f(WM.getBall().GetLocalPos().x(),WM.getBall().GetLocalPos().y()).Length()) < 5.0 )&&(gAbs(Vector2f(WM.getBall().GetLocalPos().x(),WM.getBall().GetLocalPos().y()).Length()) > 3.5 ))
        {
			DivingSave(true);
		}
		else 
			goliestate= goaround ;
        //actionRequest.actionType = ActionRequest::getup;
        //GoToPointAndTurnTo( Vector2f(-5.5 ,0), (WM.getBall().pos.to2D()-Vector2f(-5.5,0)).GetAngleDeg() );/*DIVINGSAVE();*/
        //GoToPointAndTurnTo( Vector2f (-6,0) , (WM.getBall().pos.to2D()-Vector2f(-6,0)).GetAngleDeg() );
        aLOG<<"DIVINGSAVE SUCCEED!"<<endl;
    }
    break;
    case getreadyfordivingsave :
    {
        float thetaoppandball = (WM.getOppTeamNearestToBall().pos.to2D()-WM.getBall().pos.to2D()).GetAngleDeg() ;
        float destY = WM.getBall().GetLocalPos().y() - ((  WM.getFieldInfo().fieldlength/2+WM.getBall().GetLocalPos().x() )*gSin(thetaoppandball)/gCos(thetaoppandball));
        float destX = gSin(thetaoppandball)/gCos(thetaoppandball)*((gAbs(destY)*0.8)-WM.getBall().GetLocalPos().y()) + WM.getBall().GetLocalPos().x();

        if (gAbs(destY) <= 1.02)
            GoToPointAndTurnTo( Vector2f(0.4- WM.getFieldInfo().fieldlength/2, destY/2.5 ), thetaoppandball );
        else
            GoToPointAndTurnTo(Vector2f( destX ,0.8*gSign(destY)), (WM.getBall().pos.to2D()-Vector2f(- WM.getFieldInfo().fieldlength/2,0)).GetAngleDeg());
        aLOG<<"GETREADY FOR DIVINGSAVE!"<<endl;
    }
	break;
//    case returntocenter :
//   {
 //       float theta1 = (WM.getBall().pos.to2D()-Vector2f(- WM.getFieldInfo().fieldlength/2,0)).GetAngleDeg();
 //       GoToPointAndTurnTo( Vector2f(0.5- WM.getFieldInfo().fieldlength/2,0), theta1 );
  //      aLOG<<"RETURNTOCENTER!"<<endl;
   // }
//	break;
    case debarrass :
    {		
	//	 static bool phase = 0;
	//switch (phase)
	//{	case 0:
	//	{ if (WM.CanLocalize() == false && GoBeHindBall() == false)
	//		{ GoBeHindBall();
         //          	  phase = 0;
	//		}
		
        //          else
        //	   {
	//		phase = 1;
	//	   }
	//	}
       	//	 break;
		
// 	float angle = (WM.getBall().pos.to2D()-WM.getSelf().pos.to2D()).GetAngleDeg();
// 	DribbleTo(Vector2f(WM.getBall().pos.x()<0.0 ? (WM.getBall().pos.x()+2.0) : (WM.getBall().pos.x()-2.0) , WM.getBall().pos.y()+2/tan(angle) ) );
		Vector2f ballpos = WM.getBall().pos.to2D();
		Vector2f agentpos = WM.getSelf().pos.to2D();
		Vector2f agent2ball = ballpos - agentpos;
		GoToPoint(agentpos+agent2ball*3, 2);
       // GoToRelativePoint( Vector2f ((WM.getBall().GetBallLocalPos()[0]+(Vector2f(WM.getBall().GetBallLocalPos().x(),WM.getBall().GetBallLocalPos().y()).Length()/0.75*WM.getBall().vel[0])) * 1.8 , (WM.getBall().GetBallLocalPos()[1]+(Vector2f(WM.getBall().GetBallLocalPos().x(),WM.getBall().GetBallLocalPos().y()).Length()/0.75*WM.getBall().vel[1])) * 1.8 ));/*do debarrass*/
        aLOG<<"DEBARRASS SUCCEED!"<<endl;
    }
    break;
    case goaround :
    {
        aLOG<<"GOALIE GOINGAROUND!"<<endl;

        float ydestination = 0.5 * WM.getBall().pos.to2D().y() / ( WM.getFieldInfo().fieldlength/2 +WM.getBall().pos.to2D().x() ) ;
        float theta2 = (WM.getBall().pos.to2D()-Vector2f(-WM.getFieldInfo().fieldlength/2 ,0)).GetAngleDeg();

        if (WM.getBall().pos.to2D()[0] > 0)
            GoToPointAndTurnTo( Vector2f(1-WM.getFieldInfo().fieldlength/2 ,0), theta2 );

        else if (WM.getBall().pos.to2D()[0] > -4.7)
            GoToPointAndTurnTo( Vector2f(0.6- WM.getFieldInfo().fieldlength/2 , gAbs(ydestination)<=0.7 ? ydestination : gSign(ydestination)*0.7 ), theta2 );

        else
            GoToPointAndTurnTo( Vector2f(0.6- WM.getFieldInfo().fieldlength/2 , gAbs(ydestination)<=0.7 ? ydestination : gSign(ydestination)*0.7 ), theta2 );

    }
    break;
    }
}
void Strive3DBehaviorControl::newgoliestatejudge()
{
	aLOG <<"111111111111111111111NEWGOLIESTATEJUDGE111111111111111111111"<<endl;
	Player& ourteamnearesttoball = WM.getOurTeamNearestToBall();
	Player& oppteamnearesttoball = WM.getOppTeamNearestToBall();
	Player & nearestplayertoball = WM.getPlayerNearestToBall();
	if(&oppteamnearesttoball == NULL)
	{
		bLOG << "Point Is NULL!" << endl;
		bLOG << "ourteamnearesttoball: " << &ourteamnearesttoball << endl;
		bLOG << "oppteamnearesttoball: " << &nearestplayertoball << endl;
		Stand();
	}
	else
	{
		static bool flaggolie = 0;
		float fieldwidth = WM.getFieldInfo().fieldwidth;
		float fieldlength = WM.getFieldInfo().fieldlength;
		float penaltywidth = WM.getFieldInfo().penaltywidth;
		float penaltylength = WM.getFieldInfo().penaltylength;
		float goalwidth = WM.getFieldInfo().goalwidth;
		float goaldepth = WM.getFieldInfo().goaldepth;
		Vector2f tmmpos = ourteamnearesttoball.pos.to2D();
		Vector2f opppos = oppteamnearesttoball.pos.to2D();
		Vector2f self = WM.getSelf().pos.to2D();
		Vector2f goalin =  WM.getFieldInfo().oppgoalcenter.to2D();
		Vector2f goal = WM.getFieldInfo().ourgoalcenter.to2D();
		Vector2f ball = WM.getBall().pos.to2D();
		Vector2f goaltoball = ball - goal;
		Vector2f balltogoalin = goalin - ball;
		//Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), dist * gSin(balltogoal.GetAngleRad()));
		//Vector2f dest = ball + Vector2f(dist * gCos(balltogoal.GetAngleRad()), 0);
		Vector2f selftoball = ball - self;
		Vector2f balltotmm = tmmpos - ball;
		Vector2f balltoopp = opppos - ball;
		float lengthballtotmm = balltotmm.Length();
		float lengthballtoopp = balltoopp.Length();
		switch(flaggolie)
		{
		case 0:
			if((ball.x() <= penaltylength - fieldlength/2) && ( ball.y() <= penaltywidth/2) && (ball.y() >= -penaltywidth/2 ) && ((nearestplayertoball.GetName() != WM.getSelf().GetName()) || ((nearestplayertoball.GetName() == WM.getSelf().GetName()) && (nearestplayertoball.GetUnum() == 1))))
			{
				flaggolie = 1;
				break;
			}
			else if(ball.x() > 6-fieldlength/2)
			{
				Vector2f dest = Vector2f(0.6- WM.getFieldInfo().fieldlength/2,0);
				GoToPointAndTurnTo(dest,goaltoball.GetAngleDeg());
				break;
			}
			else if(ball.x() > 0.6- WM.getFieldInfo().fieldlength/2)
			{
				float funcgoalball_k=(goal.y() - ball.y())/(goal.x() - ball.x());
				float funcgoalball_b=goal.y() - funcgoalball_k*goal.x();
				float funcoppball_k = (opppos.y() - ball.y())/(opppos.x() - ball.x());
				float funcoppball_b = ball.y() -funcoppball_k * ball.x();
				float bottomline_y = -fieldlength/2 * funcoppball_k + funcoppball_b;
				if((bottomline_y <= goalwidth/2) && (bottomline_y >= -goalwidth/2) && (ball.x() < opppos.x()))
				{
					if((WM.Oppshooting())&&(gAbs(Vector2f(WM.getBall().GetLocalPos().x(),WM.getBall().GetLocalPos().y()).Length()) > 1.8) && (opppos.x() < 5.5 - fieldlength/2))
					{
						if(bottomline_y >= self.y())
						{
							DivingSave(true);
							aLOG<<"DIVINGSAVE LEFT SUCCEED!"<<endl;
							break;
						}
						else
						{
							DivingSave(false);
							aLOG<<"DIVINGSAVE RIGHT SUCCEED!"<<endl;
							break;
						}
					}
					else
					{
						Vector2f dest = Vector2f(0.6- fieldlength/2, (0.6 - fieldlength/2)*funcoppball_k + funcoppball_b);
						GoToPointAndTurnTo(dest, balltoopp.GetAngleDeg());
						break;
					}
				}
				else
				{
					Vector2f dest = Vector2f(0.6- fieldlength/2, (0.6 - fieldlength/2)*funcgoalball_k + funcgoalball_b);
					GoToPointAndTurnTo(dest, goaltoball.GetAngleDeg());
					break;
				}
			}
			else if(ball.y() >0)
			{
				Vector2f dest = Vector2f(0.4 - fieldlength/2,goalwidth/4 );
				GoToPointAndTurnTo(dest,goaltoball.GetAngleDeg());
				break;
			}
			else
			{
				Vector2f dest = Vector2f(0.4 - fieldlength/2,-goalwidth/4);
				GoToPointAndTurnTo(dest,goaltoball.GetAngleDeg());
				break;
			}
		case 1:
			if((!((ball.x() <= 3.0 - fieldlength/2) && ( ball.y() <= penaltywidth/2) && (ball.y() >= -penaltywidth/2 ))) || ((nearestplayertoball.GetName() == WM.getSelf().GetName()) && (nearestplayertoball.GetUnum() != 1)))
			{	
				flaggolie = 0;
				break;
			}
			else
			{
//				GoToKicktheBall(1.0);
				float sideparam = 6.0/(6.0 - gAbs(ball.y()));
				float baseparam = 6.0/(9.0 - ball.x());
				if(self.x() < ball.x())
				{
				  	if(IsCanPassout() == false)
					{
						Vector2f dest = ball /*+ selftoball.Normalized() * 20*/;
						DribbleTo(dest);
						break;
					}
					else
					{
						Vector2f dest = ball + selftoball.Normalized() * 20;
						KickTo(dest);
						break;
					}
				}
				else
				{
					Vector2f dest = sideparam * baseparam * balltogoalin + selftoball + ball;
					DribbleTo(dest);
					break;
				}
			}
		}
	}
}

float Strive3DBehaviorControl::FuzzyControl(float distance,float angle)
{
	bLOG << "Fuzzy Control" << endl;
	float Kd=4.0/3;
	float Ka=4.0/45;
	float Kt=45.0/6;
	int Dn;
	int An;
	int Tn;
	const int DFF[5]={0,1,2,3,4};                               //distance(0,3)
	const int AFF[10]={-4,-3,-2,-1,-0,0,1,2,3,4};        //angle(-45,45)
	const int rule[5][10]={
						{2,3,4,5,6,-6,-5,-4,-3,-2},
						{2,3,4,5,6,-6,-5,-4,-3,-2},
						{1,2,3,4,5,-5,-4,-3,-2,-1},
						{0,1,2,3,4,-4,-3,-2,-1,0},
						{0,1,2,3,4,-4,-3,-2,-1,0}
					};
	float f_distance=distance*Kd;
	float f_angle=angle*Ka;
	bLOG <<"Kd:" <<Kd << "	Ka:" << Ka << "	Kt:" << Kt << endl;
	bLOG << "distance: " << distance << "    f_distance:"  << f_distance << endl;
	bLOG << "angle:" << angle << "    f_angle:" << f_angle <<endl;
	if(f_distance>=DFF[0] && f_distance <=DFF[4])
	{
		if(f_distance <= DFF[0]+0.5)
			Dn=0;
		else if(f_distance <= DFF[1]+0.5)
			Dn=1;
		else if(f_distance <= DFF[2]+0.5)
			Dn=2;
		else if(f_distance <= DFF[3]+0.5)
			Dn=3;
		else
			Dn=4;	
	}
	else if(f_distance < DFF[0])
		Dn=0;
	else
		Dn=4;
	if(f_angle >=AFF[0] && f_angle<=AFF[9])
	{
		if(f_angle <= AFF[0]+0.5)
			An=0;
		else if(f_angle <= AFF[1]+0.5)
			An=1;
		else if(f_angle <= AFF[2]+0.5)
			An=2;
		else if(f_angle <= AFF[3]+0.5)
			An=3;
		else if(f_angle <= AFF[4])
			An=4;
		else if(f_angle <= AFF[5]+0.5)
			An=5;
		else if(f_angle <= AFF[6]+0.5)
			An=6;
		else if(f_angle <= AFF[7]+0.5)
			An=7;
		else if(f_angle <= AFF[8]+0.5)
			An=8;
		else
			An=9;
	}
	else if(f_angle <AFF[0])
		An=0;
	else
		An=9;
	Tn=rule[Dn][An];
	float turn_angle=Tn*Kt;
	bLOG << "Dn: " << Dn << "	An:" << An << "	Tn:" << Tn  <<"	turn_angle:" << turn_angle << endl;
	return turn_angle;
}

