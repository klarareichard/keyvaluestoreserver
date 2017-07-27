

cmake_minimum_required(VERSION 3.2.2)
project(hwserver)

## use this to globally use C++11 with in our project
set(CMAKE_CXX_STANDARD 11)

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.h
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

#find_package( Boost 1.40 COMPONENTS program_options thread system filesystem REQUIRED )

#find Gtest 

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
#add_compile_options(-std=c++11 -O3 -DNDEBUG)
add_compile_options(-std=c++11 -ggdb)
add_compile_options(-pthread -openmp)
add_compile_options(-zmq)

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -frounding-math -fPIC  -std=c++11 -O3 -DNDEBUG -march=native -fopenmp")


add_executable(tests tests.cpp ${GTEST_INCLUDE_DIRS})
target_include_directories(tests PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(tests PUBLIC ${GTEST_BOTH_LIBRARIES} pthread ${ZeroMQ_LIBRARY})
add_test(AllTestsInTests tests)

add_executable(server Server.cpp)
target_include_directories(server PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(server PUBLIC ${ZeroMQ_LIBRARY})

add_executable(client Client.cpp)
target_include_directories(client PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(client PUBLIC ${ZeroMQ_LIBRARY})