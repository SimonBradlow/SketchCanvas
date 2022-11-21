// raphics.cpp
// Created by Simon Bradlow on 11/15/22.
#include "graphics.h"
#include "shape.h"
#include "circle.h"
#include <vector>
using namespace std;

GLdouble width, height;
double radius;
int colorIndex;
int eraseIndex;
bool erase;
vector<unique_ptr<Circle>> circleVec;
Circle user;
vector<color> colorVec;
const color black(0, 0, 0);
const color red(1, 0, 0);
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color orange(1, 163/255.0, 22/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color magenta(1, 0, 1);
const color cyan (0, 1, 1);
const color white (1, 1, 1);

/* Initialize user cursor */
void initUser() {
    // Set initial user size to 5
    radius = 5;
    user.setRadius(radius);

    // Set initial user color to black
    colorIndex = 0;
    user.setColor(colorVec[colorIndex]);

    // Set erase flag to false
    erase = false;
    eraseIndex = 0;

    // Place cursor in top left at start
    user.setCenter(0, 0);
}

/* Initialize global variables */
void init() {
    // Golden ratio
    width = 809;
    height = 500;

    colorVec.push_back(black);
    colorVec.push_back(red);
    colorVec.push_back(skyBlue);
    colorVec.push_back(grassGreen);
    colorVec.push_back(orange);
    colorVec.push_back(purple);
    colorVec.push_back(magenta);
    colorVec.push_back(cyan);
    colorVec.push_back(white);

    initUser();
    srand(time(0));
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White and opaque
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width*2, height*2); // DO NOT CHANGE THIS LINE (unless you are running Catalina on Mac)

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    /*
     * Draw here
     */
    for (unique_ptr<Circle> &c : circleVec) c->draw();
    user.draw();

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Escape key
    // Exits the program
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwTerminate();
        exit(0);
    }
    // Z key
    // Decrease the radius of cursor
    if (key == GLFW_KEY_Z  && action == GLFW_PRESS && radius != 0) {
        radius -= 1;
        user.setRadius(radius);
    }
    // X key
    // Increase the radius of cursor
    if (key == GLFW_KEY_X  && action == GLFW_PRESS) {
        radius += 1;
        user.setRadius(radius);
    }
    // C key
    // Cycles colors using colorVec & colorIndex
    if (key == GLFW_KEY_C  && action == GLFW_PRESS) {
        if (colorIndex == 7) colorIndex = 0;
        else colorIndex += 1;
        user.setColor(colorVec[colorIndex]);
    }
    // E key
    // Toggle to change color to white ONLY
    if (key == GLFW_KEY_E  && action == GLFW_PRESS) {
        if (!erase) {
            erase = true;
            eraseIndex = colorIndex;
            colorIndex = 8;
        }
        else {
            erase = false;
            colorIndex = eraseIndex;
        }
        user.setColor(colorVec[colorIndex]);
    }
}

/* Cursor function to move user circle */
void cursor(GLFWwindow* window, double x, double y) {
    glfwGetCursorPos(window, &x, &y);
    user.setCenter(x, y);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        circleVec.push_back(make_unique<Circle>(colorVec[colorIndex], x, y, radius));
    }
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    init();
    if (!glfwInit())
        exit(EXIT_FAILURE);

    GLFWwindow* window;
    window = glfwCreateWindow(width, height, "Canvas", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glfwSetKeyCallback(window, kbd);

    // handles mouse movement
    glfwSetCursorPosCallback(window, cursor);

    // Enter the event-processing loop
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        display();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}