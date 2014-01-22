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

#ifndef _MODULESELECTOR_H
#define _MODULESELECTOR_H

#include "SolutionRequest.h"
#include "Module.h"
#include "Logger.h"

/** Base class for module selectors. Module selectors allow to switch between different  solutions for a modules.*/
class ModuleSelector
{
protected:
  /** The id of the selected solution */
  SolutionRequest::ModuleSolutionID selectedSolutionID;
  
public:
  /** 
  * Constructor 
  * @param id The id of the module. 
  */
  ModuleSelector(SolutionRequest::ModuleID id);
  
  /** Destructor */
  ~ModuleSelector();
  
  /** 
  * Is called on start and when the selected solution changes
  * to create a specific solution.
  * @param id The id of the solution to create
  * @return The created solution or 0
  */
  virtual Module* createSolution(SolutionRequest::ModuleSolutionID id) = 0;

  /** 
  * Creates the default solution.
  * This can not be done in the constructor, 
  * as the derived selector class is not created yet.
  */
  void init();

  /**
  * Selects a module.
  * @param id id of module to select
  */
  virtual void selectSolution(SolutionRequest::ModuleSolutionID id);
  
  /** Returns the index of the selected solution. */
  SolutionRequest::ModuleSolutionID getSelectedSolution() const;
  
  /** Executes the selected module */
  virtual void execute();

  /** RunTime */
  unsigned int frameCounter;
  /** RunTime */
  double averageRunTime;
  /** RunTime */
  unsigned long maxRunTime;
  /** RunTime */
  unsigned long minRunTime;

	std::string command;//by Neil,just test.

protected:

  /** The id of the module, needed only for error messages. */
  SolutionRequest::ModuleID moduleID;

  /** The selected solution */
  Module *selectedSolution;
};


#endif /// _ModuleSelector_h_

