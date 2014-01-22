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
#include "trajectory.h"
//#include "log.h"

#define FLOAT_EPS 1e-05

using namespace std;
using namespace salt;

Trajectory::Trajectory()
{
    mModelInit = false;

    mDs3D   = Vector3f(0.0, 0.1, 0.0);
    Ds      = Vector2f(mDs3D.x(), mDs3D.y()).Length();
    k       =     1;
    Tc      =  0.14f;
    Td      =  0.2 * Tc;
    Tm      =  0.5 * Tc;
    Qgs     =  0.0f;
    Qb      =  0.22f;
    Qf      =  0.22f;
    Qge     =  0.0f;
    Xed     =  0.5 * Ds ; //0.48
    Xsd     =  0.5 * Ds; //0.34
    Lao     =  0.5 * Ds;
    Hnor    =  0.23f;
    Lan     =  0.035f;
    Laf     =  0.08f;
    Lab     =  0.08f;
    Lth     =  0.1201f;//NAO_A   //0.115f;
    Lsh     =  0.1f;//NAO_B
    Hgs     =  0.015f;
    Hge     =  0.015f;
    Hao     =  0.10f;

    Ts      =  1.0f * Tc;
    Te      =  1.0f * Tc;
    mTurnAngle = 0.0f;

    TorsoRot_X = -10.0f;;
    TorsoRelHip_Z = 0.115f;
    FootOffset_X = 0.055f;

    mHoldAnkleMatrix.Identity();
    mMoveAnkleMatrix.Identity();
    mHipMatrix.Identity();
}

Trajectory::~Trajectory()
{
}

void Trajectory::SetParams(const float params[10])
{
    this->mDs3D[0]  = params[0];
    this->mDs3D[1]  = params[1];
    Ds              = Vector2f(mDs3D.x(), mDs3D.y()).Length();
    this->Tc        = params[2];
    this->Td        = params[3];
    this->Tm        = params[4];
    this->Xed       = params[5];
    this->Xsd       = params[6];
    this->Hnor      = params[7];
    this->Hao       = params[8];
    this->mTurnAngle= params[9];
    Lao             = 0.5f * Ds;
    Ts              = 1.0f * Tc;
    Te              = 1.0f * Tc;
}

void Trajectory::InitModelParams(const float params[10])
{
    if (mModelInit) return ;

    this->Lan       =  params[0];
    this->Laf       =  params[1];
    this->Lab       =  params[2];
    this->Lth       =  params[3];
    this->Lsh       =  params[4];
    this->Hgs       =  params[5];
    this->Hge       =  params[6];
    this->TorsoRot_X    = params[7];
    this->TorsoRelHip_Z = params[8];
    this->FootOffset_X  = params[9];
    mModelInit          = true;
}

