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
#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include <boost/shared_array.hpp>
#include "matrix.h"
#include <string>
#include "types.h"
#include "Logger.h"

namespace oxygen {
    class Joint;
    class UniversalJoint;
    class HingeJoint;
};

class WorldModel;

class Robot
{
    friend class WorldModel;
public:
    static const JointID JID_ROOT   = 1;
    typedef std::vector<int> TIndex;

    struct Link {
        std::string name;
        std::string eff_name;
        int id;
        int twin;             // used for universal joint
        int sister;
        int child;
        int mother;
        salt::Vector3f a;     // axis
        salt::Vector3f b;     // relative position to mother's anchor
        salt::Vector3f c;     // geometry center in local coordinate system
        float q;              // joint angle
        float dq;
        float ddq;
        float m;
        salt::Vector3f p;
        salt::Matrix   R;
        salt::Vector3f v;
        salt::Vector3f w;

        Link() : name("<unnamed>"), eff_name("<unnamed>"),
                 id(0), twin(0), sister(0), child(0), mother(0), q(0.0f) {}
    };

    struct RobotPartInfo {
        std::string     name;
        salt::Vector3f  p;
        salt::Matrix    R;

		salt::Vector3f	localPos;	//By QuJunjun
		salt::Matrix	localRot;	//By QuJunjun
    };

    Robot();
    virtual ~Robot();

    virtual bool Init();

    virtual void UpdateLink(const salt::Matrix& mat);

    boost::shared_array<Robot::Link> GetLink() const;

    boost::shared_array<Robot::RobotPartInfo> GetRobotPartInfo() const;

    boost::shared_array<float> GetIKJointAngle() const;

    boost::shared_array<float> GetIKJointVel() const;

    salt::Matrix GetRobotPartMatrix(RobotPart part) const;

	salt::Matrix GetRobotCameraMatrix(RobotPart part) const; //add by Neil

    salt::Vector3f GetRobotPartPos(RobotPart part) const;

    RobotPart GetRobotPartMin() const { return PART_MIN; }

    RobotPart GetRobotPartMax() const { return PART_MAX; }

    JointID GetJointMin() const { return JID_MIN; }

    JointID GetJointMax() const { return JID_MAX; }

    void PrintLink(int j);

	salt::Vector3f GetRobotCoM();

    virtual bool IK_leg(const salt::Matrix& torso, const salt::Matrix& foot, bool left)
                 { return false; }

    virtual void CalcLegJointVel(const salt::Vector3f& vb, const salt::Vector3f& wb,
                                 const salt::Vector3f& vt, const salt::Vector3f& wt,
                                 bool left) {}

	void setHingeJointSenseMap ( JointID jid , HingeJointSense value);
	JointID checkJointIDMap ( std::string name );

protected:
    virtual void SetupLinks() = 0;

    virtual void SetupRobotPartInfo() = 0;

    virtual void SetupJointIDMap();


    /** A*x = B, size: A--row*column, x--colunm*1, B--row*1 */
    static bool Solve(float* X, const float** A, const float* B, int row, int column);

    static void CalcVWerr(float err[6], const salt::Matrix& ref, const salt::Matrix& now);

    TIndex FindRoute(int to);

    void ForwardKinematics(int j);

	void ForwardKinematicsForCamera(int j); //add by Neil

	float TotalMass(int j);//add by Neil
	salt::Vector3f CalcMC( int j);//add by Neil
	void CalcCoM();

    bool InverseKinematics(int to, const salt::Matrix& target);

    void CalcJacobian(float** J, const TIndex& idx);

    void ForwardVelocity(int j);

    virtual bool IK_leg(const salt::Matrix& hip, float A, float B,
                        const salt::Matrix& ankle, bool left) { return false; }

protected:
    boost::shared_array<Link> uLINK;

    boost::shared_array<RobotPartInfo> mRobotPartInfo;

	/**add for the limited vision by Neil*/
    boost::shared_array<Link> uLinkForCamera;

    boost::shared_array<RobotPartInfo> mRobotCameraInfo;
    /** joint angle calculated by IK */
    boost::shared_array<float> mIKJointAngle;

    /** joint velocities calculated by IK */
    boost::shared_array<float> mIKJointVel;

    JointID     JID_MIN;

    JointID     JID_MAX;

    RobotPart   PART_MIN;

    RobotPart   PART_MAX;

	/**Total mass*/
	float TMass;
	salt::Vector3f TMC;
	salt::Vector3f CoM;

    /** mapping from joint id to joint hinge sense object */
    THingeJointSenseMap mHingeJointSenseMap;

    /** mapping from joint id to joint hinge sense object */
    TUniversalJointSenseMap mUniversalJointSenseMap;

    /** mapping from object name to joint id */
    TJointIDMap mJointIDMap;
};


#endif
