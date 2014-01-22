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
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H
#include "Logger.h" 
#include "vector.h"
#include "object.h"
#include <queue>
using namespace std;
using namespace salt;
class DynamicObject : public Object
{
public:
	bool mIsValid;
	float mLastUpdateTime;	
	Vector3f vel;
	Vector3f pos_by_hearing;
	
public:
	DynamicObject():mIsValid(false){}
	virtual ~DynamicObject() {}
	salt::Vector3f posbyhearing;
	int mTimeNotRefresh;
	vector<Vector2f> posInMemory;
	
	queue<Vector3f> positionqueue;	//By Qu Junjun 2009.10.4		Recorde The Past Position For Vel Cauculating
	vector<Vector3f> area;			//By Qu Junjun 2009.10.4		The Posible Moving Area Of The Player

	bool isValid(){return mIsValid;}
	
	void setIsValid(bool arg){mIsValid = arg;}
	
	float getLastUpdateTime(){return mLastUpdateTime;}
	
	void setLastUpdateTime(float arg){mLastUpdateTime = arg;}
	
	//Zheng Yonglei 2009.1.19 [end]

	void SetGlobalPos(Vector3f pos_){pos = pos_;}
	
	
	inline void SetLocalPos( Vector3f localpos)
	{	
	  pos_local = localpos;
	}
	inline void SetHearingPos( Vector3f hearingpos)
	{	
	  pos_by_hearing = hearingpos;
	}
	
	inline const Vector3f GetLocalPos()
	{	return pos_local;}

	inline const Vector3f GetHearingPos()
	{	return pos_by_hearing;}
	
	inline const Vector3f GetPos()
	{	return pos;}

};

#endif
