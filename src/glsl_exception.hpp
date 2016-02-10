#ifndef GLSL_EXCEPTION_HPP
#define GLSL_EXCEPTION_HPP

#include <string>

class GLSLException
{
public:
    GLSLException(const std::string &msg) : m_msg(msg) {}

    const std::string & Msg() const { return m_msg; }

private:
    std::string m_msg;
};

#endif
