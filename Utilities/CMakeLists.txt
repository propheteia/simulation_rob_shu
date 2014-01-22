INCLUDE_DIRECTORIES(	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	Utilities_LIB_HDRS
Logger.h  MessageStorage.h  Range.h  StaticModule.h  Utilities.h  xmlParser.h
)

SET(	Utilities_LIB_SRCS
Logger.cpp  MessageStorage.cpp  Utilities.cpp  xmlParser.cpp
)

ADD_LIBRARY(Utilities ${Utilities_LIB_SRCS} ${Utilities_LIB_HDRS})
