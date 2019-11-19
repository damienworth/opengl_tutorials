add_executable(color_keying_and_blending
    src/lopengl.hpp
    src/lrect.hpp
    src/ltexture.cpp
    src/ltexture.hpp
    src/lutil.cpp
    src/lutil.hpp
    src/main.cpp)

target_include_directories(color_keying_and_blending
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    ${IL_INCLUDE_DIR})

target_compile_features(color_keying_and_blending
PRIVATE
    cxx_std_17)

target_compile_options(color_keying_and_blending
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

target_compile_definitions(color_keying_and_blending
PRIVATE
    GSL_TERMINATE_ON_CONTRACT_VIOLATION)

target_link_libraries(color_keying_and_blending
PRIVATE
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${ILUT_LIBRARIES})
