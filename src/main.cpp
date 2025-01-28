#include <OpenGL/OpenGL.h>
#include <iostream>
#include "OpenGL/gl3.h"
#include "comfy_lib.h"

#define GL_SILENCE_DEPRECATION

// #########################################################
// Platform Global Variables
// #########################################################

#ifdef __APPLE__    // If we are compiling on MacOS
#include "platform/platform.h"
#elif _WIN32        // If we are compiling on Windows
#include "platform/windows/platform_windows.h"
#elif __linux__     // If we are compiling on Linux
#include "platform/linux/platform_linux.h"
#endif

int main()
{
    std::cout << "Hello World!" << std::endl;
    if (!platform_create_window(800, 600, "Hello World!"))
    {
        SM_ERROR("Failed to create window");
        return -1;
    } else {
        SM_TRACE("Window created successfully");
    }

    // Check for any errors with opengl
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        SM_ERROR("OpenGL Error: {0}", error);
    } else {
        SM_TRACE("There are no OpenGL errors");
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (running)
    {
        platform_update_window();
        glClear(GL_COLOR_BUFFER_BIT);
        platform_flush_buffers();
    }
    platform_destroy_window();
    return 0;
}
