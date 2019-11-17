#include "lutil.hpp"

#include <array>

namespace {

static GLfloat g_camera_x = 0.f, g_camera_y = 0.f;

inline void
draw_quad(const std::array<float, 3u>& color)
{
    // clang-format off
    glBegin(GL_QUADS);
        glColor3f(color[0], color[1], color[2]);
        glVertex2f(-SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
        glVertex2f( SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
        glVertex2f( SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f);
        glVertex2f(-SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f);
    glEnd();
    // clang-format on
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

    // save the default modelview matrix
    glPushMatrix();

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
    glPopMatrix();

    // save default matrix again
    glPushMatrix();

    // move to center of the screen
    glTranslatef(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f);

    // red quad
    draw_quad({1.f, 0.f, 0.f});

    // move to the right of the screen
    glTranslatef(SCREEN_WIDTH, 0.f, 0.f);

    // green quad
    draw_quad({0.f, 1.f, 0.f});

    // move to the lower right of the screen
    glTranslatef(0.f, SCREEN_HEIGHT, 0.f);

    // blue quad
    draw_quad({0.f, 0.f, 1.f});

    // move below the screen
    glTranslatef(-SCREEN_WIDTH, 0.f, 0.f);

    // yellow quad
    draw_quad({1.f, 1.f, 0.f});

    glutSwapBuffers();
}

void
handle_keys(unsigned char key, int, int)
{
    // if the user pressed w/a/s/d, change camera position
    if (key == 'w') {
        g_camera_y += 16.f;
    } else if (key == 'a') {
        g_camera_x += 16.f;
    } else if (key == 's') {
        g_camera_y -= 16.f;
    } else if (key == 'd') {
        g_camera_x -= 16.f;
    }

    // take save matrix off the stack and reset it
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glLoadIdentity();

    // move camera to position
    glTranslatef(-g_camera_x, -g_camera_y, 0.f);

    // save default matrix again with camera translation
    glPushMatrix();
}
