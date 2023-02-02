file(GLOB_RECURSE LIB_CXX_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.hpp)

add_library(
    ${PROJECT_NAME}_lib
    ${LIB_CXX_SOURCES}
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
  ${PROJECT_SOURCE_DIR}/src/
)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC
  yaml-cpp
  glog::glog
)

get_target_property(GLOG_INCLUDES glog::glog INCLUDE_DIRECTORIES)
target_include_directories(${PROJECT_NAME}_lib SYSTEM PUBLIC
  ${GLOG_INCLUDES}
)
