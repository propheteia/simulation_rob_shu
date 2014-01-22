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

#ifndef WALKINGENGINESELECTOR_H
#define WALKINGENGINESELECTOR_H

#include "ModuleSelector.h"
#include "NaoWalkingEngine.h"
//#include "Strive3DWalkingEngine.h"

/**
* @class WalkingEngineSelector
*
* A module selector for WalkingEngine modules.
*/
class WalkingEngineSelector : public ModuleSelector, public WalkingEngine
{
public:
/**
* Constructor.
* @param handler The module handler of the process.
* @param moduleID The module id of the walking engine.
* @param interfaces The paramters of the MotionControl module.
  */
  	WalkingEngineSelector(ModuleHandler &handler,
    				SolutionRequest::ModuleID moduleID,
    				const WalkingEngineInterfaces& interfaces) 
    				: ModuleSelector(moduleID),WalkingEngine(interfaces)
  	{
    		handler.setModuleSelector(moduleID,this);
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
	    		case SolutionRequest::strive3DWalkingEngine :
       			/**return new Strive3DWalkingEngine(*this);*///by Neil

      			case SolutionRequest::naoWalkingEngine:
        		return new NaoWalkingEngine(*this);
      
			default:
        		return 0;
    		}
  	}
  
  /** Executes the module */
  	bool executeParameterized( const WalkRequest& walkRequest, double positionInWalkingCycle)
  	{
    		if (selectedSolution!=0) 
    		{
      			return ((WalkingEngine*)selectedSolution)->executeParameterized( walkRequest, positionInWalkingCycle);
    		}
    		else 
    		{	
      			return false;
    		}
  	}

};

#endif 
