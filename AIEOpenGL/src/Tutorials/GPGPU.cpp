#include "GPGPU.h"
#include "Timer.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <cstdio>

GPGPU::GPGPU() :
    m_pVectors( new glm::vec4[VECTOR_COUNT] )
{
    for (int i = 0; i < VECTOR_COUNT; i++ )
        m_pVectors[i] = glm::vec4(1);
}

GPGPU::~GPGPU()
{

}

void GPGPU::runCPU() 
{
    Timer timer("CPU");

    for (int i = 0; i < VECTOR_COUNT; i++)
        m_pVectors[i] = glm::normalize(m_pVectors[i]);
}


void GPGPU::runGPU()
{
    cl_platform_id platform;
    cl_device_id device;

    // get the first platform
    cl_int result = clGetPlatformIDs(1, &platform, nullptr);
    if (result != CL_SUCCESS)
        printf("clGetPlatformIDs failed: %i\n", result);

    // get the first device on the platform (default is GPU)
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, nullptr);
    if (result != CL_SUCCESS)
        printf("clGetDeviceIDs failed: %i\n", result);

    cl_context context;

    // create a context for a device on the specified platform
    cl_context_properties contextProperties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0 };

    context = clCreateContext(contextProperties, 1, &device, nullptr, nullptr, &result);
    if (result != CL_SUCCESS)
        printf("clCreateContext failed: %i\n", result);

    cl_command_queue queue;

    // create a command queue to process commands
    queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &result);
    if (result != CL_SUCCESS)
        printf("clCreateCommandQueue failed: %i\n", result);

    // example kernel source code
    const char* kernelSource = "__kernel void normalize_vec4( \
        __global float4* vectors) { \
			int i = get_global_id(0); \
			vectors[i] = normalize(vectors[i]); \
		}";
    unsigned int kernelSize = strlen(kernelSource);

    cl_program program;

    // build program from our source using the device context
    program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, &kernelSize, &result);
    if (result != CL_SUCCESS)
        printf("clCreateProgramWithSource failed: %i\n", result);

    result = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    if (result != CL_SUCCESS)
        printf("clBuildProgram failed: %i\n", result);


    cl_kernel kernel;

    // extract the kernel
    kernel = clCreateKernel(program, "normalize_vec4", &result);
    if (result != CL_SUCCESS)
        printf("clCreateKernel failed: %i\n", result);

    cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * VECTOR_COUNT, m_pVectors, &result);
    if (result != CL_SUCCESS)
        printf("clCreateBuffer failed: %i\n", result);

    // set the buffer as the first argument for our kernel
    result = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
    if (result != CL_SUCCESS)
        printf("clSetKernelArg failed: %i\n", result);



    {
        Timer timer("GPU");

        // process the kernel and give it an event ID
        cl_event eventID = 0;

        // we'll give it a size equal to the number of vec4's to process
        size_t globalWorkSize[] = { VECTOR_COUNT };


        result = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, globalWorkSize, nullptr,  0, nullptr, &eventID);

        if (result != CL_SUCCESS)
            printf("clEnqueueNDRangeKernel failed: %i\n", result);

        // read back the processed data but wait for an event to complete
        result = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0,
            sizeof(glm::vec4) * VECTOR_COUNT, m_pVectors,
            1, &eventID, nullptr);
        if (result != CL_SUCCESS)
            printf("clEnqueueReadBuffer failed: %i\n", result);

        // finish all opencl commands
        clFlush(queue);
        clFinish(queue);

        // how long did each event take?
        // get start / end profile data for the event
        cl_ulong clstartTime = 0;
        result = clGetEventProfilingInfo(eventID, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &clstartTime, nullptr);

        if (result != CL_SUCCESS)
            printf("clGetEventProfilingInfo failed: %i\n", result);


        cl_ulong clendTime = 0;
        result = clGetEventProfilingInfo(eventID,
            CL_PROFILING_COMMAND_END,
            sizeof(cl_ulong), &clendTime, nullptr);

        if (result != CL_SUCCESS)
            printf("clGetEventProfilingInfo failed: %i\n", result);

        // return time is in nanoseconds, so convert to seconds
        double clTime = (clendTime - clstartTime) * 1.0e-9;
        printf("GPU duration: %f\n", clTime);
    }

    clReleaseMemObject(buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

