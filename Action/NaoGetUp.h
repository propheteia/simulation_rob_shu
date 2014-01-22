/***************************************************************************
 *   Copyright (C) 2008 by Neil_Zhu   *
 *   zhuming535984@gmail.com   *
 *   Walk Test                                                             *
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
#ifndef NAOGETUP_H
#define NAOGETUP_H

#include "GetUpEngine.h"
#include "worldmodel.h"

class NaoGetupEngine: public GetupEngine
{
public:
   	enum TPhaseForGetUp 
	{	GETUP1,
		GETUP2,
		GETUP3,
		GETUP4,
		GETUP5,
		GETUP6,
		GETUP7
	};

	enum TPhaseForGetUpFromBack 
	{	GETUPFB1,
		GETUPFB2,
		GETUPFB3,
		GETUPFB4,
		GETUPFB5
	};

	enum State
	{	GETUP,
		GETUPFROMBACK,
		ONMYSIDE,
		disable
	};

public:
    	NaoGetupEngine(const GetupEngineInterfaces& interfaces);
    	~NaoGetupEngine(){};
	void Init();
    	virtual bool executeParameterized();
	
	void CalculateVel1(JointID id, float angle , float maxVel);
	void CalculateVel2(JointID id, float startAngle ,float endAngle, float maxVel);
   	void CalculateVel(JointID id, float startAngle ,float endAngle, float maxVel);
	void getUp1();
	void getUp2();
	void getUp3();
	void getUp4();
	void getUp5();
	void getUp6();
	void getUp7();

	void getUpFB1();
	void getUpFB2();
	void getUpFB3();
	void getUpFB4();
	void getUpFB5();

	void adjustment();

protected:

	void gettingUp();
	void gettingUpFromBack();

	TPhaseForGetUp mPhase;
	TPhaseForGetUpFromBack mPhase2;
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
