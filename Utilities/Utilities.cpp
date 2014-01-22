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
#include <string.h>
#include "Utilities.h"
 
 unsigned long float2ul(float num)
{
	unsigned long ul;
	memcpy(&ul, &num, sizeof(float));
	ul >>= 5;
	unsigned char a0 = ul & 0x0000003F;
	ul >>= 6;
	unsigned char a1 = ul & 0x0000003F;
	ul >>= 6;
	unsigned char a2 = ul & 0x0000003F;
	ul >>= 6;
	unsigned char a3 = ul & 0x000000FF;
	ul >>= 8;
	if(a3 >= 0x7F)
	{
		a3 -= 0x7F;
		a3 &= 0x1F;
	}
	else
	{
		a3 = 0x7F - a3;
		a3 &= 0x1F;
		a3 |= 0x10;
	}
	a3 |= ul << 5;
	if(a0 != 0x3F)
		a0 |= 1 << 6;
	if(a1 != 0x3F)
		a1 |= 1 << 6;
	if(a2 != 0x3F)
		a2 |= 1 << 6;
	if(a3 != 0x3F)
		a3 |= 1 << 6;
	
	if(a0 == 0x5C)
		a0 = 0x2E;
	if(a1 == 0x5C)
		a1 = 0x2E;
	if(a2 == 0x5C)
		a2 = 0x2E;
	if(a3 == 0x5C)
		a3 = 0x2E;
	ul = a3 << 24 | a2 << 16 | a1 << 8 | a0;
	return ul;
}

float ul2float(unsigned long num)
{
	unsigned long ul = num;
	unsigned char a0 = ul & 0xFF;
	if(a0 == 0x2E)
		a0 = 0x5C;
	else 
		a0 &= 0x3F;
	ul >>= 8;
	unsigned char a1 = ul & 0xFF;
	if(a1 == 0x2E)
		a1 = 0x5C;
	else 
		a1 &= 0x3F;
	ul >>= 8;
	unsigned char a2 = ul & 0xFF;
	if(a2 == 0x2E)
		a2 = 0x5C;
	else 
		a2 &= 0x3F;
	ul >>= 8;
	unsigned char a3 = ul & 0xFF;
	if(a3 == 0x2E)
		a3 = 0x5C;
	else 
		a3 &= 0x3F;
	unsigned char signbit = a3 >> 5;
	a3 &= 0x1F;
	if(a3 & 0x10)
	{
		a3 &= 0x0F;
		a3 = 0x7F - a3;
	}
	else
		a3 += 0x7F;
	ul = signbit << 31 | a3 << 23 | a2 << 17 | a1 << 11 | a0 << 5 | 1 << 4;
	float answer;
	memcpy(&answer, &ul, sizeof(float));
	return answer;
}