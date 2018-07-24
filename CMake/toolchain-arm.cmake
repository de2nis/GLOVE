set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSTEM_VERSION 1)

# where is the target environment
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT}/lib)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# search for libraries, headers and packages in the target and host directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

set(GTEST_PATH "${CMAKE_INSTALL_PREFIX}" CACHE PATH "" FORCE)
set(GLSLANG_PATH "${CMAKE_INSTALL_PREFIX}" CACHE PATH "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard")

set(THREADS_PTHREAD_ARG "2" CACHE STRING "Forcibly set by CMakeLists.txt." FORCE)
