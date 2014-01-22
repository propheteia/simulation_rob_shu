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

#ifndef HEADCONTROLMODE_H
#define HEADCONTROLMODE_H

/**
* Stores head modes requested by BehaviorControl
*/
class HeadControlMode
{
public:
    /** constructor */
    HeadControlMode(){};

    /** destructor */
    ~HeadControlMode(){};

    /** possible head control modes */
    enum HeadControlModes
    {
        none,
        searchForBall,
        searchAuto,
        lookBetweenFeet,
        lookLeft,
        lookRight,

        searchForBallLeft,
        searchForBallRight,


        lookAtInsertionPointFrontLeft,
        lookAtInsertionPointFrontRight,

        lookAroundCommunicatedBallPosition,

        realSlowScan,

        numOfHeadControlModes
    };

    /** The requested head control mode */
    HeadControlModes headControlMode;

    /** Returns the name of a HeadTrackMode. */
    static const char* getHeadControlModeName(HeadControlModes mode)
    {
        switch (mode)
        {
        case none:
            return "none";
        case searchForBall:
            return "searchForBall";
        case searchAuto:
            return "searchAuto";

        case lookBetweenFeet:
            return "lookBetweenFeet";
        case lookLeft:
            return "lookLeft";
        case lookRight:
            return "lookRight";

        case searchForBallLeft:
            return "searchForBallLeft";
        case searchForBallRight:
            return "searchForBallRight";

        case lookAroundCommunicatedBallPosition:
            return "lookAroundCommunicatedBallPosition";

        case realSlowScan:
            return "realSlowScan";

        default:
            return "unknown Mode";
        }
    }

    /** Parameters for direct mode */
    double directPitch, directYaw, directSpeed;
    unsigned int durationInEvolution;
};

#endif
