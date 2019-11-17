#include "lutil.hpp"

#include <array>

namespace {

static int g_viewport_mode = VIEWPORT_MODE_FULL;

inline void
draw_quad(const std::array<float, 3u>& color)
{
    // clang-format off
    glBegin(GL_QUADS);
        glColor3f(color[0], color[1], color[2]);
        glVertex2f(-SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f);
        glVertex2f( SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f);
        glVertex2f( SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f);
        glVertex2f(-SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f);
    glEnd();
    // clang-format on
}

inline void
double_quads(
    const std::array<float, 3u>& color1, const std::array<float, 3u>& color2)
{
    // clang-format off
    glBegin(GL_QUADS);
        glColor3f(color1[0], color1[1], color1[2]);
        glVertex2f(-SCREEN_WIDTH / 8.f, -SCREEN_HEIGHT / 8.f);
        glVertex2f( SCREEN_WIDTH / 8.f, -SCREEN_HEIGHT / 8.f);
        glVertex2f( SCREEN_WIDTH / 8.f,  SCREEN_HEIGHT / 8.f);
        glVertex2f(-SCREEN_WIDTH / 8.f,  SCREEN_HEIGHT / 8.f);
        glColor3f(color2[0], color2[1], color2[2]);
        glVertex2f(-SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f);
        glVertex2f( SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f);
        glVertex2f( SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f);
        glVertex2f(-SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f);
    glEnd();
    // clang-format off
}

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

    // check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "error initializing OpenGL! " << gluErrorString(error)
                  << '\n';
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

    // reset modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move to center of the screen
    glTranslatef(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f);

    // full view
    if (g_viewport_mode == VIEWPORT_MODE_FULL) {
        // fill the screen
        glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);
        draw_quad({1.f, 0.f, 0.f}); // red quad
    } else if (g_viewport_mode == VIEWPORT_MODE_HALF_CENTER) {
        // center viewport
        glViewport(
            SCREEN_WIDTH / 4.f,
            SCREEN_HEIGHT / 4.f,
            SCREEN_WIDTH / 2.f,
            SCREEN_HEIGHT / 2.f);
        draw_quad({0.f, 1.f, 0.f}); // green quad
        // clang-format on
    } else if (g_viewport_mode == VIEWPORT_MODE_HALF_TOP) {
        // viewport at top
        glViewport(
            SCREEN_WIDTH / 4.f,
            SCREEN_HEIGHT / 2.f,
            SCREEN_WIDTH / 2.f,
            SCREEN_HEIGHT / 2.f);
        draw_quad({0.f, 0.f, 1.f}); // blue quad
    }                               /* four viewports at the same time */
    else if (g_viewport_mode == VIEWPORT_MODE_QUAD) {
        // bottom left red quad
        glViewport(0.f, 0.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
        draw_quad({1.f, 0.f, 0.f});

        // bottom right green quad
        glViewport(
            SCREEN_WIDTH / 2.f, 0.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
        draw_quad({0.f, 1.f, 0.f});

        // top left blue quad
        glViewport(
            0.f, SCREEN_HEIGHT / 2.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
        draw_quad({0.f, 0.f, 1.f});

        // top right yellow quad
        glViewport(
            SCREEN_WIDTH / 2.f,
            SCREEN_HEIGHT / 2.f,
            SCREEN_WIDTH / 2.f,
            SCREEN_HEIGHT / 2.f);
        draw_quad({1.f, 1.f, 0.f});
    } /* viewport with radar subview port */
    else if (g_viewport_mode == VIEWPORT_MODE_RADAR) {
        // full size quad
        glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);
        double_quads({1.f, 1.f, 1.f}, {0.f, 0.f, 0.f});

        // radar quad
        glViewport(
            SCREEN_WIDTH / 2.f,
            SCREEN_HEIGHT / 2.f,
            SCREEN_WIDTH / 2.f,
            SCREEN_HEIGHT / 2.f);
        double_quads({1.f, 1.f, 1.f}, {0.f, 0.f, 0.f});
    }

    glutSwapBuffers();
}

void
handle_keys(unsigned char key, int, int)
{
    // if the user presses q
    if (key == 'q') {
        // cycle through viewport modes
        g_viewport_mode++;
        if (g_viewport_mode > VIEWPORT_MODE_RADAR) {
            g_viewport_mode = VIEWPORT_MODE_FULL;
        }
    }
}
