#include "UtilitySystems.h"
#include "gl_core_4_4.h"

#include "Camera.h"
#include "Gizmos.h"
#include "UtilitySystems\UtilityNPC.h"
#include "UtilitySystems\World.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

UtilitySystems::UtilitySystems() {

}

UtilitySystems::~UtilitySystems() {

}

bool UtilitySystems::startup() {
	// create a basic window
	createWindow("Tutorial - UtilitySystems", 1024, 768);

    // create a camera
    m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
    m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

    m_pWorld = new World();
    m_pNPC = new UtilityNPC(m_pWorld);

	return true;
}

void UtilitySystems::shutdown() {
    delete m_camera;

    // destroy our window properly
	destroyWindow();
}

bool UtilitySystems::update(float deltaTime) {
	
	// close the application if the window closes
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);

    m_pNPC->update(deltaTime);


	// clear the gizmos out for this frame
	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1));

	// ...for now let's add a grid to the gizmos
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));

		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	}

	// return true, else the application closes
	return true;
}

void UtilitySystems::draw() {

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_pWorld->render();
    m_pNPC->render();


	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());
}