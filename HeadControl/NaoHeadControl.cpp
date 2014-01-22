/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei , Qu Junjun  *
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
#include "NaoHeadControl.h"
#include "gmath.h"
#include "ball.h"
#include "worldmodel.h"
#include "nao.h"


NaoHeadControl::NaoHeadControl( ModuleHandler& moduleHandler, const HeadControlInterfaces& interfaces):HeadControl(interfaces),moduleHandler(moduleHandler),lastScanWasLeft(true),headControlState(ball_lost),lastHeadControlMode(HeadControlMode::none)
{
    setupMainAngles();

    jointRangeNeckYaw = Range<float>(-120.0,120.0);
    jointRangeHeadPitch  = Range<float>(-45.0, 45.0);

    // these values are the default settings rad/s
    speedNeckYaw = 1.5;
    speedHeadPitch  = 1.5;

    // setting speed in headpath planner
    headPathPlanner.headPathSpeedNeckYaw = speedNeckYaw;
    headPathPlanner.headPathSpeedHeadPitch  = speedHeadPitch;

    useCommunicatedBall = true;
}

bool NaoHeadControl::headYawIsLeft()
{
    //cout << "NECK: "<<NAO->GetLink()[Nao::JID_HEAD_2].q << endl;
    return ((NAO->GetLink()[Nao::JID_HEAD_2].q)>0);
}


void NaoHeadControl::setupMainAngles()
{
    headLeft.x() = 0.0;
    headLeft.y() = 0.0;
    headLeft.z() = 120.0;

    headLeftDown.x() = 0.0;
    headLeftDown.y() = -45.0;
    headLeftDown.z() = 120.0;

    headRight.x() = 0.0;
    headRight.y() = 0.0;
    headRight.z() = -120.0;

    headRightDown.x() = 0.0;
    headRightDown.y() = -45.0;
    headRightDown.z() = -120.0;

    headMiddleLeft.x() = 0.0;
    headMiddleLeft.y() = 0.0;
    headMiddleLeft.z() = 60.0;

    headMiddleLeftDown.x() = 0.0;
    headMiddleLeftDown.y() = -45.0;
    headMiddleLeftDown.z() = 60.0;

    headMiddleRight.x() = 0.0;
    headMiddleRight.y() = 0.0;
    headMiddleRight.z() = -60.0;

    headMiddleRightDown.x() = 0.0;
    headMiddleRightDown.y() = -45.0;
    headMiddleRightDown.z() = -60.0;

    headUp.x() = 0.0;
    headUp.y() = 45.0;
    headUp.z() = 0.0;

    headDown.x() = -0.0;
    headDown.y() = -45.0;
    headDown.z() = 0.0;

    headZero.x() = 0.0;
    headZero.y() = 0.0;
    headZero.z() = 0.0;
}



void NaoHeadControl::LookAtBall()
{
	if(WM.SeeBall())
	{
		float pitch, yaw;
		pitch = gNormalizeDeg(WM.getVisionSense(BALL).phi);
		yaw = gNormalizeDeg(WM.getVisionSense(BALL).theta);
		aLOG << "yaw : " << yaw << "  pitch : "<< pitch << endl;
		pitch = pitch + gRadToDeg(NAO->GetLink()[Nao::JID_HEAD_1].q);
		yaw = yaw + gRadToDeg(NAO->GetLink()[Nao::JID_HEAD_2].q);
	
		aLOG << "yaw : " << yaw << "  pitch : "<< pitch << endl;
		setJointsDirect(yaw,pitch);
	}
	else
	{
		ScanAllDirection();
	}
}

void NaoHeadControl::ScanLeft()
{
    float neckYaw, headPitch;
    if ( headPathPlanner.isLastPathFinished())
    {
        Vector3f points[]={  headDown,
                             headLeftDown,
                             headLeft,
                             headZero
                          };

        long durations[] =   {180,240,180,240};
        headPathPlanner.init(points,durations,sizeof(points)/sizeof(Vector3f));

        lastScanWasLeft = true;
    }

    headPathPlanner.getAngles(neckYaw, headPitch);
    setJointsDirect(neckYaw, headPitch);
    //setJoints(headPitch, neckYaw);
}

