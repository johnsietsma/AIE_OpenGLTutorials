#include "Program.h"

#include "gl_core_4_4.h"

#include <cstdio>

Program::Program(const char* pVertexShaderSource, const char* pFragmentShaderSource)
{
    int success = GL_FALSE;
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, (const char**)&pVertexShaderSource, 0);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, (const char**)&pFragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    m_programId = glCreateProgram();
    glAttachShader(m_programId, vertexShader);
    glAttachShader(m_programId, fragmentShader);
    glLinkProgram(m_programId);

    // Check for program link error
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        int infoLogLength = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength];

        glGetProgramInfoLog(m_programId, infoLogLength, 0, infoLog);
        printf("Error: Failed to link shader program!\n");
        printf("%s\n", infoLog);
        delete[] infoLog;
    }


    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
}

Program::~Program()
{
    glDeleteProgram(m_programId);
}
