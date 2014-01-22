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

#ifndef MATRIX_SPLINE_H
#define MATRIX_SPLINE_H

#include "matrix.h"
#include "spline.h"

class MatrixSpline
{
public:
    MatrixSpline();
    ~MatrixSpline();

    MatrixSpline(float* x, salt::Matrix * y, int n);

    void SetBreakPoints(float* x, salt::Matrix * y, int n,
                        float yp1 = 0.0, float ypn = 0.0);

    bool General();

    salt::Matrix GetYGivenX(float x, int der = 0) const;

private:
    float* m_x;
    
    Spline m_spline[12];
    
    float* m_matrix[12];

    float m_yp1;

    float m_ypn;

    int m_n;
};

#endif