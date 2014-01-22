/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei  Qu Junjun *
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

#ifndef HEADCONTROLSELECTOR_H
#define HEADCONTROLSELECTOR_H

#include "ModuleHandler.h"
#include "ModuleSelector.h"
#include "NaoHeadControl.h"

/**
* HeadControl selection module.
*
* A selector for head control modules.
*/
class HeadControlSelector : public ModuleSelector, public HeadControlInterfaces
{
public:
/**
* Constructor.
* @param handler The module handler of the process.
* @param interfaces The paramters of the HeadControl module.
  */
  	HeadControlSelector(ModuleHandler &handler,const HeadControlInterfaces& interfaces)
    	: ModuleSelector(SolutionRequest::headControl),
	moduleHandler(handler),
	HeadControlInterfaces(interfaces)
  	{
    		handler.setModuleSelector(SolutionRequest::headControl,this);
  	}
  
  /** 
  * Is called on start and when the selected solution changes
  * to create a specific solution.
  * @param id The id of the solution to create
  * @return The created solution or 0
  */
  	virtual Module* createSolution(SolutionRequest::ModuleSolutionID id)
  	{
    		switch(id)
    		{
    			case SolutionRequest::naoHeadControl:
      				return new NaoHeadControl(moduleHandler,*this);

    			default:
      				return 0;
    		}
  	}

  /** 
  * A reference to the ModuleHandler of the process, needed to create 
  * the submodules of DefaultActionControl
  */
  	ModuleHandler& moduleHandler;
};

#endif
