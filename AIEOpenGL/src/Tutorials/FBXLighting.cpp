#include "FBXLighting.h"
#include "gl_core_4_4.h"

#include "Camera.h"
#include "FBXFile.h"
#include "Gizmos.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

FBXLighting::FBXLighting() :
    m_lightDir(0,1,0)
{

}

FBXLighting::~FBXLighting() {

}

bool FBXLighting::startup() {
    // create a basic window
    createWindow("Tutorial - FBXLighting", 1024, 768);


    m_fbx = new FBXFile();
    m_fbx->load("./data/models/stanford/Bunny.fbx");
    createOpenGLBuffers(m_fbx);

    const char* vsSource = "#version 410\n \
                    layout(location=0) in vec4 Position; \
                    layout(location=1) in vec4 Normal; \
                    out vec4 vNormal; \
                    out vec4 vPosition; \
                    uniform mat4 ProjectionView; \
                    void main() { vNormal = Normal; \
                    vPosition = Position; \
                    gl_Position = ProjectionView*Position; }";

    const char* fsSource = "#version 410\n \
                    in vec4 vNormal; \
                    in vec4 vPosition; \
                    out vec4 FragColor; \
                    uniform vec3 LightDir; \
                    uniform vec3 LightColour; \
                    uniform vec3 CameraPos; \
                    uniform float SpecPow; \
                    void main() { \
                    float d = max(0, \
                    dot(normalize(vNormal.xyz),LightDir ) ); \
                    vec3 E = normalize( CameraPos - \
                    vPosition.xyz );\
                    vec3 R = reflect( -LightDir, \
                    vNormal.xyz ); \
                    float s = max( 0, dot( E, R ) ); \
                    s = pow( s, SpecPow ); \
                    FragColor = vec4( LightColour * d + \
                    LightColour * s, 1); }";




    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
    glCompileShader(fragmentShader);

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    return true;
}

void FBXLighting::shutdown() {
    cleanupOpenGLBuffers(m_fbx);
    glDeleteProgram(m_program);

    // destroy our window properly
    destroyWindow();
}

bool FBXLighting::update(float deltaTime) {

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

void FBXLighting::drawGUI()
{
    ImGui::ColorEdit3("Light Direction", glm::value_ptr(m_lightDir));
}

void FBXLighting::draw() {

    // clear the screen for this frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_program);

    // bind the camera
    int loc = glGetUniformLocation(m_program, "ProjectionView");
    glUniformMatrix4fv(loc, 1, GL_FALSE,
        &(m_camera->getProjectionView()[0][0]));

    int lightDirLoc = glGetUniformLocation(m_program, "LightDir");
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(m_lightDir));

    int lightColourLoc = glGetUniformLocation(m_program, "LightColour");
    glUniform3fv(lightColourLoc, 1, glm::value_ptr(glm::vec3(1, 0, 0)));

    int cameraPosLocation = glGetUniformLocation(m_program, "CameraPos");
    glUniform3fv(cameraPosLocation, 1, glm::value_ptr( glm::vec3(m_camera->getTransform()[3]) ));

    int specularPowerLocation = glGetUniformLocation(m_program, "SpecPow");
    glUniform1f(specularPowerLocation, 5.f);

    // bind our vertex array object and draw the mesh
    for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i) {

        FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);

        unsigned int* glData = (unsigned int*)mesh->m_userData;

        glBindVertexArray(glData[0]);
        glDrawElements(GL_TRIANGLES,
            (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
    }


    // display the 3D gizmos
    Gizmos::draw(m_camera->getProjectionView());
}

void FBXLighting::createOpenGLBuffers(FBXFile* fbx)
{
    // create the GL VAO/VBO/IBO data for each mesh
    for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
    {
        FBXMeshNode* mesh = fbx->getMeshByIndex(i);

        // storage for the opengl data in 3 unsigned int
        unsigned int* glData = new unsigned int[3];

        glGenVertexArrays(1, &glData[0]);
        glBindVertexArray(glData[0]);

        glGenBuffers(1, &glData[1]);
        glGenBuffers(1, &glData[2]);

        glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

        glBufferData(GL_ARRAY_BUFFER,
            mesh->m_vertices.size() * sizeof(FBXVertex),
            mesh->m_vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            mesh->m_indices.size() * sizeof(unsigned int),
            mesh->m_indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
            sizeof(FBXVertex), 0);

        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,
            sizeof(FBXVertex),
            ((char*)0) + FBXVertex::NormalOffset);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        mesh->m_userData = glData;
    }
}

void FBXLighting::cleanupOpenGLBuffers(FBXFile* fbx) {

    // clean up the vertex data attached to each mesh
    for (unsigned int i = 0; i < fbx->getMeshCount(); ++i) {
        FBXMeshNode* mesh = fbx->getMeshByIndex(i);

        unsigned int* glData = (unsigned int*)mesh->m_userData;

        glDeleteVertexArrays(1, &glData[0]);
        glDeleteBuffers(1, &glData[1]);
        glDeleteBuffers(1, &glData[2]);

        delete[] glData;
    }
}



