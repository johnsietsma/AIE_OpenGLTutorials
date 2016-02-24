#pragma once

#include "BaseApplication.h"

#include <glm/vec3.hpp>

class Camera;
class FBXFile;

class FBXLighting : public BaseApplication {
public:

    FBXLighting();
    virtual ~FBXLighting();

    virtual bool startup() override;
    virtual void shutdown() override;

    virtual bool update(float deltaTime) override;
    virtual void draw() override;
    virtual void drawGUI() override;


private:
    void createOpenGLBuffers(FBXFile* fbx);
    void cleanupOpenGLBuffers(FBXFile* fbx);

    FBXFile* m_fbx;
    unsigned int m_program;

    glm::vec3 m_lightDir;
};