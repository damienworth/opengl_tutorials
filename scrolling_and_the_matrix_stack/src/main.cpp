#include "lutil.hpp"

#include <cstdlib> // for EXIT_SUCCESS and EXIT_FAILURE

void run_main_loop(int);
/*
Pre Condition:
 -Initialized freeGLUT
Post Condition:
 -Calls the main loop functions and sets itself to be called back in 1000 /
SCREEN_FPS milliseconds Side Effects: -Sets glutTimerFunc
*/

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
    glutCreateWindow("Scrolling and the matrix stack");

    // do post window/context creation initialization
    if (!initGL()) {
        std::cerr << "unable to initialize graphics library\n";
        return EXIT_FAILURE;
    }

    // set keyboard handler
    glutKeyboardFunc(handle_keys);

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
