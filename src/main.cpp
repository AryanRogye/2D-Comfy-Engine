#include <OpenGL/OpenGL.h>
#include <iostream>
#include "input.h"
#include "OpenGL/gl3.h"
#include "comfy_lib.h"


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

#include "gl_renderer.cpp"

int main()
{
    BumpAllocator transientStorage = make_bump_allocator(MB(50));

    // Create A Window
    if (!platform_create_window(1000, 800, "Hello World!"))
    {
        SM_ERROR("Failed to create window");
        return -1;
    } else {
        SM_TRACE("Window created successfully");
    }

    input.screenSizeX = 1000;
    input.screenSizeY = 800;

    // Check for any errors with OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        SM_ERROR("OpenGL Error: {0}", error);
    } else {
        SM_TRACE("There are no OpenGL errors");
    }
    
    gl_init(&transientStorage);

    while (running)
    {
        platform_update_window();
        gl_render();
        platform_flush_buffers();
    }
    platform_destroy_window();
    return 0;
}
