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
#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <string>
#include <map>
#include <cassert>
#include <sstream>
#include "vector.h"
#include "gmath.h"
#include "soccertypes.h"


struct VisionSense
{
    /** distance perceptor to object */
    double distance;

    /** theta is the angle in the X-Y (horizontal) plane */
    double theta;

    /** phi is the latitude angle */
    double phi;

    double distanceToSelf;
    
    salt::Vector3f localPos;//local position for the robot's torso coordinate

    salt::Vector3f realPos;

    salt::Vector3f localPosInVision;//local position for the vision coordinate

    /** init with some big number to find out bug easlier */
    VisionSense() : distance(-1000.0), theta(-1000.0), phi(-1000.0) {};
};

struct HingeJointSense
{
    /** joint angle */
    float angle;

    /** joint angle rate */
    float rate;

    HingeJointSense() : angle(0), rate(0) {};
};

struct UniversalJointSense
{
    /** joint angle axis 1*/
    float angle1;

    /** joint angle axis 2*/
    float angle2;

    /** joint angle rate axis 1*/
    float rate1;

    /** joint angle rate axis 2*/
    float rate2;

    UniversalJointSense() : angle1(0), angle2(0), rate1(0), rate2(0) {};
};

struct HearInfo
{
    float		time;
    std::string		sender;
    float		direction;
    std::string		message;
};

/** typedef */
typedef int JointID;
typedef int RobotPart;
typedef std::map<JointID, HingeJointSense> THingeJointSenseMap;
typedef std::map<JointID, UniversalJointSense> TUniversalJointSenseMap;
typedef std::map<std::string, JointID> TJointIDMap;

/** enum */
enum EFallDir
{
    NO_FALL,
    FALL_FRONT,
    FALL_BACK,
    FALL_LEFT,
    FALL_RIGHT
};

enum VisionObject
{
    FLAG_1_L    = 0,
    FLAG_1_R    = 1,
    FLAG_2_L    = 2,
    FLAG_2_R    = 3,
    GOAL_1_L    = 4,
    GOAL_1_R    = 5,
    GOAL_2_L    = 6,
    GOAL_2_R    = 7,
    BALL        = 8,

    TEAMMATE_1_HEAD	= 9,
    TEAMMATE_1_LEFTARM,
    TEAMMATE_1_RIGHTARM,
    TEAMMATE_1_LEFTFOOT,
    TEAMMATE_1_RIGHTFOOT,
    TEAMMATE_2_HEAD,
    TEAMMATE_2_LEFTARM,
    TEAMMATE_2_RIGHTARM,
    TEAMMATE_2_LEFTFOOT,
    TEAMMATE_2_RIGHTFOOT,
    TEAMMATE_3_HEAD,
    TEAMMATE_3_LEFTARM,
    TEAMMATE_3_RIGHTARM,
    TEAMMATE_3_LEFTFOOT,
    TEAMMATE_3_RIGHTFOOT,
    TEAMMATE_4_HEAD,
    TEAMMATE_4_LEFTARM,
    TEAMMATE_4_RIGHTARM,
    TEAMMATE_4_LEFTFOOT,
    TEAMMATE_4_RIGHTFOOT,
    TEAMMATE_5_HEAD,
    TEAMMATE_5_LEFTARM,
    TEAMMATE_5_RIGHTARM,
    TEAMMATE_5_LEFTFOOT,
    TEAMMATE_5_RIGHTFOOT,
    TEAMMATE_6_HEAD,
    TEAMMATE_6_LEFTARM,
    TEAMMATE_6_RIGHTARM,
    TEAMMATE_6_LEFTFOOT,
    TEAMMATE_6_RIGHTFOOT,
    TEAMMATE_7_HEAD,
    TEAMMATE_7_LEFTARM,
    TEAMMATE_7_RIGHTARM,
    TEAMMATE_7_LEFTFOOT,
    TEAMMATE_7_RIGHTFOOT,
    TEAMMATE_8_HEAD,
    TEAMMATE_8_LEFTARM,
    TEAMMATE_8_RIGHTARM,
    TEAMMATE_8_LEFTFOOT,
    TEAMMATE_8_RIGHTFOOT,
    TEAMMATE_9_HEAD,
    TEAMMATE_9_LEFTARM,
    TEAMMATE_9_RIGHTARM,
    TEAMMATE_9_LEFTFOOT,
    TEAMMATE_9_RIGHTFOOT,
    TEAMMATE_10_HEAD,
    TEAMMATE_10_LEFTARM,
    TEAMMATE_10_RIGHTARM,
    TEAMMATE_10_LEFTFOOT,
    TEAMMATE_10_RIGHTFOOT,
    TEAMMATE_11_HEAD,
    TEAMMATE_11_LEFTARM,
    TEAMMATE_11_RIGHTARM,
    TEAMMATE_11_LEFTFOOT,
    TEAMMATE_11_RIGHTFOOT = 63,

