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
#include "self.h"
#include "worldmodel.h"
#include "fieldinfo.h"
#include "nao.h"
#include "geometry.h"

using namespace std;
using namespace salt;
using namespace boost;
//using namespace zeitgeist;

Self::Self() :
        CurrentCycle(0),mGyroAngularVel(0.0f,0.0f,0.0f),axisX(1.0,0.0,0.0),
        mTorsoPitchAngleByGyro(0.0f),mTorsoRollAngleByGyro(0.0f),mTorsoYawAngleByGyro(0.0f),mRobotCoM(0.0f,0.0f,0.0f),
        mRobotModel(EUnknownModel), mAcceleration(0.0f, 0.0f, 9.8f)
{
}

Self::~Self()
{
}



string Self::GetModelFileName() const
{
    string name;

    switch (mRobotModel)
    {
    case ENaoModel:
        name = "rsg/agent/nao/nao.rsg";
        break;
    case ESoccerbotModel:
        name = "rsg/agent/soccerbot058/soccerbot.rsg";
        break;
    default:
        cerr << "(OnLink) Robot model unknown\n";
        break;
    }
    return name;
}

bool Self::Init()
{
    //robot type according to name
    string type = "nao";
    if (type == "nao")
    {
        mRobotModel = ENaoModel;
        if (! NAO->Init())
        {
            cerr << "[Self] ERROR: Robot init failed\n";
            return false;
        }
    }
    else if (type == "soccerbot") mRobotModel = ESoccerbotModel;
    else mRobotModel = EUnknownModel;

    transformMat.Identity();
    transformMat.Identity();
    transformMat.RotationZ(gDegToRad(90.0f));
    transformMat2.RotationZ(gDegToRad(-90.0f));
    return true;
}


void Self::setGyroAngularVel ( int i, float value )
{
    mGyroAngularVel[i] = value;
}

void Self::setAcceleration ( int i, float value )
{
	mAcceleration[i] = value;
}

void Self::setLFCenterVel ( int i,float value )
{
    mLFCenter[i] = value;
}

void Self::setRFCenterVel ( int i,float value )
{
    mRFCenter[i] = value;
}

void Self::setLFForce( int i, float value )
{
    mLFForce[i] = value;
}

void Self::setRFForce( int i, float value )
{
    mRFForce[i] = value;
}

void Self::UpdateRobotMatrix(const Matrix& mat)
{
    ++CurrentCycle;
    pos = mat.Pos();
    aLOG << "pos from matrix:" << pos << endl;
    aLOG<<"before transform: " << endl
    << mat.Right() << endl
    << mat.Up() << endl
    << mat.Forward() << endl;

    UpdateRobotTorsoInfo( mat );
    Matrix tMat = transformMat * mat;//need? by Neil,test,need

    NAO->UpdateLink(tMat);
    UpdateRobotCoM();
}

void Self::UpdateRobotCoM()
{
    mRobotCoM = transformMat2 * NAO->GetRobotCoM();
    aLOG << "RobotCoM : " << mRobotCoM << endl;
}


void Self::UpdateRobotTorsoInfo( const Matrix& mat )
{
    aLOG<<"===========================TorsoMatrix ============================= " << endl
    << "left or right round x-axis:roll "<< mat.Right() << endl
    << "up or down round y-axis:pitch "<<mat.Up() << endl
    << "forward or backward round z-axis:yaw "<<mat.Forward() << endl;
    SetTorsoYawAngle  ( mat );
    SetTorsoPitchAngle( mat, mTorsoYawAngle );
    SetTorsoRollAngle ( mat, mTorsoYawAngle );

    torsoMatrixByGyro = Rodrigues( axisX,gDegToRad(-mGyroAngularVel.x()) );
    aLOG<<"torsoMatrixByGyroX = " << endl
    << torsoMatrixByGyro.Right() << endl
    << torsoMatrixByGyro.Up() << endl
    << torsoMatrixByGyro.Forward() << endl;

    SetTorsoPitchAngleByGyro ( torsoMatrixByGyro );

    mTorsoYawAngle = gRadToDeg(mTorsoYawAngle);
    mTorsoRollAngle = gRadToDeg(mTorsoRollAngle);
    mTorsoPitchAngle = gRadToDeg(mTorsoPitchAngle);
    aLOG << "TorsoRollAngle:" << mTorsoRollAngle << " TorsoPitchAngle:"<< mTorsoPitchAngle
    << " TorsoYawAngle: "<< mTorsoYawAngle << endl
    << " GyroAngularVel" << mGyroAngularVel*0.02f << endl
    << "TorsoRollAngleByGyro:" << mTorsoRollAngleByGyro << " TorsoPitchAngleByGyro:"<< mTorsoPitchAngleByGyro
    << " TorsoYawAngleByGyro: "<< mTorsoYawAngleByGyro << endl
    <<"=================================================================="<<endl;
}

void Self::SetTorsoRollAngle ( const Matrix& mat,const float YawAngle )
{
    mTorsoRollAngle = gArcTan2( gSin(YawAngle)*mat.Forward().x()-gCos(YawAngle)*mat.Forward().y(),-gSin(YawAngle)*mat.Up().x()+gCos(YawAngle)*mat.Up().y());
}

void Self::SetTorsoPitchAngle( const Matrix& mat,const float YawAngle )
{
    mTorsoPitchAngle = gArcTan2( -mat.Right().z(),gCos(YawAngle)*mat.Right().x()+gSin(YawAngle)*mat.Right().y());
}

