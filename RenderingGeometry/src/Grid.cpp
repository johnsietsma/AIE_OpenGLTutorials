#include "Grid.h"

#include "Vertex.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cmath>

using glm::vec3;
using glm::vec4;

void Grid::create()
{
    m_pVertices = new Vertex[m_rowCount * m_columnCount];
    for (unsigned int rowIndex = 0; rowIndex < m_rowCount; ++rowIndex) {
        for (unsigned int columnIndex = 0; columnIndex < m_columnCount; ++columnIndex) {
            int vertIndex = rowIndex * m_columnCount + columnIndex;

            m_pVertices[vertIndex].position = vec4((float)columnIndex, 0, (float)rowIndex, 1);

            // create some arbitrary colour based off something 
            // that might not be related to tiling a texture
            vec3 colour = vec3(sinf((columnIndex / (float)(m_columnCount - 1)) *  (rowIndex / (float)(m_rowCount - 1))));
            m_pVertices[vertIndex].colour = vec4(colour, 1);
        }
    }


    // -- Generate grid indices 

    // defining index count based off quad count (2 triangles per quad)
    const int indexCount = (m_rowCount - 1) * (m_columnCount - 1) * 6;
    m_pIndices = new unsigned int[indexCount];

    unsigned int index = 0;
    for (unsigned int rowIndex = 0; rowIndex < (m_rowCount - 1); ++rowIndex) {
        for (unsigned int columnIndex = 0; columnIndex < (m_columnCount - 1); ++columnIndex) {
            // triangle 1
            int triangleStartIndex = rowIndex * m_columnCount + columnIndex;
            m_pIndices[index++] = triangleStartIndex;
            m_pIndices[index++] = (rowIndex + 1) * m_columnCount + columnIndex;
            m_pIndices[index++] = (rowIndex + 1) * m_columnCount + (columnIndex + 1);

            // triangle 2
            m_pIndices[index++] = rowIndex * m_columnCount + columnIndex;
            m_pIndices[index++] = (rowIndex + 1) * m_columnCount + (columnIndex + 1);
            m_pIndices[index++] = rowIndex * m_columnCount + (columnIndex + 1);
        }
    }
}

void Grid::destroy()
{
    delete[] m_pVertices;
    delete[] m_pIndices;
}