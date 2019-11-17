add_executable(scrolling_and_the_matrix_stack
    src/lopengl.hpp
    src/lutil.cpp
    src/lutil.hpp
    src/scrolling_and_the_matrix_stack.cpp)

target_include_directories(scrolling_and_the_matrix_stack
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_compile_features(scrolling_and_the_matrix_stack
PRIVATE
    cxx_std_17)

target_compile_options(scrolling_and_the_matrix_stack
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

target_link_libraries(scrolling_and_the_matrix_stack
PRIVATE
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU)
