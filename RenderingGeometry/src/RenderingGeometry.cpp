#include "RenderingGeometry.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Grid.h"
#include "Gizmos.h"
#include "Vertex.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;


void RenderingGeometry::generateOpenGLBuffers(Vertex* pVertices, unsigned int vertCount, unsigned int* pIndices, unsigned int indexCount)
{
    // -- Generate buffers ---
    // create and bind buffers to a vertex array object
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    //Add the following line to generate a VertexArrayObject
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);

    unsigned int vertBytesSize = vertCount * sizeof(Vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertBytesSize, pVertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    unsigned int indexByteSize = indexCount * sizeof(unsigned int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteSize, pIndices, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



}

RenderingGeometry::RenderingGeometry()
	: m_camera(nullptr) 
{
}

RenderingGeometry::~RenderingGeometry() 
{

}

bool RenderingGeometry::startup() 
{
	// create a basic window
	const glm::ivec2 windowSize(1024, 768);
	createWindow("AIE OpenGL Application", windowSize.x, windowSize.y);

	// start the gizmo system that can draw basic shapes
	Gizmos::create();

	// create a camera
	m_camera = new Camera(glm::radians(45.f), windowSize.x/(float)windowSize.y, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

    Grid grid(10, 10);
    generateOpenGLBuffers(grid.getVertices(), grid.getVertexCount(), grid.getIndices(), grid.getIndexCount());
    m_indexCount = grid.getIndexCount();

    // create shaders
    const char* vsSource = "#version 410\n \
				layout(location=0) in vec4 Position; \
				layout(location=1) in vec4 Colour; \
                out vec4 vColour; \
				uniform mat4 ProjectionView; \
                uniform float time; \
                uniform float heightScale; \
				void main() { vColour = Colour; vec4 P = Position; P.y += sin( time + Position.x ) * heightScale; gl_Position = ProjectionView * P; }";

    const char* fsSource = "#version 410\n \
				in vec4 vColour; \
				out vec4 FragColor; \
				void main()	{ FragColor = vColour; }";


    int success = GL_FALSE;
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
    glCompileShader(fragmentShader);

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);

    // Check for program link error
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        int infoLogLength = 0;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength];

        glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
        printf("Error: Failed to link shader program!\n");
        printf("%s\n", infoLog);
        delete[] infoLog;
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);


	return true;
}

void RenderingGeometry::shutdown() 
{
	// delete our camera and cleanup gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool RenderingGeometry::update(float deltaTime) 
{
    m_timer += deltaTime;
	
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

void RenderingGeometry::draw() {

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 projectionViewMatrix = m_camera->getProjectionView();

    glUseProgram(m_programID);
    unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "ProjectionView");
    glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(projectionViewMatrix));

    unsigned int timeUniform = glGetUniformLocation(m_programID, "time");
    glUniform1f(timeUniform, m_timer);

    unsigned int heightScaleUniform = glGetUniformLocation(m_programID, "heightScale");
    glUniform1f(heightScaleUniform, 1);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);


	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());
}