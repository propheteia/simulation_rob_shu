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
#include "convexhullmanager.h"
#include "Logger.h"

#define MERGEDISTANCE 0.2

//Tool Function
#define eps 1e-6
#define inf 9e23

float DisPointToSeg ( Vector3f& p1, Vector3f& p2, Vector3f& p3 )
{
	Vector3f tmp = p1 - p2;
	float a = tmp.to2D().Length();
	tmp = p1 - p3;
	float b = tmp.to2D().Length();
	tmp = p2 - p3;
	float c = tmp.to2D().Length();
	
	if ( gAbs ( a + b - c ) < eps )
		return 0;
	
	if ( gAbs ( a + c - b ) < eps || gAbs ( b + c - a ) < eps )
		return gMin ( a, b );
	
	float t1 = -a * a + b * b + c * c;
	float t2 = a * a - b * b + c * c;
	
	if ( t1 <= 0 || t2 <= 0 )
		return gMin ( a, b );
	
	float la = p3.y() - p2.y();
	float lb = p2.x() - p3.x();
	float lc = p3.x() * p2.y() - p3.y() * p2.x();
	
	return gAbs ( la * p1.x() + lb * p1.y() + lc ) / gSqrt ( la * la + lb * lb );
}

float DisPallSeg ( Vector3f& p1, Vector3f& p2, Vector3f& p3, Vector3f& p4 )
{
	return 	gMin ( gMin ( DisPointToSeg ( p1, p3, p4 ), DisPointToSeg ( p2, p3, p4 ) ),
				   gMin ( DisPointToSeg ( p3, p1, p2 ), DisPointToSeg ( p4, p1, p2 ) ) 
				 );
}

bool SegmentCross(Vector3f A,Vector3f B, Vector3f C, Vector3f D)
{
	double Line1p = (B.x() - A.x()) * (C.y() - A.y()) - (B.y() - A.y()) * (C.x() - A.x());
	double Line2p = (B.x() - A.x()) * (D.y() - A.y()) - (B.y() - A.y()) * (D.x() - A.x());
	
	if((Line1p * Line2p) >= 0 && !(Line1p == 0 && Line2p == 0))
		return false;
	Line1p = (D.x() - C.x()) * (A.y() - C.y()) - (D.y() - C.y()) * (A.x() - C.x());
	Line2p = (D.x() - C.x()) * (B.y() - C.y()) - (D.y() - C.y()) * (B.x() - C.x());

	if((Line1p * Line2p) >= 0 && !(Line1p == 0 && Line2p == 0))
		return false;
	return true; 
}

//Tool Function End

ConvexHullManager::ConvexHullManager()
{
}


ConvexHullManager::~ConvexHullManager()
{
}

bool ConvexHullManager::AddArea(const vector<Vector3f> & area)
{
	ConvexHull *ptrtoch = new ConvexHull;
	ptrtoch->AddPolygon(area);
	mConvexHullList.push_back(ptrtoch);
// 	aLOG << "AddArea()" << endl;
}

void ConvexHullManager::Clear()
{
// 	aLOG << "Clear Start" << endl;
	list<ConvexHull *>::iterator itr;
	for (itr = mConvexHullList.begin(); itr != mConvexHullList.end(); itr++)
	{
		delete *itr;
	}
	mConvexHullList.clear();
// 	aLOG << "Clear() End" << endl;
}

