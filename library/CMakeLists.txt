cmake_minimum_required(VERSION 3.5)
project(wplexserver)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME}
        wplexserver/ApiServer.hpp
        wplexserver/ApiServer.cpp
        wplexserver/RequestParser.cpp
        wplexserver/RequestParser.hpp)

# third_party
add_subdirectory(wplexserver/third_party/workerplex)
include_directories(wplexserver/third_party/workerplex/library)

# Boost library
find_package(Boost 1.58.0 REQUIRED COMPONENTS thread)

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE workerplex restbed ${Boost_LIBRARIES})
