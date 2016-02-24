#include "BaseApplication.h"

#include "Camera.h"
#include "Gizmos.h"
#include "gl_core_4_4.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw_gl3.h"

#include <assert.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

BaseApplication::~BaseApplication() 
{
    assert(m_window == nullptr && "destroyWindow has not been called.");
}


bool BaseApplication::createWindow(const char* title, int width, int height) {
    assert(m_window == nullptr);


    if (glfwInit() == GL_FALSE)
        return false;

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h){ glViewport(0, 0, w, h); });

    auto major = ogl_GetMajorVersion();
    auto minor = ogl_GetMinorVersion();
    std::cout << "GL: " << major << "." << minor << std::endl;

    glClearColor(0.25f, 0.25f, 0.25f, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // create a camera
    m_camera = new Camera(glm::radians(45.f), width / (float)height, 0.1f, 1000.f);
    m_camera->setLookAtFrom(glm::vec3(10, 10, 10), glm::vec3(0));

    // start the gizmo system that can draw basic shapes
    Gizmos::create();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = (float)width;
    io.DisplaySize.y = (float)height;
    ImGui_ImplGlfwGL3_Init(m_window, true);

    return true;
}

void BaseApplication::destroyWindow() 
{
    ImGui_ImplGlfwGL3_Shutdown();

    // delete our camera and cleanup gizmos
    delete m_camera;
    Gizmos::destroy();

    glfwDestroyWindow(m_window);
    glfwTerminate();
    m_window = nullptr;
}

void BaseApplication::run() {

    double prevTime = glfwGetTime();
    double currTime = 0;

    while (currTime = glfwGetTime(),
        update((float)(currTime - prevTime))) {

        ImGui_ImplGlfwGL3_NewFrame();

        glfwPollEvents();
        draw();
        drawGUI();

        ImGui::Render();

        glfwSwapBuffers(m_window);

        prevTime = currTime;
    }
}
