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
#ifndef _SPLINE_H
#define _SPLINE_H

/** spline interpolation from book "Numerical Recipes in C"
    array [] start from 1 (not 0)
    and please make sure that m_x[1] < m_x[2] < m_x[3] < ...
*/
class Spline
{
public:
    Spline();
    ~Spline();

    Spline(float* x, float* y, int n);

    void SetBreakPoints(float* x, float* y, int n,
                        float yp1 = 0.0, float ypn = 0.0);

    bool General();

    float GetYGivenX(float x, int der = 0) const;

private:
    float* m_x;

    float* m_y;

    float* m_y2;

    float m_yp1;

    float m_ypn;

    int m_n;
};

#endif