void NaoHeadControl::ScanRight()
{
    float neckYaw, headPitch;
    if ( headPathPlanner.isLastPathFinished())
    {
        Vector3f points[]={         headDown,
                                    headRightDown,
                                    headRight,
                                    headZero
                          };

        long durations[] =   {180,240,180,240};
        headPathPlanner.init(points,durations,sizeof(points)/sizeof(Vector3f));

        lastScanWasLeft = false;
    }
    headPathPlanner.getAngles(neckYaw, headPitch);
    setJointsDirect(neckYaw, headPitch);
    //setJoints(headPitch, neckYaw);
}

void NaoHeadControl::ScanAllDirection()
{
    float neckYaw,headPitch;
    bool headPanSide=headYawIsLeft();
    int jumped=0;
	aLOG << "ScanAllDirection" << endl;
    if (headPathPlanner.isLastPathFinished())
    {
        Vector3f leftRightSweepTop,
        leftRightSweepBottom,
        halfLeftRightSweep,
        halfLeftRightSweepBottom;

        /* if (!basicBehaviorWasActiveDuringLastExecutionOfEngine)
         {
             if (headControlMode.headControlMode == HeadControlMode::searchForBallLeft)
                 lastScanWasLeft = false;
             else if (headControlMode.headControlMode == HeadControlMode::searchForBallRight)
                 lastScanWasLeft = true;
             else
                 lastScanWasLeft=!headPanSide;
         }*/

        if (!lastScanWasLeft)//ֵ
        {
            leftRightSweepTop = headLeft;
            leftRightSweepBottom = headLeftDown;
            halfLeftRightSweep = headMiddleLeft;
            halfLeftRightSweepBottom = headMiddleLeftDown;
        }
        else
        {
            leftRightSweepTop = headRight;
            leftRightSweepBottom = headRightDown;
            halfLeftRightSweep = headMiddleRight;
            halfLeftRightSweepBottom = headMiddleRightDown;
        }

        Vector3f points[]={headDown,
                           headDown,
                           halfLeftRightSweepBottom,
                           leftRightSweepBottom,
                           leftRightSweepTop,
                           halfLeftRightSweep,
                           headZero,
                           headDown
                          };
        long durations[] =   {45,60,60,120,120,120,60,45};

        // jump over positions, which are far away from actual headposition

        Vector3f *ppoints = points;
        long *pdurations = durations;

        /*while (headPositionDistanceToActualPosition(points[jumped],headPanSide)<0
                && jumped<3)
        {
            ppoints++;
            pdurations++;
            jumped++;
        }*/

        headPathPlanner.init(ppoints,pdurations,(sizeof(points)/sizeof(Vector3f))-jumped);
        lastScanWasLeft = !lastScanWasLeft;
    }
    headPathPlanner.getAngles(neckYaw,headPitch);
    setJointsDirect(neckYaw, headPitch);
}

void NaoHeadControl::IntelligenceMode()
{
	float ball_x = WM.getBall().pos.x();
	float self_x = WM.getSelf().pos.x();
	if(self_x - ball_x > 0.3)
	{
		LookAtOurGoal();
		//cout << "LookAtOurGoal" << endl;
	}
	else
	{
		LookAtOppGoal();
		//cout << "LookAtOppGoal" << endl;
	}
}

