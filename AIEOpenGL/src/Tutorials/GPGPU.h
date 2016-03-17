#pragma once
#include <CL\cl.h>
#include <glm\vec4.hpp>

class GPGPU  {
public:
    GPGPU();
    ~GPGPU();

    void runCPU();
    void runGPU();

private:
    static const int VECTOR_COUNT = 5000000;

    glm::vec4*          m_pVectors;
};