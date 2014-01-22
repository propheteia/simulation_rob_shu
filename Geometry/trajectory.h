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
#ifndef _TRAJECTORY_H
#define _TRAJECTORY_H

#include "tvector.h"
#include "matrix.h"
#include "gmath.h"

#include "spline.h"
#include "StaticModule.h"

//#define TT StaticModule<Trajectory>::getInstancePtr()


/** \class Trajectory produces the trajectories
 *  for walking. It plans the positions of hip
 *  and ankle according to this paper 
 *  'Planning walking patterns for a biped robot'.
 *  It has been expanded into three dimesion.
 */


class Trajectory 
{
public:
    Trajectory();

    ~Trajectory();

    bool General();

    inline bool ModelInited() const { return mModelInit; }

    void SetParams(const float params[10]);

    void InitModelParams(const float params[10]);

    /** matrix of normal trajectory */
    bool GetTrajectoryMatrix(float t,
                             salt::Matrix& torso,
                             salt::Matrix& holdfoot,
                             salt::Matrix& movefoot,
                             bool lefthold
                            );

    /** matrix of start trajectory */
    bool GetStartTrajectoryMatrix(float t,
                                  salt::Matrix& torso,
                                  salt::Matrix& holdfoot,
                                  salt::Matrix& movefoot,
                                  bool lefthold
                                 );

    /** matrix of end trajectory */
    bool GetEndTrajectoryMatrix(float t,
                                salt::Matrix& torso,
                                salt::Matrix& holdfoot,
                                salt::Matrix& movefoot,
                                bool lefthold
                               );

    /** return step size */
    float GetDs() const { return Ds; }

    /** return time of walking cycle */
    float GetTc() const { return Tc; }

    /** return time of double support phase */
    float GetTd() const { return Td; }

    /** return time of start phase */
    float GetTs() const { return Ts; }

    /** return time of end phase */
    float GetTe() const { return Te; }

protected:
    /** trajectory produced in 2D
     */
    float GetHipX(float t, int der = 0) const;

    float GetHipZ(float t, int der = 0) const;

    float GetHipTheta(float t, int der = 0) const;

    float GetFootX(float t, int der = 0) const;

    float GetFootZ(float t, int der = 0) const;

    float GetFootTheta(float t, int der = 0) const;

    /** calculate matrx of trajectory :
     *  hip, holdfoot and movefoot
     */
    void  CalTrajectoryMatrix(float t);

    void  CalStartTrajectoryMatrix(float t);

    void  CalEndTrajectoryMatrix(float t);

    void  CalWalkTrajectoryMatrix(bool lefthold);

protected:
    bool mModelInit;

    salt::Vector3f mDs3D;

    int k;

    /** step length */
    float Ds;

    /** walking cycle */
    float Tc;

    /** double support time of walking cycle */
    float Td;

    /** when foot reach the highest point in z axis */
    float Tm;

    float Qgs;

    float Qb;

    float Qf;

    float Qge;

    float Xed;

    float Xsd;

    /** hip.z */
    float Hnor;

    float Lan;

    float Laf;

    float Lab;

    float Lao;

    float Lth;

    float Lsh;

    float Hgs;

    float Hge;

    /** foot's highest point in z axis */
    float Hao;

    /** start phase time */
    float Ts;

    /** end phase time */
    float Te;

    /** turn angle */
    float mTurnAngle;

    float TorsoRot_X;

    float TorsoRelHip_Z;

    float FootOffset_X;

    /** normal phase */
    Spline foot_x;
    Spline foot_z;
    Spline foot_theta;
    Spline hip_x;
    Spline hip_z;

    /** start phase */
    Spline foot_start_x;
    Spline foot_start_z;
    Spline foot_start_theta;
    Spline hip_start_x;
    Spline hip_start_z;

    /** end phase */
    Spline foot_end_x;
    Spline foot_end_z;
    Spline foot_end_theta;
    Spline hip_end_x;
    Spline hip_end_z;

    salt::Matrix mHoldAnkleMatrix;
    salt::Matrix mMoveAnkleMatrix;
    salt::Matrix mHipMatrix;

    salt::Matrix mHoldFootMatrix;
    salt::Matrix mMoveFootMatrix;
    salt::Matrix mTorsoMatrix;
};

#endif
