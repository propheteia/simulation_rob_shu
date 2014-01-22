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

#ifndef HEADPATHPLANNER_H
#define HEADPATHPLANNER_H

#include "vector.h"
#include "Range.h"

using namespace salt;

/**
* Calculate a smooth series of head joint angles from a gives
* set of way points and an overall duration.
*/
class HeadPathPlanner
{
public:
    /**
    * Initializes a set of points to visit in a certain time
    * @param vectors set of arcs to visit
    * @param durations a set of timings which described the time between the arcs
    * @param numberOfVectors number of Vector3s in param vectors
    * @param optimizeTimings the timings will be optimized by the distance in angles for optimal moving speed
    */
    void init(const Vector3f* vectors=0, long* durations=0, int numberOfVectors=0,bool optimizeTimings=true);
    void oldInit(const Vector3f* vectors=0,int numberOfVectors=0, long duration=0)
    {
        if (numberOfVectors==1)
        {
            long durations[1]={duration};
            init(vectors,durations,1);
        }
        else
        {
            // divide in equal chunks
            if (numberOfVectors!=0)
            {
                // division by zero work around
                long chunkedDuration = duration / numberOfVectors;
                long durations[maxNumberOfPoints+1];
                // first time
                durations[0]=0;
                for (int i=1;i<=numberOfVectors;i++)
                    durations[i]=chunkedDuration;
                init(vectors,durations,numberOfVectors);
            }
        }
    }

    /**
    * default constructor
    */

    HeadPathPlanner():lastHeadPitch(0),lastNeckYaw(0),currentPoint(0),currentFrame(0),numberOfFrames(0),numberOfPoints(0) {}


    /**
    * Calculates the angles for yaw and rpitch
    * @return true if the last point is reached.
    */
    bool getAngles(float& yaw, float& pitch);

    /**
    * Return whether the last initialized path is already finished
    * @return true if last path is finished.
    */
    inline bool isLastPathFinished()
    {
        return (currentFrame>=numberOfFrames);
    }


    /* returns the minimum time which is needed between the given headposition */
    long calculateHeadTiming(Vector3f &pos1,Vector3f &pos2);

    /* returns true, if the head has reached the given position */
    bool headPositionReached(Vector3f pos);


    /** The minimum head speed in rad per frame: 0.004 = 28.6?s */
    static const float minimumHeadSpeed;

    /** The pitch calculated in the last frame */
    float lastHeadPitch;

    /** The yaw calculated in the last frame */
    float lastNeckYaw;

    /** the maximum speed of all angles */
    float headPathSpeedHeadPitch,headPathSpeedNeckYaw;



private:

    /**
    * Return the whole Duration of the Headpath
    * @return the sum of duration of the headpath
    */
    long calculateDurationsSum(long* duration, int durations);

    /** index of the most recently reached point in head path,
     * so the head is between points[currentPoint] and points[currentPoint+1] */
    long currentPoint;

    /** number of frames (a 8ms) since start of head path */
    long currentFrame;

    /** requested duration of head path in frames (a 20ms) */
    long numberOfFrames;


    /** maximum number of allowed points in head path*/
    enum {maxNumberOfPoints = 20};

    /** number of points in requested path */
    long numberOfPoints;

    /** the points the head shall visit during the head path*/
    Vector3f points[maxNumberOfPoints];

    /** number of the first frame after head path start for a certain point in head path */
    float firstFrame[maxNumberOfPoints];

};


#endif //HeadPathPlanner