    OPPONENT_1_HEAD = 64,
    OPPONENT_1_LEFTARM,
    OPPONENT_1_RIGHTARM,
    OPPONENT_1_LEFTFOOT,
    OPPONENT_1_RIGHTFOOT,
    OPPONENT_2_HEAD,
    OPPONENT_2_LEFTARM,
    OPPONENT_2_RIGHTARM,
    OPPONENT_2_LEFTFOOT,
    OPPONENT_2_RIGHTFOOT,
    OPPONENT_3_HEAD,
    OPPONENT_3_LEFTARM,
    OPPONENT_3_RIGHTARM,
    OPPONENT_3_LEFTFOOT,
    OPPONENT_3_RIGHTFOOT,
    OPPONENT_4_HEAD,
    OPPONENT_4_LEFTARM,
    OPPONENT_4_RIGHTARM,
    OPPONENT_4_LEFTFOOT,
    OPPONENT_4_RIGHTFOOT,
    OPPONENT_5_HEAD,
    OPPONENT_5_LEFTARM,
    OPPONENT_5_RIGHTARM,
    OPPONENT_5_LEFTFOOT,
    OPPONENT_5_RIGHTFOOT,
    OPPONENT_6_HEAD,
    OPPONENT_6_LEFTARM,
    OPPONENT_6_RIGHTARM,
    OPPONENT_6_LEFTFOOT,
    OPPONENT_6_RIGHTFOOT,
    OPPONENT_7_HEAD,
    OPPONENT_7_LEFTARM,
    OPPONENT_7_RIGHTARM,
    OPPONENT_7_LEFTFOOT,
    OPPONENT_7_RIGHTFOOT,
    OPPONENT_8_HEAD,
    OPPONENT_8_LEFTARM,
    OPPONENT_8_RIGHTARM,
    OPPONENT_8_LEFTFOOT,
    OPPONENT_8_RIGHTFOOT,
    OPPONENT_9_HEAD,
    OPPONENT_9_LEFTARM,
    OPPONENT_9_RIGHTARM,
    OPPONENT_9_LEFTFOOT,
    OPPONENT_9_RIGHTFOOT,
    OPPONENT_10_HEAD,
    OPPONENT_10_LEFTARM,
    OPPONENT_10_RIGHTARM,
    OPPONENT_10_LEFTFOOT,
    OPPONENT_10_RIGHTFOOT,
    OPPONENT_11_HEAD,
    OPPONENT_11_LEFTARM,
    OPPONENT_11_RIGHTARM,
    OPPONENT_11_LEFTFOOT,
    OPPONENT_11_RIGHTFOOT = 118,
};

struct Limb
{
	salt::Vector3f 	head;
	salt::Vector3f 	leftarm;
	salt::Vector3f 	rightarm;
	salt::Vector3f	leftfoot;
	salt::Vector3f	rightfoot;
	bool	seehead;
	bool	seeleftarm;
	bool	seerightarm;
	bool	seeleftfoot;
	bool	seerightfoot;
};
#endif