bool Trajectory::General()
{
    float t[5];
    float f[5];

    t[0] = k * Tc;
    t[1] = k * Tc + Td;
    t[2] = (k + 1) * Tc;
    t[3] = (k + 1) * Tc + Td;

    f[0] = Qgs;
    f[1] = -Qb;
    f[2] = Qf;
    f[3] = -Qge;

    foot_theta.SetBreakPoints(t, f, 4);
    if (! foot_theta.General()) return false;

    t[0] = k * Tc;
    t[1] = k * Tc + Td;
    t[2] = k * Tc + Tm;
    t[3] = (k + 1) * Tc;
    t[4] = (k + 1) * Tc + Td;

    f[0] = k * Ds;
    f[1] = k * Ds + Lan * sin(Qb) + Laf * (1.0f - cos(Qb));
    f[2] = k * Ds + Lao;
    f[3] = (k + 2) * Ds - Lan * sin(Qf) - Lab * (1.0f - cos(Qf));
    f[4] = (k + 2) * Ds;

    foot_x.SetBreakPoints(t, f, 5);
    if (! foot_x.General()) return false;

    f[0] = Hgs + Lan;
    f[1] = Hgs + Laf * sin(Qb) + Lan * cos(Qb);
    f[2] = Hao;
    f[3] = Hge + Lab * sin(Qf) + Lan * cos(Qf);
    f[4] = Hge + Lan;

    foot_z.SetBreakPoints(t, f, 5);
    if (! foot_z.General()) return false;

    t[0] = k * Tc + 0.5 * Td;
    t[1] = k * Tc + 0.5 * (Tc - Td);
    t[2] = (k + 1) * Tc + 0.5 * Td;

    f[0] = Hnor;
    f[1] = Hnor;
    f[2] = Hnor;

    hip_z.SetBreakPoints(t, f, 3);
    if (! hip_z.General()) return false;

    /** *********************** start phase splines ************/
    t[0] = 0.0f;
    t[1] = Ts;

    f[0] = 0.0f;
    f[1] = Qf;

    foot_start_theta.SetBreakPoints(t, f, 2);
    if (! foot_start_theta.General()) return false;

    t[0] = 0.0f;
    t[1] = Ts;

    f[0] = 0.0f;
    f[1] = Ds - Lan * sin(Qf) - Lab * (1.0f - cos(Qf));

    foot_start_x.SetBreakPoints(t, f, 2);
    if (! foot_start_x.General()) return false;

    t[0] = 0.0f;
    t[1] = Td;
    t[2] = Ts;

    f[0] = Hgs + Lan;
    f[1] = Hgs + Laf * sin(Qb) + Lan * cos(Qb);
    f[2] = Hge + Lab * sin(Qf) + Lan * cos(Qf);

    foot_start_z.SetBreakPoints(t, f, 3);
    if (! foot_start_z.General()) return false;

    t[0] = 0.0f;
    t[1] = Ts;

    f[0] = 0.0f;
    f[1] = Xed;

    hip_start_x.SetBreakPoints(t, f, 2);
    if (! hip_start_x.General()) return false;

    t[0] = 0.0f;
    t[1] = 0.5f * Td;
    t[2] = Ts;

    f[0] = Hnor;
    f[1] = Hnor;
    f[2] = Hnor;

    hip_start_z.SetBreakPoints(t, f, 3);
    if (! hip_start_z.General()) return false;

    /** ***************** end phase *************************/
    t[0] = 0.0f;
    t[1] = Te;

    f[0] = Lan * sin(Qb) + Laf * (1.0f - cos(Qb));
    f[1] = Ds;

    foot_end_x.SetBreakPoints(t, f, 2);
    if (! foot_end_x.General()) return false;

    t[0] = 0.0f;
    t[1] = Te;

    f[0] = Hgs + Laf * sin(Qb) + Lan * cos(Qb);
    f[1] = Hge + Lan;

    foot_end_z.SetBreakPoints(t, f, 2);
    if (! foot_end_z.General()) return false;

    t[0] = 0.0f;
    t[1] = Te;

    f[0] = Ds - Xsd;
    f[1] = Ds;

    hip_end_x.SetBreakPoints(t, f, 2);
    if (! hip_end_x.General()) return false;

    t[0] = 0.0f;
    t[1] = 0.5f * Td;
    t[2] = Te;

    f[0] = Hnor;
    f[1] = Hnor;
    f[2] = Hnor;

    hip_end_z.SetBreakPoints(t, f, 3);
    if (! hip_end_z.General()) return false;

    return true;
}

