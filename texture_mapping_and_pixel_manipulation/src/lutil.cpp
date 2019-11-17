#include "lutil.hpp"

#include <array>
#include <cstring>
#include <gsl/gsl_util>

#include "ltexture.hpp"

namespace {

static ltexture g_checker_board_texture;

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

    return true;
}

bool
load_media()
{
    // checkboard pixels
    constexpr unsigned int CHECKBOARD_WIDTH  = 128;
    constexpr unsigned int CHECKBOARD_HEIGHT = 128;
    constexpr int CHECKBOARD_PIXEL_COUNT = CHECKBOARD_WIDTH * CHECKBOARD_HEIGHT;
    std::array<GLuint, CHECKBOARD_PIXEL_COUNT> checker_board;

    for (auto i = 0; i != CHECKBOARD_PIXEL_COUNT; ++i) {
        auto colors = reinterpret_cast<GLubyte*>(&checker_board[i]);

        /*
        The fancy stuff in "if" condition is made to generate checker board
        texture. Colors used are white and red, so we'll have a red'n'white
        checker board.
        */

        // if the 5th bit of the x and y offsets of the pixel do not match
        if (((((i / 128) & 16) ^ i) % 128) & 16) {
            // set pixel to white
            std::memset(colors, 0xff, 4u);
        } else {
            // set pixel to red
            const auto red = std::array<GLubyte, 4>{0xff, 0u, 0u, 0xff};
            std::memcpy(colors, &red[0], red.size());
        }
    }

    // load texture
    if (!g_checker_board_texture.load_from_pixels32(
            &checker_board[0],
            std::array{CHECKBOARD_WIDTH, CHECKBOARD_HEIGHT})) {
        std::cerr << "unable to load checkerboard texture\n";
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

    // calculate centered offsets
    const std::array<GLfloat, 2> xy = {
        gsl::narrow<GLfloat>(
            SCREEN_WIDTH - g_checker_board_texture.get_dimensions()[0]) /
            2.f,
        gsl::narrow<GLfloat>(
            SCREEN_HEIGHT - g_checker_board_texture.get_dimensions()[1]) /
            2.f};

    // render checkerboard texture
    g_checker_board_texture.render(xy);

    // update screen
    glutSwapBuffers();
}
