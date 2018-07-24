message(STATUS "Building Demos")

# --- ENGINE MACROS ---
# Enable (A) OpenGL ES API and (B) Asset Management errors debug
set(DEBUG               "-DDEBUG_OPENGLES -DDEBUG_ASSET_MANAGEMENT")

# Enable GPU Time information
set(PROFILE             "-DFPS_DISPLAY -DFPS_TIME_PERIOD=1")

# --- DEMOS MACROS ---
# Enable (A) auto terminate application after X seconds and (B) print info display.
set(CONFIG              "-DKILL_APP_PERIOD=10 -DINFO_DISPLAY")
# Use it to load a Precomplied Shader Program (Binary Format)
set(BINARY_PROG         "-DBINARY_PROG")
# Set the Window Size
set(WINDOW_SIZE         "-DWIDTH=600 -DHEIGHT=600")

# Set c/cpp flag definitions for the compiler.
set(C_REDUCE_ERRORS     "-Wno-unused-variable -Wno-unused-result")
set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS} ${C_REDUCE_ERRORS} ${DEBUG} ${PROFILE} ${CONFIG} ${WINDOW_SIZE}")

# Sets the libs variable to contain the libraries that the examples will
# need to be linked against to.
set(LIBS
    m
    X11
    X11-xcb
    xcb
    EGL
    GLESv2
)

include_directories(${CMAKE_SOURCE_DIR}/EGL/include
                    ${CMAKE_SOURCE_DIR}/GLES/include)

link_directories(${CMAKE_BINARY_DIR}/EGL
                 ${CMAKE_BINARY_DIR}/GLES
                 ${CMAKE_INSTALL_FULL_LIBDIR})

add_subdirectory(assets)
add_subdirectory(eglut)
add_subdirectory(engine)
add_subdirectory(tools)
add_subdirectory(demos)