float Trajectory::GetHipX(float t, int der) const
{
    /** this should not happen */
    if (t < k * Tc - FLOAT_EPS || t > (k + 1) * Tc + FLOAT_EPS)
    {
        cerr << "(Trajectory getHipX error)\n";
        return 0.0f;
    }

    float rec = 0.0f;

    float A = k * Tc - t;
    float B = Tc - Td;
    float C = Ds - Xsd;
    float D = Ds + Xed;
    float E = Ds - Xed - Xsd;

    if (t < k * Tc + Td)
    {
        rec = k * Ds + E / (B * Td * Td) *
            ( (Td + A) * (Td + A) * (Td + A) - (-A) * (-A) * (-A) 
              - Td * Td * (Td + A) + Td * Td * (-A) )
            + Xed / Td * (Td + A)
            + C / Td * (-A);
    }
    else
    {
        rec = k * Ds + E / (B * B * Td) *
            ( (-A - Td) * (-A - Td) * (-A - Td) - (Tc + A) * (Tc + A) * (Tc + A) 
              + B * B * (Tc + A) - B * B * (-A - Td)) 
            + C / B * (Tc + A) 
            + D / B * (-A - Td);
    }

    return rec;
}

float Trajectory::GetHipZ(float t, int der) const
{
    /** this should not happen */
    if (t < k * Tc + 0.5f * Td - FLOAT_EPS ||
        t > (k + 1) * Tc + 0.5f * Td + FLOAT_EPS)
    {
        cerr << "(Trajectory getHipZ error)\n";
        return 0.0f;
    }

    float rec = 0.0f;

    rec = hip_z.GetYGivenX(t, der);

    return rec;
}

float Trajectory::GetHipTheta(float t, int der) const
{
    float rec = 0.0f;

    if (der == 0)
    {
        rec = M_PI / 2;
    }
    else if (der >= 1)
    {
        rec = 0.0f;
    }
    else
    {
        cerr << "(Trajectory getHipTheta der error)\n";
    }

    return rec;
}

float Trajectory::GetFootX(float t, int der) const
{
    /** this should not happen */
    if (t < k * Tc - FLOAT_EPS || t > (k + 2) * Tc + FLOAT_EPS)
    {
        cerr << "(Trajectory) getFootX error\n";
        return 0.0f;
    }

    float rec = 0.0f;

    if (t < (k + 1) * Tc + Td)
    {
        rec = foot_x.GetYGivenX(t, der);
    }
    else
    {
        if (der == 0)
        {
            rec = (k + 2) * Ds;
        }
        else if (der >= 1)
        {
            rec = 0.0f;
        }
        else
        {
            cerr << "(Trajectory getFootX der error)\n";
        }
    }

    return rec;
}

float Trajectory::GetFootZ(float t, int der) const
{
    /** this should not happen */
    if (t < k * Tc - FLOAT_EPS || t > (k + 2) * Tc + FLOAT_EPS)
    {
        cerr << "(Trajectory) getFootZ error\n";
        return 0.0f;
    }

    float rec = 0.0f;

    if (t < (k + 1) * Tc + Td)
    {
        rec = foot_z.GetYGivenX(t, der);
    }
    else
    {
        if (der == 0)
        {
            rec = Hge + Lan;
        }
        else if (der >= 1)
        {
            rec = 0.0f;
        }
        else
        {
             cerr << "(Trajectory getFootZ der error)\n";
        }
    }

    return rec;
}

float Trajectory::GetFootTheta(float t, int der) const
{
    /** this should not happen */
    if (t < k * Tc - FLOAT_EPS || t > (k + 2) * Tc + FLOAT_EPS) return 0.0f;

    float rec = 0.0f;

    if (t < (k + 1) * Tc + Td)
    {
        rec = foot_theta.GetYGivenX(t, der);
    }
    else
    {
        if (der == 0)
        {
            rec = -Qge;
        }
        else if (der >= 1)
        {
            rec = 0.0f;
        }
        else
        {
            cerr << "(Trajectory getFootTheta der error)\n";
        }
    }

    return rec;
}

