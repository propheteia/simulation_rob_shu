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

#include "HeadPathPlanner.h"
#include "gmath.h"

const float HeadPathPlanner::minimumHeadSpeed=180.0;//degree/s,just a test by Neil;

void HeadPathPlanner::init(const Vector3f* vectors, long* durations,int numberOfVectors, bool optimizeTimings)
{
    if (numberOfVectors > maxNumberOfPoints-1) numberOfVectors=maxNumberOfPoints-1;

    enum {FRAMEQUOTIENT = 20};

    //const Range<double> jointRangeNeckTilt(robotDimensions.jointLimitNeckTiltN,robotDimensions.jointLimitNeckTiltP);
    //const Range<double> jointRangeHeadPan( robotDimensions.jointLimitHeadPanN, robotDimensions.jointLimitHeadPanP);
    //const Range<double> jointRangeHeadTilt(robotDimensions.jointLimitHeadTiltN,robotDimensions.jointLimitHeadTiltP);
    const Range<float> jointRangeNeckYaw(-120,120);
    const Range<float> jointRangeHeadPitch( -45, 45);

    //start from current position
    points[0].x() = 0;
    points[0].y() = jointRangeHeadPitch.limit(lastHeadPitch);
    points[0].z() = jointRangeNeckYaw.limit(lastNeckYaw);
    // sometimes the values are weird, so the first frame should be ignored
    if (points[0].y() != lastHeadPitch || points[0].z() != lastNeckYaw)
    {
        points[0].y() = jointRangeHeadPitch.limit(vectors[0].y());
        points[0].z() = jointRangeNeckYaw.limit(vectors[0].z());
        durations[0]=20;
    }

    currentPoint = 0;
    numberOfPoints = numberOfVectors;
    currentFrame = 0;

    //calculate total distance and speed
    int i;
    for (i=0;i<numberOfVectors;i++)
    {
        //clip arcs to physical limits
        points[i+1].y() = jointRangeHeadPitch.limit(vectors[i].y());
        points[i+1].z() = jointRangeNeckYaw.limit(vectors[i].z());
        if (optimizeTimings)
        {
            long tempTime = calculateHeadTiming(points[i],points[i+1]);
            if (durations[i] < tempTime)
                durations[i] = tempTime;
        }
    }

    long overAllDuration = calculateDurationsSum(durations, numberOfVectors);
    numberOfFrames = overAllDuration / FRAMEQUOTIENT;

    //calculate duration for each part of the route
    firstFrame[0] = 0;
    for (i=0;i<numberOfVectors;i++)
    {
        firstFrame[i + 1] = firstFrame[i] + (durations[i] / FRAMEQUOTIENT);
    }
}

long HeadPathPlanner::calculateDurationsSum(long* duration, int durations)
{
    long sum=0;
    for (int i=0;i<durations;i++)
    {
        // correcting, if duration is shorter than min movement time
        if (duration[i]<20) duration[i]=20;
        sum+=duration[i];
    }
    return sum;
}


bool HeadPathPlanner::headPositionReached(Vector3f pos)
{
    /**need change by Neil*/
    /*double grad2Rad = toMicroRad(pi / 180);
    Vector3f posInMicroRad;

    posInMicroRad.x() = (long) toMicroRad(pos.x());
    posInMicroRad.y() = (long) toMicroRad(pos.y());
    posInMicroRad.z() = (long) toMicroRad(pos.z());

    if (   abs(sensorDataBuffer.lastFrame().data[SensorData::neckTilt]-posInMicroRad.x()) < 8 * grad2Rad
            && abs(sensorDataBuffer.lastFrame().data[SensorData::headPan]-posInMicroRad.y())  < 8 * grad2Rad
            )
        return true;
    return false;*/
}


bool HeadPathPlanner::getAngles(float& neckYaw, float& headPitch)
{
    if (currentFrame<numberOfFrames)
    {
        currentFrame++;
        while ((currentFrame>firstFrame[currentPoint+1])&&
                (currentPoint<numberOfPoints-1)&&
                (currentFrame<numberOfFrames))
        {
            currentPoint++;
        }

        float distanceInFrames=0;
        if (currentPoint<numberOfPoints)
            distanceInFrames = firstFrame[currentPoint+1]-firstFrame[currentPoint];

        if (distanceInFrames==0)
        {
            headPitch  = points[currentPoint].y();
            neckYaw    = points[currentPoint].z();
        }
        else
        {
            float leftFactor = (firstFrame[currentPoint+1]-currentFrame)/distanceInFrames;
            float rightFactor = 1-leftFactor;

            headPitch  = (leftFactor*points[currentPoint].y() + rightFactor*points[currentPoint+1].y());
            neckYaw = (leftFactor*points[currentPoint].z() + rightFactor*points[currentPoint+1].z());
        }
    }
    else
    {

        headPitch  = (points[currentPoint+1].y());
        neckYaw = (points[currentPoint+1].z());
    }
    return (currentFrame>=numberOfFrames);
}

long HeadPathPlanner::calculateHeadTiming(Vector3f &pos1,Vector3f &pos2)
{
    Vector3f minTime;

    minTime.y() = fabs(pos1.y()-pos2.y())/headPathSpeedHeadPitch;
    minTime.z() = fabs(pos1.z()-pos2.z())/headPathSpeedNeckYaw;

    // explain slowest speed
    return (long) gMax(minTime.y(),minTime.z());
}
