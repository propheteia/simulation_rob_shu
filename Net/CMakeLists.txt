INCLUDE_DIRECTORIES(	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

ADD_DEFINITIONS( -DHAVE_CONFIG_H )

SET(	Net_LIB_HDRS
addr.hpp       handler.hpp         Net.h              socketstreambuf.hpp
config.h       iosocketstream.hpp  osocketstream.hpp  tcpsocket.hpp
exception.hpp  isocketstream.hpp   socket.hpp         udpsocket.hpp
)

SET(	Net_LIB_SRCS
addr.cpp       handler.cpp  socket.cpp     udpsocket.cpp
exception.cpp  Net.cpp      tcpsocket.cpp
)

ADD_LIBRARY(Net ${Net_LIB_SRCS} ${Net_LIB_HDRS})
