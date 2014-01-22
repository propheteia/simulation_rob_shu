/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei, Qu Junjun   *
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

#ifndef STRIVE3DBEHAVIORCONTROL_H
#define STRIVE3DBEHAVIORCONTROL_H
#include "BehaviorControl.h"
#include "ModuleHandler.h"
#include "AdvancedAgent.h"
#include "Net.h"
#include <vector>

class Strive3DBehaviorControl: public BehaviorControl
{
public:
	enum BallState
	{
		OnOurField = 0,
		OnOppField = 1
	};
	
	enum BallHolder
	{
		Self = 0,
		Tmm = 1,
		Opp = 2
	};

	//2009,12,23 by ianpan	
	enum DefenceState
	{
		oppshooting = 0,
		oppoppreadyforshoot =1,
		oppdribbling = 2,
		nobehavior
	}; 
	
	enum GolieState
	{
		divingsave = 0,
		getreadyfordivingsave =1,
		returntocenter = 2,
		debarrass = 3,
		goaround = 4,
		stand=5
	};

    Strive3DBehaviorControl( ModuleHandler& moduleHandler,const BehaviorControlInterfaces& interfaces);
  
  /** destructor */
  ~Strive3DBehaviorControl(){};

  	virtual void execute();
	virtual std::string GetActionCommand() {return "";}
	void GoToBallByCommunication();
	void GoToKicktheBall(float force);
	void GoToBallWithoutTurning();
	void GoToPoint(salt::Vector2f dest, double speedfactor);
	
	void GoToRelativePoint(salt::Vector2f Destination);
	void Stop();
	void Stand();

	void TurnTo(salt::Vector2f dest, double angle);
	bool GoToPointAndTurnTo(salt::Vector2f dest, double angle);
	void DribbleTo(salt::Vector2f dest);
	bool AdjustmentForKick(salt::Vector2f dest);
	void KickTo(salt::Vector2f dest);
	void LocalKickTo(Vector2f dest);
	bool GoToBallToTargetLine();
	bool AdjustmentForPass(salt::Vector2f dest);
	void PassTo(salt::Vector2f dest);
	void LocalPassTo(salt::Vector2f dest);
	
	//!2009.12.19 BY Qu Junjun
	bool	Decision();	
	bool	OffensiveDecision();
	bool	DefensiveDecision();
	bool	GoalieDecision();
	bool	PrepareDecision();
	
	bool 	GoToBallForShoot(salt::Vector2f destination);
	void	GoAroundBall(bool clockwise);
	bool	GoBeHindBall();
	bool	LocalViewGoToBallForShoot(salt::Vector2f destination);
	bool 	IsCanShoot();
	bool 	IsCanPassout();

	void	DivingSave(bool left = true);
	void	CoverTeammate(float dist);
	void	CoverTeammate(float dist, salt::Vector2f offset);
	void 	ManToManDef(Player & player, float disttoall);
	void	ClearanceKick();
	void 	ObstacleAvoidanceTo(const salt::Vector2f goalPos);
	void 	sendMsg2GuiDebugger();

	// 	2009,12,24 by ianpan
	void	defencestatejudge();
	void	goliestatejudge();
	void newgoliestatejudge();    //2010,7,8  by luan

	void	Forward1OffDecision();
	void	Forward2OffDecision();
	void	CenterOffDecision();
	void	Guard1OffDecision();
	void	Guard2OffDecision();
	void	Guard3OffDecision();
	void	Guard4OffDecision();
	void	Guard5OffDecision();

	void	Forward1DefDecision();
	void	Forward2DefDecision();
	void	CenterDefDecision();
	void	Guard1DefDecision();
	void	Guard2DefDecision();
	void	Guard3DefDecision();
	void	Guard4DefDecision();
	void	Guard5DefDecision();
	void KeepGuard2DefDecision();

	void	KickDecision();
	void	PassDecision();
	float FuzzyControl(float distance,float angle);
	//2012.11
	void Guard6DefDecision();
	void Guard7DefDecision();
	void Guard6OffDecision();
	void Guard7OffDecision();
//2013.2
    void Debug(double walkparams_x,double walkparams_y,double rotation);
    void MoveToDest(Vector2f dest,double speedfactor);
    void TurnBeforeMoveToDest(Vector2f dest,double speedfactor);
    void TurnAndMoveToDest(Vector2f dest,double speedfactor);
    void FineTune(Vector2f dest,double angle);
protected:
  	/** 
  	* A reference to the ModuleHandler of the Process. 
  	* Needed to create new solutions.
  	*/
  	ModuleHandler& moduleHandler;

  	
  	//!2009.12.19 BY Qu Junjun
  	BallState ballState;
	BallHolder ballHolder;

	std::vector<salt::Vector2f> mTmpPosVect;

	//2009.12,23 by ianpan
	DefenceState defencestate;
	GolieState goliestate;
};

#endif 
