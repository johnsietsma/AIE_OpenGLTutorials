#pragma once

#include "BaseApplication.h"

#include "Grid.h"

class Camera;
struct Vertex;

class RenderingGeometry : public BaseApplication {
public:

	RenderingGeometry();
	virtual ~RenderingGeometry();

	virtual bool startup() override;
	virtual void shutdown() override;

	virtual bool update(float deltaTime) override;
	virtual void draw() override;

private:
    void generateOpenGLBuffers(Vertex* pVertices, unsigned int vertCount, unsigned int* pIndices, unsigned int indexCount);

    Camera* m_camera;

    // our vertex and index buffers
    unsigned int	m_VAO;
    unsigned int 	m_VBO;
    unsigned int	m_IBO;
    unsigned int    m_indexCount;

    unsigned int m_programID;

    float m_timer = 0;
};