void NaoHeadControl::AutoMode()
{
	aLOG << "BallTime_SinceLastSeen : " << BallTime_SinceLastSeen() << endl;
    aLOG << "BallTime_KeepSeeing: " <<BallTime_KeepSeeing() << endl;
    switch (headControlState)
    {
    case ball_lost:
        if (WM.SeeBall())
        {
            headControlState = ball_found_again;
        }
        else
        {
            ScanAllDirection();
        }
        break;
    case ball_found_again:
        if (!WM.SeeBall())
        {
            headControlState = ball_lost;
        }
        else if (BallTime_KeepSeeing() > 200)
        {
            headControlState = ball_see;
        }
        else
        {
            LookAtBall();
        }
        break;
    case ball_see:
        if (BallTime_SinceLastSeen() > 20)
            headControlState = ball_lost;
        else if (!WM.CanLocalize() && WM.getSelf().roleinteam != Self::forward1)
            ScanAwayFromBall();
		else
 			LookAtBall();
        break;


    default:
        break;
    }

    //ScanAwayFromBall();
    //lastHeadControlMode = headControlMode.headControlMode;
//     LookAtBall();
    //LookLeft();
    //LookRight();
	//LookBetweenFeetForKick()
    //searchForBallLeft();
    //searchForBallRight();
    //RealSlowScan();
}

void NaoHeadControl::HandMode()
{
	
}

void NaoHeadControl::LookForward()
{
	headMotionRequest.yaw = 0.0;
	headMotionRequest.pitch = 0.0;
}

void NaoHeadControl::LookAtOurGoal()
{
	oLOG << "Time: " << WM.getFieldInfo().match_time;
	const VisionSense & ourLeftGoal = WM.getVisionSense(GOAL_1_L);
	const VisionSense & ourRightGoal = WM.getVisionSense(GOAL_2_L);
	const VisionSense & ball = WM.getVisionSense(BALL);
	static bool InScanMode = false;
	if( InScanMode == true || (WM.getVisionState() && (ourLeftGoal.distance < 0.0 || ourRightGoal.distance < 0.0 || ball.distance < 0.0)) )
	{
		ScanAllDirection();
		if(	WM.getVisionState() && 
			(	ourLeftGoal.distance > 0.0 && 
				ourRightGoal.distance > 0.0 && ball.distance > 0.0 )	)
			InScanMode = false;
		else
			InScanMode = true;	
	}
	else
	{
		oLOG << "Can See Both of the Goals" << endl;
		float yaw, pitch;
		yaw = (ourLeftGoal.theta + ourRightGoal.theta) / 2.0;
		yaw = (yaw + gNormalizeDeg(ball.theta)) / 2.0;
		pitch = (ourLeftGoal.phi + ourRightGoal.phi) / 2.0;
		pitch = (pitch + gNormalizeDeg(ball.phi)) / 2.0;
		yaw = gNormalizeDeg(yaw) + gRadToDeg(NAO->GetLink()[Nao::JID_HEAD_2].q);
		pitch = gNormalizeDeg(pitch) + gRadToDeg(NAO->GetLink()[Nao::JID_HEAD_1].q);
		setJointsDirect(yaw,pitch);
		oLOG << "yaw: " << yaw << " pitch: " << pitch << endl;
		oLOG << "oppLeftGoal.phi: " << ourLeftGoal.phi 
			 << " oppLeftGoal.theta: " << ourLeftGoal.theta << endl;
		oLOG << "oppRightGoal.phi: " << ourRightGoal.phi
			 << " oppRightGoal.theta: " << ourRightGoal.theta << endl;
		boost::shared_array<Robot::Link> uLink = NAO->GetLink();
		oLOG << gRadToDeg(uLink[Nao::JID_HEAD_1].q) << ' ' 
		 	 << gRadToDeg(uLink[Nao::JID_HEAD_2].q) << endl;
	}
}

