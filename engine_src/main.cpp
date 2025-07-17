#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <unordered_map>
#include "../engine_include/Model.hpp"
#include "../engine_include/Plan.hpp"
#include "../engine_include/Grid.hpp"
#include "../engine_include/helpers.hpp"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::unordered_map<int, bool> keyPressed;

Model *model;
Plan *plan;
Grid *grid;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My first game", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint vhShader; // vh = Visual Helper (Plan and grid)
    makeShader("engine_shaders/vhShader.vs", "engine_shaders/vhShader.fs", &vhShader, "VISUAL HELPER");

    plan = new Plan(vhShader);
    grid = new Grid(vhShader, plan);
    plan->setGrid(grid);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        plan->render();
        grid->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Nettoyage
    glDeleteProgram(vhShader);
    delete plan;
    delete grid;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    (void)window;
    glViewport(0, 0, width, height);
}

void handleKey(int key, GLFWwindow *window) {
    switch (key) {
        case GLFW_KEY_P:
            plan->toggleDisplay();
            break;
        case GLFW_KEY_G:
            grid->toggleDisplay();
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        default:
            std::cout << "This key has no effect" << std::endl;
            break;
    }
}

void processInput(GLFWwindow* window) {
    for (int key : {GLFW_KEY_P, GLFW_KEY_G, GLFW_KEY_ESCAPE}) {
        bool isDown = glfwGetKey(window, key) == GLFW_PRESS;

        if (isDown && !keyPressed[key]) {
            handleKey(key, window);
            keyPressed[key] = true;
        }

        if (!isDown && keyPressed[key]) {
            keyPressed[key] = false;
        }
    }
}