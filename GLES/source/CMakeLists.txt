message(STATUS "  Building GLESv2 Library")

# Sets the SOURCES variable to contain all the source files needed by
# GLESv2 shared lib to be built.
set(SOURCES
    api/gl.cpp
    api/eglInterface.cpp
    context/context.cpp
    context/contextBufferObject
    context/contextFrameBuffer.cpp
    context/contextRenderBuffer.cpp
    context/contextRendering.cpp
    context/contextShader.cpp
    context/contextShaderProgram.cpp
    context/contextSpecialFunctions.cpp
    context/contextStateFragmentOperations.cpp
    context/contextStateFramebufferOperations.cpp
    context/contextStateManager.cpp
    context/contextStatePixelOperations.cpp
    context/contextStateQueries.cpp
    context/contextStateRasterization.cpp
    context/contextStateViewportTransformation.cpp
    context/contextTexture.cpp
    context/contextUtilities.cpp
    context/contextVertexAttributes.cpp
    glslang/glslangCompiler.cpp
    glslang/glslangLinker.cpp
    glslang/glslangIoMapResolver.cpp
    glslang/glslangShaderCompiler.cpp
    glslang/shaderConverter.cpp
    resources/attachment.cpp
    resources/bufferObject.cpp
    resources/framebuffer.cpp
    resources/genericVertexAttributes.cpp
    resources/resourceManager.cpp
    resources/renderbuffer.cpp
    resources/shader.cpp
    resources/shaderProgram.cpp
    resources/shaderReflection.cpp
    resources/shaderResourceInterface.cpp
    resources/texture.cpp
    resources/rect.cpp
    resources/sampler.cpp
    state/stateManager.cpp
    state/stateActiveObjects.cpp
    state/stateInputAssembly.cpp
    state/stateFragmentOperations.cpp
    state/stateFramebufferOperations.cpp
    state/statePixelStorage.cpp
    state/stateRasterization.cpp
    state/stateHintAspects.cpp
    state/stateViewportTransformation.cpp
    utils/GlToVkConverter.cpp
    utils/parser_helpers.cpp
    utils/VkToGlConverter.cpp
    utils/glLogger.cpp
    utils/glUtils.cpp
    vulkan/cbManager.cpp
    vulkan/clearPass.cpp
    vulkan/renderPass.cpp
    vulkan/buffer.cpp
    vulkan/memory.cpp
    vulkan/sampler.cpp
    vulkan/image.cpp
    vulkan/imageView.cpp
    vulkan/pipeline.cpp
    vulkan/framebuffer.cpp
    vulkan/context.cpp
    vulkan/utils.cpp
)

# Sets the LIBS variable to contain the libraries that our GLESv2 shared library
# will need to be linked against to.
set(LIBS
    ${Vulkan_LIBRARY}
    glslang
    SPIRV
    HLSL
    OGLCompiler
    OSDependent
    pthread
    m
)

# Where the .h files included in the source files can be found.
include_directories(${GLES_PATH}/source
                    ${GLES_PATH}/include
                    ${EGL_PATH}/include
                    ${GLSLANG_PATH}/include
                    ${Vulkan_INCLUDE_DIR}
                    ${CMAKE_INSTALL_FULL_INCLUDEDIR})

link_directories(${GLSLANG_PATH}/lib
                 ${CMAKE_INSTALL_FULL_LIBDIR})

# Ensure we re-link if our external libraries change.
add_library(glslang STATIC IMPORTED)
set_target_properties(glslang PROPERTIES IMPORTED_LOCATION ${GLSLANG_PATH}/lib/libglslang.a)

add_library(SPIRV STATIC IMPORTED)
set_target_properties(SPIRV PROPERTIES IMPORTED_LOCATION ${GLSLANG_PATH}/lib/libSPIRV.a)

add_library(HLSL STATIC IMPORTED)
set_target_properties(HLSL PROPERTIES IMPORTED_LOCATION ${GLSLANG_PATH}/lib/libHLSL.a)

add_library(OGLCompiler STATIC IMPORTED)
set_target_properties(OGLCompiler PROPERTIES IMPORTED_LOCATION ${GLSLANG_PATH}/lib/libOGLCompiler.a)

add_library(OSDependent STATIC IMPORTED)
set_target_properties(OSDependent PROPERTIES IMPORTED_LOCATION ${GLSLANG_PATH}/lib/libOSDependent.a)

add_library(GLESv2 SHARED ${SOURCES})
set_target_properties(GLESv2 PROPERTIES POSITION_INDEPENDENT_CODE ON
                                        LINK_FLAGS "-Wl,-Bsymbolic"
                                        INSTALL_RPATH_USE_LINK_PATH TRUE
                                        INSTALL_RPATH "\$ORIGIN")
target_link_libraries(GLESv2 ${LIBS})

install(TARGETS GLESv2 LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
