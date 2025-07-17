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

void makeShader(const char *vertexPath, const char *fragmentPath, GLuint *shader, const char *subType);

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

void checkCompileErrors(unsigned int shader, std::string type, const char *subType)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::" << subType << "::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::" << subType << "::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void makeShader(const char *vertexPath, const char *fragmentPath, GLuint *shader, const char *subType)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str()
;    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX", subType);
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT", subType);
    // shader Program
    *shader = glCreateProgram();
    glAttachShader(*shader, vertex);
    glAttachShader(*shader, fragment);
    glLinkProgram(*shader);
    checkCompileErrors(*shader, "PROGRAM", subType);
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}