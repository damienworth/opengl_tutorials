#ifndef LUTIL_HPP
#define LUTIL_HPP

#include "lopengl.hpp"

#include <string_view>

// screen constants
constexpr int SCREEN_WIDTH  = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int SCREEN_FPS    = 60;

/*
pre-conditions:
    * a valid OpenGL context
post-conditions:
    * initializes matrices and clear color
    * reports to console if there was an OpenGL error
    * returns false if there was an error in initialization
side-effects:
    * projection matrix is set to identity matrix
    * Modelview matrix is set to identity matrix
    * matrix mode is set to modelview
    * clear color is set to black
    * texturing is enabled
*/
bool initGL();

/*
pre-conditions:
    * a valid OpenGL context
post-conditions:
    * loads media to use in the program
    * reports to console if there was an error in loading the media
    * returns true if the media loaded successfully
side-effects: n/a
*/
bool load_media(std::string_view path);

/*
pre-conditions: n/a
post-conditions:
    * does per frame logic
side-effects: n/a
*/
void update();

/*
pre-conditions:
    * a valid OpenGL context
    * active modelview matrix
post-conditions:
    * renders the scene
side-effects:
    * clears the color buffer
    * swaps the front/back buffer
*/
void render();

#endif // LUTIL_HPP