void Trajectory::CalTrajectoryMatrix(float t)
{
    if (t < k * Tc - FLOAT_EPS || t > (k + 2) * Tc + FLOAT_EPS)
    {
        cerr << "Trajectory calTrajectoryMatrix error\n";
    }

    Vector3f hip_pos, holdankle_pos, moveankle_pos;
    float hx, hz, fx1, fz1, fx2, fz2, ft1, ft2;

    if (t < (k + 1) * Tc)
    {
        hx = GetHipX(t);
    }
    else
    {
        hx = GetHipX(t - Tc) + Ds;
    }

    if (t < k * Tc + 0.5 * Td)
    {
        hz = GetHipZ(t + Tc);
    }
    else if ( t < (k + 1) * Tc + 0.5 * Td)
    {
        hz = GetHipZ(t);
    }
    else
    {
        hz = GetHipZ(t - Tc);
    }

    if (t < (k + 1) * Tc + Td)
    {
        fx1 = GetFootX(t);
        ft1 = GetFootTheta(t);
        fz1 = GetFootZ(t);
    }
    else
    {
        fx1 = (k + 2) * Ds;
        ft1 = -Qge;
        fz1 = Hge + Lan;
    }

    if (t < k * Tc + Td)
    {
        fx2 = GetFootX(t + Tc) - Ds;
        ft2 = GetFootTheta(t + Tc);
        fz2 = GetFootZ(t + Tc);
    }
    else if (t < (k + 1) * Tc )
    {
        fx2 = GetFootX(t + Tc) - Ds;
        ft2 = GetFootTheta(t + Tc);
        fz2 = GetFootZ(t + Tc);
    }
    else
    {
        fx2 = GetFootX(t - Tc) + Ds;
        ft2 = GetFootTheta(t - Tc);
        fz2 = GetFootZ(t - Tc);
    }

    hip_pos = hx / Ds * mDs3D;
    hip_pos[2] = hz;
    mHipMatrix.Pos() = hip_pos;

    holdankle_pos = fx2 / Ds * mDs3D;
    holdankle_pos[2] = fz2;
    mHoldAnkleMatrix.Pos() = holdankle_pos;

    moveankle_pos = fx1 / Ds * mDs3D;
    moveankle_pos[2] = fz1;
    mMoveAnkleMatrix.Pos() = moveankle_pos;
}

bool Trajectory::GetTrajectoryMatrix(float t,
                                     salt::Matrix& torso,
                                     salt::Matrix& holdfoot,
                                     salt::Matrix& movefoot,
                                     bool lefthold
                                    )
{
    if (t < k * Tc - FLOAT_EPS || t > (k + 2) * Tc + FLOAT_EPS)
    {
        cerr << "Trajectory getTrajectoryMatrix error\n";
        return false;
    }

    CalTrajectoryMatrix(t);
    CalWalkTrajectoryMatrix(lefthold);
    torso = mTorsoMatrix;
    movefoot = mMoveFootMatrix;
    holdfoot = mHoldFootMatrix;

    /** turn in normal step (not used in start or end phase)
     *  hold foot matrix should not be rotated.
     */
    //torso.RotateZ(gDegToRad(-mTurnAngle));
    //movefoot.RotateZ(gDegToRad(-mTurnAngle));
    holdfoot.RotateZ(gDegToRad(mTurnAngle));

    return true;
}

void Trajectory::CalStartTrajectoryMatrix(float t)
{
    if ((t < 0.0f - FLOAT_EPS) || (t > Ts + FLOAT_EPS))
    {
        cerr << "Trajectory calStartTrajectoryMatrix error\n";
        return ;
    }

    Vector3f hip_pos, holdankle_pos, moveankle_pos;
    float hx, hz, fx1, fz1, fx2, fz2;

    hx = hip_start_x.GetYGivenX(t);
    hz = hip_start_z.GetYGivenX(t);
    fx1 = 0.0f;
    fz1 = Hge + Lan;
    fx2 = foot_start_x.GetYGivenX(t);
    fz2 = foot_start_z.GetYGivenX(t);

    hip_pos = hx / Ds * mDs3D;
    hip_pos[2] = hz;
    mHipMatrix.Pos() = hip_pos;

    holdankle_pos = fx1 / Ds * mDs3D;
    holdankle_pos[2] = fz1;
    mHoldAnkleMatrix.Pos() = holdankle_pos;

    moveankle_pos = fx2 / Ds * mDs3D;
    moveankle_pos[2] = fz2;
    mMoveAnkleMatrix.Pos() = moveankle_pos;
}

