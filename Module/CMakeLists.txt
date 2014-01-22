INCLUDE_DIRECTORIES( 	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	Module_LIB_HDRS
Module.h  ModuleHandler.h  ModuleSelector.h  SolutionRequest.h
)

SET(	Module_LIB_SRCS
ModuleHandler.cpp  ModuleSelector.cpp  SolutionRequest.cpp
)

ADD_LIBRARY(Module ${Module_LIB_SRCS} ${Module_LIB_HDRS})
