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

#ifndef BEHAVIORCONTROLSELECTOR_H
#define BEHAVIORCONTROLSELECTOR_H

#include "ModuleHandler.h"
#include "ModuleSelector.h"
#include "Strive3DBehaviorControl.h"
//#include "NaoBehaviorControl.h"
//#include "WalkPathPlanningControl.h"

/**
* BehaviorControl selection module.
*
* A selector for behavior control modules.
*/
class BehaviorControlSelector : public ModuleSelector, public BehaviorControlInterfaces
{
public:
/**
* Constructor.
* @param handler The module handler of the process.
* @param interfaces The paramters of the BehaviorControl module.
  */
  BehaviorControlSelector(ModuleHandler &handler, const BehaviorControlInterfaces& interfaces)
    : ModuleSelector(SolutionRequest::behaviorControl), 
      moduleHandler(handler),
      BehaviorControlInterfaces(interfaces)
  {
    handler.setModuleSelector(SolutionRequest::behaviorControl, this);
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
    	case SolutionRequest::strive3DBehaviorControl:
      		return new Strive3DBehaviorControl(moduleHandler,*this);
//	case SolutionRequest::naoBehaviorControl:
//		return new NaoBehaviorControl(moduleHandler,*this);
//	case SolutionRequest::walkPathPlanningControl:
//		return new WalkPathPlanningControl(moduleHandler,*this);

    //case SolutionRequest::strive3DBehaviorControlAgentBasicsTester:
      //return new Xabsl2BehaviorControlAgentInterface("basics-tester",new Strive3DBehaviorControl(*this),*this);

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
