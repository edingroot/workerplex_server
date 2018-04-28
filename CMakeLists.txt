cmake_minimum_required(VERSION 3.5)
project(wplexserver-demo)

# third_party
add_subdirectory(library)
include_directories(library/wplexserver/third_party/workerplex/library)

# Boost library
find_package(Boost 1.58.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# If this is not a sub-project
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(CMAKE_CXX_STANDARD 14)

    set(SOURCE_FILES
            src/main.cpp
            src/workers/pingpong/PingPong.hpp
            src/workers/pingpong/PingPong.cpp
            src/workers/longtask/LongTask.hpp
            src/workers/longtask/LongTask.cpp)
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    target_include_directories(${PROJECT_NAME} PRIVATE library)
    target_link_libraries(${PROJECT_NAME} wplexserver restbed ${Boost_LIBRARIES})
endif()
