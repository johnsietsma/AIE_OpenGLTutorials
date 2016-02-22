#pragma once

struct Vertex;

/*
    A helper class that will create grid geometry.
    It doesn't interact with OpenGL at all, it's simply for managing the grid vertex and index data.
*/
class Grid
{
public:
    // Create a new grid
    Grid(unsigned int rowCount, unsigned int columnCount) :
        m_rowCount(rowCount),
        m_columnCount(columnCount)
    {
        create();
    }

    ~Grid()
    {
        destroy();
    }

    // Size is the number of cells in the grid
    unsigned int getSize() const { return (m_rowCount-1) * (m_columnCount-1); }

    unsigned int getVertexCount() const { return m_rowCount * m_columnCount; }
    Vertex* getVertices() const { return m_pVertices;  }

    unsigned int getIndexCount() const { return getSize() * 6; }
    unsigned int* getIndices() const { return m_pIndices; }

private:
    void create();
    void destroy();

    unsigned int m_rowCount;
    unsigned int m_columnCount;

    Vertex* m_pVertices;
    unsigned int* m_pIndices;
};
