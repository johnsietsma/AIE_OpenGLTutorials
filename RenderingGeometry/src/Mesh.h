#pragma once

#include "gl_core_4_4.h"

struct Vertex;

/*
    A Mesh encapsulates OpenGL buffers and a vertex array that are required to render geometry.
*/
class Mesh
{
public:
    // Create the mesh from the given Vertices and indices.
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