bool Trajectory::GetStartTrajectoryMatrix(float t,
                                          salt::Matrix& torso,
                                          salt::Matrix& holdfoot,
                                          salt::Matrix& movefoot,
                                          bool lefthold
                                         )
{
    if ((t < 0.0f - FLOAT_EPS) || (t > Ts + FLOAT_EPS))
    {
        cerr << "Trajectory getStartTrajectoryMatrix error\n";
        return false;
    }

    CalStartTrajectoryMatrix(t);
    CalWalkTrajectoryMatrix(lefthold);
    torso = mTorsoMatrix;
    movefoot = mMoveFootMatrix;
    holdfoot = mHoldFootMatrix;

    return true;
}

void Trajectory::CalEndTrajectoryMatrix(float t)
{
    if ((t < 0.0f - FLOAT_EPS) || (t > Te + FLOAT_EPS))
    {
        cerr << "Trajectory calEndTrajectoryMatrix error\n";
        return ;
    }

    Vector3f hip_pos, holdankle_pos, moveankle_pos;
    float hx, hz, fx1, fz1, fx2, fz2;

    hx = hip_end_x.GetYGivenX(t);
    hz = hip_end_z.GetYGivenX(t);
    fx1 = Ds;
    fz1 = Hge + Lan;
    fx2 = foot_end_x.GetYGivenX(t);
    fz2 = foot_end_z.GetYGivenX(t);

    hip_pos = hx / Ds * mDs3D;
    hip_pos[2] = hz;
    mHipMatrix.Pos() = hip_pos;

    holdankle_pos = fx1 / Ds * mDs3D;
    holdankle_pos[2] = fz1;
    mHoldAnkleMatrix.Pos() = holdankle_pos;

    moveankle_pos = fx2 / Ds * mDs3D;
    moveankle_pos[2] = fz2;
    mMoveAnkleMatrix.Pos() = moveankle_pos;
}

bool Trajectory::GetEndTrajectoryMatrix(float t,
                                        salt::Matrix& torso,
                                        salt::Matrix& holdfoot,
                                        salt::Matrix& movefoot,
                                        bool lefthold
                                       )
{
    if ((t < 0.0f - FLOAT_EPS) || (t > Te + FLOAT_EPS))
    {
        cerr << "Trajectory getEndTrajectoryMatrix error\n";
        return false;
    }

    CalEndTrajectoryMatrix(t);
    CalWalkTrajectoryMatrix(lefthold);
    torso = mTorsoMatrix;
    movefoot = mMoveFootMatrix;
    holdfoot = mHoldFootMatrix;

    return true;
}

/** the comment code: should the offset both
 * in axis 'x' and 'y' be considered ?
 */
void Trajectory::CalWalkTrajectoryMatrix(bool lefthold)
{
    mTorsoMatrix                = mHipMatrix;
    // mTorsoMatrix.Pos()[1]      += 0.01f;
    mTorsoMatrix.Pos()[2]      += TorsoRelHip_Z;
    mTorsoMatrix.RotateX(gDegToRad(TorsoRot_X));

    mHoldFootMatrix             = mHoldAnkleMatrix;
    float offset = lefthold ? FootOffset_X : -FootOffset_X;
    mHoldFootMatrix.Pos()[0]   -= offset;
    //mHoldFootMatrix.Pos()[1]   += 0.005f;// - 0.03f;
    mHoldFootMatrix.Pos()[2]   -= Lan;

    mMoveFootMatrix             = mMoveAnkleMatrix;
    mMoveFootMatrix.Pos()[0]   += offset;
    //mMoveFootMatrix.Pos()[1]   += 0.005f;// - 0.03f;
    mMoveFootMatrix.Pos()[2]   -= Lan;
}
