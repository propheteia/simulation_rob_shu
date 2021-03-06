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
#ifndef NAOSPECIALACTION_H
#define NAOSPECIALACTION_H

#include "SpecialAction.h"
#include "worldmodel.h"
#include "self.h"

class NaoSpecialAction: public SpecialAction
{
public:
   	enum TPhaseForDivingSave 
	{	DIVINGSAVE1,
		DIVINGSAVE2,
		DIVINGSAVE3,
	};

	enum State
	{	DivingAction,
		disable
	};

public:
    	NaoSpecialAction(const SpecialActionInterfaces& interfaces);
    	~NaoSpecialAction(){};
	void Init();
    	virtual bool executeParameterized();

	
   	void CalculateVel(JointID id, float angle, float maxVel);
	void ApplyJVel(bool left, float slowgain);

	void divingSave1();
	void divingSave2();
	void divingSave3();

protected:
	void DivingSave();
	void InitPosition();

	TPhaseForDivingSave mPhase;

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
