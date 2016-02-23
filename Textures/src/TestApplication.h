#pragma once

#include "BaseApplication.h"

#include "Program.h"
#include "Mesh.h"
#include "Texture.h"

#include <memory>

class Camera;

class TestApplication : public BaseApplication {
public:

	TestApplication();
	virtual ~TestApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	std::shared_ptr<Camera>		m_camera;

    Program m_program;
    Mesh m_mesh;
    Texture m_texture;

    //unsigned int m_vao;
    //unsigned int m_vbo;
    //unsigned int m_ibo;
};