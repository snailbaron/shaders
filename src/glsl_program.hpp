#ifndef GLSL_PROGRAM_HPP
#define GLSL_PROGRAM_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include "glsl_exception.hpp"

enum class GLSLShaderType
{
    VERTEX          = GL_VERTEX_SHADER,
    FRAGMENT        = GL_FRAGMENT_SHADER,
    GEOMETRY        = GL_GEOMETRY_SHADER,
    TESS_CONTROL    = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
    COMPUTE         = GL_COMPUTE_SHADER,
};

class GLSLProgram
{
public:
    GLSLProgram();
    ~GLSLProgram();

    void CompileShader(const char *file);
    void CompileShader(const char *file, GLSLShaderType type);
    void CompileShader(const std::string &source, GLSLShaderType type, const char *fileName = nullptr);

    void Link();
    void Use();
    //void Validate() throw(GLSLException);

    GLuint Handle() const { return m_handle; }
    bool IsLinked() const { return m_linked; }

    void BindAttribLocation(GLuint location, const char *name);
    //void BindFragDataLocation(GLuint location, const char *name);

    void SetUniform(const char *name, float x, float y, float z);
    void SetUniform(const char *name, const glm::vec3 &v);
    void SetUniform(const char *name, const glm::vec4 &v);
    void SetUniform(const char *name, const glm::mat3 &m);
    void SetUniform(const char *name, const glm::mat4 &m);
    void SetUniform(const char *name, float val);
    void SetUniform(const char *name, int val);
    //void SetUniform(const char *name, bool val);

    void PrintActiveUniforms();
    void PrintActiveAttribs();
    // TODO: void PrintActiveUniformBlocks();

private:
    GLuint _GetUniformLocation(const char *name);

    GLuint m_handle;
    bool m_linked;
    std::map<std::string, int> m_uniformLocations;
};

#endif
