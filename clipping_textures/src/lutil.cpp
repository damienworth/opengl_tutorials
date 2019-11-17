#include "lutil.hpp"

#include <array>
#include <cstring>
#include <gsl/gsl_util>

#include <IL/il.h>
#include <IL/ilu.h>

#include "lrect.hpp"
#include "ltexture.hpp"

namespace {

// sprite texture
static ltexture g_arrow_texture;

// sprite area
static std::array<lfrect, 4> g_arrow_clips;

} // namespace

bool
initGL()
{
    // set the viewport
    glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);

    // initialize projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

    // initialize model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // enable texturing
    glEnable(GL_TEXTURE_2D);

    // check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "error initializing OpenGL! " << gluErrorString(error)
                  << '\n';
        return false;
    }

    // initialize DevIL
    ilInit();
    ilClearColour(0xff, 0xff, 0xff, 0xff);

    // check for error
    ILenum il_error = ilGetError();
    if (il_error != IL_NO_ERROR) {
        std::cerr << "error initializing DevIL! " << iluErrorString(il_error)
                  << '\n';
        return false;
    }

    return true;
}

bool
load_media(std::string_view path)
{
    // set clip rectangles
    g_arrow_clips = {lfrect{0.f, 0.f, 128.f, 128.f},
                     lfrect{128.f, 0.f, 128.f, 128.f},
                     lfrect{0.f, 128.f, 128.f, 128.f},
                     lfrect{128.f, 128.f, 128.f, 128.f}};

    // load texture
    if (!g_arrow_texture.load_from_file(path)) {
        std::cerr << "unable to load file texture\n";
        return false;
    }

    return true;
}

void
update()
{
    // TBD
}

void
render()
{
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // render arrows
    g_arrow_texture.render({0.f, 0.f}, g_arrow_clips[0]);
    g_arrow_texture.render(
        {SCREEN_WIDTH - g_arrow_clips[1][2], 0.f}, g_arrow_clips[1]);
    g_arrow_texture.render(
        {0.f, SCREEN_HEIGHT - g_arrow_clips[2][2]}, g_arrow_clips[2]);
    g_arrow_texture.render(
        {SCREEN_WIDTH - g_arrow_clips[3][2],
         SCREEN_HEIGHT - g_arrow_clips[3][3]},
        g_arrow_clips[3]);

    // update screen
    glutSwapBuffers();
}
