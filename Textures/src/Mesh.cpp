#include "Mesh.h"

#include "Vertex.h"

#include "gl_core_4_4.h"

#include <cstddef>

void Mesh::setupVerts(Vertex_Color* pVerts)
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Color), (void*)(offsetof(Vertex_Color, position)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Color), (void*)(offsetof(Vertex_Color, colour)));

}

void Mesh::setupVerts(Vertex_Normal* pVerts)
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Normal), (void*)(offsetof(Vertex_Normal, position)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Normal), (void*)(offsetof(Vertex_Normal, normal)));

}

void Mesh::setupVerts(Vertex_TexCoord* pVerts)
{
    glEnableVertexAttribArray(0);
    // Tell OpenGL how to get to vertex data.
    glVertexAttribPointer(0,
        4, // How many elements
        GL_FLOAT, // Element type
        GL_FALSE, // Norm
        sizeof(float) * 6, // Stride, how many bytes to skip to to get to the next position
        0);  // Byte offset to first position

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
        2, // uv
        GL_FLOAT,
        GL_FALSE,
        sizeof(float) * 6, // Stride
        ((char*)0) + sizeof(float) * 4);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteBuffers(1, &m_VBO);
}
