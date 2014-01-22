/*
   Copyright (C) 2007, 2008 by Team Apollo
   Nanjing University of Posts and Telecommunications, China

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef _GEOMETRY_H
#define _GEOMETRY_H
#include <iostream>
#include "tvector.h"
#include "matrix.h"
#include "gmath.h"

inline salt::Vector2f MakeVector2f(const salt::Vector3f& v)
{
    return salt::Vector2f(v.x(), v.y());
}

inline salt::Vector2f MakeVector2f(float angleDeg)
{
    return salt::Vector2f(salt::gCos(salt::gDegToRad(angleDeg)),
                          salt::gSin(salt::gDegToRad(angleDeg)));
}

inline float Precision(float value, float remain = 3.0f)
{	//std::cout << "Precision" << std::endl;
    float tmp = salt::gPow(10.0f, remain);
    return int(value * tmp) / tmp;
}

inline float Constrain(float value, float min, float max)
{
    return std::min(max, std::max(min, value));
}

bool            IsRi(const salt::Matrix& R);

bool            InverseMatrix(salt::Matrix& m);

salt::Vector3f  RotToOmega(const salt::Matrix& R);

salt::Matrix    RotationMatrixFrom2Vec(salt::Vector3f from, salt::Vector3f to);

salt::Matrix    Rodrigues(const salt::Vector3f& axis, float theta);

float   Cross(const salt::Vector2f& O, const salt::Vector2f& end1, const salt::Vector2f& end2);

float   Dot  (const salt::Vector2f& O, const salt::Vector2f& end1, const salt::Vector2f& end2);

float   DistanceToLine(const salt::Vector2f& p, const salt::Vector2f& lineStart,
                       const salt::Vector2f& lineEnd, salt::Vector2f& proj);

float   GetProjectionDistance(const salt::Vector2f& p, const salt::Vector2f& lineStart,
                              const salt::Vector2f& lineEnd);

bool    IsObjectInTheWay(const salt::Vector2f& objectPos, const salt::Vector2f& lineStart,
                         const salt::Vector2f& lineEnd, float radius);

float   GetVector2fAngleDeg(const salt::Vector2f &v);

float   GetMatrixRotateXRad(const salt::Matrix &m);

float   GetMatrixRotateYRad(const salt::Matrix &m);

float   GetMatrixRotateZRad(const salt::Matrix &m);

bool    svd(float* X, const float** A, const float* B, int M, int N);

#endif
