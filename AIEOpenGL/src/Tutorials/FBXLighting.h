#pragma once

#include "BaseApplication.h"

class Camera;
class FBXFile;

class FBXLighting : public BaseApplication {
public:

    FBXLighting();
	virtual ~FBXLighting();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:
    void createOpenGLBuffers(FBXFile* fbx);
    void cleanupOpenGLBuffers(FBXFile* fbx);

    FBXFile* m_fbx;
    unsigned int m_program;
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ibo;
};