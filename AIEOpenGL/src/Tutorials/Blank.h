#pragma once

#include "BaseApplication.h"

class Camera;

class Blank : public BaseApplication {
public:

    Blank();
	virtual ~Blank();

	virtual bool startup() override;
	virtual void shutdown() override;

	virtual bool update(float deltaTime) override;
	virtual void draw() override;
};