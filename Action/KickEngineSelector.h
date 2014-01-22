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

#ifndef KICKENGINESELECTOR_H
#define KICKENGINESELECTOR_H

#include "ModuleHandler.h"
#include "NaoKick.h"
#include "NaoDynamicKick.h"


/**
* A module selector for KickEngine modules.
*/
class KickEngineSelector : public ModuleSelector, public KickEngineInterfaces
{
public:
/**
* Constructor.
* @param handler The module handler of the process.
* @param interfaces The paramters of the KickEngine module.
  */
  KickEngineSelector(ModuleHandler &handler, const KickEngineInterfaces& interfaces) 
    : ModuleSelector(SolutionRequest::kickEngine),
    KickEngineInterfaces(interfaces)
  {
    handler.setModuleSelector(SolutionRequest::kickEngine,this);
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
    	case SolutionRequest::naoKickEngine:
      		return new NaoKickEngine(*this);
		case SolutionRequest::naoDynamicKickEngine:
			return new NaoDynamicKickEngine(*this);
	//case SolutionRequest::strive3DKickEngine:
	//	return new Strive3DKickEngine(*this);
    	default:
      		return 0;
    }
  }
  
  /** Executes the module*/
  bool executeParameterized()
  {
    if (selectedSolution!=0)
      	return ((KickEngine*)selectedSolution)->executeParameterized();
    else
      	return false;
  }
};

#endif 
