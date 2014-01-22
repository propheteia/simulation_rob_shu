INCLUDE_DIRECTORIES( 	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	AdvancedAgent_LIB_HDRS
AdvancedAgent.h  GoaliePlan.h  PathPlanning.h  PlanManager.h
DribblePlan.h    GoalPlan.h    Plan.h          PositioningPlan.h
)

SET(	AdvancedAgent_LIB_SRCS
AdvancedAgent.cpp  GoaliePlan.cpp  PathPlanning.cpp  PositioningPlan.cpp
DribblePlan.cpp    GoalPlan.cpp    PlanManager.cpp
)

ADD_LIBRARY(AdvancedAgent ${AdvancedAgent_LIB_SRCS} ${AdvancedAgent_LIB_HDRS})
