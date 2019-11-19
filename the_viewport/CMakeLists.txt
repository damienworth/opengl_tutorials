add_executable(the_viewport
    src/lopengl.hpp
    src/lutil.cpp
    src/lutil.hpp
    src/main.cpp)

target_include_directories(the_viewport
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_compile_features(the_viewport
PRIVATE
    cxx_std_17)

target_compile_options(the_viewport
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

target_link_libraries(the_viewport
PRIVATE
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU)