void NaoHeadControl::LookAtOppGoal()
{
	oLOG << "Time: " << WM.getFieldInfo().match_time;
	const VisionSense & oppLeftGoal = WM.getVisionSense(GOAL_1_R);
	const VisionSense & oppRightGoal = WM.getVisionSense(GOAL_2_R);
	const VisionSense & ball = WM.getVisionSense(BALL);
	static bool InScanMode = false;
	if( InScanMode == true || (WM.getVisionState() && (oppLeftGoal.distance < 0.0 || oppRightGoal.distance < 0.0 || ball.distance < 0.0)) )
	{
		ScanAllDirection();
		if(	WM.getVisionState() && 
			(	oppLeftGoal.distance > 0.0 && 
				oppRightGoal.distance > 0.0 && ball.distance > 0.0 )	)
			InScanMode = false;
		else
			InScanMode = true;	
	}
	else
	{
		oLOG << "Can See Both of the Goals" << endl;
		float yaw, pitch;
		yaw = (oppLeftGoal.theta + oppRightGoal.theta) / 2.0;
		yaw = (yaw + gNormalizeDeg(ball.theta)) / 2.0;
		pitch = (oppLeftGoal.phi + oppRightGoal.phi) / 2.0;
		pitch = (pitch + gNormalizeDeg(ball.theta)) / 2.0;
		yaw = gNormalizeDeg(yaw) + gRadToDeg(NAO->GetLink()[Nao::JID_HEAD_2].q);
		pitch = gNormalizeDeg(pitch) + gRadToDeg(NAO->GetLink()[Nao::JID_HEAD_1].q);
		setJointsDirect(yaw,pitch);
		oLOG << "yaw: " << yaw << " pitch: " << pitch << endl;
		oLOG << "oppLeftGoal.phi: " << oppLeftGoal.phi 
			 << " oppLeftGoal.theta: " << oppLeftGoal.theta << endl;
		oLOG << "oppRightGoal.phi: " << oppRightGoal.phi
			 << " oppRightGoal.theta: " << oppRightGoal.theta << endl;
		boost::shared_array<Robot::Link> uLink = NAO->GetLink();
		oLOG << gRadToDeg(uLink[Nao::JID_HEAD_1].q) << ' ' 
		 	 << gRadToDeg(uLink[Nao::JID_HEAD_2].q) << endl;
	}
}

/**void NaoHeadControl::beginBallSearchAt(Vector2<double> ballPosition2d)
{
  Vector3<double> ballPosition3d (ballPosition2d.x,ballPosition2d.y,ballRadius);

	Vector3<double> leftRightSweepTop,
                  leftRightSweepBottom,
                  halfLeftRightSweep,
                  halfLeftRightSweepBottom;

	Vector2<double> toBall = ballPosition2d - robotPose.translation;
	double angleToBall = normalize(atan2(toBall.y,toBall.x))-robotPose.rotation;

  // center the ball view in the middle of the image
	// if the ball is near, the ball should be seen, if we look halfway down
  // constante definition of distance to ball
	enum { ballNearBy = 500 };
	Vector2<int> cameraImageOffset(0,25);

  double neckTilt,headPan,headTilt;
  simpleLookAtPointOnField(ballPosition3d,cameraImageOffset,neckTilt,headPan,headTilt);
  Vector3<double> ballAngles (neckTilt,headPan,headTilt);

	if (angleToBall>0)
	{
		leftRightSweepTop        = headLeft;
		leftRightSweepBottom     = headLeftDown;
		halfLeftRightSweep       = headMiddleLeft;
    halfLeftRightSweepBottom = headMiddleLeftDown;

	}
	else
	{
		leftRightSweepTop         = headRight;
		leftRightSweepBottom      = headRightDown;
		halfLeftRightSweep        = headMiddleRight;
    halfLeftRightSweepBottom  = headMiddleRightDown;
	}

  Vector3<double> points[]={ballAngles,
                            ballAngles,
                            leftRightSweepBottom,
                            leftRightSweepTop,
                            halfLeftRightSweep,
                            headUp,
                            headDown};

	long durations[] = {0,100,160,120,160,100,80};
	headPathPlanner.init(points,durations,sizeof(points)/sizeof(Vector3<double>));
	lastScanWasLeft = (angleToBall>0);


}*/

void NaoHeadControl::LookLeft()
{
    float neckYaw, headPitch;

    Vector3f left(0.0, 0.0, 120.0);
    lastScanWasLeft = true;
    if (1/**headControl.lastHeadControlMode != headControlMode.headControlMode*/)
    {
        Vector3f points[]={left};
        headPathPlanner.oldInit(points, sizeof(points)/sizeof(Vector3f), 240);
    }
    headPathPlanner.getAngles(neckYaw, headPitch);
    setJointsDirect(neckYaw, headPitch);
}

