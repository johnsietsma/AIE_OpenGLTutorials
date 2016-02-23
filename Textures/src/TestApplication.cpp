#include "TestApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"

#include "Vertex.h"


using glm::vec3;
using glm::vec4;
using glm::mat4;

TestApplication::TestApplication()
	: m_camera(nullptr)
{

}

TestApplication::~TestApplication() {

}

bool TestApplication::startup() {


	// create a basic window
	const glm::ivec2 windowSize(1024, 768);
	createWindow("AIE OpenGL Application", windowSize.x, windowSize.y);

	// start the gizmo system that can draw basic shapes
	Gizmos::create();

	// create a camera
	m_camera = std::make_shared<Camera>(glm::radians(45.f), windowSize.x/(float)windowSize.y, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));
	
    if (!m_texture.load("../data/textures/crate.png")) return false;

    const char* vsSource = "#version 410\n \
					layout(location=0) in vec4 Position; \
					layout(location=1) in vec2 TexCoord; \
					out vec2 vTexCoord; \
					uniform mat4 ProjectionView; \
					void main() { \
                    vTexCoord = TexCoord; \
                    gl_Position= ProjectionView * Position;\
                }";

    const char* fsSource = "#version 410\n \
					in vec2 vTexCoord; \
					out vec4 FragColor; \
					uniform sampler2D diffuse; \
					void main() { \
                    FragColor = texture(diffuse,vTexCoord); }";

    m_program.create(vsSource, fsSource);

    Vertex_TexCoord pVertexData[] = {
        { glm::vec4(-5, 0, 5, 1), glm::vec2(0, 1) },
        { glm::vec4(5, 0, 5, 1), glm::vec2(1, 1) },
        { glm::vec4(5, 0, -5, 1), glm::vec2(1, 0) },
        { glm::vec4(-5, 0, -5, 1), glm::vec2(0, 0) },
    };

    unsigned int pIndexData[] = {
        0, 1, 2,
        0, 2, 3,
    };

    m_mesh.create(pVertexData, 4, pIndexData, 6);

 	return true;
}

void TestApplication::shutdown() {
	// delete our camera and cleanup gizmos
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool TestApplication::update(float deltaTime) {
	
	// close the application if the window closes
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);

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

void TestApplication::draw() {

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use our texture program
    glUseProgram(m_program.getId());

    // bind the camera
    int loc = glGetUniformLocation(m_program.getId(), "ProjectionView");
    glUniformMatrix4fv(loc, 1, GL_FALSE,
        &(m_camera->getProjectionView()[0][0]));

    // set texture slot
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture.getId());

    // tell the shader where it is
    loc = glGetUniformLocation(m_program.getId(), "diffuse");
    glUniform1i(loc, 0);

    // draw
    glBindVertexArray(m_mesh.getVAO());
    glDrawElements(GL_TRIANGLES, m_mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);


	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(m_camera->getProjectionView());
}