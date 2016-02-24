#pragma once

#include "BaseApplication.h"

class Camera;

class Textures : public BaseApplication {
public:
	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

    unsigned int m_texture;
    unsigned int m_program;
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ibo;
};