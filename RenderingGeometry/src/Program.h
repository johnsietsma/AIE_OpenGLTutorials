#pragma once

#include "gl_core_4_4.h"

class Program
{
public:
    Program(const char* pVertexShaderSource, const char* pFragmentShaderSource);
    ~Program();

    GLuint getId() const { return m_programId;  }

private:
    GLuint m_programId;
};
