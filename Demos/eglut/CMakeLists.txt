message(STATUS "  Building eglut library")

# Create EGLUT Lib
set(EGLUT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/eglut.c
    ${CMAKE_CURRENT_SOURCE_DIR}/eglut_x11.c
)

add_library(EGLUT SHARED ${EGLUT_SOURCES})
