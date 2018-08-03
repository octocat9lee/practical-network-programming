cmake_minimum_required(VERSION 2.6)

SET(CMAKE_CXX_COMPILER "g++")
SET(CXX_FLAGS
 -g
 -Wall
)

STRING(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(BOOST_LIBS libboost_chrono.so libboost_context.so libboost_coroutine.so 
	libboost_system.so libboost_thread.so libboost_system.so 
	libboost_locale.so libboost_filesystem.so)

INCLUDE_DIRECTORIES(/home/zhoul/install/boost_1_59_0/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
LINK_DIRECTORIES(/home/zhoul/install/boost_1_59_0/lib)

ADD_EXECUTABLE(connection_1 connection_1.cpp)
TARGET_LINK_LIBRARIES(connection_1 pthread ${BOOST_LIBS})

ADD_EXECUTABLE(connection_2 connection_2.cpp)
TARGET_LINK_LIBRARIES(connection_2 pthread ${BOOST_LIBS})

ADD_EXECUTABLE(connection_5 connection_5.cpp Connection.cpp Connections.cpp)
TARGET_LINK_LIBRARIES(connection_5 pthread ${BOOST_LIBS})
