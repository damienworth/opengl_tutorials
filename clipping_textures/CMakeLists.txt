add_executable(clipping_textures
    src/clipping_textures.cpp
    src/lopengl.hpp
    src/lrect.hpp
    src/ltexture.cpp
    src/ltexture.hpp
    src/lutil.cpp
    src/lutil.hpp)

target_include_directories(clipping_textures
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    ${IL_INCLUDE_DIR})

target_compile_features(clipping_textures
PRIVATE
    cxx_std_17)

target_compile_options(clipping_textures
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

target_compile_definitions(clipping_textures
PRIVATE
    GSL_TERMINATE_ON_CONTRACT_VIOLATION)

target_link_libraries(clipping_textures
PRIVATE
    stdc++fs
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${ILUT_LIBRARIES})
