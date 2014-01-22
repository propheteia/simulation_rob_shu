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
#ifndef CONVEXHULLMANAGER_H
#define CONVEXHULLMANAGER_H

#include "convexhull.h"
#include <list>
#include <set>
#include <vector>
#include "vector.h"
 
using namespace std;
using namespace salt;
 
class ConvexHullManager
{
	public:
	ConvexHullManager();
	~ConvexHullManager();
	bool AddArea(const vector<Vector3f> & area);
	void Clear();
	void MergeConvexHull();
	vector< vector<Vector3f> > & GetConvexHull() { return mOutput; }
	inline list<Vector3f>::iterator GetNextIterator(list<Vector3f> & List, list<Vector3f>::iterator Current)
	{
		if( (++Current) != List.end() )
			return Current;
		else 
			return List.begin();
	}
	
	bool IsSegmentCross(Vector3f point1, Vector3f point2);
	bool IsSegmentCross(Vector3f point1, Vector3f point2, set<ConvexHull *>& crossConvexHullList);
	
	void Test();
    
	private:
		float DistTowConvexHulls(ConvexHull& ch1, ConvexHull& ch2);
		void CauculateOutPut();
	
	private:
		list<ConvexHull *> mConvexHullList;
		vector< vector<Vector3f> >	mOutput;

};

#endif
