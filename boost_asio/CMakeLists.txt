cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_COMPILER "g++")
set(CXX_FLAGS
 -g
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 -std=c++0x
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

INCLUDE_DIRECTORIES(/home/zhoul/install/boost_1_59_0/include)
LINK_DIRECTORIES(/home/zhoul/install/boost_1_59_0/lib)

ADD_EXECUTABLE(connection_1 connection_1.cpp)
TARGET_LINK_LIBRARIES(connection_1 pthread libboost_chrono.so libboost_context.so libboost_coroutine.so libboost_system.so libboost_thread.so libboost_system.so libboost_locale.so libboost_filesystem.so)
