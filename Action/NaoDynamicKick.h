/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei                          *
 *   Copyright (C) 2009 by Zhu_Ming,Zheng Yonglei,Qu Junjun,Liu Zhengliang *
 *                         Pan Yan,Yaoyuan                                 *
 *   zhuming535984@gmail.com,zhengyonglei@gmail.com                        *
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

#ifndef NAODYNAMICKICK_H
#define NAODYNAMICKICK_H

//#include <vector>

#include "worldmodel.h"
#include "self.h"
#include "KickEngine.h"
#include "spline.h"

class NaoDynamicKickEngine: public KickEngine
{
public:
   	enum TPhaseForKickLeft 
	{	KICKL1,
		KICKL2,
		KICKL3,
		KICKL4,
		KICKL5,
		KICKL6,
		KICKL7,
	};

	enum TPhaseForKcikRight 
	{	KICKR1,
		KICKR2,
		KICKR3,
		KICKR4,
		KICKR5,
		KICKR6,
		KICKR7,
	};
	
	enum TPhaseForPass
	{	PASS1,
		PASS2,
		PASS3,
		PASS4,
		PASS5,
		PASS6,
		PASS7,  
	};
	
	enum TPhaseForPassQuick
	{
		PASSQ1,
		PASSQ2,
		PASSQ3,
		PASSQ4,
		PASSQ5,
		PASSQ6,
		PASSQ7,
	};

	enum State
	{	KICKLEFT,
		KICKRIGHT,
		PASS,
		PASSQ,
		disable
	};

public:
    	NaoDynamicKickEngine(const KickEngineInterfaces& interfaces);
    	~NaoDynamicKickEngine(){};
	void Init();
    virtual bool executeParameterized();

	
   	void CalculateVel(JointID id, float angle, float maxVel);
	void CalculateVel2(JointID id, float angle, float maxVel);
	void CalculateVel3(JointID id, float angle, float maxVel);

	void ApplyJVel(bool left, float slowgain);

	void kickLeft1();
	void kickLeft2();
	void kickLeft3();
	void kickLeft4();
	void kickLeft5();
	void kickLeft6();
	void kickLeft7();

	void kickRight1();
	void kickRight2();
	void kickRight3();
	void kickRight4();
	void kickRight5();
	void kickRight6();
	void kickRight7();
	
	void pass1();
	void pass2();
	void pass3();
	void pass4();
	void pass5();
	void pass6();
	void pass7();
	
	void passq1();
	void passq2();
	void passq3();
	void passq4();
	void passq5();
	void passq6();
	void passq7();

	void adjustment();


protected:

	void KickingLeft();
	void KickingRight();
	void Passing();
	void PassingQuick();

	TPhaseForKickLeft mPhase;
	TPhaseForKcikRight mPhase2;
	TPhaseForPass mPhase3;
	TPhaseForPassQuick mPhase4;
	State state;
	 /** according to time */

    double              mPhaseTime;
    double              mPhaseStartTime;

    /** according to cycle */

    int                 mPhaseCycle;
    int                 mPhaseStartCycle;

	Spline				mSplineTorsorX;
	Spline				mSplineTorsorY;
	Spline				mSplineTorsorZ;
	Spline				mSplineTorsorRx;
	Spline				mSplineTorsorRy;
	Spline				mSplineTorsorRz;
	
	Spline				mSplineKickFootX;
	Spline				mSplineKickFootY;
	Spline				mSplineKickFootZ;
	Spline				mSplineKickFootRx;
	Spline				mSplineKickFootRy;
	Spline				mSplineKickFootRz;

	bool mInit;
	bool mIsDone;

};

#endif