#pragma once

#include "gl_core_4_4.h"

class Program
{
public:
    bool create(const char* pVertexShaderSource, const char* pFragmentShaderSource);

    ~Program();

    GLuint getId() const { return m_programId;  }

private:
    GLuint m_programId;
};
