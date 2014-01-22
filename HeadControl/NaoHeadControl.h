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

#ifndef NAOHEADCONTROL_H
#define NAOHEADCONTROL_H

#include "HeadControl.h"
#include "HeadPathPlanner.h"
#include "ModuleHandler.h"
#include "Range.h"
#include "worldmodel.h"

using namespace salt;

class NaoHeadControl : public HeadControl
{
public:
    NaoHeadControl( ModuleHandler& moduleHandler, const HeadControlInterfaces& interfaces);
    ~NaoHeadControl() {}

    virtual void execute();
    virtual std::string GetActionCommand() {return "";}

    /** An instance of the head path planner */
    HeadPathPlanner headPathPlanner;

    /** If true, the last head movement was directed to the left side (of the ball) */
    bool lastScanWasLeft;

    /** Is true, if the head is on the left side */
    bool headYawIsLeft();

    /** returns a distance between actual position and comp. the small results are better */
    float headPositionDistanceToActualPosition(Vector3f comp,bool leftSide);

    /** The head control mode that was executed in the last frame */
    HeadControlMode::HeadControlModes lastHeadControlMode;


    /** The minimum head speed in urad per frame */
    enum {minHeadSpeed=2};


    /** deals with setting the head joints and performs
    optimizations so that the head does not move too fast
    angles are in RAD and NOT EVER AGAIN(!!) IN MICRORAD!
    */
    void setJoints(float yaw, float pitch, float speed=0);

    /* Writes the joint angles to the HeadMotionRequest without optimization and smoothing */
    void setJointsDirect( float yaw,float pitch);

    void simpleLookAtPointRelativeToRobot( Vector3f offset, float& neckYaw, float& headPitch);

    /** Simplified "look at 3d-point" on field with offset point in camera image
    this is straight-forward for the ERS210, but the ERS7 has two
    tilt joints that can both be used to look at something. Lookatpoint
    uses the two following methods to find a 'good' solution.  */
    void simpleLookAtPointOnField(const Vector3f pos, Vector2f offset, double& neckYaw, double& headPitch);


    /** look at 3d-point on field with offset point in camera image
    this is straight-forward for the ERS210, but the ERS7 has two
    tilt joints that can both be used to look at something. Lookatpoint
    uses the two following methods to find a 'good' solution.  */
    void lookAtPoint(const Vector3f &pos,const Vector2f &offset,float& pitch,float& yaw);


    /** stores the number of frames "setjoints" has been close to its destination */
    int setJointsIsCloseToDestination;

    /** true if the max pan of the head joint is reached */
    bool setJointsMaxPanReached;

    /** initial the main angles for the head movement */
    void setupMainAngles();


    /** basic headpositions for the gaze */
    Vector3f headLeft;
    Vector3f headRight;
    Vector3f headMiddleLeft;
    Vector3f headMiddleLeftDown;
    Vector3f headMiddleRight;
    Vector3f headMiddleRightDown;
    Vector3f headRightDown;
    Vector3f headLeftDown;
    Vector3f headUp;
    Vector3f headDown;
    Vector3f headZero;

    /** speed in rad/s for head movement. its used for headpathplanner */
    float speedNeckYaw;
    float speedHeadPitch;

    /* if the ball get lost, he believes in the communicated ball */

    bool useCommunicatedBall;

    /* return true, if the position is reached more or less */
    /**bool headPositionReached(Vector3f pos)
    {
        return headPathPlanner.headPositionReached(pos);
    }*/

    /** looks to the left/right side an aearch for ball. Used for kicks */
    void ScanLeft();
    void ScanRight();

    void ScanAllDirection();

    /** begin a ball search by the given start position */
    void beginBallSearchAt(Vector2f ballPosition2d);

	void IntelligenceMode();
	void AutoMode();
	void HandMode();
	void LookForward();
	void LookLeft();
    void LookRight();
    void LookAtBall();
	void LookAtOurGoal();
	void LookAtOppGoal();

    void RealSlowScan();
    void ReturnToBall();
    void ScanAwayFromBall();
    void LookBetweenFeetForKick();
    void LookAtBallAndClosestLandmark();//needed? by Neil
    inline const int BallTime_SinceLastSeen()
    {
        if (WM.SeeBall())
        {
            startCycleForBallLastSeen = WM.getSelf().CurrentCycle;
            return (WM.getSelf().CurrentCycle - startCycleForBallLastSeen);
        }
        else
            return (WM.getSelf().CurrentCycle - startCycleForBallLastSeen);
    }
    inline const int BallTime_KeepSeeing()
    {
        if (!WM.SeeBall())
        {
            startCycleForBallKeepSeeing = WM.getSelf().CurrentCycle;
            return (WM.getSelf().CurrentCycle - startCycleForBallKeepSeeing);
        }
        else
            return (WM.getSelf().CurrentCycle - startCycleForBallKeepSeeing);
    }

private:
    Range<float> jointRangeNeckYaw, jointRangeHeadPitch;

    /** The state of the ActionControl */
    enum HeadControlState
    {
        other_modes ,
        ball_found_again,
        ball_lost,
        ball_see,
        scan_away_from_ball,
        return_to_ball,
		look_at_our_goal,
		look_at_opp_goal,
    } headControlState;

    HeadControlState lastHeadControlState;

    int startCycle;
    int startCycleForBallLastSeen;
    int startCycleForBallKeepSeeing;
protected:
    /**
    * A reference to the ModuleHandler of the Process.
    * Needed to create new solutions.
    */
    ModuleHandler& moduleHandler;

};

#endif
