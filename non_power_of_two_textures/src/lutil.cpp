#include "lutil.hpp"

#include <array>
#include <cstring>
#include <gsl/gsl_util>

#include <IL/il.h>
#include <IL/ilu.h>

#include "lrect.hpp"
#include "ltexture.hpp"
#include "macro_helpers.hpp"

namespace {

static ltexture g_non_2n_texture;

}

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
    iluInit();
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
    // load texture
    if (!g_non_2n_texture.load_from_file(path)) {
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

    // render texture
    g_non_2n_texture.render({0.f, 0.f}/*{gsl::narrow_cast<GLfloat>(SCREEN_WIDTH - Wv(g_non_2n_texture.get_dimensions())),
        gsl::narrow_cast<GLfloat>(SCREEN_HEIGHT - Hv(g_non_2n_texture.get_dimensions()))}*/);

    // update screen
    glutSwapBuffers();
}
