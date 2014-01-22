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
#include "ModuleHandler.h"
#include "Logger.h"

ModuleHandler::ModuleHandler()
{
  for(int i = 0; i < SolutionRequest::numOfModules; i++)
    pModuleSelectors[i] = 0;
}

ModuleSelector* ModuleHandler::pGetModuleSelector(SolutionRequest::ModuleID id) const
{
  return pModuleSelectors[id];
}


void ModuleHandler::setModuleSelector(SolutionRequest::ModuleID id, ModuleSelector *module)
{
  pModuleSelectors[id] = module;
  if (module!=0) module->init();
}

SolutionRequest::ModuleSolutionID ModuleHandler::getSelectedSolution(SolutionRequest::ModuleID id) const
{
  if (pModuleSelectors[id] != 0)
    return pModuleSelectors[id]->getSelectedSolution();
  else
    return SolutionRequest::disabled;
}

void ModuleHandler::selectSolution(SolutionRequest::ModuleID id, SolutionRequest::ModuleSolutionID solution)
{
  if (pModuleSelectors[id] != 0 && pModuleSelectors[id]->getSelectedSolution()!=solution)
  {
	aLOG << " Switched solution for " << SolutionRequest::getModuleName(id) << " from " << SolutionRequest::getModuleSolutionName(id,pModuleSelectors[id]->getSelectedSolution()) <<
      " to " << SolutionRequest::getModuleSolutionName(id,solution) << endl;
    	pModuleSelectors[id]->selectSolution(solution);
  }
}

void ModuleHandler::selectSolutions(SolutionRequest request)
{
  for (int i=0; i < SolutionRequest::numOfModules; i++) 
    selectSolution((SolutionRequest::ModuleID)i,request.solutions[i]);
}