float ConvexHullManager::DistTowConvexHulls(ConvexHull& ch1, ConvexHull& ch2)
{
	if(    !(ch2.mPtrToXMax->x()  <= ch1.mPtrToXMin->x() 
		|| ch2.mPtrToXMin->x() >= ch1.mPtrToXMax->x() 
		|| ch2.mPtrToYMin->y() >= ch1.mPtrToYMax->y() 
		|| ch2.mPtrToYMax->y() <= ch1.mPtrToYMin->y())
	)
		return 0;
		
// 	aLOG << "****************************DistTowConvexHulls Test Start***************************" << endl;
	float Slope = 0;
	float MinDist = inf;
	list<Vector3f>::iterator CurrentPoint1 = ch1.mPtrToYMin;
	list<Vector3f>::iterator CurrentPoint2 = ch2.mPtrToYMax;
	list<Vector3f>::iterator NextPoint1 = GetNextIterator(ch1.mPointList, CurrentPoint1);
	list<Vector3f>::iterator NextPoint2 = GetNextIterator(ch2.mPointList, CurrentPoint2);
	Vector2f tmp1, tmp2;
	float CurrentAngle1 = 0;
	float CurrentAngle2 = 180;
	float AngleToNextPoint1 = 0;
	float AngleToNextPoint2 = 0;
	float ChangeAngle = 0;
	float TmpDist = 0;

	while (Slope <= 360)
	{
// 		aLOG << "CurrentPoint1: " << *CurrentPoint1 << " CurrentPoint2: " << *CurrentPoint2 << endl;
		tmp1 = NextPoint1->to2D() - CurrentPoint1->to2D();
		tmp2 = NextPoint2->to2D() - CurrentPoint2->to2D();
		AngleToNextPoint1 = gNormalizeDeg(tmp1.GetAngleDeg() - CurrentAngle1);
		AngleToNextPoint1 = AngleToNextPoint1 < 0 ? AngleToNextPoint1 + 360 : AngleToNextPoint1;
		AngleToNextPoint2 = gNormalizeDeg(tmp2.GetAngleDeg() - CurrentAngle2);
		AngleToNextPoint2 = AngleToNextPoint2 < 0 ? AngleToNextPoint2 + 360 : AngleToNextPoint2;
// 		aLOG << "tmp: " << tmp1 << " tmp2: " << tmp2 << endl
// 			 << " AngleToNextPoint1: " << AngleToNextPoint1
// 			 << " AngleToNextPoint2: " << AngleToNextPoint2 << endl;
		if ( gAbs ( AngleToNextPoint1 - AngleToNextPoint2 ) < eps )
		{
			TmpDist = DisPallSeg(*CurrentPoint1, *NextPoint1, *CurrentPoint2, *NextPoint2);
			if ( TmpDist < MinDist )
				MinDist = TmpDist;
// 			aLOG << "TmpDist: " << TmpDist << " MinDist: " << MinDist << endl;
			CurrentPoint1 = NextPoint1;
			NextPoint1 = GetNextIterator(ch1.mPointList, CurrentPoint1);
			CurrentPoint2 = NextPoint2;
			NextPoint2 = GetNextIterator(ch2.mPointList, CurrentPoint2);
			ChangeAngle = gMax(AngleToNextPoint1, AngleToNextPoint2);
			Slope += ChangeAngle;
			CurrentAngle1 += ChangeAngle;
			CurrentAngle2 += ChangeAngle;
// 			aLOG << "ChangeAngle: " << ChangeAngle << " Slope: " << Slope << " CurrentAngle1: " << CurrentAngle1 << " CurrentAngle2: " << CurrentAngle2 << endl;
		}
		else if ( AngleToNextPoint1 < AngleToNextPoint2 )
		{
			TmpDist = DisPointToSeg ( *CurrentPoint2, *CurrentPoint1, *NextPoint1 );
			if ( TmpDist < MinDist )
				MinDist = TmpDist;
// 			aLOG << "TmpDist: " << TmpDist << " MinDist: " << MinDist << endl;
			CurrentPoint1 = NextPoint1;
			NextPoint1 = GetNextIterator(ch1.mPointList, CurrentPoint1);
			ChangeAngle = AngleToNextPoint1;
			Slope += ChangeAngle;
			CurrentAngle1 += ChangeAngle;
			CurrentAngle2 += ChangeAngle;
// 			aLOG << "ChangeAngle: " << ChangeAngle << " Slope: " << Slope << " CurrentAngle1: " << CurrentAngle1 << " CurrentAngle2: " << CurrentAngle2 << endl;
		}
		else
		{
			TmpDist = DisPointToSeg ( *CurrentPoint1, *CurrentPoint2, *NextPoint2 );
			if ( TmpDist < MinDist )
				MinDist = TmpDist;
// 			aLOG << "TmpDist: " << TmpDist << " MinDist: " << MinDist << endl;
			CurrentPoint2 = NextPoint2;
			NextPoint2 = GetNextIterator(ch2.mPointList, CurrentPoint2);
			ChangeAngle = AngleToNextPoint2;
			Slope += ChangeAngle;
			CurrentAngle1 += ChangeAngle;
			CurrentAngle2 += ChangeAngle;
// 			aLOG << "ChangeAngle: " << ChangeAngle << " Slope: " << Slope << " CurrentAngle1: " << CurrentAngle1 << " CurrentAngle2: " << CurrentAngle2 << endl;
		}
	}
// 	aLOG << "****************************DistTowConvexHulls Test End***************************" << endl;
	return  MinDist;
}

void ConvexHullManager::MergeConvexHull()
{
// 	aLOG << "MergeConvexHull()" << endl;
	list<ConvexHull *>::iterator itrOuter;
	list<ConvexHull *>::iterator itrInner;
	for(itrOuter = mConvexHullList.begin(); itrOuter != mConvexHullList.end(); itrOuter++)
	{
		itrInner = itrOuter;
		for(++itrInner; itrInner != mConvexHullList.end(); itrInner++)
		{
			float Dist = DistTowConvexHulls(**itrOuter, **itrInner);
// 			aLOG << "Dist: " << Dist << endl;
			if( Dist < MERGEDISTANCE)
			{
				list<ConvexHull *>::iterator itrTem = itrInner;
				itrTem--;
				(*itrOuter)->AddConvexHull(**itrInner);
				delete *itrInner;
				mConvexHullList.erase(itrInner);
				itrInner = itrTem;
			}
		}
	}
	CauculateOutPut();
// 	aLOG << "MergeConvexHull() End" << endl;
}

