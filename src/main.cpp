#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>
#include "errors.hpp"
#include "glsl_program.hpp"
#include "glsl_exception.hpp"

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;
static const char WINDOW_TITLE[] = "Window";

// Print general OpenGL info (version, extensions, etc.)
void PrintOglInfo()
{
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "OpenGL info:" << std::endl;
    std::cout << "  Renderer: " << renderer << std::endl;
    std::cout << "  Vendor: " << vendor << std::endl;
    std::cout << "  Version: " << version << std::endl;
    std::cout << "  GLSL Version: " << glslVersion << std::endl;
    std::cout << "  OpenGL Version: " << major << "." << minor << std::endl;
    GLint extNum;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extNum);
    if (extNum > 0) {
        std::cout << extNum << " extensions available:" << std::endl;
        for (GLint i = 0; i < extNum; i++) {
            const GLubyte *ext = glGetStringi(GL_EXTENSIONS, i);
            std::cout << "  " << ext << std::endl;
        }
    }
}

const char * ShaderTypeToString(GLenum shaderType)
{
    switch (shaderType) {
        case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
        default:                 return "UNKNOWN";
    }
}

void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *param)
{
    std::cout << "[DEBUG] " << message << std::endl;
}

static glm::vec3 g_movement(0.0f);

void OnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // Ignore key repeats
    if (action == GLFW_REPEAT) {
        return;
    }

    std::map<int, glm::vec3> moveVectors;
    moveVectors[GLFW_KEY_W] = glm::vec3(0.0f, 0.0f, -1.0f);
    moveVectors[GLFW_KEY_S] = glm::vec3(0.0f, 0.0f, 1.0f);
    moveVectors[GLFW_KEY_A] = glm::vec3(-1.0f, 0.0f, 0.0f);
    moveVectors[GLFW_KEY_D] = glm::vec3(1.0f, 0.0f, 0.0f);
    moveVectors[GLFW_KEY_SPACE] = glm::vec3(0.0f, 1.0f, 0.0f);
    moveVectors[GLFW_KEY_C] = glm::vec3(0.0f, -1.0f, 0.0f);

    std::map<int, glm::vec3>::iterator it = moveVectors.find(key);
    if (it != moveVectors.end()) {
        float direction = (action == GLFW_PRESS ? -1.0f : 1.0f);

        std::cout << "Moving!" << std::endl;
        g_movement += it->second * direction;
    }
}

static glm::mat4 g_rotation(1.0f);

void OnMouseMove(GLFWwindow *window, double xpos, double ypos)
{
    static double xx = xpos, yy = ypos;
    
    glm::vec2 moveVec(xpos - xx, ypos - yy);
    xx = xpos;
    yy = ypos;
    moveVec *= 0.005f;

    glm::mat4 rot = glm::eulerAngleXY(moveVec.y, moveVec.x);
    g_rotation = g_rotation * rot;
}

int main()
{
    try {

    if (!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    float aspectRatio = (float)windowWidth / windowHeight;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, 0);
    glfwSetKeyCallback(window, OnKeyPress);
    glfwSetCursorPosCallback(window, OnMouseMove);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return 1;
    }

    glDebugMessageCallback(DebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    glEnable(GL_DEPTH_TEST);

    GLSLProgram program;
    program.CompileShader("../src/diffuse.vert");
    program.CompileShader("../src/diffuse.frag");

    program.BindAttribLocation(0, "VertexPosition");
    program.BindAttribLocation(1, "VertexNormal");
    program.BindAttribLocation(2, "VertexColor");

    program.Link();
    program.Use();

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    GLfloat vertexData[] = {
        // -X side
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // +X side        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        // -Y side
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // +Y side
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // -Z side
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,

        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // +Z side
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    GLfloat normalData[] = {
        // -X normal
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // +X normal
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // -Y normal
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        // +Y normal
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // -Z normal
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // +Z normal
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normalData), normalData, GL_STATIC_DRAW);

    GLfloat colorData[] = {
        // -X
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // +X
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        // -Y
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,

        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,

        // +Y
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,

        // -Z
        0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,

        0.0f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f,

        // +Z
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
    };
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexBuffer(0, vertexBuffer, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(1, normalBuffer, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(2, colorBuffer, 0, sizeof(GLfloat) * 3);

    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, 0);

    glVertexAttribBinding(0, 0);
    glVertexAttribBinding(1, 1);
    glVertexAttribBinding(2, 2);

    program.PrintActiveAttribs();

    glm::mat4 projectionMatrix = glm::perspective(45.0f, aspectRatio, 0.01f, 100.0f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glm::vec3 lightPosition(10.0f, 10.0f, 2.0f);
    glm::vec3 lightIntensityRGB(1.0f, 0.5f, 1.0f);
    glm::vec3 reflectionRGB(0.8f, 1.0f, 0.7f);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 velocity(0.0f);
        if (glm::length(g_movement) > 0.0f) {
            velocity = glm::normalize(g_movement) * 0.1f;
        }          

        //viewMatrix = glm::translate(viewMatrix, velocity);
        viewMatrix = glm::translate(glm::mat4(1.0f), velocity) * viewMatrix;

        viewMatrix = g_rotation * viewMatrix;
        g_rotation = glm::mat4(1.0f);

        glm::mat4 modelViewMatrix = viewMatrix;
        glm::mat3 normalMatrix = glm::inverse(glm::transpose(glm::mat3(modelViewMatrix)));
        
        program.SetUniform("ModelViewMatrix", modelViewMatrix);
        program.SetUniform("ProjectionMatrix", projectionMatrix);
        program.SetUniform("LightPosition", lightPosition);
        program.SetUniform("NormalMatrix", normalMatrix);
        program.SetUniform("Ld", lightIntensityRGB);
        program.SetUniform("Kd", reflectionRGB);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    } catch (GLSLException ex) {
        std::cerr << "GLSL Exception:" << std::endl << ex.Msg() << std::endl;
    }


    return 0;
}
