/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei  Qu Junjun *
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
#ifndef CONVEXHULL_H
#define CONVEXHULL_H
#include "gmath.h"
#include <vector>
#include <list>
#include "vector.h"

using namespace std;
using namespace salt;

class ConvexHull
{
	friend class ConvexHullManager;
	public:
		ConvexHull();
		~ConvexHull();
		void Clear() { mPointList.clear(); }
		
		inline float XLength() { return mPtrToXMax->x() - mPtrToXMin->x(); }
		inline float YLength() { return mPtrToYMax->y() - mPtrToYMin->y(); }
		
		float MaxLength();

		inline const Vector3f& Center() { return  mCenterPoint; }
		inline const Vector3f& XMaxPoint() { return *mPtrToXMax; }
		inline const Vector3f& YMaxPoint() { return *mPtrToYMax; }
		inline const Vector3f& XMinPoint() { return *mPtrToXMin; }
		inline const Vector3f& YMinPoint() { return *mPtrToYMin; }
		inline bool Empty() { return mPointList.empty(); }
		
		inline const list<Vector3f> & GetPointList() { return mPointList; }
		
		bool AddPolygon(const vector<Vector3f> & polygon);
		void AddConvexHull(ConvexHull& ch);
		
	private:
		list<Vector3f>::iterator FindNextPoint(list<Vector3f>::iterator currentpoint, float currentangle);
		
		void CauculateConvexHull();
		void Update();
		void Test();
		
	private:
		list<Vector3f> 		mPointList;
		list<Vector3f>::iterator 	mPtrToXMax;
		list<Vector3f>::iterator 	mPtrToXMin;
		list<Vector3f>::iterator 	mPtrToYMax;
		list<Vector3f>::iterator 	mPtrToYMin;
		Vector3f 				mCenterPoint;
		
};
























#endif