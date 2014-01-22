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

#ifndef SPECIALACTIONSELECTOR_H
#define SPECIALACTIONSELECTOR_H

#include "ModuleHandler.h"
#include "NaoSpecialAction.h"


/**
* A module selector for GetupEngine modules.
*/
class SpecialActionSelector : public ModuleSelector, public SpecialActionInterfaces
{
public:
/**
* Constructor.
* @param handler The module handler of the process.
* @param interfaces The paramters of the GetupEngine module.
  */
  SpecialActionSelector(ModuleHandler &handler, const SpecialActionInterfaces& interfaces) 
    : ModuleSelector(SolutionRequest::specialAction),
    SpecialActionInterfaces(interfaces)
  {
    handler.setModuleSelector(SolutionRequest::specialAction,this);
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
    	case SolutionRequest::naoSpecialAction:
      		return new NaoSpecialAction(*this);

    	default:
      		return 0;
    }
  }
  
  /** Executes the module*/
  bool executeParameterized()
  {aLOG << "DivingSave: " << endl;
    if (selectedSolution!=0)
{	
      	return ((SpecialAction*)selectedSolution)->executeParameterized();}
    else
      	return false;
  }
};

#endif 

