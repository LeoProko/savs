set(WITH_GFLAGS off)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

FetchContent_Declare(
        glog
        GIT_REPOSITORY https://github.com/google/glog.git
        GIT_TAG        v0.6.0
        CMAKE_ARGS -DBUILD_TESTING=OFF
)

FetchContent_MakeAvailable(glog)
