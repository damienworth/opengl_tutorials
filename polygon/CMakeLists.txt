add_executable(polygon
    src/lopengl.hpp
    src/lutil.cpp
    src/lutil.hpp
    src/main.cpp)

target_include_directories(polygon
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_compile_features(polygon
PRIVATE
    cxx_std_17)

target_compile_options(polygon
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

target_link_libraries(polygon
PRIVATE
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU)
