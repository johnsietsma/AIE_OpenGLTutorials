#pragma once

#include "gl_core_4_4.h"

#include <assert.h>

struct Vertex_Color;
struct Vertex_TexCoord;
struct Vertex_Normal;

/*
    A Mesh encapsulates OpenGL buffers and a vertex array that are required to render geometry.
*/
class Mesh
{
public:
    // Create the mesh from the given vertices and indices.
    template<typename T>
    void create(T* pVertices, unsigned int vertCount, unsigned int* pIndices, unsigned int indexCount);

    ~Mesh();

    GLuint getVAO() const { return m_VAO; }
    GLuint getIndexCount() const { return m_indexCount; }

private:
    // Use function overloading to get different vertex attributes setup for different vertex types.
    void setupVerts(Vertex_Color* pVerts);
    void setupVerts(Vertex_TexCoord* pVerts);
    void setupVerts(Vertex_Normal* pVerts);
    
    // our vertex and index buffers
    GLuint	m_VAO;
    GLuint 	m_VBO;
    GLuint	m_IBO;
    GLuint  m_indexCount;
};

template<typename T>
void Mesh::create(T* pVertices, unsigned int vertCount, unsigned int* pIndices, unsigned int indexCount)
{
    m_indexCount = indexCount;

    // create and bind buffers to a vertex array object
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    //Add the following line to generate a VertexArrayObject
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);

    unsigned int vertBytesSize = vertCount * sizeof(T);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertBytesSize, pVertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    unsigned int indexByteSize = indexCount * sizeof(unsigned int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteSize, pIndices, GL_STATIC_DRAW);

    setupVerts(pVertices);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
