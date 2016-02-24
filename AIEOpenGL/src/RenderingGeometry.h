#pragma once

#include "BaseApplication.h"

class Camera;

class RenderingGeometry : public BaseApplication {
public:
	virtual bool startup() override;
	virtual void shutdown() override;

	virtual bool update(float deltaTime) override;
	virtual void draw() override;

private:
    void generateGrid(unsigned int rows, unsigned int cols);

    // our vertex and index buffers
    unsigned int	m_VAO;
    unsigned int 	m_VBO;
    unsigned int	m_IBO;

    unsigned int m_programID;

    int m_rows;
    int m_cols;

    float m_timer = 0;
};