INCLUDE_DIRECTORIES(	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	Trainer_Client_LIB_HDRS
common.h  control.h  trainer_client.h
)

SET(	Trainer_Client_LIB_SRCS
common.cpp  control.cpp  trainer_client.cpp
)

ADD_LIBRARY(Trainer_Client ${Trainer_Client_LIB_SRCS} ${Trainer_Client_LIB_HDRS})
