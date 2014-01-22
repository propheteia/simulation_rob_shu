INCLUDE_DIRECTORIES( 	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	Action_LIB_HDRS
ActionControl.h        KickEngineSelector.h       PIDController.h
ActionInfo.h           NaoActionControl.h         SpecialAction.h
ActionRequest.h        NaoDynamicKick.h           SpecialActionSelector.h
ActionSelector.h       Strive3DActionControl.h
DebugActionControl.h   NaoGetUp.h                 Strive3DGetUp.h
GetUpEngine.h          NaoKick.h                  WalkingEngine.h
GetUpEngineSelector.h  NaoSpecialAction.h         WalkingEngineSelector.h
KickEngine.h           NaoWalkingEngine.h
)

SET(	Action_LIB_SRCS
ActionRequest.cpp     NaoWalkingEngine.cpp
KickEngine.cpp        NaoGetUp.cpp                 SpecialAction.cpp
NaoActionControl.cpp  NaoKick.cpp                  Strive3DActionControl.cpp
NaoDynamicKick.cpp    NaoSpecialAction.cpp         Strive3DGetUp.cpp

)

ADD_LIBRARY(Action ${Action_LIB_SRCS} ${Action_LIB_HDRS})
