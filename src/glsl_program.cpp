#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include "glsl_program.hpp"

void CheckFileExists(const boost::filesystem::path &path)
{
    if (!boost::filesystem::exists(path)) {
        throw GLSLException(std::string("Shader file does not exist: ") + path.string());
    }
    if (!boost::filesystem::is_regular_file(path)) {
        throw GLSLException(std::string("Provided shader file is not a regular file: ") + path.string());
    }
}

GLSLShaderType GetShaderTypeByPath(const boost::filesystem::path &path)
{
    std::string fname = path.filename().string();
    std::string ext = path.extension().string();
    GLSLShaderType type;
    if (ext == ".vert") {
        type = GLSLShaderType::VERTEX;
    } else if (ext == ".frag") {
        type = GLSLShaderType::FRAGMENT;
    } else {
        throw GLSLException(std::string("Unknown shader file extension: ") + ext);
    }
    return type;
}

const std::string ReadFileContent(const char *file)
{
    std::ifstream ifs(file);
    std::string source((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return source;
}

const char * OglTypeToString(GLenum type)
{
    switch (type) {
        case GL_FLOAT:                                     return "float";
        case GL_FLOAT_VEC2:                                return "vec2";
        case GL_FLOAT_VEC3:                                return "vec3";
        case GL_FLOAT_VEC4:                                return "vec4";
        case GL_DOUBLE:                                    return "double";
        case GL_DOUBLE_VEC2:                               return "dvec2";
        case GL_DOUBLE_VEC3:                               return "dvec3";
        case GL_DOUBLE_VEC4:                               return "dvec4";
        case GL_INT:                                       return "int";
        case GL_INT_VEC2:                                  return "ivec2";
        case GL_INT_VEC3:                                  return "ivec3";
        case GL_INT_VEC4:                                  return "ivec4";
        case GL_UNSIGNED_INT:                              return "uint";
        case GL_UNSIGNED_INT_VEC2:                         return "uvec2";
        case GL_UNSIGNED_INT_VEC3:                         return "uvec3";
        case GL_UNSIGNED_INT_VEC4:                         return "uvec4";
        case GL_BOOL:                                      return "bool";
        case GL_BOOL_VEC2:                                 return "bvec2";
        case GL_BOOL_VEC3:                                 return "bvec3";
        case GL_BOOL_VEC4:                                 return "bvec4";
        case GL_FLOAT_MAT2:                                return "mat2";
        case GL_FLOAT_MAT3:                                return "mat3";
        case GL_FLOAT_MAT4:                                return "mat4";
        case GL_FLOAT_MAT2x3:                              return "mat2x3";
        case GL_FLOAT_MAT2x4:                              return "mat2x4";
        case GL_FLOAT_MAT3x2:                              return "mat3x2";
        case GL_FLOAT_MAT3x4:                              return "mat3x4";
        case GL_FLOAT_MAT4x2:                              return "mat4x2";
        case GL_FLOAT_MAT4x3:                              return "mat4x3";
        case GL_DOUBLE_MAT2:                               return "dmat2";
        case GL_DOUBLE_MAT3:                               return "dmat3";
        case GL_DOUBLE_MAT4:                               return "dmat4";
        case GL_DOUBLE_MAT2x3:                             return "dmat2x3";
        case GL_DOUBLE_MAT2x4:                             return "dmat2x4";
        case GL_DOUBLE_MAT3x2:                             return "dmat3x2";
        case GL_DOUBLE_MAT3x4:                             return "dmat3x4";
        case GL_DOUBLE_MAT4x2:                             return "dmat4x2";
        case GL_DOUBLE_MAT4x3:                             return "dmat4x3";
        case GL_SAMPLER_1D:                                return "sampler1D";
        case GL_SAMPLER_2D:                                return "sampler2D";
        case GL_SAMPLER_3D:                                return "sampler3D";
        case GL_SAMPLER_CUBE:                              return "samplerCube";
        case GL_SAMPLER_1D_SHADOW:                         return "sampler1DShadow";
        case GL_SAMPLER_2D_SHADOW:                         return "sampler2DShadow";
        case GL_SAMPLER_1D_ARRAY:                          return "sampler1DArray";
        case GL_SAMPLER_2D_ARRAY:                          return "sampler2DArray";
        case GL_SAMPLER_CUBE_MAP_ARRAY:                    return "samplerCubeArray";
        case GL_SAMPLER_1D_ARRAY_SHADOW:                   return "sampler1DArrayShadow";
        case GL_SAMPLER_2D_ARRAY_SHADOW:                   return "sampler2DArrayShadow";
        case GL_SAMPLER_2D_MULTISAMPLE:                    return "sampler2DMS";
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:              return "sampler2DMSArray";
        case GL_SAMPLER_CUBE_SHADOW:                       return "samplerCubeShadow";
        case GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW:             return "samplerCubeArrayShadow";
        case GL_SAMPLER_BUFFER:                            return "samplerBuffer";
        case GL_SAMPLER_2D_RECT:                           return "sampler2DRect";
        case GL_SAMPLER_2D_RECT_SHADOW:                    return "sampler2DRectShadow";
        case GL_INT_SAMPLER_1D:                            return "isampler1D";
        case GL_INT_SAMPLER_2D:                            return "isampler2D";
        case GL_INT_SAMPLER_3D:                            return "isampler3D";
        case GL_INT_SAMPLER_CUBE:                          return "isamplerCube";
        case GL_INT_SAMPLER_1D_ARRAY:                      return "isampler1DArray";
        case GL_INT_SAMPLER_2D_ARRAY:                      return "isampler2DArray";
        case GL_INT_SAMPLER_CUBE_MAP_ARRAY:                return "isamplerCubeArray";
        case GL_INT_SAMPLER_2D_MULTISAMPLE:                return "isampler2DMS";
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:          return "isampler2DMSArray";
        case GL_INT_SAMPLER_BUFFER:                        return "isamplerBuffer";
        case GL_INT_SAMPLER_2D_RECT:                       return "isampler2DRect";
        case GL_UNSIGNED_INT_SAMPLER_1D:                   return "usampler1D";
        case GL_UNSIGNED_INT_SAMPLER_2D:                   return "usampler2D";
        case GL_UNSIGNED_INT_SAMPLER_3D:                   return "usampler3D";
        case GL_UNSIGNED_INT_SAMPLER_CUBE:                 return "usamplerCube";
        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:             return "usampler1DArray";
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:             return "usampler2DArray";
        case GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY:       return "usamplerCubeArray";
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:       return "usampler2DMS";
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return "usampler2DMSArray";
        case GL_UNSIGNED_INT_SAMPLER_BUFFER:               return "usamplerBuffer";
        case GL_UNSIGNED_INT_SAMPLER_2D_RECT:              return "usampler2DRect";
        case GL_IMAGE_1D:                                  return "image1D";
        case GL_IMAGE_2D:                                  return "image2D";
        case GL_IMAGE_3D:                                  return "image3D";
        case GL_IMAGE_2D_RECT:                             return "image2DRect";
        case GL_IMAGE_CUBE:                                return "imageCube";
        case GL_IMAGE_BUFFER:                              return "imageBuffer";
        case GL_IMAGE_1D_ARRAY:                            return "image1DArray";
        case GL_IMAGE_2D_ARRAY:                            return "image2DArray";
        case GL_IMAGE_CUBE_MAP_ARRAY:                      return "imageCubeArray";
        case GL_IMAGE_2D_MULTISAMPLE:                      return "image2DMS";
        case GL_IMAGE_2D_MULTISAMPLE_ARRAY:                return "image2DMSArray";
        case GL_INT_IMAGE_1D:                              return "iimage1D";
        case GL_INT_IMAGE_2D:                              return "iimage2D";
        case GL_INT_IMAGE_3D:                              return "iimage3D";
        case GL_INT_IMAGE_2D_RECT:                         return "iimage2DRect";
        case GL_INT_IMAGE_CUBE:                            return "iimageCube";
        case GL_INT_IMAGE_BUFFER:                          return "iimageBuffer";
        case GL_INT_IMAGE_1D_ARRAY:                        return "iimage1DArray";
        case GL_INT_IMAGE_2D_ARRAY:                        return "iimage2DArray";
        case GL_INT_IMAGE_CUBE_MAP_ARRAY:                  return "iimageCubeArray";
        case GL_INT_IMAGE_2D_MULTISAMPLE:                  return "iimage2DMS";
        case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:            return "iimage2DMSArray";
        case GL_UNSIGNED_INT_IMAGE_1D:                     return "uimage1D";
        case GL_UNSIGNED_INT_IMAGE_2D:                     return "uimage2D";
        case GL_UNSIGNED_INT_IMAGE_3D:                     return "uimage3D";
        case GL_UNSIGNED_INT_IMAGE_2D_RECT:                return "uimage2DRect";
        case GL_UNSIGNED_INT_IMAGE_CUBE:                   return "uimageCube";
        case GL_UNSIGNED_INT_IMAGE_BUFFER:                 return "uimageBuffer";
        case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:               return "uimage1DArray";
        case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:               return "uimage2DArray";
        case GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY:         return "uimageCubeArray";
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:         return "uimage2DMS";
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:   return "uimage2DMSArray";
        case GL_UNSIGNED_INT_ATOMIC_COUNTER:               return "atomic_uint";
        default:                                           return "UNKNOWN";
    }
}

GLSLProgram::GLSLProgram() :
    m_handle(0),
    m_linked(false),
    m_uniformLocations()
{
    m_handle = glCreateProgram();
    if (m_handle == 0) {
        throw GLSLException("Failed to create program");
    }
}

GLSLProgram::~GLSLProgram()
{
    glDeleteProgram(m_handle);
}

void GLSLProgram::CompileShader(const char *file)
{
    boost::filesystem::path path(file);
    CheckFileExists(path);
    GLSLShaderType type = GetShaderTypeByPath(path);

    std::string source = ReadFileContent(file);

    CompileShader(source, type, file);
}

void GLSLProgram::CompileShader(const char *file, GLSLShaderType type)
{
    boost::filesystem::path path(file);
    CheckFileExists(path);

    std::ifstream ifs(file);
    std::string source((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    CompileShader(source, type, file);
}

void GLSLProgram::CompileShader(const std::string &source, GLSLShaderType type, const char *fileName)
{
    GLuint shader = glCreateShader((GLenum)type);
    if (shader == 0) {
        throw GLSLException("Failed to create shader");
    }

    const GLchar *sourcePtr = source.c_str();
    GLint sourceLen = static_cast<GLint>(source.length());
    glShaderSource(shader, 1, &sourcePtr, &sourceLen);

    glCompileShader(shader);
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {
        GLint logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen);
        glGetShaderInfoLog(shader, logLen, nullptr, &log[0]);
        std::ostringstream msg;
        msg << "Shader (" << fileName << ") compilation failed:" << std::endl << &log[0] << std::endl;
        throw GLSLException(msg.str());
    }

    glAttachShader(m_handle, shader);
}

void GLSLProgram::Use()
{
    if (m_linked) {
        glUseProgram(m_handle);
    }
}

void GLSLProgram::Link()
{
    glLinkProgram(m_handle);
    GLint linkStatus;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {
        GLint logLen;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen);
        glGetProgramInfoLog(m_handle, logLen, nullptr, &log[0]);
        std::ostringstream msg;
        msg << "Program link failed:" << std::endl << &log[0] << std::endl;
        throw GLSLException(msg.str());
    }

    m_linked = true;
}

void GLSLProgram::BindAttribLocation(GLuint location, const char *name)
{
    glBindAttribLocation(m_handle, location, name);
}

GLuint GLSLProgram::_GetUniformLocation(const char *name)
{
    std::map<std::string, int>::iterator it = m_uniformLocations.find(name);
    if (it == m_uniformLocations.end()) {
        GLint location = glGetUniformLocation(m_handle, name);
        m_uniformLocations[name] = location;
    }

    return m_uniformLocations[name];
}

void GLSLProgram::SetUniform(const char *name, float x, float y, float z)
{
    GLuint location = _GetUniformLocation(name);
    glUniform3f(location, x, y, z);
}

void GLSLProgram::SetUniform(const char *name, const glm::vec3 &v)
{
    GLuint location = _GetUniformLocation(name);
    glUniform3fv(location, 1, &v[0]);
}

void GLSLProgram::SetUniform(const char *name, const glm::vec4 &v)
{
    GLuint location = _GetUniformLocation(name);
    glUniform4fv(location, 1, &v[0]);
}
    
void GLSLProgram::SetUniform(const char *name, const glm::mat3 &m)
{
    GLuint location = _GetUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, &m[0][0]);
}

void GLSLProgram::SetUniform(const char *name, const glm::mat4 &m)
{
    GLuint location = _GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
}

void GLSLProgram::SetUniform(const char *name, float val)
{
    GLuint location = _GetUniformLocation(name);
    glUniform1f(location, val);
}

void GLSLProgram::SetUniform(const char *name, int val)
{
    GLuint location = _GetUniformLocation(name);
    glUniform1i(location, val);
}

void GLSLProgram::PrintActiveUniforms()
{
    std::cout << "Program uniforms:" << std::endl;
    GLint uniformNum;
    glGetProgramInterfaceiv(m_handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformNum);
    GLenum uniformProps[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };
    for (int i = 0; i < uniformNum; i++) {
        GLint results[4];
        glGetProgramResourceiv(m_handle, GL_UNIFORM, i, 4, uniformProps, 4, nullptr, results);

        if (results[3] != -1) {
            std::cout << "Uniform in block!" << std::endl;
            continue;
        }

        GLint nameBufSize = results[0] + 1;
        std::vector<char> name(nameBufSize);
        glGetProgramResourceName(m_handle, GL_UNIFORM, i, nameBufSize, nullptr, &name[0]);

        printf("  [%d] %s (%s)\n", results[2], &name[0], OglTypeToString(results[1]));
    }
}

void GLSLProgram::PrintActiveAttribs()
{
    std::cout << "Program attributes:" << std::endl;
    GLint attrNum;
    glGetProgramInterfaceiv(m_handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &attrNum);
    GLenum props[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
    for (int i = 0; i < attrNum; i++) {
        GLint results[3];
        glGetProgramResourceiv(m_handle, GL_PROGRAM_INPUT, i, 3, props, 3, nullptr, results);

        GLint nameBufSize = results[0] + 1;
        std::vector<char> name(nameBufSize);
        glGetProgramResourceName(m_handle, GL_PROGRAM_INPUT, i, nameBufSize, nullptr, &name[0]);

        printf("  [%d] %s (%s)\n", results[2], &name[0], OglTypeToString(results[1]));
    }
}