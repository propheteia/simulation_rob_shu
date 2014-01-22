/*
   Copyright (C) 2007, 2008 by Team Apollo
   Nanjing University of Posts and Telecommunications, China

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
#ifndef NAO_H
#define NAO_H

#include "robot.h"

#include "StaticModule.h"

#define NAO StaticModule<Nao>::getInstancePtr()

class Nao : public Robot
{
public:
    static const JointID JID_HEAD_1         = 2;
    static const JointID JID_HEAD_2         = 3;
    static const JointID JID_LARM_1         = 4;
    static const JointID JID_RARM_1         = 5;
    static const JointID JID_LARM_2         = 6;
    static const JointID JID_RARM_2         = 7;
    static const JointID JID_LARM_3         = 8;
    static const JointID JID_RARM_3         = 9;
    static const JointID JID_LARM_4         = 10;
    static const JointID JID_RARM_4         = 11;
    static const JointID JID_LLEG_1         = 12;
    static const JointID JID_RLEG_1         = 13;
    static const JointID JID_LLEG_2         = 14;
    static const JointID JID_RLEG_2         = 15;
    static const JointID JID_LLEG_3         = 16;
    static const JointID JID_RLEG_3         = 17;
    static const JointID JID_LLEG_4         = 18;
    static const JointID JID_RLEG_4         = 19;
    static const JointID JID_LLEG_5         = 20;
    static const JointID JID_RLEG_5         = 21;
    static const JointID JID_LLEG_6         = 22;
    static const JointID JID_RLEG_6         = 23;

    static const RobotPart PART_BODY        = JID_ROOT;
    static const RobotPart PART_HEAD        = JID_HEAD_1;
    static const RobotPart PART_NECK        = JID_HEAD_2;
    static const RobotPart PART_LSHOULDER   = JID_LARM_1;
    static const RobotPart PART_RSHOULDER   = JID_RARM_1;
    static const RobotPart PART_LUPPERARM   = JID_LARM_2;
    static const RobotPart PART_RUPPERARM   = JID_RARM_2;
    static const RobotPart PART_LELBOW      = JID_LARM_3;
    static const RobotPart PART_RELBOW      = JID_RARM_3;
    static const RobotPart PART_LLOWERARM   = JID_LARM_4;
    static const RobotPart PART_RLOWERARM   = JID_RARM_4;
    static const RobotPart PART_LHIP_1      = JID_LLEG_1;
    static const RobotPart PART_RHIP_1      = JID_RLEG_1;
    static const RobotPart PART_LHIP_2      = JID_LLEG_2;
    static const RobotPart PART_RHIP_2      = JID_RLEG_2;
    static const RobotPart PART_LTHIGH      = JID_LLEG_3;
    static const RobotPart PART_RTHIGH      = JID_RLEG_3;
    static const RobotPart PART_LSHANK      = JID_LLEG_4;
    static const RobotPart PART_RSHANK      = JID_RLEG_4;
    static const RobotPart PART_LANKLE      = JID_LLEG_5;
    static const RobotPart PART_RANKLE      = JID_RLEG_5;
    static const RobotPart PART_LFOOT       = JID_LLEG_6;
    static const RobotPart PART_RFOOT       = JID_RLEG_6;

public:
    Nao();
    virtual ~Nao();

    bool IK_leg(const salt::Matrix& torso, const salt::Matrix& foot, bool left);

	bool IK_leg(const salt::Matrix& torso, const salt::Matrix& leftfoot, const salt::Matrix& rightfoot);

    void CalcLegJointVel(const salt::Vector3f& vb, const salt::Vector3f& wb,
                         const salt::Vector3f& vt, const salt::Vector3f& wt,
                         bool left);

protected:
    virtual void SetupLinks();

    virtual void SetupRobotPartInfo();

    bool IK_leg(const salt::Matrix& hip, float A, float B,
                const salt::Matrix& ankle, bool left);

private:
    /** foots */
    float FootLength;
    float FootWidth;
    float FootHeight;
    float FootMass;

    float FootRelAnkle_X;
    float FootRelAnkle_Y;
    float FootRelAnkle_Z;

    /** ankles */
    float AnkleRelShank_X;
    float AnkleRelShank_Y;
    float AnkleRelShank_Z;
	float AnkleMass;

    /** shanks */
    float ShankLength;
    float ShankWidth;
    float ShankHeight;
    float ShankMass;

    float ShankRelThigh_X;
    float ShankRelThigh_Y;
    float ShankRelThigh_Z;

    /** thighs */
    float ThighLength;
    float ThighWidth;
    float ThighHeight;
    float ThighMass;

    float ThighRelHip2_X;
    float ThighRelHip2_Y;  //set 0.005 to check the tri-angle algorithm
    float ThighRelHip2_Z;

    /** hips */
    float Hip1Axis_X ;
    float Hip1Axis_Y ;
    float Hip1Axis_Z ;

    float Hip1RelTorso_X;
    float Hip1RelTorso_Y;
    float Hip1RelTorso_Z;

    float Hip2RelHip1_X;
    float Hip2RelHip1_Y;
    float Hip2RelHip1_Z;
	float Hip1Mass;
	float Hip2Mass;

    /** torso */
    float TorsoLength;
    float TorsoWidth;
    float TorsoHeight;
    float TorsoMass;

    float TorsoInitX;
    float TorsoInitY;
    float TorsoInitZ;

	/** head and neck*/
	float NeckRelTorso_X;
	float NeckRelTorso_Y;
	float NeckRelTorso_Z;
	float NeckMass;

	float HeadRelNeck_X;
	float HeadRelNeck_Y;
	float HeadRelNeck_Z;
	float HeadMass;

	/**should*/
	float ShoulderRelTorso_X;
	float ShoulderRelTorso_Y;
	float ShoulderRelTorso_Z;
	float ShoulderMass;

	/**upperarm */
	float UpperarmRelShoulder_X;
	float UpperarmRelShoulder_Y;
	float UpperarmRelShoulder_Z;
	float UpperarmMass;

	/**elbow */
	float ElbowRelUpperarm_X;
	float ElbowRelUpperarm_Y;
	float ElbowRelUpperarm_Z;
	float ElbowMass;

	/**lowerarm*/
	float LowerarmElbow_X;
	float LowerarmElbow_Y;
	float LowerarmElbow_Z;
	float LowerarmMass;


    /** distance and vector */
    float NAO_A;
    float NAO_B;
    salt::Vector3f NAO_Dl;
    salt::Vector3f NAO_Dr;
    salt::Vector3f NAO_E;
    salt::Vector3f NAO_F;
};


#endif
