#include "gl_renderer.h"
#include <OpenGL/OpenGL.h>

// #########################################################
// OpenGL Structs
// #########################################################
struct GLContext
{
    GLuint programID;
};

// #########################################################
// OpenGl Globals
// #########################################################
static GLContext glContext;

// #########################################################
// OpenGl Functions
// #########################################################
GLuint VAO;
bool gl_init(BumpAllocator* transientStorage) {
    const char* version = (const char*)glGetString(GL_VERSION);
    SM_TRACE("OpenGL Version: %s", version);

    GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    int fileSize = 0;
    char* vertShader = read_file(const_cast<char*>("assets/shaders/quad.vert"), &fileSize, transientStorage);
    char* fragShader = read_file(const_cast<char*>("assets/shaders/quad.frag"), &fileSize, transientStorage);

    if (!vertShader || !fragShader) {
        SM_ASSERT(false, "Failed to read shader files");
        return false;
    }

    glShaderSource(vertShaderID, 1, &vertShader, 0);
    glShaderSource(fragShaderID, 1, &fragShader, 0);

    glCompileShader(vertShaderID);
    glCompileShader(fragShaderID);

    {
        int success;
        char shaderLog[2048] = {};
        glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragShaderID, 2048, 0, shaderLog);
            SM_ASSERT(false, "Failed to compile vertex shader: %s", shaderLog);
        }
    }
    {
        int success;
        char shaderLog[2048] = {};
        glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertShaderID, 2048, 0, shaderLog);
            SM_ASSERT(false, "Failed to compile vertex shader: %s", shaderLog);
        }
    }
        
    glContext.programID = glCreateProgram(); 
    glAttachShader(glContext.programID, vertShaderID);
    glAttachShader(glContext.programID, fragShaderID);

    glLinkProgram(glContext.programID);

    glDetachShader(glContext.programID, vertShaderID);
    glDetachShader(glContext.programID, fragShaderID);
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);
    
    // This has to be done even though im not using vertex arrays
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glUseProgram(glContext.programID);
    
    return true;

}

void gl_render()
{
    glClearDepth(1.0f);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, input.screenSizeX, input.screenSizeY);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
