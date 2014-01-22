INCLUDE_DIRECTORIES(	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	RobotModel_LIB_HDRS
nao.h  robot.h
)

SET(	RobotModel_LIB_SRCS
nao.cpp  robot.cpp
)

ADD_LIBRARY(RobotModel ${RobotModel_LIB_SRCS} ${RobotModel_LIB_HDRS})
