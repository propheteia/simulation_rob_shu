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

#include "ModuleSelector.h"

ModuleSelector::ModuleSelector(SolutionRequest::ModuleID id) 
: moduleID(id), selectedSolution(0)
, maxRunTime(0), minRunTime(0xffffffff), averageRunTime(0), frameCounter(0)
{
  selectedSolutionID = SolutionRequest::getDefaultSolution(id);
}

ModuleSelector::~ModuleSelector()
{
  if (selectedSolution != 0) delete selectedSolution;
}

void ModuleSelector::selectSolution(SolutionRequest::ModuleSolutionID id) 
{
  assert(id < solutionRequestMaxNumOfSolutions);
  if (id != selectedSolutionID)
  {
    if (selectedSolution != 0) 
    {
      delete selectedSolution;
    }
    selectedSolutionID=id;
    init();
  }
}

SolutionRequest::ModuleSolutionID ModuleSelector::getSelectedSolution() const 
{
  return selectedSolutionID;
}

void ModuleSelector::execute() 
{
  /**unsigned long time = SystemCall::getCurrentSystemTime();*/
  	if (selectedSolution != 0) 
		selectedSolution->execute();
		command = selectedSolution->GetActionCommand();//by Neil,just test.
 
/* time = SystemCall::getTimeSince(time);
  if (time > maxRunTime)
  {
    maxRunTime = time;
  }
  if (time < minRunTime)
  {
    minRunTime = time;
  }
  averageRunTime = averageRunTime*frameCounter + time;
  frameCounter++;
  averageRunTime /= frameCounter;*/
}


void ModuleSelector::init()
{
  selectedSolution = this->createSolution(selectedSolutionID);
	aLOG<<"ModuleSelector::init(): create default solution \""
      	<< SolutionRequest::getModuleSolutionName(moduleID,selectedSolutionID) 
      	<< "\" of module \"" 
      	<< SolutionRequest::getModuleName(moduleID) << "\"." << endl;
  if ((selectedSolution == 0)&&(selectedSolutionID != SolutionRequest::disabled))
  {     
    	aLOG<<"ModuleSelector::init(): could not create default solution \""
      		<< SolutionRequest::getModuleSolutionName(moduleID,selectedSolutionID) 
      		<< "\" of module \"" 
      		<< SolutionRequest::getModuleName(moduleID) << "\"." << endl;
  }
}