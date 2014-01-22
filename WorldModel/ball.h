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
#ifndef BALL_H
#define BALL_H

#include "dynamicobject.h"

#define DEFAULTBALLAREA 0.15

using namespace salt;

class Ball : public DynamicObject
{
	enum BallState
	{
		UNKNOWN 	= 0,
		DRIBBLE		= 1,
		AFTERKICK	= 2,	
		STILL		= 3,		
	};
	
public:
    Ball();
    ~Ball();
		
	void setPos(Vector3f Pos)
	{
		pos = Pos;
		setIsValid(true);
		mTimeNotRefresh = 0;
		positionqueue.push(pos);
		if(positionqueue.size() > 10)
		{
			vel = (pos - positionqueue.front()) / 0.6;
			positionqueue.pop();
		}
		
	}
	
	void renew()
	{
		if(mTimeNotRefresh > 4)
		{
			setIsValid(false);
			while(positionqueue.size())
				positionqueue.pop();
		}
		else 
		{
			mTimeNotRefresh++;
		}

		//!Cauculate the vel of Ball
		positionqueue.push(pos);
		if(positionqueue.size() >= 40)
		{
			vel = (pos - positionqueue.front()) / 0.8;
			positionqueue.pop();
		}
		//!Cauculate the area of the ball.
		for(int i = 0; i < 8; i++)
		{
			area[i] = DefaultPoint[i] + pos;
		} 
	}
private:
	Vector3f ballLocalPos;
	Vector3f ballPosByCommunication;
	double ballDistance;
	double ballTheta;
	double ballPhi;
	
	static const Vector3f DefaultPoint[8];
};

#endif
