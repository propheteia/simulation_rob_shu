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

#ifndef GETUPENGINESELECTOR_H
#define GETUPENGINESELECTOR_H

#include "ModuleHandler.h"
#include "NaoGetUp.h"
#include "Strive3DGetUp.h"

/**
* A module selector for GetupEngine modules.
*/
class GetupEngineSelector : public ModuleSelector, public GetupEngineInterfaces
{
public:
/**
* Constructor.
* @param handler The module handler of the process.
* @param interfaces The paramters of the GetupEngine module.
  */
  GetupEngineSelector(ModuleHandler &handler, const GetupEngineInterfaces& interfaces) 
    : ModuleSelector(SolutionRequest::getupEngine),
    GetupEngineInterfaces(interfaces)
  {
    handler.setModuleSelector(SolutionRequest::getupEngine,this);
  }
  
  /** 
  * Is called on start and when the selected solution changes
  * to create a specific solution.
  * @param id The id of the solution to create
  * @return The created solution or 0
  */
  virtual Module* createSolution(SolutionRequest::ModuleSolutionID id)
  {
    switch (id)
    {
    	case SolutionRequest::naoGetupEngine:
      		return new NaoGetupEngine(*this);
	case SolutionRequest::strive3DGetupEngine:
		return new Strive3DGetupEngine(*this);
    	default:
      		return 0;
    }
  }
  
  /** Executes the module*/
  bool executeParameterized()
  {
    if (selectedSolution!=0)
      	return ((GetupEngine*)selectedSolution)->executeParameterized();
    else
      	return false;
  }
};

#endif 
