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

#ifndef NAOKICK_H
#define NAOKICK_H

//#include <vector>

#include "worldmodel.h"
#include "self.h"
#include "KickEngine.h"

class NaoKickEngine: public KickEngine
{
public:
   	enum TPhaseForKickLeft 
	{	KICKL1,
		KICKL2,
		KICKL3,
		KICKL4,
		KICKL5,
// 		KICKL6,
// 		KICKL7,
	};

	enum TPhaseForKcikRight 
	{	KICKR1,
		KICKR2,
		KICKR3,
		KICKR4,
		KICKR5,
// 		KICKR6,
// 		KICKR7,
	};

	enum State
	{	KICKLEFT,
		KICKRIGHT,
		disable
	};

public:
    	NaoKickEngine(const KickEngineInterfaces& interfaces);
    	~NaoKickEngine(){};
	void Init();
    	virtual bool executeParameterized();

	
   	void CalculateVel(JointID id, float angle, float maxVel);
	void CalculateVel2(JointID id, float angle, float maxVel);
	void CalculateVel3(JointID id, float angle, float maxVel);
	void kickLeft1();
	void kickLeft2();
	void kickLeft3();
	void kickLeft4();
	void kickLeft5();
// 	void kickLeft6();
// 	void kickLeft7();

	void kickRight1();
	void kickRight2();
	void kickRight3();
	void kickRight4();
	void kickRight5();
	void kickRight6();
	//void kickRight7();

	void adjustment();


protected:

	void KickingLeft();
	void KickingRight();

	TPhaseForKickLeft mPhase;
	TPhaseForKcikRight mPhase2;
	State state;
	 /** according to time */

    double              mPhaseTime;
    double              mPhaseStartTime;

    /** according to cycle */

    int                 mPhaseCycle;
    int                 mPhaseStartCycle;



	bool mInit;
	bool mIsDone;

};

#endif