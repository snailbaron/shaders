#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error
{
public:
    Error(const std::string &file, unsigned long line, const std::string &msg) : m_file(file), m_line(line), m_msg(msg) {}
    Error(const Error &e) : m_file(e.m_file), m_line(e.m_line), m_msg(e.m_msg) {}
    virtual ~Error() {}

    const std::string & Msg() const { return m_msg; }

protected:
    std::string m_file;
    unsigned long m_line;
    std::string m_msg;
};

#define THROW(EXCEPTION, ...) throw EXCEPTION(__FILE__, __LINE__, __VA_ARGS__)

#endif
