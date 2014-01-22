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
#ifndef HEADMOTIONREQUEST_H
#define HEADMOTIONREQUEST_H
#include "gmath.h"
/** 
* @class HeadMotionRequest 
*
* Represents a motion request for the head.
*/
class HeadMotionRequest
{
public:
	/** ids for all HeadMotion types */
    enum HeadMotionID
    {
		intelligencemode,
		automode,
		handmode,
		lookforward,
		lookleft,
		lookright,
		lookatball,
		lookatourgoal,
		lookatoppgoal,
		numOfHeadMotion
    };

	/** returns names for HeadMotion ids */
    static const char* getHeadMotionName(HeadMotionID id)
    {
        switch (id)
        {
			case intelligencemode:
				return "intelligencemode";
			case automode:
				return "automode";
			case handmode:
				return "handmode";
			case lookforward:
				return "lookforward";
			case lookleft:
				return "lookleft";
			case lookright:
				return "lookright";
			case lookatball:
				return "lookatball";
			case lookatourgoal:
				return "lookatourgoal";
			case lookatoppgoal:
				return "lookatoppgoal";
			default:
				return "Unknown HeadMotion name, please edit HeadMotionRequest::getHeadMotionName";
        }
    }

	bool operator==(const HeadMotionRequest& other) const
    {
        switch (headMotionType)
        {
			case intelligencemode:
			case automode:
			case lookforward:
			case lookleft:
			case lookright:
			case lookatball:
			case lookatourgoal:
			case lookatoppgoal:
				return (headMotionType == other.headMotionType);
			case handmode:
			default:
            return (
                       headMotionType == other.headMotionType &&
                       salt::gAbs(pitch - other.pitch) < 1e-4 && salt::gAbs(yaw - other.yaw) < 1e-4);
        }
    }

	/** = operator */
	void operator=(const HeadMotionRequest& other);

	/** returns name of current head motion */
    const char* getHeadMotionName() const
    {
        return getHeadMotionName(headMotionType);
    };

  	/** requested head joint angles */
  	float pitch,yaw;

	/** head motion type */
	HeadMotionID headMotionType;

  	/** constructor */
  	HeadMotionRequest();

  	/** destructor */
  	~HeadMotionRequest();
};

#endif