void Self::SetTorsoYawAngle  ( const Matrix& mat )
{
    //if (WM.CanLocalize())
        mTorsoYawAngle = gArcTan2( mat.Right().y() ,mat.Right().x());
    //else
    //{	mTorsoYawAngle = gDegToRad(mTorsoYawAngle + RotationfromWalkingEngine/0.16*0.02);
    //}
}

float Self::GetTorsoRollAngle() const
{
    return mTorsoRollAngle;
}

float Self::GetTorsoPitchAngle() const
{
    return mTorsoPitchAngle;
}

float Self::GetTorsoYawAngle () const
{
    return mTorsoYawAngle;
}

Vector3f Self::GetRobotCoM () const
{
    return mRobotCoM;
}

Vector3f Self::GetAcceleration() const
{
	return mAcceleration;
}

void Self::SetTorsoRollAngleByGyro ( const Matrix& mat )
{
    //mTorsoRollAngleByGyro = gArcTan2();
}

void Self::SetTorsoPitchAngleByGyro( const Matrix& mat )
{
    mTorsoPitchAngleByGyro += gRadToDeg(gArcTan2( mat.Up().z() ,mat.Up().y()))*0.02f;

}

void Self::SetTorsoYawAngleByGyro  ( const Matrix& mat )
{
    //mTorsoRollAngleByGyro = gArcTan2();
}

float Self::GetTorsoRollAngleByGyro() const
{
    return mTorsoRollAngleByGyro;
}

float Self::GetTorsoPitchAngleByGyro() const
{
    return mTorsoPitchAngleByGyro;
}

float Self::GetTorsoYawAngleByGyro () const
{
    return mTorsoYawAngleByGyro;
}
bool Self::onMyfeet()
{
    aLOG << "x: " << mLFForce[0] << " y: "<< mLFForce[1] << " z: "<< mLFForce[2] << endl;
    aLOG << "x: " << mRFForce[0] << " y: "<< mRFForce[1] << " z: "<< mRFForce[2] << endl;
    if ( mLFForce[2] > 0 && mRFForce[2] > 0 )
    {
        aLOG << "OK,On My feet" << endl;
    }
}

bool Self::onMyback()
{
    if ( mTorsoPitchAngle < -60.0)
        return true;
    return false;
}

bool Self::onMyBelly()
{
    if ( mTorsoPitchAngle > 60.0)
        return true;
    return false;
}

bool Self::onMySide()
{
    if (gAbs( mTorsoRollAngle ) >60.0)
        return true;
    return false;
}

//以下为重心稳定判断————2009.12.20 BY ian.pan
bool Self::mCoMIsStable()
{
    aLOG<<"##################### CoM Stability justify ##################"<<endl;
    aLOG<<"LFCENTER :"<<mLFCenter[0]<<","<<mLFCenter[1]<<","<<mLFCenter[2]<<endl;
    aLOG<<"RFCENTER :"<<mRFCenter[0]<<","<<mRFCenter[1]<<","<<mRFCenter[2]<<endl;
    if (mLFForce.Length()>0)
    {
        if (mRFForce.Length()>0)
        {
            float eCoM1=sqrt ( ((mRobotCoM[0]-mLFCenter[0])*(mRobotCoM[0]-mLFCenter[0])) + ((mRobotCoM[1]-mLFCenter[1])*(mRobotCoM[1]-mLFCenter[1])) - (((mLFCenter[0]-mRFCenter[0])*(mLFCenter[0]-mRFCenter[0])+(mLFCenter[1]-mRFCenter[1])*(mLFCenter[1]-mRFCenter[1]))/4) );

            float eCoM2=sqrt ( ((mRobotCoM[0]-mRFCenter[0])*(mRobotCoM[0]-mRFCenter[0])) + ((mRobotCoM[1]-mRFCenter[1])*(mRobotCoM[1]-mRFCenter[1])) - (((mLFCenter[0]-mRFCenter[0])*(mLFCenter[0]-mRFCenter[0])+(mLFCenter[1]-mRFCenter[1])*(mLFCenter[1]-mRFCenter[1]))/4) );

            float eCoM = ( eCoM1 + eCoM2 ) / 2 ;
            if (eCoM<0.08)
            {
                aLOG<<"ON 2 feet,CoM is true"<< eCoM << endl;
                return true;
            }
            else
            {
                aLOG<<"ON 2 feet,CoM is false"<< eCoM <<endl;
                return false;
            }
        }

        else
        {
            float dLCoM = sqrt ((mRobotCoM[0]-mLFCenter[0])*(mRobotCoM[0]-mLFCenter[0])) + ((mRobotCoM[1]-mLFCenter[1])*(mRobotCoM[1]-mLFCenter[1]));
            if (dLCoM < 0.10)
            {
                aLOG<<"On Leftfoot, CoM is true"<< endl;
                return true;
            }
            else
            {
                aLOG<<"On Leftfoot, CoM is false"<<endl;
                return false;
            }
        }
    }

    else if ( mRFForce.Length()>0 )
    {
        float dRCoM = sqrt ((mRobotCoM[0]-mRFCenter[0])*(mRobotCoM[0]-mRFCenter[0])) + ((mRobotCoM[1]-mRFCenter[1])*(mRobotCoM[1]-mRFCenter[1]));
        if (dRCoM < 0.10)
        {
            aLOG<<"OnRightfoot,CoM is true"<<endl;
            return true;
        }
        else
        {
            aLOG<<"OnRightfoot,CoM is false"<<endl;
            return false;
        }
    }
    else
    {
        aLOG<<"in the air,CoM is false"<<endl;
        return false;
    }
}

