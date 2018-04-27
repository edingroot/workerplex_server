cmake_minimum_required(VERSION 3.8)
project(workerplex_server)

# Use C++14
set(CMAKE_CXX_STANDARD 14)

# workerplex library
add_subdirectory(third_party/workerplex/lib)

# Boost library
find_package(Boost 1.58.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(THIRD_PARTY_INCLUDES
        third_party/workerplex/lib)

set(SOURCE_FILES
        src/main.cpp
        src/apiserver/ApiServer.hpp
        src/apiserver/ApiServer.cpp
        src/apiserver/RequestParser.cpp
        src/apiserver/RequestParser.hpp
        src/workers/pingpong/PingPong.hpp
        src/workers/pingpong/PingPong.cpp
        src/workers/longtask/LongTask.hpp
        src/workers/longtask/LongTask.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_INCLUDES})
target_link_libraries(${PROJECT_NAME} workerplex restbed ${Boost_LIBRARIES})
