#pragma once

#include "gl_core_4_4.h"

struct Vertex;

class Mesh
{
public:
    Mesh(Vertex* pVertices, unsigned int vertCount, unsigned int* pIndices, unsigned int indexCount);
    ~Mesh();

    GLuint getVAO() const { return m_VAO; }
    GLuint getIndexCount() const { return m_indexCount; }

private:
    // our vertex and index buffers
    GLuint	m_VAO;
    GLuint 	m_VBO;
    GLuint	m_IBO;
    GLuint  m_indexCount;

};
