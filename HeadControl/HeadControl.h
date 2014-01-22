/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei  Qu Junjun *
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

#ifndef HEADCONTROL_H
#define HEADCONTROL_H

#include "Module.h"

#include "HeadControlMode.h"
#include "HeadMotionRequest.h"
#include "ActionInfo.h"
#include "ActionRequest.h"



/**
* @class HeadControlInterfaces
* 
* The interfaces of the HeadControl module.
*/
class HeadControlInterfaces
{
public:
  /** Constructor. */
  HeadControlInterfaces(
    const ActionRequest& actionRequest,
    const ActionInfo& actionInfo,
    const HeadControlMode& headControlMode,
    const bool headIsBlockedBySpecialActionOrWalk,
    HeadMotionRequest& headMotionRequest)
    :
    headControlMode(headControlMode),
    actionRequest(actionRequest),
    actionInfo(actionInfo),
    headIsBlockedBySpecialActionOrWalk(headIsBlockedBySpecialActionOrWalk),
    headMotionRequest(headMotionRequest)
  {}

protected:

  /** A modus from the behavior how to move the head */
  const HeadControlMode& headControlMode;

  /** motionRequest that is currently executed */
  const ActionRequest& actionRequest;

  /** information about the executed actions */
  const ActionInfo& actionInfo;

  /** Specifies if the head is blocked by a special action or walk.*/
  const bool headIsBlockedBySpecialActionOrWalk;

  /** Head joint angles that have to be set. */
  HeadMotionRequest& headMotionRequest;
};


class HeadControl : public Module, public HeadControlInterfaces
{
public:
/** Constructor.
* @param interfaces The paramters of the HeadControl module.
  */
  HeadControl(const HeadControlInterfaces& interfaces)
    : HeadControlInterfaces(interfaces){}
  
  /** Destructor */
  virtual ~HeadControl() {}
};

#endif 