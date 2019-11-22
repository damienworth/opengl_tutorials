cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

project(opengl_tutorials VERSION 0.1.0 LANGUAGES CXX)

include(FindGLUT)

if (NOT GLUT_FOUND)
    message(FATAL_ERROR "couldn't find freeglut")
endif (NOT GLUT_FOUND)

include(FindOpenGL)
if (NOT OPENGL_FOUND)
    message(FATAL_ERROR "couldn't find opengl")
endif (NOT OPENGL_FOUND)

if (NOT OPENGL_GLU_FOUND)
    message(FATAL_ERROR "couldn't find GLU")
endif (NOT OPENGL_GLU_FOUND)

include(FindDevIL)
if (NOT DevIL_FOUND)
    message(FATAL_ERROR "couldn't find DevIL library")
endif (NOT DevIL_FOUND)

add_subdirectory(clipping_textures EXCLUDE_FROM_ALL)
add_subdirectory(color_keying_and_blending EXCLUDE_FROM_ALL)
add_subdirectory(loading_a_texture EXCLUDE_FROM_ALL)
add_subdirectory(matrices_and_coloring_polygons EXCLUDE_FROM_ALL)
add_subdirectory(non_power_of_two_textures EXCLUDE_FROM_ALL)
add_subdirectory(polygon EXCLUDE_FROM_ALL)
add_subdirectory(scrolling_and_the_matrix_stack EXCLUDE_FROM_ALL)
add_subdirectory(texture_mapping_and_pixel_manipulation EXCLUDE_FROM_ALL)
add_subdirectory(the_viewport EXCLUDE_FROM_ALL)
add_subdirectory(updating_textures EXCLUDE_FROM_ALL)
