#pragma once

struct Vertex;

class Grid
{
public:
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

    unsigned int getSize() const { return m_rowCount * m_columnCount; }

    unsigned int getVertexCount() const { return getSize(); }
    Vertex* getVertices() const { return m_pVertices;  }

    unsigned int getIndexCount() const { return (m_rowCount - 1) * (m_columnCount - 1) * 6; }
    unsigned int* getIndices() const { return m_pIndices; }

private:
    void create();
    void destroy();

    unsigned int m_rowCount;
    unsigned int m_columnCount;

    Vertex* m_pVertices;
    unsigned int* m_pIndices;
};
