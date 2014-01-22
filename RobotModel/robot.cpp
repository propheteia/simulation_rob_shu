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
#include "robot.h"
//#include "log"
#include "geometry.h"

using namespace std;
using namespace salt;
using namespace boost;

Robot::Robot() :
    JID_MIN(0),
    JID_MAX(0),
    PART_MIN(0),
    PART_MAX(0),
	TMass(0),
	TMC(0.0,0.0,0.0),
	CoM(0.0,0.0,0.0)
{
}

Robot::~Robot()
{
}

bool Robot::Init()
{
    uLINK.reset(new Link[JID_MAX + 1]);
    mRobotPartInfo.reset(new RobotPartInfo[PART_MAX + 1]);

	/**add for the limited vision by Neil*/
	uLinkForCamera.reset(new Link[JID_MAX + 1]);
    	mRobotCameraInfo.reset(new RobotPartInfo[PART_MAX + 1]);

    mIKJointAngle.reset(new float[JID_MAX + 1]);
    mIKJointVel.reset(new float[JID_MAX + 1]);

    SetupLinks(); // must before 'SetupJointIDMap'
    SetupRobotPartInfo();
    SetupJointIDMap();
    return true;
}

void Robot::SetupJointIDMap()
{
    mJointIDMap.clear();
    for (int i = JID_MIN; i <= JID_MAX; ++i)
        mJointIDMap[uLINK[i].name] = i;
}

void Robot::setHingeJointSenseMap ( JointID jid , HingeJointSense value)
{	mHingeJointSenseMap[jid] = value;
}

JointID Robot::checkJointIDMap( std::string name )
{	
	TJointIDMap::iterator idIter = mJointIDMap.find ( name ) ;
	if (idIter == mJointIDMap.end())
    	{	pLOG << "[Robot] unknown joint id: " << name << endl;
        }
	return (*idIter).second;
	
}

void Robot::UpdateLink(const salt::Matrix& mat)
{

    /** update joint angles */
    for (int i = GetJointMin(); i <= GetJointMax(); ++i)
    {
        if (i == JID_ROOT) continue;
        /** NOTE if the link is the top one or not been
         * set up correctly, it will not be updated */
        // if (uLINK[i].mother == 0) continue;
        int twin = uLINK[i].twin;
        if (twin == 0) // hinge joint effector
        {
            uLINK[i].q = gDegToRad(mHingeJointSenseMap[i].angle);
        }
        else // universal joint effector
        {
            int child = uLINK[i].child;
            if (twin == child) // first twin
            {
                uLINK[i].q      = gDegToRad(mUniversalJointSenseMap[i].angle1);
                uLINK[twin].q   = gDegToRad(mUniversalJointSenseMap[i].angle2);
            }
        }
    }
	/** update root in local coordinates*/
    uLINK[JID_ROOT].R = mat.GetIdentity();
    uLINK[JID_ROOT].p = uLINK[JID_ROOT].R.Pos();

	/** update robot part information in local coordinates*/ 
	ForwardKinematics(JID_ROOT);

	for (RobotPart part = GetRobotPartMin();
         part <= GetRobotPartMax();
         ++part)
    {
        mRobotPartInfo[part].localPos = uLINK[part].p +
                                 uLINK[part].R.Rotate(uLINK[part].c);
        mRobotPartInfo[part].localRot = uLINK[part].R;

        // NOTE (for old code)
        mRobotPartInfo[part].R.Pos() = mRobotPartInfo[part].p;
    }


    /** update root in globle coordinates*/
	uLINK[JID_ROOT].R = mat;
    uLINK[JID_ROOT].p = mat.Pos();
    ForwardKinematics(JID_ROOT);
	//cout <<"****************************PrintLink*************************"<< endl;
	//PrintLink(JID_ROOT);//test the link,by Neil.
	TMC = CalcMC(JID_ROOT);
	aLOG << "TMC : " << TMC << endl;
	CalcCoM();
	aLOG << "CoM : " << CoM << endl;

    /** update robot part information in globle coordinates*/
    for (RobotPart part = GetRobotPartMin();
         part <= GetRobotPartMax();
         ++part)
    {
        mRobotPartInfo[part].p = uLINK[part].p +
                                 uLINK[part].R.Rotate(uLINK[part].c);
        mRobotPartInfo[part].R = uLINK[part].R;

        // NOTE (for old code)
        mRobotPartInfo[part].R.Pos() = mRobotPartInfo[part].p;
    }


	/**add for the limited vision by Neil*/
    for (int i = JID_ROOT; i <= 3; ++i)
    {
        if (i == JID_ROOT) continue;
        /** NOTE if the link is the top one or not been
         * set up correctly, it will not be updated */
        // hinge joint effector
	if ( i == 2)
	{	//for setting up new coordinate
            	uLinkForCamera[i].q = gDegToRad(-1.0 * mHingeJointSenseMap[i].angle);
	}
	else
		uLinkForCamera[i].q = gDegToRad(mHingeJointSenseMap[i].angle);	
    }
    	ForwardKinematicsForCamera(JID_ROOT);

	for (RobotPart part = 1; part <= 3; ++part)
    	{
        	mRobotCameraInfo[part].p = uLinkForCamera[part].p + uLinkForCamera[part].R.Rotate(uLinkForCamera[part].c);
		mRobotCameraInfo[part].R = uLinkForCamera[part].R;

        	mRobotCameraInfo[part].R.Pos() = mRobotCameraInfo[part].p;
    	}
			aLOG<<"Camera Matrix: " << endl 
		<< mRobotCameraInfo[2].R.Right() << endl
		<< mRobotCameraInfo[2].R.Up() << endl
		<< mRobotCameraInfo[2].R.Forward() << endl
		<< mRobotCameraInfo[2].R.Pos() << endl;
		//cout << mRobotCameraInfo[PART_HEAD].R.Pos() << endl;
		aLOG<<"Body Matrix for camera: " << endl 
		<< mRobotCameraInfo[1].R.Right() << endl
		<< mRobotCameraInfo[1].R.Up() << endl
		<< mRobotCameraInfo[1].R.Forward() << endl
		<< mRobotCameraInfo[1].R.Pos() << endl;

}