void NaoHeadControl::LookRight()
{
    float neckYaw, headPitch;
    Vector3f right(0.0, 0.0, -120.0);

    lastScanWasLeft = false;
    if (1/**headControl.lastHeadControlMode != headControlMode.headControlMode*/)
    {
        Vector3f points[]={right};
        headPathPlanner.oldInit(points, sizeof(points)/sizeof(Vector3f), 240);
    }
    headPathPlanner.getAngles(neckYaw, headPitch);
    setJointsDirect(neckYaw, headPitch);
}

void NaoHeadControl::RealSlowScan()
{
    float neckYaw, headPitch;
    Vector3f left(0.0,-0.0,120.0), middle(0.0,-35.0,0.0), right(0.0,-0.0,-120.0);

    if (headPathPlanner.getAngles(neckYaw, headPitch))
    {
        if (lastScanWasLeft)
        {
            Vector3f points[3]={left,middle, right};
            headPathPlanner.oldInit(points, sizeof(points)/sizeof(Vector3f), 1200);
            lastScanWasLeft = !lastScanWasLeft;
        }
        else
        {
            Vector3f points[3]={right, middle, left};
            headPathPlanner.oldInit(points, sizeof(points)/sizeof(Vector3f), 1200);
            lastScanWasLeft = !lastScanWasLeft;
        }
    }
    setJointsDirect(neckYaw, headPitch);
}

void NaoHeadControl::ReturnToBall()
{
    float pitch, yaw;
    pitch = gNormalizeDeg(WM.getVisionSense(BALL).phi);
    yaw = gNormalizeDeg(WM.getVisionSense(BALL).theta);
    aLOG << "yaw : " << yaw << "  pitch : "<< pitch << endl;

    setJoints(yaw, pitch);
}

void NaoHeadControl::ScanAwayFromBall()
{

    float neckYaw, headPitch;
    if ( headPathPlanner.isLastPathFinished())
    {
        if (lastScanWasLeft)
        {
            Vector3f centerTop(0.0,45.0,headPathPlanner.lastNeckYaw), rightTop(0.0,45.0,-120.0), rightBottom(0.0,-45.0,-120.0);
            Vector3f points[4]={centerTop,rightTop, rightBottom, headDown};
            headPathPlanner.oldInit(points, sizeof(points)/sizeof(Vector3f), 600);
        }
        else
        {
            Vector3f centerTop(0.0,45.0,headPathPlanner.lastNeckYaw), leftTop(0.0,45.0,120.0), leftBottom(0.0,-45.0,120.0);
            Vector3f points[4]={centerTop,leftTop, leftBottom, headDown};
            headPathPlanner.oldInit(points, sizeof(points)/sizeof(Vector3f), 600);
        }
        lastScanWasLeft = !lastScanWasLeft;
    }

    headPathPlanner.getAngles(neckYaw, headPitch);
    setJointsDirect(neckYaw, headPitch);
}

void NaoHeadControl::LookBetweenFeetForKick()
{	    float neckYaw, headPitch;

    if (1/**headControl.lastHeadControlMode != headControlMode.headControlMode*/)
    {
        Vector3f points[]={headDown};
        headPathPlanner.oldInit(points, sizeof(points)/sizeof(Vector3f), 180);
    }
    headPathPlanner.getAngles(neckYaw, headPitch);
    setJointsDirect(neckYaw, headPitch);
}

void NaoHeadControl::simpleLookAtPointRelativeToRobot( Vector3f offsetInImage,
        float& neckYaw, float& headPitch )
{

    /** now that the angles are found, add the offset-in-image: */

    /** clip image offset to image bounderies: */
    const Range<float> cameraResY(-45, 45);
    const Range<float> cameraResX(-45, 45);
    offsetInImage.x() = cameraResX.limit(offsetInImage.x());
    offsetInImage.y() = cameraResY.limit(offsetInImage.y());

    /** add angles for offset in image: */
    //headPan  += ((double)offsetInImage.x/cameraInfo.resolutionWidth)*cameraInfo.openingAngleWidth;
    //headTilt += ((double)offsetInImage.y/cameraInfo.resolutionHeight)*cameraInfo.openingAngleHeight;

    /** perform clipping to ranges of possible joint values: */
    const Range<float>jointRangeNeckYaw(-120.0,120.0);
    const Range<float>jointRangeHeadPitch(-45.0, 45.0);

    neckYaw = jointRangeNeckYaw.limit(neckYaw);
    headPitch = jointRangeHeadPitch.limit(headPitch);
}

