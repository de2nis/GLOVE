########################################################################
# CMake build script for GLOVE
########################################################################

# Sets the minimum required version of cmake for a project.
# If the current version of CMake is lower than that required it will stop
# processing the project.
cmake_minimum_required(VERSION 2.8.12)

project(GLOVE)

include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected. Default: Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default: Release)" FORCE)
endif()

# Enables/Disables output of compile commands during generation.
# If enabled, generates a compile_commands.json file containing the exact
# compiler calls for all translation units of the project in machine-readable
# form.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

# Search for Vulkan library
if(VULKAN_LIBRARY)
    set(Vulkan_FOUND ON)
    set(Vulkan_LIBRARY "${VULKAN_LIBRARY}" CACHE PATH "" FORCE)
    if(VULKAN_INCLUDE_PATH)
        set(Vulkan_INCLUDE_DIR "${VULKAN_INCLUDE_PATH}" CACHE PATH "" FORCE)
    else()
        get_filename_component(VULKAN_INCLUDE_DIR "${VULKAN_LIBRARY}" DIRECTORY)
        set(Vulkan_INCLUDE_DIR "${VULKAN_INCLUDE_DIR}/../include" CACHE PATH "" FORCE)
    endif()
else()
    if(NOT CMAKE_VERSION VERSION_LESS 3.7.2)
        find_package(Vulkan)
    else()
        find_library(Vulkan_LIBRARY NAMES libvulkan.so libvulkan.so.1 HINTS ${CMAKE_INSTALL_FULL_LIBDIR})
        find_path(Vulkan_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS ${CMAKE_INSTALL_FULL_LIBDIR})
        if(Vulkan_LIBRARY)
            set(Vulkan_FOUND ON)
        endif()
    endif()
endif()
if(Vulkan_FOUND)
    message(STATUS "Found Vulkan: ${Vulkan_LIBRARY}")
else()
    message(FATAL_ERROR "Could not find Vulkan library: ${Vulkan_LIBRARY}")
endif()

option(TRACE_BUILD "Build GLOVE with debug logs enabled" OFF)
if(TRACE_BUILD)
    message(STATUS "Building GLOVE with debug logs enabled")
    add_definitions(-DTRACE_BUILD)
else()
    remove_definitions(-DTRACE_BUILD)
endif()

option(WORKAROUNDS "Enable GLOVE's workarounds" ON)
if(WORKAROUNDS)
    add_definitions(-DWORKAROUNDS)
else()
    message(STATUS "Disabling GLOVE's workarounds")
    remove_definitions(-DWORKAROUNDS)
endif()

# Set c/cpp flag definitions for the compiler.
set(C_REDUCE_ERRORS "-Wno-unused-parameter -Wno-unused-function")
set(CXX_REDUCE_ERRORS "-Wno-unused-parameter -Wno-unused-function")
set(PEDANTIC "-Wall -Wextra -Winline -Wreturn-type -Wuninitialized -Winit-self -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES -DGL_GLEXT_PROTOTYPES -std=c++11 ${PEDANTIC} ${CXX_REDUCE_ERRORS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES -DGL_GLEXT_PROTOTYPES -std=c99 ${PEDANTIC} ${C_REDUCE_ERRORS}")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")
endif()

# Recurse into the the following subdirectories. This does not actually cause
# another cmake executable to run. The same process will walk through the
# project's entire directory structure.
add_subdirectory(EGL)
add_subdirectory(GLES)
add_subdirectory(Demos)
