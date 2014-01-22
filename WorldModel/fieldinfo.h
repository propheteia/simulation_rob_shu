/*
   Copyright (C) 2007, 2008 by Team Apollo
   Nanjing University of Posts and Telecommunications, China

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef FIELDINFO_H
#define FIELDINFO_H


#include "vector.h"
#include "types.h"

#include "StaticModule.h"

//#define  StaticModule<FieldInfo>::getInstance()

class WorldModel;

class FieldInfo
{
public:
    typedef std::map<std::string, TPlayMode> TPlayModeMap;
    TPlayModeMap mPlayModeMap;

    FieldInfo();
    ~FieldInfo();
	
    void Init();
    void SetOurTeamName(std::string name);
    void setOppTeamName(std::string name);

    float match_time;

    std::string our_teamname;

    std::string opp_teamname;

    TTeamIndex team_index;

    TPlayMode play_mode;

    float fieldlength;
    float fieldwidth;
    float goalwidth;
    float goaldepth;
    float goalheight;
    float penaltylength;
    float penaltywidth;

    salt::Vector3f ourgoalcenter;
    salt::Vector3f oppgoalcenter;
    salt::Vector3f oppleftgoallocal;
    salt::Vector3f opprightgoallocal;
    salt::Vector3f oppgoalcenterlocal;
    
    salt::Vector3f ourleftgoallocal;
    salt::Vector3f ourrightgoallocal;
    salt::Vector3f ourgoalcenterlocal;

protected:

    void SetupPlayModeMap();
};


#endif
