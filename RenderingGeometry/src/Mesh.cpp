#include "Mesh.h"

#include "Vertex.h"

#include "gl_core_4_4.h"

#include <cstddef>

Mesh::Mesh(Vertex* pVertices, unsigned int vertCount, unsigned int* pIndices, unsigned int indexCount)
{
    m_indexCount = indexCount;

    // create and bind buffers to a vertex array object
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    //Add the following line to generate a VertexArrayObject
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);

    unsigned int vertBytesSize = vertCount * sizeof(Vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertBytesSize, pVertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    unsigned int indexByteSize = indexCount * sizeof(unsigned int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteSize, pIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, colour)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteBuffers(1, &m_VBO);
}
