add_executable(texture_mapping_and_pixel_manipulation
    src/lopengl.hpp
    src/ltexture.cpp
    src/ltexture.hpp
    src/lutil.cpp
    src/lutil.hpp
    src/texture_mapping_and_pixel_manipulation.cpp)

target_include_directories(texture_mapping_and_pixel_manipulation
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_compile_features(texture_mapping_and_pixel_manipulation
PRIVATE
    cxx_std_17)

target_compile_options(texture_mapping_and_pixel_manipulation
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

target_link_libraries(texture_mapping_and_pixel_manipulation
PRIVATE
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU)
