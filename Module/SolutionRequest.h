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

#ifndef _SOLUTIONREQUEST_H
#define _SOLUTIONREQUEST_H

/** maximum number of solutions per module */
const int solutionRequestMaxNumOfSolutions = 40;

/** Manages runtime exchangeable modules. */
class SolutionRequest
{
public:
    /**
    * Enumerator for exchange modules
    * Add new exchangeable module here.
    * Create new selector module for new exchangeable modules
    */
    enum ModuleID
    {
        behaviorControl, /*!< See class BehaviorControlSelector */
        headControl, /*!< See class HeadControlSelector */
        walkingEngine, /*!< See class WalkingEngineSelector */
        actionControl, /*!< See class ActionControlSelector */
        getupEngine, /*!< See class GetupEngineSelector */
        kickEngine,   /*!< See class KickEngineSelector */
        specialAction,
        numOfModules /*!< Number of exchangeable modules */
    };

    /** enumerator for available solutions for all exchangeable modules
    * \attention add new solutions here
    * \attention dont forget to edit selector module too
    */
    enum ModuleSolutionID
    {
        disabled = -1,

        // BehaviorControl solutions
        strive3DBehaviorControl = 0,
        naoBehaviorControl,
        walkPathPlanningControl,
        strive3DBehaviorControlAgentBasicsTester,
        numOfBehaviorControlSolutions,

        // WalkingEngine solutions
        strive3DWalkingEngine = 0,
        naoWalkingEngine,
        numOfWalkingEngineSolutions,

        // HeadControl solutions
        strive3DHeadControl = 0,
        naoHeadControl,
        numOfHeadControlSolutions,

        // GetupEngine solutions
        strive3DGetupEngine = 0,
        naoGetupEngine,
        numOfGetupEngineSolutions,

        //KickEngine solution
        strive3DKickEngine = 0,
        naoKickEngine,
		naoDynamicKickEngine,
        numOfKickEngineSolutions,

        //SpecialAction solution
        strive3DSpecialAction = 0,
        naoSpecialAction,
        numOfSpecialActionSolutions,

        // ActionControl solutions
        strive3DActionControl = 0,
        naoActionControl,
        debugActionControl,
        numOfActionControlSolutions,

    };

    /** returns names for module ids
    * Add new exchangeable module here
    */
    static const char* getModuleName(ModuleID id)
    {
        switch (id)
        {
        case behaviorControl:
            return "BehaviorControl";
        case headControl:
            return "HeadControl";
        case walkingEngine:
            return "WalkingEngine";
        case getupEngine:
            return "GetupEngine";
        case actionControl:
            return "ActionControl";
        case kickEngine:
            return "KickEngine";
        case specialAction:
            return "SpeciaAction";

        default:
            return "please edit SolutionRequest::getModuleName";
        }
    }

    /** Returns number of solutions in module id
    * \attention add new exchangeable module here
    */
    static int getNumOfSolutions(ModuleID id)
    {
        switch (id)
        {
        case behaviorControl:
            return numOfBehaviorControlSolutions;
        case headControl:
            return numOfHeadControlSolutions;
        case walkingEngine:
            return numOfWalkingEngineSolutions;
        case getupEngine:
            return numOfGetupEngineSolutions;
        case actionControl:
            return numOfActionControlSolutions;
        case kickEngine:
            return numOfKickEngineSolutions;
        case specialAction:
            return numOfSpecialActionSolutions;

        default:
            return 0;
        }
    }

    /**
    * Returns names for module solution ids.
    * Add new solutions here.
    */
    static const char* getModuleSolutionName(ModuleID module, ModuleSolutionID solution)
    {
        if (solution == disabled) return "disabled";
        switch (module)
        {
        case behaviorControl:
            switch (solution)
            {
            case strive3DBehaviorControl:
                return "Strive3D--Behavior-Control";
            case naoBehaviorControl:
                return "Nao-Behavior-Control";
            case walkPathPlanningControl:
                return "WalkPath-Planning-Control-Debug";
            case strive3DBehaviorControlAgentBasicsTester:
                return "Strive3D-basics-tester";
            }

        case walkingEngine:
            switch (solution)
            {
            case strive3DWalkingEngine:
                return "Strive3D-Walking-Engine";
            case naoWalkingEngine:
                return "Nao-Walking-Engine";
			case naoDynamicKickEngine:
				return "Nao-Dynamic-Walking-Enging";
            }

        case headControl:
            switch (solution)
            {
            case strive3DHeadControl:
                return "Strive3D-Head-Control";
            case naoHeadControl:
                return "Nao-Head-Control";
            }

        case getupEngine:
            switch (solution)
            {
            case strive3DGetupEngine:
                return "Strive3D-GetUp-Engine";
            case naoGetupEngine:
                return "nao-Getup-Engine";
            }

        case actionControl:
            switch (solution)
            {
            case strive3DActionControl:
                return "Strive3D-Action-Control";
            case naoActionControl:
                return "Nao-Action-Control";
            case debugActionControl:
                return "Debug-Action-Control";
            }
        case kickEngine:
            switch (solution)
            {
            case naoKickEngine:
                return "Nao-Kick-Engine";
            case strive3DKickEngine:
                return "Strive3D-Kick-Engine";
			case naoDynamicKickEngine:
				return "Nao-Dynamic-Kick-Engine";
            }
        case specialAction:
            switch (solution)
            {
            case naoSpecialAction:
                return "Nao-Special-Action";
            }

        }

        return "please edit SolutionRequest::getModuleSolutionName";
    }

    /** an array containing the selected solution for each module */
    ModuleSolutionID solutions[numOfModules];

    /** Constructor
    * @param setDefault if true default modules will be selected else all modules are disabled
    */
    SolutionRequest(bool setDefault = false);

    /**
    * Returns default solution for module.
    */
    static ModuleSolutionID getDefaultSolution(ModuleID id)
    {
        static SolutionRequest defaultRequest(true);
        return defaultRequest.solutions[id];
    }

    /**
    * Sets all modules to default solutions.
    */
    void setDefaultSolutions();

    /** the == operator */
    bool operator == (const SolutionRequest& other);

    /** the != operator */
    bool operator != (const SolutionRequest& other)
    {
        return ! (*this == other);
    }

    /** IDs for the module solutions using a XabslEngine */
    enum xabslEngineID
    {
        strive3D,
        strive3D_headControl,
        nao,
        nao_headControl,
        numOfXabslBehaviors,
        undefined
    };

    /** returns the name of a xabslEngineID */
    static const char* getXabslEngineIDName(xabslEngineID id)
    {
        switch (id)
        {
        case strive3D:
            return "STRIVE3D-XabslEngine";
        case strive3D_headControl:
            return "STRIVE3D-HeadControl-XabslEngine";
        case nao:
            return "NAO-XabslEngine";
        case nao_headControl:
            return "NAO-HeadControl-XabslEngine";
        default:
            return "unknown-XabslEngine";
        }
    }

    /** returns the beginning of the file name of a config file
    * for a xabslEngine
    */
    static const char* getXabslEngineFileID(xabslEngineID id)
    {
        switch (id)
        {
        case strive3D:
            return "strive_BC";
        case strive3D_headControl:
            return "strive_HC";
        case nao:
            return "nao_BC";
        case nao_headControl:
            return "nao_HC";
        default:
            return "unknown XabslEngine File";
        }
    }

};

#endif

