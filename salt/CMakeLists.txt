SET(	salt_LIB_HDRS
	bounds.h
   	frustum.h  
	matrix.h  
	random.h  
	tvector.h
	defines.h  
	gmath.h    
	plane.h   
	rect.h    
	vector.h
)

SET(	salt_LIB_SRCS
	bounds.cpp  
	frustum.cpp  
	matrix.cpp  
	plane.cpp
)

ADD_LIBRARY(salt ${salt_LIB_SRCS} ${salt_LIB_HDRS})
