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

/** Declaration of class ModuleHandler*/

#ifndef _MODULEHANDLER_H
#define _MODULEHANDLER_H

#include "ModuleSelector.h"

/** Manages runtime exchangeable modules.*/
class ModuleHandler
{
private: 
  /** Pointers to all module selectors */
  ModuleSelector *pModuleSelectors[SolutionRequest::numOfModules];

public:
  /** Constructor */
  ModuleHandler();
  
  /**
  * Returns pointer to selector module.
  * @param id id of module.
  */
  ModuleSelector *pGetModuleSelector(SolutionRequest::ModuleID id) const;
  
  /**
  * Set pointer to selector module.
  * @param id id of module.
  * @param module a pointer to the module selector
  */
  void setModuleSelector(SolutionRequest::ModuleID id,ModuleSelector* module);
  
  /** Returns the id of the selected solution for a module id */
  SolutionRequest::ModuleSolutionID getSelectedSolution(SolutionRequest::ModuleID id) const;
  
  /** Selects solution defined by solution in module id */
  void selectSolution(SolutionRequest::ModuleID id, SolutionRequest::ModuleSolutionID solution);
  
  /** Selects solutions as defined by a solution request */
  void selectSolutions(SolutionRequest request);
  
};

#endif //_ModuleHandler_h_

