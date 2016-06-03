PROJECT(TennisBallDetection)
find_package(OpenCV REQUIRED )

SET(TennisBallDetection_SRCS
	main.cpp
)



INCLUDE_DIRECTORIES(
	${CMAKE_BINARY_DIR}
	${OpenCV_INCLUDE_DIRS}/include
  )
  
  
LINK_DIRECTORIES(
				${OpenCV_LIBRARY_DIRS}
				)
  
SET( DEPS
	
	${OpenCV_LIBS}
	)
	
ADD_EXECUTABLE(TennisBallDetection
  ${TennisBallDetection_SRCS} 
  )

TARGET_LINK_LIBRARIES(TennisBallDetection ${DEPS} )





