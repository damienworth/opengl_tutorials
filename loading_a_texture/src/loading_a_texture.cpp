#include "lutil.hpp"

#include <cstdlib> // for EXIT_SUCCESS and EXIT_FAILURE

/*
pre-condition:
    * initialized freeGLUT
post-condition:
    * calls the main loop functions and sets itself to be called back in 1000 /
      SCREEN_FPS milliseconds
side-effects:
    * sets glutTimerFunc
*/
void run_main_loop(int);

int
main(int argc, char** args)
{
    // initialize freeGLUT
    glutInit(&argc, args);

    // create opengl 2.1 context
    glutInitContextVersion(2, 1);

    // create double-buffered window
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Loading a texture");

    // do post window/context creation initialization
    if (!initGL()) {
        std::cerr << "unable to initialize graphics library\n";
        return EXIT_FAILURE;
    }

    // load media
    if (!load_media()) {
        std::cerr << "unable to load media\n";
        return EXIT_FAILURE;
    }

    // set rendering function
    glutDisplayFunc(render);

    // set main loop
    glutTimerFunc(1000 / SCREEN_FPS, run_main_loop, 0);

    // start GLUT main loop
    glutMainLoop();

    return EXIT_SUCCESS;
}

void
run_main_loop(int val)
{
    // frame logic
    update();
    render();

    // run frame once more time
    glutTimerFunc(1000 / SCREEN_FPS, run_main_loop, val);
}
