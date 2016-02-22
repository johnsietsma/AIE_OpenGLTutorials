#pragma once

#include "BaseApplication.h"

#include "Grid.h"

#include <memory>

class Camera;
class Mesh;
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

    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Mesh> m_mesh;

    unsigned int m_programID;

    float m_timer = 0;
};