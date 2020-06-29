cmake_minimum_required(VERSION  2.8)
project(coredumper C CXX)

# We do not wish to work without threading support
# TODO: remove related ifdefs
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(COREDUMPER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(src)
IF (NOT DEFINED BUILD_TESTING)
  MESSAGE(STATUS "Coredumper - BUILD_TESTING has not been defined")
  include(CTest)
ELSE ()
  MESSAGE(STATUS "Coredumper - BUILD_TESTING has been defined")
ENDIF()
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
