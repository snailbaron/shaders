#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

GLuint LoadShader(const char *fileName, GLenum shaderType)
{
    std::cout << "Compiling shader (" << ShaderTypeToString(shaderType) << "): " << fileName << std::endl;

    // Load shader source code
    std::ifstream ifs(fileName);
    if (!ifs.is_open()) {
        THROW(Error, std::string("Failed to open shader file: ") + std::string(fileName));
    }
    std::string source((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    std::cout << "Loaded shader source" << std::endl;

    // Create shader object
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        THROW(Error, "Failed to create shader");
    }
    std::cout << "Created shader object" << std::endl;

    // Compile source code
    const GLchar *sourcePtr = source.c_str();
    GLint sourceLen = static_cast<GLint>(source.length());
    glShaderSource(shader, 1, &sourcePtr, &sourceLen);
    glCompileShader(shader);
    std::cout << "Compiled shader" << std::endl;

    // Check compilation status
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    std::cout << "Compilation status: " << (compileStatus ? "OK" : "FAIL") << std::endl;

    // Get compilation log
    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        std::vector<GLchar> log(logLen);
        GLsizei writtenLen;
        glGetShaderInfoLog(shader, logLen, &writtenLen, &log[0]);
        std::cout << "Compilation log:" << std::endl << &log[0] << std::endl;
    } else {
        std::cout << "Compilation log is empty" << std::endl;
    }

    return shader;
}

GLuint LoadProgram(const std::vector<GLuint> &shaders)
{
    GLuint program = glCreateProgram();
    if (program == 0) {
        THROW(Error, "Failed to create program");
    }
    std::cout << "Created program" << std::endl;

    for (size_t i = 0; i < shaders.size(); i++) {
        glAttachShader(program, shaders[i]);
    }
    std::cout << "Attached " << shaders.size() << " shaders" << std::endl;

    glLinkProgram(program);
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    std::cout << "Link status: " << (linkStatus ? "OK" : "FAIL") << std::endl;

    GLint logLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        std::vector<GLchar> log(logLen);
        GLsizei writtenLen;
        glGetProgramInfoLog(program, logLen, &writtenLen, &log[0]);
        std::cout << "Link log:" << std::endl << &log[0] << std::endl;
    } else {
        std::cout << "Link log is empty" << std::endl;
    }

    glUseProgram(program);

    return program;
}

void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *param)
{
    std::cout << "Debug message received!" << std::endl;
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

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return 1;
    }

    glDebugMessageCallback(DebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    GLSLProgram program;
    program.CompileShader("../src/basic.vert");
    program.CompileShader("../src/basic.frag");
    program.Link();
    program.Use();

    //GLuint vertShader = LoadShader("../src/basic.vert", GL_VERTEX_SHADER);
    //GLuint fragShader = LoadShader("../src/basic.frag", GL_FRAGMENT_SHADER);
    //std::vector<GLuint> shaders = { vertShader, fragShader };
    //GLuint program = LoadProgram(shaders);

    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    GLfloat vertexData[] = {
        -0.7f, -0.8f, 0.0f,
        0.7f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    GLfloat colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

    GLint vertexColorLoc = glGetAttribLocation(program.Handle(), "VertexColor");
    GLint vertexPosLoc = glGetAttribLocation(program.Handle(), "VertexPosition");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(vertexColorLoc);
    glEnableVertexAttribArray(vertexPosLoc);

    //glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    //glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindVertexBuffer(0, vertexBuffer, 0, sizeof(GLfloat) * 3);

    //glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    //glVertexAttribPointer(vertexColorLoc, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindVertexBuffer(1, colorBuffer, 0, sizeof(GLfloat) * 3);

    glVertexAttribFormat(vertexPosLoc, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(vertexPosLoc, 0);
    glVertexAttribFormat(vertexColorLoc, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(vertexColorLoc, 1);

    

    GLfloat angle = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        angle += 0.01f;
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
        GLuint location = glGetUniformLocation(program.Handle(), "RotationMatrix");
        if (location >= 0) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
        }

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);


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