float NaoHeadControl::headPositionDistanceToActualPosition(Vector3f comp,bool leftSide)
{
    float pos;
    pos = gRadToDeg(NAO->GetLink()[Nao::JID_HEAD_1].q) ;

    return -gAbs(pos-comp.y());

}

void NaoHeadControl::setJointsDirect(float yaw,float pitch)
{
    yaw = jointRangeNeckYaw.limit(yaw);
    pitch = jointRangeHeadPitch.limit(pitch);

    headPathPlanner.lastNeckYaw = yaw;
    headPathPlanner.lastHeadPitch  = pitch;

    headMotionRequest.pitch  = pitch;
    headMotionRequest.yaw   = yaw;
}

void NaoHeadControl::setJoints(float neckYaw, float headPitch, float speed)
{
    ///const float closeToDesiredAngles = 0.01;

    /// test if head has reached his position
    /**setJointsIsCloseToDestination = headPathPlanner.headPositionReached(Vector3f (0.0,headPitch,neckYaw);

    setJointsMaxPanReached = false;

    Vector2f
    direction(headTilt - headPathPlanner.lastHeadTilt,
              headPan - (headPathPlanner.lastHeadPan));

    if (speed > 0)
    {
        Vector2f directionWithSpeed = direction;
        directionWithSpeed.normalize();
        directionWithSpeed *= speed/125;
        if (directionWithSpeed.abs() < direction.abs())
            direction = directionWithSpeed;
    }

    if (direction.abs() < closeToDesiredAngles)
        setJointsIsCloseToDestination++;
    else
        setJointsIsCloseToDestination = 0;

    headPathPlanner.lastNeckTilt = neckTilt;
    headPathPlanner.lastHeadPan  += direction.y;
    headPathPlanner.lastHeadTilt += direction.x;

    headPathPlanner.lastNeckTilt = jointRangeNeckTilt.limit(headPathPlanner.lastNeckTilt);
    headPathPlanner.lastHeadPan  = jointRangeHeadPan.limit(headPathPlanner.lastHeadPan);
    headPathPlanner.lastHeadTilt = jointRangeHeadTilt.limit(headPathPlanner.lastHeadTilt);

    if (fabs(headPathPlanner.lastHeadPan) == jointRangeHeadPan.max)
        setJointsMaxPanReached = true;

    headMotionRequest.tilt  = (long)(headPathPlanner.lastNeckTilt*1000000.0);
    headMotionRequest.pan   = (long)(headPathPlanner.lastHeadPan *1000000.0);
    headMotionRequest.roll  = (long)(headPathPlanner.lastHeadTilt*1000000.0);*/

}

void NaoHeadControl::execute()
{
	switch(headMotionRequest.headMotionType)
	{
		case HeadMotionRequest::intelligencemode:
			IntelligenceMode();
			break;
		case HeadMotionRequest::automode:
			AutoMode();
			break;
		case HeadMotionRequest::handmode:
			HandMode();
			break;
		case HeadMotionRequest::lookforward:
			LookForward();
			break;
		case HeadMotionRequest::lookleft:
			LookLeft();
			break;
		case HeadMotionRequest::lookright:
			LookRight();
			break;
		case HeadMotionRequest::lookatball:
			LookAtBall();
			break;
		case HeadMotionRequest::lookatourgoal:
			LookAtOurGoal();
			break;
		case HeadMotionRequest::lookatoppgoal:
			LookAtOppGoal();
			break;
		default:
			cerr << "There is No Such headMotionType!" << endl;
	}	
}
