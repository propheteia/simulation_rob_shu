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
#include "convexhull.h"
#include "Logger.h"

ConvexHull::ConvexHull() : mCenterPoint(Vector3f(0, 0, 0))
{
	
}

ConvexHull::~ConvexHull()
{

}

float ConvexHull::MaxLength()
{
	float d1 =  (mPtrToXMax->x() - mPtrToXMin->x()) * (mPtrToXMax->x() - mPtrToXMin->x()) 
			  + (mPtrToXMax->y() - mPtrToXMin->y()) * (mPtrToXMax->y() - mPtrToXMin->y());
	d1 = gSqrt(d1);
	float d2 =  (mPtrToYMax->x() - mPtrToYMin->x()) * (mPtrToYMax->x() - mPtrToYMin->x()) 
			  + (mPtrToYMax->y() - mPtrToYMin->y()) * (mPtrToYMax->y() - mPtrToYMin->y());
	d2 = gSqrt(d2);
	return gMax(d1, d2);
}

bool ConvexHull::AddPolygon(const vector<Vector3f> & polygon)
{
// 	aLOG << "************************************AddPolygon Start************************************" << endl;
	if(mPointList.empty())
	{
		for(int i = 0; i < polygon.size(); i++)
		{
			mPointList.push_back(polygon[i]);
		}
		Update();
		return true;
	}
	else
	{
		return false;
	}
// 	aLOG << "************************************AddPolygon End************************************" << endl;
}

void ConvexHull::AddConvexHull(ConvexHull& ch)
{
// 	aLOG << "AddConvexHull()" << endl;
	list<Vector3f>::const_iterator itr;
	for(itr = ch.GetPointList().begin(); itr != ch.GetPointList().end(); itr++)
	{
		mPointList.push_back(*itr);
	}
	Update();
	CauculateConvexHull();
// 	aLOG << "AddConvexHull() End" << endl;
}

list<Vector3f>::iterator ConvexHull::FindNextPoint(list<Vector3f>::iterator currentpoint, float currentangle)
{
// 	aLOG << "***********************************FindNextPoint Test Start***********************************" << endl;
// 	aLOG << "currentangle: " << currentangle << endl;
	list<Vector3f>::iterator next;
	float minangle = 180;
	for(list<Vector3f>::iterator itr = mPointList.begin(); itr != mPointList.end(); itr++)
	{
		if(itr == currentpoint)
			continue;
		else
		{
			Vector3f vctr = (*itr) - (*currentpoint);
			float angle = gNormalizeDeg(vctr.to2D().GetAngleDeg() - currentangle);
			angle = angle < 0 ? angle+360 : angle;
// 			aLOG << "vctr: " << vctr << " itr: " << *itr << " currentpoint: " << *currentpoint << " Angle: " << angle << endl;
			if( angle < minangle )
			{
				minangle = angle;
				next = itr;
			}
		}
	}
// 	aLOG << "***********************************FindNextPoint Test End***********************************" << endl;
	return next;
	
}

void ConvexHull::CauculateConvexHull()
{
// 	aLOG << "******************************************Test Start***************************************" << endl;
	list<Vector3f> tmp;
	list<Vector3f>::iterator Current;
	list<Vector3f>::iterator Next;
	tmp.push_back(*mPtrToYMin);
// 	aLOG << *mPtrToYMin << endl; 
	Current = mPtrToYMin;
	float angle = 0;
	Next = FindNextPoint(Current, angle);
	Vector3f vctr = (*Next) - (*Current);
	angle = vctr.to2D().GetAngleDeg();
// 	aLOG << "Current: " << *Current << " Next: " << *Next << " Angle: " << angle << endl; 
	tmp.push_back(*Next);
 	Current = Next;

	while( (Next = FindNextPoint(Current, angle)) != mPtrToYMin )
	{
		vctr = (*Next) - (*Current);
		angle = vctr.to2D().GetAngleDeg();
		tmp.push_back(*Next);
// 		aLOG <<"Current: " << *Current << " Next:  " << *Next << " Angle: " << angle << " End? " << (bool)(Next == mPtrToYMin) << endl;
 		mPointList.erase(Current);
		Current = Next;
	}
	mPointList = tmp;
	Update();
// 	aLOG << "****************************************Test End*******************************************" << endl;
}

void ConvexHull::Update()
{
// 	aLOG << "****************************************Update Start***************************************" << endl;
	mCenterPoint.x() = 0;
	mCenterPoint.y() = 0;
	mCenterPoint.z() = 0;
	mPtrToXMax = mPtrToXMin = mPtrToYMax = mPtrToYMin = mPointList.begin();
	for(list<Vector3f>::iterator itr = mPointList.begin(); itr != mPointList.end(); itr++)
	{
		if(itr->x() >= mPtrToXMax->x())
			mPtrToXMax = itr;
		if(itr->x() <= mPtrToXMin->x())
			mPtrToXMin = itr;
		if(itr->y() >= mPtrToYMax->y())
			mPtrToYMax = itr;
		if(itr->y() <= mPtrToYMin->y())
			mPtrToYMin = itr;
		mCenterPoint += *itr;
// 		aLOG << *itr << endl;
	}
	mCenterPoint.x() /= mPointList.size();
	mCenterPoint.y() /= mPointList.size();
	mCenterPoint.z() /= mPointList.size();
// 	aLOG << "****************************************Update End***************************************" << endl;
}

void ConvexHull::Test()
{
	aLOG <<"*********************************ConvexHull Test Start*************************************" << endl;
	for(list<Vector3f>::iterator itr = mPointList.begin(); itr != mPointList.end(); itr++)
	{
		aLOG << *itr << endl;
	}
	aLOG << "XLength: " << XLength() << " YLength: " << YLength() << " MaxLength: " << MaxLength() << "Center: " << Center() << endl
									 << " XMaxPoint: " << *mPtrToXMax << " Angle: " << mPtrToXMax->to2D().GetAngleDeg() << endl
									 << " YMaxPoint: " << *mPtrToYMax << " Angle: " << mPtrToYMax->to2D().GetAngleDeg() << endl
									 << " XMinPoint: " << *mPtrToXMin << " Angle: " << mPtrToXMin->to2D().GetAngleDeg() << endl 
									 << " YMinPoint: " << *mPtrToYMin << " Angle: " << mPtrToYMin->to2D().GetAngleDeg() << endl;
	aLOG << "********************************ConvexHull Test End***************************************" << endl;
}