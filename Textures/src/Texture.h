#pragma once

#include "gl_core_4_4.h"

class Texture
{
public:
    GLuint getId() const { return m_textureId; }

    bool load(const char* pFilename);

private:
    GLuint m_textureId;
};
