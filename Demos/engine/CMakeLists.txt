message(STATUS "  Building graphics-engine library")

# Create GRAPHICS_ENGINE Lib
set(GRAPHICS_ENGINE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/asset_manager/shaderManager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/asset_manager/tgaLoader.c
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/framebufferobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/rendering.c
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/shading.c
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/viewport.c
    ${CMAKE_CURRENT_SOURCE_DIR}/scenegraph/camera.c
    ${CMAKE_CURRENT_SOURCE_DIR}/scenegraph/mesh.c
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities/profiler.c
)

add_library(GRAPHICS_ENGINE SHARED ${GRAPHICS_ENGINE_SOURCES})