shared_array<Robot::Link> Robot::GetLink() const
{
    return uLINK;
}

shared_array<Robot::RobotPartInfo> Robot::GetRobotPartInfo() const
{
    return mRobotPartInfo;
}

boost::shared_array<float> Robot::GetIKJointAngle() const
{
    return mIKJointAngle;
}

boost::shared_array<float> Robot::GetIKJointVel() const
{
    return mIKJointVel;
}

Matrix Robot::GetRobotPartMatrix(RobotPart part) const
{
    if (part < PART_MIN || part > PART_MAX)
    {
        cerr
            << "(Robot) ERROR: (GetRobotPartMatrix) part unknown\n";
        return Matrix();
    }

    return mRobotPartInfo[part].R;
}

Matrix Robot::GetRobotCameraMatrix(RobotPart part) const
{
    if (part < PART_MIN || part > PART_MAX)
    {
        cerr
            << "(Robot) ERROR: (GetRobotPartMatrix) part unknown\n";
        return Matrix();
    }

    return mRobotCameraInfo[part].R;
}

Vector3f Robot::GetRobotPartPos(RobotPart part) const
{
    if (part < PART_MIN || part > PART_MAX)
    {
        cerr
            << "(Robot) ERROR: (GetRobotPartPos) part unknown\n";
        return Vector3f();
    }

    return mRobotPartInfo[part].p;
}

void Robot::PrintLink(int j)
{
    if (j != 0)
    {
        printf("j = %d : %s, %.3f\n", j, uLINK[j].name.c_str(), gRadToDeg(uLINK[j].q));
        cout << "a = " << uLINK[j].a << " , b =  " << uLINK[j].b << ", c = "
             << uLINK[j].c << endl 
		<< "P :" << uLINK[j].p <<  endl 
                << "R: " << endl 
		<< uLINK[j].R.Right() << endl
		<< uLINK[j].R.Up() << endl
		<< uLINK[j].R.Forward() << endl
		<< uLINK[j].R.Pos() <<endl;
        PrintLink(uLINK[j].child);
        PrintLink(uLINK[j].sister);
    }
}

Vector3f Robot::GetRobotCoM()
{	return CoM;
}

Robot::TIndex Robot::FindRoute(int to)
{
    Robot::TIndex idx;
    int i = uLINK[to].mother;

    if (i == 0)
    {
        // this should not happen
        assert(false);
    }
    else if (i == 1)
    {
        idx.push_back(to);
    }
    else
    {
        idx = FindRoute(i);
        idx.push_back(to);
    }

    return idx;
}

void Robot::ForwardKinematics(int j)
{
    if (j == 0) return ;
    if (j != 1)
    {
        int i = uLINK[j].mother;
        uLINK[j].p = uLINK[i].R.Rotate(uLINK[j].b) + uLINK[i].p;
        uLINK[j].R = uLINK[i].R * Rodrigues(uLINK[j].a, uLINK[j].q);
    }
    ForwardKinematics(uLINK[j].child);
    ForwardKinematics(uLINK[j].sister);
}

void Robot::ForwardKinematicsForCamera(int j)
{
    if (j == 0) return ;
    if (j != 1)
    {
        int i = uLinkForCamera[j].mother;
        uLinkForCamera[j].p = uLinkForCamera[i].R.Rotate(uLinkForCamera[j].b) + uLinkForCamera[i].p;
        uLinkForCamera[j].R = uLinkForCamera[i].R * Rodrigues(uLinkForCamera[j].a, uLinkForCamera[j].q);
    }
    ForwardKinematicsForCamera(uLinkForCamera[j].child);
    ForwardKinematicsForCamera(uLinkForCamera[j].sister);
}

float Robot::TotalMass(int j)
{
	if(j == 0)	
	{	TMass = 0.0f;
		return TMass;
	}
	else
	{	
		TMass = uLINK[j].m + TotalMass(uLINK[j].sister) + TotalMass(uLINK[j].child);
		return TMass;
	}
	
}

