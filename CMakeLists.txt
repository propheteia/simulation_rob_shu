PROJECT( Strive3D )

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

ADD_SUBDIRECTORY( Action )
ADD_SUBDIRECTORY( AdvancedAgent )
ADD_SUBDIRECTORY( BehaviorControl )
ADD_SUBDIRECTORY( Geometry )
ADD_SUBDIRECTORY( HeadControl )
ADD_SUBDIRECTORY( Module )
ADD_SUBDIRECTORY( Net )
ADD_SUBDIRECTORY( Parser )
ADD_SUBDIRECTORY( RobotModel )
ADD_SUBDIRECTORY( salt )
ADD_SUBDIRECTORY( Trainer_Client )
ADD_SUBDIRECTORY( Utilities )
ADD_SUBDIRECTORY( WorldModel )
ADD_SUBDIRECTORY( Xabsl ) 


INCLUDE_DIRECTORIES(	./ ./Action ./AdvancedAgent ./BehaviorControl 
			./Geometry ./HeadControl ./Module ./Net
			./Parser ./RobotModel ./salt ./Trainer_Client 
			./Utilities ./WorldModel ./Xabsl 
)

SET( INCLUDE_LIB 
Action AdvancedAgent BehaviorControl Geometry HeadControl Module
Net Parser RobotModel salt Trainer_Client Utilities WorldModel Xabsl xml2 
)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

ADD_EXECUTABLE( strive3d ${DIR_SRCS} )

TARGET_LINK_LIBRARIES( strive3d ${INCLUDE_LIB}  )

SET( CMAKE_INSTALL_PREFIX .)

INSTALL( DIRECTORY Configs Logs
	 DESTINATION . )


