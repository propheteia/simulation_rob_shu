INCLUDE_DIRECTORIES( 	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	HeadControl_LIB_HDRS
HeadControl.h          HeadMotionRequest.h             NaoHeadControl.h
HeadControlMode.h      HeadPathPlanner.h
HeadControlSelector.h  NaoHeadControlBasicBehaviors.h
)

SET(	HeadControl_LIB_SRCS
HeadControlMode.cpp    HeadPathPlanner.cpp               NaoHeadControl.cpp
HeadMotionRequest.cpp  NaoHeadControlBasicBehaviors.cpp
)

ADD_LIBRARY(HeadControl ${HeadControl_LIB_SRCS} ${HeadControl_LIB_HDRS})
