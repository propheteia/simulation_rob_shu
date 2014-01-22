INCLUDE_DIRECTORIES(	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	Xabsl_LIB_HDRS
XabslAction.h             XabslDecimalExpression.h     XabslState.h
XabslAgent.h              XabslEngineExecutor.h        XabslStatement.h
XabslArray.h              XabslEngine.h                XabslSymbols.h
XabslBasicBehavior.h      XabslEnumeratedExpression.h  XabslTools.h
XabslBehavior.h           XabslOption.h
XabslBooleanExpression.h  XabslParameters.h
)

SET(	Xabsl_LIB_SRCS
XabslAction.cpp             XabslEngineExecutor.cpp        XabslStatement.cpp
XabslAgent.cpp              XabslEnumeratedExpression.cpp  XabslSymbols.cpp
XabslBooleanExpression.cpp  XabslOption.cpp                XabslTools.cpp
XabslDecimalExpression.cpp  XabslParameters.cpp
XabslEngine.cpp             XabslState.cpp
)

ADD_LIBRARY(Xabsl ${Xabsl_LIB_SRCS} ${Xabsl_LIB_HDRS})
