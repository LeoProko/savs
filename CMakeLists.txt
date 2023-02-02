cmake_policy(SET CMP0077 NEW)
cmake_minimum_required(VERSION 3.10)
project(
  savs 
  VERSION 0.0.0
  DESCRIPTION "simple and very secure"
  LANGUAGES CXX
)

include(FetchContent)
include(cmake/flags.cmake)
include(cmake/find_yaml-cpp.cmake)
include(cmake/find_glog.cmake)
include(cmake/savs.cmake)


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${PROJECT_NAME}_lib
  yaml-cpp
  glog::glog
)
