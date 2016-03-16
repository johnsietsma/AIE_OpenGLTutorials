#pragma once

#include "BaseApplication.h"

class Camera;

class GPGPU : public BaseApplication {
public:
	virtual bool startup() override;
	virtual void shutdown() override;

	virtual bool update(float deltaTime) override;
	virtual void draw() override;

private:

    unsigned int m_texture;
    unsigned int m_program;
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ibo;
};