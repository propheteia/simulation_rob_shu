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

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H
class PIDController
{
public:
	PIDController(float Kp = 1.0, float Ki = 0.0, float Kd = 0.0, float max = 10.0, float min = -10.0)
	{
		mKp = Kp;
		mKi = Ki;
		mKd = Kd;
		if(max > min)
		{
			mMax = max;
			mMin = min;
		}
		else
		{
			mMax = min;
			mMin = mMax;
		}
		mErr = mErr_1 = mErr_2 = 0.0f;
		mInput = mOutput = 0.0f;
	}

	void setKp(float Kp) { mKp = Kp; }
	void setKi(float Ki) { mKi = Ki; }
	void setKd(float Kd) { mKd = Kd; }

	float getKp() { return mKp; }
	float getKi() { return mKi; }
	float getKd() { return mKd; }

	void Input(float e)
	{
		mInput = e;
		mErr_2 = mErr_1;
		mErr_1 = mErr;
		mErr = mInput;
		mOutput_1 = mOutput;
		mOutput = mOutput_1 + mKp * (mErr - mErr_1) + mKi * mErr + mKd * (mErr - 2 * mErr_1 + mErr_2);
		if(mOutput > mMax)
			mOutput = mMax;
		else if(mOutput < mMin)
			mOutput = mMin;
	}

	float Output() { return mOutput; }

protected:
	float mKp;
	float mKi;
	float mKd;
	float mMax;
	float mMin;
	float mErr;			//!e(n) = mErr
	float mErr_1;		//!e(n-1) = mErr_1
	float mErr_2;		//!e(n-2) = mErr_2
	float mInput;		//!mInput = mErr = e(n)
	float mOutput; 		//!mOutput = u(n)
	float mOutput_1;	//!mOutput = u(n-1)
};
#endif
