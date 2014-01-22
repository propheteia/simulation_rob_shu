INCLUDE_DIRECTORIES(	../ ../Action ../AdvancedAgent ../BehaviorControl 
			../Geometry ../HeadControl ../Module ../Net
			../Parser ../RobotModel ../salt ../Trainer_Client 
			../Utilities ../WorldModel ../Xabsl
 )

SET(	Parser_LIB_HDRS
cstring.h  faststack.h  malloc_util.h  Parser.h  sexp.h  sexp_ops.h
)

SET(	Parser_LIB_SRCS
cstring.c    io.c           parser.c    sexp.c
faststack.c  malloc_util.c  Parser.cpp  sexp_ops.c
)

ADD_LIBRARY(Parser ${Parser_LIB_SRCS} ${Parser_LIB_HDRS})
