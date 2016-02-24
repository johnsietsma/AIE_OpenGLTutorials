#include "RenderingGeometry.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Gizmos.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Vertex {
    vec4 position;
    vec4 colour;
};

void RenderingGeometry::generateGrid(unsigned int rows, unsigned int cols)
{
    m_rows = rows;
    m_cols = cols;

    Vertex* aoVertices = new Vertex[rows * cols];
    for (unsigned int r = 0; r < rows; ++r) {
        for (unsigned int c = 0; c < cols; ++c) {
            aoVertices[r * cols + c].position = vec4(
                (float)c, 0, (float)r, 1);

            // create some arbitrary colour based off something 
            // that might not be related to tiling a texture
            vec3 colour = vec3(sinf((c / (float)(cols - 1)) *
                (r / (float)(rows - 1))));
            aoVertices[r * cols + c].colour = vec4(colour, 1);
        }
    }

    // defining index count based off quad count (2 triangles per quad)
    unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

    unsigned int index = 0;
    for (unsigned int r = 0; r < (rows - 1); ++r) {
        for (unsigned int c = 0; c < (cols - 1); ++c) {
            // triangle 1
            auiIndices[index++] = r * cols + c;
            auiIndices[index++] = (r + 1) * cols + c;
            auiIndices[index++] = (r + 1) * cols + (c + 1);

            // triangle 2
            auiIndices[index++] = r * cols + c;
            auiIndices[index++] = (r + 1) * cols + (c + 1);
            auiIndices[index++] = r * cols + (c + 1);
        }
    }

    // create and bind buffers to a vertex array object
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    //Add the following line to generate a VertexArrayObject
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex),
        aoVertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)(sizeof(vec4)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] aoVertices;
}

bool RenderingGeometry::startup() 
{
	// create a basic window
	createWindow("Tutorial - Rendering Geometry", 1024, 768);

    generateGrid(10, 10);

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
    unsigned int indexCount = (m_rows - 1) * (m_cols - 1) * 6;
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());
}