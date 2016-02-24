#pragma once

#include "BaseApplication.h"

class Camera;

class Blank : public BaseApplication {
public:

    Blank();
	virtual ~Blank();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();
};