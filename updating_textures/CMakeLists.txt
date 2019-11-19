add_executable(updating_textures
    src/lopengl.hpp
    src/lrect.hpp
    src/ltexture.cpp
    src/ltexture.hpp
    src/lutil.cpp
    src/lutil.hpp
    src/main.cpp)

target_include_directories(updating_textures
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    ${IL_INCLUDE_DIR})

target_compile_features(updating_textures
PRIVATE
    cxx_std_17)

target_compile_options(updating_textures
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

target_compile_definitions(updating_textures
PRIVATE
    GSL_TERMINATE_ON_CONTRACT_VIOLATION)

target_link_libraries(updating_textures
PRIVATE
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${ILUT_LIBRARIES})
