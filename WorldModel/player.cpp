/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei , Qu Junjun  *
 *   zhuming535984@gmail.com,zhengyonglei@gmail.com   *
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
#include <player.h>
#include <Logger.h>
#include "gmath.h"
#define DEFAULTPLAYERAREA 0.3
#define SAFERATE 1.5
//Tool Functions
Vector3f GetVectorFromLengthAndAngle(float length, float angle)
{
	return Vector3f(length * gCos(angle), length * gSin(angle), 0);
}


//Tool Functions End

Player::Player() : mUnum(0), mName("<unnamed>") 
{
	area.clear();
	area.push_back(pos + Vector3f(DEFAULTPLAYERAREA, 0, 0));
	area.push_back(pos + Vector3f(0, DEFAULTPLAYERAREA, 0));
	area.push_back(pos + Vector3f(-DEFAULTPLAYERAREA, 0, 0));
	area.push_back(pos + Vector3f(0, -DEFAULTPLAYERAREA, 0));
	mSafeArea.clear();
	mSafeArea.push_back(pos + Vector3f(DEFAULTPLAYERAREA * SAFERATE, 0, 0));
	mSafeArea.push_back(pos + Vector3f(0, DEFAULTPLAYERAREA * SAFERATE, 0));
	mSafeArea.push_back(pos + Vector3f(-DEFAULTPLAYERAREA * SAFERATE, 0, 0));
	mSafeArea.push_back(pos + Vector3f(0, -DEFAULTPLAYERAREA * SAFERATE, 0));
}

