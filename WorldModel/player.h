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
#ifndef PLAYER_H
#define PLAYER_H

#include "dynamicobject.h"
#include "types.h"

#define FASTSPEEDUPERTHRESHOLD 0.5
#define FASTSPEEDLOWERTHRESHOLD 0.3
#define SLOWSPEEDUPTHRESHOLD 0.4
#define SLOWSPEEDLOWTHRESHOLD 0.1
#define STILLUPTHRESHOLD 0.2
#define STILLLOWTHRESHOLD 0

using namespace std;
using namespace salt;

enum PlayerState
{
	STILL		= 0,
	SLOWWALK	= 1,
	FASTWALK	= 2,
	TUMBLE 		= 3,
	UNKNOWN 	= 4,
};

class Player : public DynamicObject
{
	
public:
	Player();
	~Player() {}
	
	inline int GetUnum() const { return mUnum; }
	inline std::string GetName() const { return mName; }
	inline void setUnum( int Unum  ) { mUnum = Unum; }
	inline void setName( const std::string name) { mName = name; }
	
	void setPos(Vector3f Pos)
	{
		pos = Pos;
		setIsValid(true);
		mTimeNotRefresh = 0;
		mGlobleLimb.head = Pos;
		mGlobleLimb.seehead = true;
	}
	inline void SetHead(Vector3f local, Vector3f globle) 
	{ 
		setPos(globle);
		setIsValid(true);
		mTimeNotRefresh = 0;
		mLocalLimb.head = local;
		mLocalLimb.seehead = true;
		mGlobleLimb.head = globle;
		mGlobleLimb.seehead = true;
		pos = globle;
		setIsValid(true);
	}
	
	inline void SetLeftArm(Vector3f local, Vector3f globle) 
	{ 
		mLocalLimb.leftarm = local;
		mLocalLimb.seeleftarm = true;
		mGlobleLimb.leftarm = globle;
		mGlobleLimb.seeleftarm = true;
	}
	
	inline void SetRightArm(Vector3f local, Vector3f globle) 
	{ 
		mLocalLimb.rightarm = local;
		mLocalLimb.seerightarm = true;
		mGlobleLimb.rightarm = globle;
		mGlobleLimb.seerightarm = true;
	}
	
	inline void SetLeftFoot(Vector3f local, Vector3f globle) 
	{ 
		mLocalLimb.leftfoot = local;
		mLocalLimb.seeleftfoot = true;
		mGlobleLimb.leftfoot = globle;
		mGlobleLimb.seeleftfoot = true;
	}
	
	inline void SetRightFoot(Vector3f local, Vector3f globle) 
	{ 
		mLocalLimb.rightfoot = local;
		mLocalLimb.seerightfoot = true;
		mGlobleLimb.rightfoot = globle;
		mGlobleLimb.seerightfoot = true;
	}
	
	inline const Vector3f &	GetHeadLocalPosition() { return mLocalLimb.head; }
	inline const Vector3f & 	GetLeftArmLocalPosition() { return mLocalLimb.leftarm; }
	inline const Vector3f &  	GetRightArmLocalPosition() { return mLocalLimb.rightarm; }
	inline const Vector3f &  	GetLeftFootLocalPosition() { return mLocalLimb.leftfoot; }
  	inline const Vector3f &  	GetRightFootLocalPosition() { return mLocalLimb.rightfoot; }
	inline const Vector3f &  	GetHeadGloblePosition() { return mGlobleLimb.head; }
	inline const Vector3f &  	GetLeftArmGloblePosition() { return mGlobleLimb.leftarm; }
	inline const Vector3f &  	GetRightArmGloblePosition() { return mGlobleLimb.rightarm; }
	inline const Vector3f &  	GetLeftFootGloblePosition() { return mGlobleLimb.leftfoot; }
  	inline const Vector3f & 	GetRightFootGloblePosition() { return mGlobleLimb.rightfoot; }
	inline bool		ICanSeeHead() { return mLocalLimb.seehead; }
	inline bool		ICanSeeLeftArm() { return mLocalLimb.seeleftarm; }
	inline bool		ICanSeeRightArm() { return mLocalLimb.seerightarm; }
	inline bool		ICanSeeLeftFoot() { return mLocalLimb.seeleftfoot; }
	inline bool		ICanSeeRightFoot() { return mLocalLimb.seerightfoot; }
	inline bool		ICanSeeHim() { return (ICanSeeHead() || ICanSeeLeftArm() || ICanSeeRightArm() || ICanSeeLeftFoot() || ICanSeeRightFoot()); }
	inline PlayerState GetState() { return mState; }
	inline void BlindEye()
	{
		mLocalLimb.seehead 		= false;
		mLocalLimb.seeleftarm 	= false;
		mLocalLimb.seerightarm 	= false;
		mLocalLimb.seeleftfoot 	= false;
		mLocalLimb.seerightfoot = false;
		mGlobleLimb.seehead 	= false;
		mGlobleLimb.seeleftarm 	= false;
		mGlobleLimb.seerightarm = false;
		mGlobleLimb.seeleftfoot = false;
		mGlobleLimb.seerightfoot = false;
	}
	
	inline vector<Vector3f> & GetSafeArea() { return mSafeArea; }
	void renew();

protected:
	int mUnum;
	Limb mLocalLimb;
	Limb mGlobleLimb;
	std::string mName;
	int mTimeNotRefresh;
	PlayerState mState;
	vector<Vector3f> mSafeArea;
};


#endif
