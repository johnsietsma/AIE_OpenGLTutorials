#pragma once

#include "BaseApplication.h"

class Camera;

class AdvancedTexturing : public BaseApplication {
public:

    AdvancedTexturing();
    virtual ~AdvancedTexturing();

    virtual bool startup() override;
    virtual void shutdown() override;

    virtual bool update(float deltaTime) override;
    virtual void draw() override;

private:
    unsigned int m_texture, m_normalmap;
    unsigned int m_program;
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ibo;

};