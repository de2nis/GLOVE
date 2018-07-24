message(STATUS "  Building tools")

set(TOOLS
    offline_shader_compiler
)

foreach(tool ${TOOLS})
    set_property(SOURCE ${tool}.c APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_BINARY_DIR}/Demos/assets/shaders)
    add_executable(${tool} ${tool}.c)
    target_link_libraries(${tool} GRAPHICS_ENGINE EGLUT ${LIBS})
    add_dependencies(${tool} GLESv2 EGL)
endforeach()
