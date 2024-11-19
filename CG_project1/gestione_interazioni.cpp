#include "gestione_interazioni.h"


extern float direction; 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    direction = 0.0;
    switch (key) 
    {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;

    case GLFW_KEY_RIGHT:
        if (action == GLFW_PRESS) direction = 4.0f;
        break; 
    case GLFW_KEY_LEFT:
        if (action == GLFW_PRESS) direction = -4.0f;
        break;

    default:
        break;
    }
}