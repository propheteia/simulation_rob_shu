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

#include "xmlParser.h"
#include "SolutionRequest.h"

SolutionRequest::SolutionRequest(bool setDefault)
{
    if (setDefault)
        setDefaultSolutions();
    else
    {
        for (int i=0;i<numOfModules;i++)
        {
            solutions[i] = disabled;
        }
    }
}

void SolutionRequest::setDefaultSolutions()
{  /**need to set Default Solution*/ //by Neil
    solutions[behaviorControl] = ModuleSolutionID(CONF.getXMLValue("/Strive/Module/BehaviorControl").asInt());
    solutions[actionControl] = ModuleSolutionID(CONF.getXMLValue("/Strive/Module/ActionControl").asInt());
    solutions[walkingEngine] = ModuleSolutionID(CONF.getXMLValue("/Strive/Module/WalkingEngine").asInt());
    solutions[getupEngine] = ModuleSolutionID(CONF.getXMLValue("/Strive/Module/GetupEngine").asInt());
    solutions[headControl] = ModuleSolutionID(CONF.getXMLValue("/Strive/Module/HeadControl").asInt());
    solutions[kickEngine] = ModuleSolutionID(CONF.getXMLValue("/Strive/Module/KickEngine").asInt());
    solutions[specialAction] = ModuleSolutionID(CONF.getXMLValue("/Strive/Module/SpecialAction").asInt());
}

bool SolutionRequest::operator == (const SolutionRequest& other)
{
    for (int i=0;i<numOfModules;i++)
    {
        if (solutions[i] != other.solutions[i]) return false;
    }
    return true;
}
