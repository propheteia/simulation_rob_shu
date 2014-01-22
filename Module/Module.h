/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei   *
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

#ifndef _MODULE_H
#define _MODULE_H

#include <string>
//using namespace std;

/**Base class for all modules.*/

class Module
{
public:
  /** executes the module */
//#ifdef NDEBUG
	virtual void execute() = 0;

	virtual std::string GetActionCommand() = 0;//by Neil
//#else
  /**
  * When doing a debug build, we save the time, when we entered the module, so we can find endless loops
  */
//  virtual void execute() {
//    entryTime = SystemCall::getCurrentSystemTime ();
//  };
//#endif
	Module() {};
	virtual ~Module() {};

//#ifndef NDEBUG
  /**
  * Used for finding endless loops
  */
	unsigned long EntryTime;
	unsigned long LeaveTime;
//#endif
};

#endif //__Module_h_
