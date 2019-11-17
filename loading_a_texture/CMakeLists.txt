add_executable(loading_a_texture
    src/lopengl.hpp
    src/ltexture.cpp
    src/ltexture.hpp
    src/lutil.cpp
    src/lutil.hpp
    src/loading_a_texture.cpp)

target_include_directories(loading_a_texture
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    ${IL_INCLUDE_DIR})

target_compile_features(loading_a_texture
PRIVATE
    cxx_std_17)

target_compile_options(loading_a_texture
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

target_compile_definitions(loading_a_texture
PRIVATE
    GSL_TERMINATE_ON_CONTRACT_VIOLATION)

target_link_libraries(loading_a_texture
PRIVATE
    stdc++fs
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${ILUT_LIBRARIES})