Vector3f Robot::CalcMC( int j )
{	//aLOG << "TMC :" << TMC << endl;
	if ( j == 0 )
	{	TMC = Vector3f(0.0,0.0,0.0);
		return TMC;
	}
	else
	{
		TMC = uLINK[j].m * (uLINK[j].p + uLINK[j].R.Rotate(uLINK[j].c))+ CalcMC(uLINK[j].sister) + CalcMC(uLINK[j].child);
		return TMC;
	}	
}

void Robot::CalcCoM()
{
	CoM = TMC / TMass;
}

void Robot::ForwardVelocity(int j)
{
    if (j == 0) return ;
    if (j != 1)
    {
        int i = uLINK[j].mother;
        uLINK[j].v = uLINK[i].v + uLINK[i].w.Cross(uLINK[i].R.Rotate(uLINK[j].b));
        uLINK[j].w = uLINK[i].w + uLINK[i].R.Rotate(uLINK[j].a * uLINK[j].dq);
    }
    ForwardVelocity(uLINK[j].child);
    ForwardVelocity(uLINK[j].sister);
}

bool Robot::InverseKinematics(int to, const salt::Matrix& target)
{
    bool rec = true;
    const float lambda = 0.5f;
    TIndex idx = FindRoute(to);
    int jsize = idx.size();

    float err[6];
    float* dq = new float[jsize];

    // allot memory for Jacobian matrix
    float** J = new float*[6];
    for (int i = 0; i < 6; ++i)
    {
        J[i] = new float[jsize];
    }

    int n;
    for (n = 1; n <= 30; ++n)
    {
        CalcJacobian(J, idx);
        Matrix m = uLINK[to].R;
        m.Pos() = uLINK[to].p;
        CalcVWerr(err, target, m);

        float l = 0.0f;
        for (int i = 0; i < 6; ++i)
        {
            l += err[i] * err[i];
        }
        if (gSqrt(l) < 1e-3)
            break ;

        // solve J * dq = err
        // Solve(dq, J, err, 6, jsize);
        if (! Solve(dq, (const float **)J, (const float *)err, 6, jsize))
        {
            rec = false;
            goto over;
        }

#ifdef _DEBUG_
        printf("n = %d\n", n);
        printf("err: ");
        for (int i = 0; i < 6; ++i)
            printf("%.3f ", gRadToDeg(err[i]));
        printf("\n");
        printf("dq: ");
#endif
        for (int i = 0; i < jsize; ++i)
        {
            dq[i] *= lambda;
#ifdef _DEBUG_
            printf("%.3f ", gRadToDeg(dq[i]));
#endif
        }
#ifdef _DEBUG_
        printf("\n");
        printf("q: ");
#endif
        for (int nn = 0; nn < jsize; ++nn)
        {
            int j = idx.at(nn);
            uLINK[j].q += dq[nn];
            uLINK[j].q = gNormalizeRad(uLINK[j].q);
#ifdef _DEBUG_
            printf("%.3f ", gRadToDeg(uLINK[j].q));
#endif
        }
#ifdef _DEBUG_
        printf("\n");
        printf("J:\n");
        for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < jsize; ++j)
                printf("%.3f ", J[i][j]);
            printf("\n");
        }
        printf("\n");
#endif
        ForwardKinematics(1);
    }
#ifdef _DEBUG_
    printf("n = %d\n", n);
#endif

over:
    for (int i = 0; i < 6; ++i)
    {
        delete [] J[i];
        J[i] = NULL;
    }
    delete [] J;
    delete [] dq;
    J = NULL;
    dq = NULL;

    return rec;
}

void Robot::CalcJacobian(float** J, const TIndex& idx)
{
    int jsize = idx.size();
    Vector3f target = uLINK[idx.at(jsize - 1)].p;

    for (int n = 0; n < jsize; ++n)
    {
        int j = idx.at(n);
        Vector3f a = uLINK[j].R * uLINK[j].a;
        Vector3f c = a.Cross(target - uLINK[j].p);
        J[0][n] = c.x();
        J[1][n] = c.y();
        J[2][n] = c.z();
        J[3][n] = a.x();
        J[4][n] = a.y();
        J[5][n] = a.z();
    }
}

void Robot::CalcVWerr(float err[6], const salt::Matrix& ref, const salt::Matrix& now)
{
    Vector3f perr = ref.Pos() - now.Pos();
    Matrix invNow = now;
    invNow.InvertRotationMatrix();
    // if (! InverseMatrix(invNow)) cout << "inverse matrix failed!" << endl;
    Matrix Rerr = invNow * ref;
    Vector3f werr = now.Rotate(RotToOmega(Rerr));
    err[0] = perr[0];
    err[1] = perr[1];
    err[2] = perr[2];
    err[3] = werr[0];
    err[4] = werr[1];
    err[5] = werr[2];
}

bool Robot::Solve(float* X, const float** A, const float* B, int row, int column)
{
    return svd(X, A, B, row, column);
}
