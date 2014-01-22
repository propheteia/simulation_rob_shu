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

#include "MatrixSpline.h"
#include <stdio.h>
#define isnan(x) ((x) != (x))

using namespace salt;

MatrixSpline::MatrixSpline() :
    m_x(NULL),
    m_yp1(0.0),
    m_ypn(0.0),
    m_n(0)
{
    for(int i = 0; i < 12; i++)
        m_matrix[i] = NULL;
}

MatrixSpline::~MatrixSpline()
{
    delete [] m_x;
    m_x = NULL;
    for(int i = 0; i < 12; i++)
    {
        delete [] m_matrix[i];
        m_matrix[i] = NULL;
    }
}

MatrixSpline::MatrixSpline(float* x, salt::Matrix * y, int n)
{
    m_x = NULL;
    for(int i = 0; i < 12; i++)
        m_matrix[i] = NULL;
    SetBreakPoints(x, y, n);
}

void MatrixSpline::SetBreakPoints(float* x, Matrix * y, int n, float yp1, float ypn)
{
    delete [] m_x;
    m_x = NULL;
    for(int i = 0; i < 12; i++)
    {
        delete [] m_matrix[i];
        m_matrix[i] = NULL;
    }
    m_n = n;
    m_yp1 = yp1;
    m_ypn = ypn;

    m_x  = new float[m_n + 1];
    for(int i = 0; i < 12; i++)
    {
        m_matrix[i] = new float[m_n + 1];
    }

    for (int i = 1; i <= m_n; ++i)
    {
        m_x[i] = x[i - 1];
        m_matrix[0][i] = y[i - 1](0, 0);
        m_matrix[1][i] = y[i - 1](0, 1);
        m_matrix[2][i] = y[i - 1](0, 2);
        m_matrix[3][i] = y[i - 1](1, 0);
        m_matrix[4][i] = y[i - 1](1, 1);
        m_matrix[5][i] = y[i - 1](1, 2);
        m_matrix[6][i] = y[i - 1](2, 0);
        m_matrix[7][i] = y[i - 1](2, 1);
        m_matrix[8][i] = y[i - 1](2, 2);
        m_matrix[9][i] = y[i - 1](0, 3);
        m_matrix[10][i] = y[i - 1](1, 3);
        m_matrix[11][i] = y[i - 1](2, 3);
    }
}

bool MatrixSpline::General()
{
    for(int i = 0; i < 12; i++)
    {
        m_spline[i].SetBreakPoints(m_x, m_matrix[i], m_n);
        if( !m_spline[i].General() )
            return false;
    }
    return true;
}

Matrix MatrixSpline::GetYGivenX(float x, int der) const
{
    Matrix tmp(Matrix::GetIdentity()); 
    
    tmp(0, 0) = m_spline[0].GetYGivenX(x, der);
    tmp(0, 1) = m_spline[1].GetYGivenX(x, der);
    tmp(0, 2) = m_spline[2].GetYGivenX(x, der);
    tmp(1, 0) = m_spline[3].GetYGivenX(x, der);
    tmp(1, 1) = m_spline[4].GetYGivenX(x, der);
    tmp(1, 2) = m_spline[5].GetYGivenX(x, der);
    tmp(2, 0) = m_spline[6].GetYGivenX(x, der);
    tmp(2, 1) = m_spline[7].GetYGivenX(x, der);
    tmp(2, 2) = m_spline[8].GetYGivenX(x, der);
    tmp(0, 3) = m_spline[9].GetYGivenX(x, der);
    tmp(1, 3) = m_spline[10].GetYGivenX(x, der);
    tmp(2, 3) = m_spline[11].GetYGivenX(x, der);
    
    return tmp;
}