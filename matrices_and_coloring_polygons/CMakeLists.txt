add_executable(matrices_and_coloring_polygons
    src/lopengl.hpp
    src/lutil.cpp
    src/lutil.hpp
    src/main.cpp)

target_include_directories(matrices_and_coloring_polygons
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_compile_features(matrices_and_coloring_polygons
PRIVATE
    cxx_std_17)

target_compile_options(matrices_and_coloring_polygons
PRIVATE
    -Wall
    -Werror
    -Wextra
    -pedantic
    -Wconversion
    -Winit-self
    -Woverloaded-virtual
    -Wunreachable-code
    -Wold-style-cast
    -Wsign-promo
    -Wshadow
    -fno-exceptions)

target_link_libraries(matrices_and_coloring_polygons
PRIVATE
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU)