void Player::renew()
{
	//If I haven't seen this player more than 10 cycles, set this player invalid.
    aLOG<<"renew "<<endl;
	if(mTimeNotRefresh > 10)
	{
		setIsValid(false);
		while(positionqueue.size())
			positionqueue.pop();
            BlindEye();
	}
	else 
	{
		mTimeNotRefresh++;
	}
    aLOG << "Ican See larm rarm lfoot rfoot head: " << mGlobleLimb.seeleftarm << ' ' << mGlobleLimb.seerightarm << ' '
         << mGlobleLimb.seeleftfoot << ' ' << mGlobleLimb.seerightfoot << ' ' << mGlobleLimb.seehead << endl;
    if(mGlobleLimb.seerightfoot && mGlobleLimb.seeleftfoot)
    {
        pos = (mGlobleLimb.leftfoot + mGlobleLimb.rightfoot)/2;
        aLOG<<"limb pos:"<<pos<<endl;
    }
        
	//Cauculate the vel of the player, using a list of pos.
	positionqueue.push(pos);
	if(positionqueue.size() >= 40)
	{
		vel = (pos - positionqueue.front()) / 0.8;
		positionqueue.pop();
	}
	//Cauculate the state of the Player.
	float v = gSqrt(vel.x() * vel.x() + vel.y() * vel.y());
	if( !ICanSeeHead() )
		mState = UNKNOWN;
	else
	{
		if( (ICanSeeLeftFoot() || ICanSeeRightFoot()) 
			&& (gAbs(mGlobleLimb.head.z() - mGlobleLimb.leftfoot.z()) < 0.3 
			|| gAbs(mGlobleLimb.head.z() - mGlobleLimb.rightfoot.z()) < 0.3) )
			mState = TUMBLE;
			
		else
		{
			switch ( mState )
			{
				case TUMBLE:
					if( (gAbs(mGlobleLimb.head.z() - mGlobleLimb.leftfoot.z()) < 0.4 || gAbs(mGlobleLimb.head.z() - mGlobleLimb.rightfoot.z()) < 0.4) )
						break;
				case UNKNOWN:
					if( v > SLOWSPEEDUPTHRESHOLD )
						mState = FASTWALK;
					else if( v > SLOWSPEEDLOWTHRESHOLD )
						mState = SLOWWALK;
					else 
						mState = STILL;
					break;
				case FASTWALK:
					if( v < FASTSPEEDLOWERTHRESHOLD )
						mState = SLOWWALK;
					break;
				case SLOWWALK:
					if( v < SLOWSPEEDLOWTHRESHOLD )
						mState = STILL;
					else if ( v > SLOWSPEEDUPTHRESHOLD )
						mState = FASTWALK;
					break;
				case STILL:
					if( v > STILLUPTHRESHOLD )
						mState = SLOWWALK;
					break;
			}
		}
	}
	
	if(mState != TUMBLE)
	{
		//Cauculate the area of player.
		Vector3f DirectionVector = GetVectorFromLengthAndAngle(DEFAULTPLAYERAREA + vel.to2D().Length() * 0.5, vel.to2D().GetAngleRad());
		Vector3f CrossVector = GetVectorFromLengthAndAngle(DEFAULTPLAYERAREA, vel.to2D().GetAngleRad() - M_PI_2); 
		area.clear();
		area.push_back(pos + CrossVector);
		area.push_back(pos + DirectionVector);
		area.push_back(pos - CrossVector);
		area.push_back(pos - DirectionVector);
		DirectionVector = GetVectorFromLengthAndAngle((DEFAULTPLAYERAREA + vel.to2D().Length() * 0.5) * SAFERATE
							, vel.to2D().GetAngleRad());
		CrossVector = GetVectorFromLengthAndAngle(DEFAULTPLAYERAREA * SAFERATE, vel.to2D().GetAngleRad() - M_PI_2);
		mSafeArea.clear();
		mSafeArea.push_back(pos + CrossVector);
		mSafeArea.push_back(pos + DirectionVector);
		mSafeArea.push_back(pos - CrossVector);
		mSafeArea.push_back(pos - DirectionVector);
		
	}
	else
	{
		Vector3f Center = mGlobleLimb.head + mGlobleLimb.rightarm + mGlobleLimb.rightfoot + mGlobleLimb.leftarm + mGlobleLimb.leftfoot;
		Center = Vector3f(Center.x() / 5, Center.y() / 5, Center.z() / 5);
		
		Vector3f Head = mGlobleLimb.head - Center;
		Vector3f RightArm = mGlobleLimb.rightarm - Center;
		Vector3f RightFoot = mGlobleLimb.rightfoot - Center;
		Vector3f LeftFoot = mGlobleLimb.leftfoot - Center;
		Vector3f LeftArm = mGlobleLimb.leftarm - Center;
		
		int Rate = 2;
		
		Head = GetVectorFromLengthAndAngle(Head.to2D().Length() * Rate, Head.to2D().GetAngleRad());
		RightArm = GetVectorFromLengthAndAngle(RightArm.to2D().Length() * Rate, RightArm.to2D().GetAngleRad());
		RightFoot = GetVectorFromLengthAndAngle(RightFoot.to2D().Length() * Rate, RightFoot.to2D().GetAngleRad());
		LeftFoot = GetVectorFromLengthAndAngle(LeftFoot.to2D().Length() * Rate, LeftFoot.to2D().GetAngleRad());
		LeftArm = GetVectorFromLengthAndAngle(LeftArm.to2D().Length() * Rate, LeftArm.to2D().GetAngleRad());
		
		area.clear();
		area.push_back(Head + Center);
		area.push_back(RightArm + Center);
		area.push_back(RightFoot + Center);
		area.push_back(LeftFoot + Center);
		area.push_back(LeftArm + Center);
		
		Head = mGlobleLimb.head - Center;
		RightArm = mGlobleLimb.rightarm - Center;
		RightFoot = mGlobleLimb.rightfoot - Center;
		LeftFoot = mGlobleLimb.leftfoot - Center;
		LeftArm = mGlobleLimb.leftarm - Center;
		
		 Rate = Rate * SAFERATE;
		
		Head = GetVectorFromLengthAndAngle(Head.to2D().Length() * Rate, Head.to2D().GetAngleRad());
		RightArm = GetVectorFromLengthAndAngle(RightArm.to2D().Length() * Rate, RightArm.to2D().GetAngleRad());
		RightFoot = GetVectorFromLengthAndAngle(RightFoot.to2D().Length() * Rate, RightFoot.to2D().GetAngleRad());
		LeftFoot = GetVectorFromLengthAndAngle(LeftFoot.to2D().Length() * Rate, LeftFoot.to2D().GetAngleRad());
		LeftArm = GetVectorFromLengthAndAngle(LeftArm.to2D().Length() * Rate, LeftArm.to2D().GetAngleRad());
		
		mSafeArea.clear();
		mSafeArea.push_back(Head + Center);
		mSafeArea.push_back(RightArm + Center);
		mSafeArea.push_back(RightFoot + Center);
		mSafeArea.push_back(LeftFoot + Center);
		mSafeArea.push_back(LeftArm + Center);
	}
}
