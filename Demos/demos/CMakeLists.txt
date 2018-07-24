message(STATUS "  Building demos")

set(DEMOS
    triangle2d_one_color
    triangle2d_split_colors
    circle2d_sdf
    texture2d_color
    cube3d_vertexcolors
    cube3d_textures
    render_to_texture_filter_gamma
    render_to_texture_filter_invert
    render_to_texture_filter_grayscale
    render_to_texture_filter_sobel
    render_to_texture_filter_boxblur
)

foreach(example ${DEMOS})
    set_property(SOURCE ${example}.c APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_BINARY_DIR}/Demos/assets/shaders ${CMAKE_BINARY_DIR}/Demos/assets/textures)
    add_executable(${example} ${example}.c)
    target_link_libraries(${example} GRAPHICS_ENGINE EGLUT ${LIBS})
    add_dependencies(${example} GLESv2 EGL)
endforeach()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run_all_samples.sh ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
