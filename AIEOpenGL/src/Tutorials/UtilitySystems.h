#pragma once

#include "BaseApplication.h"

class Camera;
class BaseNPC;
class World;

class UtilitySystems : public BaseApplication {
public:

    UtilitySystems();
	virtual ~UtilitySystems();

	virtual bool startup() override;
	virtual void shutdown() override;

	virtual bool update(float deltaTime) override;
	virtual void draw() override;

private:
    Camera*		m_camera;
    World*		m_pWorld;
    BaseNPC*	m_pNPC;
};