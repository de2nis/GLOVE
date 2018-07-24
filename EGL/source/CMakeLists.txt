message(STATUS "  Building EGL Library")

# Sets the SOURCES variable to contain all the source files needed by
# EGL shared lib to be built.
set(SOURCES
    api/eglContext.cpp
    api/eglConfig.cpp
    api/egl.cpp
    api/eglSurface.cpp
    display/displayDriver.cpp
    display/displayDriversContainer.cpp
    thread/renderingThread.cpp
    platform/platformFactory.cpp
    platform/vulkan/WSIPlaneDisplay.cpp
    platform/vulkan/vulkanWindowInterface.cpp
    platform/vulkan/vulkanWSI.cpp
    platform/vulkan/vulkanAPI.cpp
    rendering_api/rendering_api.c
    utils/eglLogger.cpp
)

# Sets the LIBS variable to contain the libraries that GLESv2 shared library
# will need to be linked against to.
set(LIBS
    ${Vulkan_LIBRARY}
)

find_package(X11)
if(X11_FOUND)
    find_package(ECM REQUIRED NO_MODULE)
    ecm_use_find_modules(DIR "${CMAKE_SOURCE_DIR}/CMake"
                         MODULES FindXCB.cmake)
    find_package(XCB REQUIRED XCB)

    set(SOURCES ${SOURCES} platform/vulkan/WSIXcb.cpp)

    set(LIBS ${LIBS} X11-xcb)
else()
    # Take action for other platforms
    message(FATAL_ERROR "Implement EGL for your platform")
endif()

include_directories(${CMAKE_SOURCE_DIR}/EGL/source
                    ${CMAKE_SOURCE_DIR}/EGL/include
                    ${CMAKE_SOURCE_DIR}/GLES/include
                    ${Vulkan_INCLUDE_DIR}
                    ${CMAKE_INSTALL_FULL_INCLUDEDIR})

link_directories(${CMAKE_INSTALL_FULL_LIBDIR})

add_library(EGL SHARED ${SOURCES})
set_target_properties(EGL PROPERTIES POSITION_INDEPENDENT_CODE ON
                                     LINK_FLAGS "-Wl,-Bsymbolic"
                                     INSTALL_RPATH_USE_LINK_PATH TRUE
                                     INSTALL_RPATH "\$ORIGIN")
target_link_libraries(EGL ${LIBS})

install(TARGETS EGL LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
