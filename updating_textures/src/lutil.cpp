#include "lutil.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <gsl/gsl_util>
#include <limits>

#include <IL/il.h>
#include <IL/ilu.h>

#include "lrect.hpp"
#include "ltexture.hpp"
#include "macro_helpers.hpp"

namespace {

static ltexture g_circle_texture;

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
    if (!g_circle_texture.load_from_file(path)) {
        std::cerr << "unable to load file texture\n";
        return false;
    }

    // lock texture for modification
    g_circle_texture.lock();

    // calculate target color
    GLuint   target_color;
    GLubyte* colors = reinterpret_cast<GLubyte*>(&target_color);
    Rc(colors)      = 0x0;
    Gc(colors)      = 0xff;
    Bc(colors)      = 0xff;
    Ac(colors)      = 0xff;

    // replace target color with transparent black
    GLuint*     pixels      = g_circle_texture.data();
    const auto& dims        = g_circle_texture.get_dimensions();
    GLuint      pixel_count = Wv(dims) * Hv(dims);

    for (int i = 0; i != gsl::narrow<int>(pixel_count); ++i) {
        pixels[i] = pixels[i] == target_color ? 0x0 : pixels[i];
    }

    // diagonal lines
    for (int y = 0; y < gsl::narrow<int>(Hv(dims)); ++y) {
        for (int x = 0; x < gsl::narrow<int>(Wv(dims)); ++x) {
            if (y % 10 != x % 10) {
                g_circle_texture.set_pixel(
                    {gsl::narrow<GLuint>(x), gsl::narrow<GLuint>(y)}, 0);
            }
        }
    }

    // update texture
    g_circle_texture.unlock();

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

    const auto& dims = g_circle_texture.get_dimensions();

    // render texture
    g_circle_texture.render(
        {gsl::narrow<float>(SCREEN_WIDTH - Wv(dims)) / 2.f,
         gsl::narrow<float>(SCREEN_HEIGHT - Hv(dims)) / 2.f});

    // update screen
    glutSwapBuffers();
}
