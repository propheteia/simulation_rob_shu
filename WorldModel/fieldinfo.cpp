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
#include "fieldinfo.h"
#include "xmlParser.h"
using namespace std;
using namespace salt;

FieldInfo::FieldInfo() :
    match_time(0.0f),
    our_teamname("Strive3D"),
    opp_teamname(""),
    team_index(TI_NONE),
    play_mode(PM_NONE),
    fieldlength(-1.0f),
    fieldwidth(-1.0f),
    goalwidth(-1.0f),
    goaldepth(-1.0f),
    goalheight(-1.0f),
    penaltylength(-1.0f),
    penaltywidth(-1.0f)
{
	Init();
}

FieldInfo::~FieldInfo()
{
}

void FieldInfo::Init()
{
    SetupPlayModeMap();
    fieldlength   = CONF.getXMLValue("/Strive/WorldModel/FieldInfo/FieldSize/FieldLength").asFloat();
    fieldwidth    = CONF.getXMLValue("/Strive/WorldModel/FieldInfo/FieldSize/FieldWidth").asFloat();
    goalwidth     = CONF.getXMLValue("/Strive/WorldModel/FieldInfo/GoalSize/GoalWidth").asFloat();
    goaldepth     = CONF.getXMLValue("/Strive/WorldModel/FieldInfo/GoalSize/GoalDepth").asFloat();
    goalheight    = CONF.getXMLValue("/Strive/WorldModel/FieldInfo/GoalSize/GoalHeight").asFloat();
    penaltylength = CONF.getXMLValue("/Strive/WorldModel/FieldInfo/PenaltySize/PenaltyLength").asFloat();
    penaltywidth  = CONF.getXMLValue("/Strive/WorldModel/FieldInfo/PenaltySize/PenaltyWidth").asFloat();
    
    ourgoalcenter = Vector3f(-fieldlength/2, 0.0f, 0.0f);
    oppgoalcenter = Vector3f(fieldlength/2, 0.0f, 0.0f);
}

void FieldInfo::SetupPlayModeMap()
{
    mPlayModeMap.clear();

    mPlayModeMap[STR_PM_BeforeKickOff]     = PM_BeforeKickOff;
    mPlayModeMap[STR_PM_KickOff_Left]      = PM_KickOff_Left;
    mPlayModeMap[STR_PM_KickOff_Right]     = PM_KickOff_Right;
    mPlayModeMap[STR_PM_PlayOn]            = PM_PlayOn;
    mPlayModeMap[STR_PM_KickIn_Left]       = PM_KickIn_Left;
    mPlayModeMap[STR_PM_KickIn_Right]      = PM_KickIn_Right;
    mPlayModeMap[STR_PM_CORNER_KICK_LEFT]  = PM_CORNER_KICK_LEFT;
    mPlayModeMap[STR_PM_CORNER_KICK_RIGHT] = PM_CORNER_KICK_RIGHT;
    mPlayModeMap[STR_PM_GOAL_KICK_LEFT]    = PM_GOAL_KICK_LEFT;
    mPlayModeMap[STR_PM_GOAL_KICK_RIGHT]   = PM_GOAL_KICK_RIGHT;
    mPlayModeMap[STR_PM_OFFSIDE_LEFT]      = PM_OFFSIDE_LEFT;
    mPlayModeMap[STR_PM_OFFSIDE_RIGHT]     = PM_OFFSIDE_RIGHT;
    mPlayModeMap[STR_PM_GameOver]          = PM_GameOver;
    mPlayModeMap[STR_PM_Goal_Left]         = PM_Goal_Left;
    mPlayModeMap[STR_PM_Goal_Right]        = PM_Goal_Right;
    mPlayModeMap[STR_PM_FREE_KICK_LEFT]    = PM_FREE_KICK_LEFT;
    mPlayModeMap[STR_PM_FREE_KICK_RIGHT]   = PM_FREE_KICK_RIGHT;
    mPlayModeMap[STR_PM_Unknown]           = PM_NONE;
}

void FieldInfo::SetOurTeamName(std::string name)
{
    our_teamname = name;
}

void FieldInfo::setOppTeamName(std::string name)
{
    opp_teamname = name;
}
