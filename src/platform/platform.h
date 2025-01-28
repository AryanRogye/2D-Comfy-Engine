#pragma once

// #########################################################
// Windows Platform Functions
// #########################################################
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

// #########################################################
// Windows Globals
// #########################################################
#ifdef _WIN32
static HWND window;
#endif

// #########################################################
// Platform Functions
// #########################################################
bool platform_create_window(int width, int height, const char* title);
void platform_update_window();
void platform_destroy_window();
void* platform_load_gl_function(char* funName);
void platform_flush_buffers();

extern bool running;
