add_executable(non_power_of_two_textures
    src/lopengl.hpp
    src/lrect.hpp
    src/ltexture.cpp
    src/ltexture.hpp
    src/lutil.cpp
    src/lutil.hpp
    src/non_power_of_two_textures.cpp)

target_include_directories(non_power_of_two_textures
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    ${IL_INCLUDE_DIR})

target_compile_features(non_power_of_two_textures
PRIVATE
    cxx_std_17)

target_compile_options(non_power_of_two_textures
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

target_compile_definitions(non_power_of_two_textures
PRIVATE
    GSL_TERMINATE_ON_CONTRACT_VIOLATION)

target_link_libraries(non_power_of_two_textures
PRIVATE
    GLUT::GLUT
    OpenGL::GL
    OpenGL::GLU
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${ILUT_LIBRARIES})
