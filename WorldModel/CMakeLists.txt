INCLUDE_DIRECTORIES(	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	WorldModel_LIB_HDRS
object.h	ball.h        convexhullmanager.h  fieldinfo.h  self.h    newBall.h    newDynamicObject.h
convexhull.h  dynamicobject.h      player.h     worldmodel.h     drawing.h  rvdraw.h
)

SET(	WorldModel_LIB_SRCS
ball.cpp        convexhullmanager.cpp  player.cpp  worldmodel.cpp  newBall.cpp newDynamicObject.cpp
convexhull.cpp  fieldinfo.cpp          self.cpp     drawing.cpp  rvdraw.cpp
)

ADD_LIBRARY(WorldModel ${WorldModel_LIB_SRCS} ${WorldModel_LIB_HDRS})
