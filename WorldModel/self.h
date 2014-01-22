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
#ifndef __SELF_H
#define __SELF_H

#include "types.h"
#include "robot.h"
#include "player.h"

#include "nao.h"


#include "StaticModule.h"

//#define WM.getSelf() StaticModule<Self>::getInstance()


class TrainServer;
class WorldModel;
class FieldInfo;
class Action;

class Self : public Player
{
    friend class WorldModel;
public:
    enum ERobotModel
    {
        EUnknownModel,
        ENaoModel,
        ESoccerbotModel
    };

	enum RoleInTeam
	{
		golie = 0,
		forward1 = 1,
		forward2 = 2,
		center = 3,
		guard1 = 4,
		guard2 = 5,
		guard3 = 6,
		guard4 = 7,
		guard5 = 8,
        guard6 = 9,
		guard7 = 10,
		unknow = -1
	};

public:
    Self();
    ~Self();

    int CurrentCycle;

    bool Init();

    void UpdateRobotMatrix(const Matrix& mat);
    void UpdateRobotTorsoInfo( const Matrix& mat);//by Neil
    void UpdateRobotCoM();

    std::string GetModelFileName() const;

    ERobotModel GetRobotModel() const;
    /** add these function by Neil*/
    void setGyroAngularVel ( int i, float value );
	void setAcceleration   ( int i, float value );	//!By Qu Junjun 2010.1.23
    void setLFCenterVel    ( int i, float value );
    void setRFCenterVel    ( int i, float value );
    void setLFForce        ( int i, float value );
    void setRFForce        ( int i, float value );
    //this method could not be used under restricted vision
    void SetTorsoRollAngle ( const Matrix& mat,const float YawAngle );
    void SetTorsoPitchAngle( const Matrix& mat,const float YawAngle );
    void SetTorsoYawAngle  ( const Matrix& mat );
    float GetTorsoRollAngle() const;
    float GetTorsoPitchAngle() const;
    float GetTorsoYawAngle () const;

	const salt::Vector3f & getGyroAngulaVel() { return mGyroAngularVel; }


	/** Get the FRP Infomation, 	By QuJunjun */
	const salt::Vector3f & getLFCenter() { 	return mLFCenter; }
	const salt::Vector3f & getRFCenter() { 	return mRFCenter; }
	const salt::Vector3f & getLFForce()  {	return mLFForce;  }
	const salt::Vector3f & getRFForce()  {	return mRFForce; }

    Vector3f GetRobotCoM() const;

	Vector3f GetAcceleration() const;	//!By Qu Junjun 2010.1.23

    //this method could be used under restricted vision
    void SetTorsoRollAngleByGyro ( const Matrix& mat );
    void SetTorsoPitchAngleByGyro( const Matrix& mat );
    void SetTorsoYawAngleByGyro  ( const Matrix& mat );
    float GetTorsoRollAngleByGyro() const;
    float GetTorsoPitchAngleByGyro() const;
    float GetTorsoYawAngleByGyro () const;

    bool onMyfeet();
    bool onMyback();
    bool onMyBelly();
    bool onMySide();
    bool mCoMIsStable();//by Pan Yan

    inline void SetWalkingDsForLocalize(Vector3f Ds,double rotation)//add by Neil
    {
        DsfromWalkingEngine = Ds ;
	RotationfromWalkingEngine = rotation;
    }

    float TorsoYawAngleByHearing;//By Qu JunJun 2010.3.23 This YawAngle is calculated according to infomation of hearing
    float mTorsoYawAngle;

	RoleInTeam roleinteam;
private:

    ERobotModel                 mRobotModel;

    /** hear infomation received from hear msg*/
    HearInfo                    mHearInfo;

    /** center position of left foot force resistance perceptor
     *  (in local coordinate)
     */
    Vector3f              mLFCenter;

    /** center position of right foot force resistance perceptor
     *  (in local coordinate)
     */
    Vector3f              mRFCenter;

    /** force of left foot force resistance perceptor
     *  (in local coordinate)
     */
    Vector3f              mLFForce;

    /** force of right foot force resistance perceptor
     *  (in local coordinate)
     */
    Vector3f              mRFForce;

    /** angular veolcity of torso (in local coordinate, in degrees)
     */
    Vector3f              mGyroAngularVel;

	Vector3f              mAcceleration;	//!By Qu Junjun 2010.1.23

    Matrix	              torsoMatrixByGyro;
    Vector3f              axisX;

    Vector3f              mRobotCoM;

    Matrix transformMat;//for change the Vision coordinate to Robot's world coordinate
    Matrix transformMat2;

    /** the roll,pitch.and yaw of the robot torso*/
    float mTorsoRollAngle;
    float mTorsoPitchAngle;

    float mTorsoRollAngleByGyro;
    float mTorsoPitchAngleByGyro;
    float mTorsoYawAngleByGyro;

    Vector3f DsfromWalkingEngine;//for localization
    double RotationfromWalkingEngine;

};

#endif //__SELF_H
