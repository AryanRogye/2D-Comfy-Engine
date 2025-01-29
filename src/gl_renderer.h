#pragma once

#include <OpenGL/gl3.h>
#include "comfy_lib.h"
#include "input.h"
bool gl_init(BumpAllocator* transientStorage);
void gl_render();
