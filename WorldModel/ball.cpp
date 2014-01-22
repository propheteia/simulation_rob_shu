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
#include "ball.h"

Ball::Ball()
{
	for(int i = 0; i < 8; i++)
	{
		area.push_back(DefaultPoint[i] + pos);
	}
}

Ball::~Ball()
{
}

const Vector3f Ball::DefaultPoint[8] = {	
	Vector3f(DEFAULTBALLAREA * gCos(gDegToRad(247.5)), DEFAULTBALLAREA * gSin(gDegToRad(247.5)), 0),
	Vector3f(DEFAULTBALLAREA * gCos(gDegToRad(292.5)), DEFAULTBALLAREA * gSin(gDegToRad(292.5)), 0),
	Vector3f(DEFAULTBALLAREA * gCos(gDegToRad(-22.5)), DEFAULTBALLAREA * gSin(gDegToRad(-22.5)), 0),
	Vector3f(DEFAULTBALLAREA * gCos(gDegToRad(22.5)), DEFAULTBALLAREA * gSin(gDegToRad(22.5)), 0),
	Vector3f(DEFAULTBALLAREA * gCos(gDegToRad(67.5)), DEFAULTBALLAREA * gSin(gDegToRad(67.5)), 0),
	Vector3f(DEFAULTBALLAREA * gCos(gDegToRad(112.5)), DEFAULTBALLAREA * gSin(gDegToRad(112.5)), 0),
	Vector3f(DEFAULTBALLAREA * gCos(gDegToRad(157.5)), DEFAULTBALLAREA * gSin(gDegToRad(157.5)), 0),
	Vector3f(DEFAULTBALLAREA * gCos(gDegToRad(202.5)), DEFAULTBALLAREA * gSin(gDegToRad(202.5)), 0),
										};