void ConvexHullManager::CauculateOutPut()
{
// 	aLOG << "********************************CauculateOutPut Start************************************" << endl;
	mOutput.clear();
	list<ConvexHull *>::const_iterator itrCH;
	list<Vector3f>::const_iterator itrV3f;
	for( itrCH = mConvexHullList.begin(); itrCH != mConvexHullList.end(); itrCH++ )
	{
		vector<Vector3f> tmp;
		for (itrV3f = (*itrCH)->mPointList.begin(); itrV3f != (*itrCH)->mPointList.end(); itrV3f++)
		{
			tmp.push_back(*itrV3f);
// 			aLOG << *itrV3f << endl;
		}		
		mOutput.push_back(tmp);
	}
// 	aLOG << "********************************CauculateOutPut End************************************" << endl;
}

bool ConvexHullManager::IsSegmentCross(Vector3f point1, Vector3f point2)
{
	aLOG << "****************************IsSegmentCross Test******************************" << endl;
	list<ConvexHull *>::iterator itrCH;
	for (itrCH = mConvexHullList.begin(); itrCH != mConvexHullList.end(); itrCH++)
	{
		list<Vector3f>::const_iterator itrV3f;
		for (itrV3f = (*itrCH)->mPointList.begin(); itrV3f != (*itrCH)->mPointList.end(); itrV3f++)
		{
			list<Vector3f>::const_iterator itrNextV3f = itrV3f;
			itrNextV3f++;
			if(itrNextV3f == (*itrCH)->mPointList.end())
				continue;
			if(SegmentCross(point1, point2, *itrV3f, *itrNextV3f))
				return true;
		}
		list<Vector3f>::const_iterator itrbeg = (*itrCH)->mPointList.begin();
		list<Vector3f>::const_iterator itrlast = (*itrCH)->mPointList.end();
		itrlast--;
		if(SegmentCross(point1, point2, *itrlast, *itrbeg))
			return true;
	}
	return false;
}

//Yonglei Zheng 2009.12.20
bool ConvexHullManager::IsSegmentCross(Vector3f point1, Vector3f point2, set<ConvexHull *>& crossConvexHullSet)
{
	aLOG << "****************************IsSegmentCross Test******************************" << endl;
	bool crossed = false;
	list<ConvexHull *>::iterator itrCH;
	for (itrCH = mConvexHullList.begin(); itrCH != mConvexHullList.end(); itrCH++)
	{
		list<Vector3f>::const_iterator itrV3f;
		for (itrV3f = (*itrCH)->mPointList.begin(); itrV3f != (*itrCH)->mPointList.end(); itrV3f++)
		{
			list<Vector3f>::const_iterator itrNextV3f = itrV3f;
			itrNextV3f++;
			if(itrNextV3f == (*itrCH)->mPointList.end())
				continue;
			if(SegmentCross(point1, point2, *itrV3f, *itrNextV3f))
			{
				crossed = true;
				crossConvexHullSet.insert(*itrCH);
				//list<Vector3f>::const_iterator it = std::find (crossConvexHullList.begin(), crossConvexHullList.end(), *itrCH);
				//list<Vector3f>::const_iterator it = crossConvexHullList.find (*itrCH);
				//if (it == crossConvexHullList.end())
				//	crossConvexHullList.push_back(*itrCH);
			}
		}
		list<Vector3f>::const_iterator itrbeg = (*itrCH)->mPointList.begin();
		list<Vector3f>::const_iterator itrlast = (*itrCH)->mPointList.end();
		itrlast--;
		if(SegmentCross(point1, point2, *itrlast, *itrbeg))
		{
			crossed = true;
			crossConvexHullSet.insert(*itrCH);
			//list<Vector3f>::const_iterator it = std::find (crossConvexHullList.begin(), crossConvexHullList.end(), *itrCH);
			//list<Vector3f>::const_iterator it = crossConvexHullList.find (*itrCH);
			//if (it == crossConvexHullList.end())
			//	crossConvexHullList.push_back(*itrCH);
		}
	}

	return crossed;
}
//[End]Yonglei Zheng 2009.12.20

void ConvexHullManager::Test()
{
	aLOG << "********************************ConvexHullManager Test Start******************************" << endl;
	
	list<ConvexHull *>::iterator itrCH;
	int numofconvex = 0;
	for (itrCH = mConvexHullList.begin(); itrCH != mConvexHullList.end(); itrCH++)
	{
		aLOG << "ConvexHull No." << numofconvex++ << ": " << endl;
		list<Vector3f>::const_iterator itrV3f;
		for (itrV3f = (*itrCH)->mPointList.begin(); itrV3f != (*itrCH)->mPointList.end(); itrV3f++)
		{
		aLOG << *itrV3f << endl;
		}
	}
	aLOG << "********************************ConvexHullManager Test End********************************" << endl;
}


