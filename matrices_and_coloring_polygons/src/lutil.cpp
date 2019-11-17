#include "lutil.hpp"

namespace {

static int     g_color_mode       = COLOR_MODE_CYAN;
static GLfloat g_projection_scale = 1.f;

} // namespace

bool
initGL()
{
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

    // render quad
    if (g_color_mode == COLOR_MODE_CYAN) {
        // solid cyan
        // clang-format off
        glBegin(GL_QUADS);
            glColor3f(0.f, 1.f, 1.f);
            glVertex2f(-50.f, -50.f);
            glVertex2f( 50.f, -50.f);
            glVertex2f( 50.f,  50.f);
            glVertex2f(-50.f,  50.f);
        glEnd();
        // clang-format on
    } else {
        // RYGB mix
        glBegin(GL_QUADS);
        glColor3f(1.f, 0.f, 0.f);
        glVertex2f(-50.f, -50.f);
        glColor3f(1.f, 1.f, 0.f);
        glVertex2f(50.f, -50.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex2f(50.f, 50.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex2f(-50.f, 50.f);
        glEnd();
    }

    glutSwapBuffers();
}

void
handle_keys(unsigned char key, int x, int y)
{
    // unused so far
    (void)x;
    (void)y;

    // if the user presses q
    if (key == 'q') {
        // toggle color mode
        if (g_color_mode == COLOR_MODE_CYAN) {
            g_color_mode = COLOR_MODE_MULTI;
        } else {
            g_color_mode = COLOR_MODE_CYAN;
        }
    } else if (key == 'e') {
        // cycle through projection scales
        if (g_projection_scale == 1.f) {
            // zoom out
            g_projection_scale = 2.f;
        } else if (g_projection_scale == 2.f) {
            // zoom in
            g_projection_scale = 0.5f;
        } else if (g_projection_scale == 0.5f) {
            // regular zoom
            g_projection_scale = 1.f;
        }

        // update projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(
            0.0,
            SCREEN_WIDTH * g_projection_scale,
            SCREEN_HEIGHT * g_projection_scale,
            0.0,
            1.0,
            -1.0);
    }
}
