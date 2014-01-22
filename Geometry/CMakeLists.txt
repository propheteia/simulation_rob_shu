INCLUDE_DIRECTORIES( 	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	Geometry_LIB_HDRS
geometry.h  spline.h  trajectory.h MatrixSpline.h
)

SET(	Geometry_LIB_SRCS
geometry.cpp  spline.cpp  trajectory.cpp MatrixSpline.cpp
)

ADD_LIBRARY(Geometry ${Geometry_LIB_SRCS} ${Geometry_LIB_HDRS})
