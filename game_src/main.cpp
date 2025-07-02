#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include "../include/Camera.hpp"
#include "../game_include/Player.hpp"
#include "../game_include/Bird.hpp"
#include "../game_include/Cloud.hpp"
#include "../game_include/Landscape.hpp"
#include "../game_include/stb_image.h"
#include <iostream>
#include <random>
#include <list>
#include <chrono>

using namespace std::chrono;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Player &player);
void makeShader(const char *vertexPath, const char *fragmentPath, GLuint *shader, const char *subType);
void checkCompileErrors(unsigned int shader, std::string type, const char *subType);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool showHitbox = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
constexpr auto cloudTimer = seconds(1);

static std::random_device rd;
static std::mt19937 motor(rd());
std::uniform_int_distribution<int> randomizer(0, 100);

int main()
{
	// glfw: initialize and configure
    // ------------------------------
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

    // Enable blending to create transparency (png with alpha canal as a texture)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    GLuint objectShader, hitboxShader; 
    GLint positionLoc, hitboxPositionLoc, hitboxColorLoc;

    // Make the shaders
    makeShader("game_shaders/object_shader.vs", "game_shaders/object_shader.fs", &objectShader, "OBJECT");
    makeShader("game_shaders/hitbox_shader.vs", "game_shaders/hitbox_shader.fs", &hitboxShader, "HITBOX");

    // Keep track of uniform variables in the shader programs
    positionLoc = glGetUniformLocation(objectShader, "position");
    hitboxPositionLoc = glGetUniformLocation(hitboxShader, "position");
    hitboxColorLoc = glGetUniformLocation(hitboxShader, "Color");

    Loader *loader;

    loader = new Loader(DINO_GAME, objectShader, hitboxShader, positionLoc, hitboxPositionLoc, hitboxColorLoc);
    Player player(loader);

    std::list<Cloud*> clouds;
    std::list<Object*> enemies;
    std::list<Landscape*> landscape;
    clouds.push_back(new Cloud(loader, randomizer, motor));
    landscape.push_back(new Landscape(loader, randomizer, motor));

    while(!glfwWindowShouldClose(window))
    {
        static auto lastCloud = steady_clock::now();
        auto now = steady_clock::now();
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (now - lastCloud >= cloudTimer)
        {
            int random = randomizer(motor);
            if (random == 100)
            {
                clouds.push_back(new Cloud(loader, randomizer, motor));
                lastCloud = now;
            }
        }

        if ((*landscape.rbegin())->getPosition() < 0.40f)
            landscape.push_back(new Landscape(loader, randomizer, motor));

        processInput(window, player);

        glClearColor(0.97f, 0.97f, 0.97f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        player.render(showHitbox);
        // bird.render(showHitbox);
        for (auto it = clouds.begin(); it != clouds.end();)
        {
            (*it)->move(deltaTime);
            (*it)->render(showHitbox);
            if ((*it)->getPosition() < -1.5f)
            {
                delete *it;
                it = clouds.erase(it);
            }
            else
                it++;
        }

        for (auto it = landscape.begin(); it != landscape.end();)
        {
            (*it)->move(deltaTime);
            (*it)->render(showHitbox);
            if ((*it)->getPosition() < -2.0f)
            {
                delete *it;
                it = landscape.erase(it);
            }
            else
                it++;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (auto it = clouds.begin(); it != clouds.end(); it++)
        delete *it;
    for (auto it = landscape.begin(); it != landscape.end(); it++)
        delete *it;
    delete loader;

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, Player &player)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player.move(DOWN, deltaTime);
    else
        player.move(UP, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //     land.move(deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !showHitbox)
    //     showHitbox = true;
    // else if (glfwGetKey(window, GLFW_KEY_H) != GLFW_PRESS && showHitbox)
    //     showHitbox = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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
    const char * fShaderCode = fragmentCode.c_str()
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