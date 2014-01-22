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
#ifndef __UTILITIES_H
#define __UTILITIES_H

//!Transfer float(32 bits) into unsigned long(32 bits:4 chars) for say effector
//!After transfering, every char will be in the ASCII printing character subset[0x20,0x7E]
unsigned long float2ul(float num);

//!Transfer unsigned long into float for hear perceptor
float ul2float(unsigned long num);

#endif // __UTILITIES_H
