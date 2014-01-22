INCLUDE_DIRECTORIES( 	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl ../DataAnalyse
 )

SET(	BehaviorControl_LIB_HDRS
BehaviorControl.h          
BehaviorControlSelector.h  Strive3DBehaviorControl.h
)

SET(	BehaviorControl_LIB_SRCS
Strive3DBehaviorControl.cpp  
)

ADD_LIBRARY(BehaviorControl ${BehaviorControl_LIB_SRCS} ${BehaviorControl_LIB_HDRS})
