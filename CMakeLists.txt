
cmake_minimum_required(VERSION 2.8.12)
project(webserver)

#find_package( Boost 1.40 COMPONENTS program_options thread system filesystem REQUIRED )

#find Gtest 

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
#add_compile_options(-std=c++11 -O3 -DNDEBUG)
add_compile_options(-std=c++11 -ggdb)
add_compile_options(-pthread)
add_compile_options(-lzmq)

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -frounding-math -fPIC  -std=c++11 -O3 -DNDEBUG -march=native -fopenmp")


add_executable(tests tests.cpp ${GTEST_INCLUDE_DIRS})
target_link_libraries(tests ${GTEST_BOTH_LIBRARIES} pthread)
add_test(AllTestsInTests tests)

add_executable(server Server.cpp)

add_executable(client Client.cpp)