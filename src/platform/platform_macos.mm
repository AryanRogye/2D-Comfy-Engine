#include <Cocoa/Cocoa.h>
#include <OpenGL/OpenGL.h>
#include <AppKit/AppKit.h>
#include <__config>
#include "OpenGL/gl3.h"

#include "platform.h"


// Global Variable for tracking window state
bool running = true;
void* window;
void* glContext; // Open GL Context

// Create the macOS window
bool platform_create_window(int width, int height, const char* title)
{
    @autoreleasepool {
        // Create the shared application instance (macOS equivalent of an event loop)
        [NSApplication sharedApplication];

        // Define the window's dimensions and style
        NSRect rect = NSMakeRect(100, 100, width, height);
        NSWindow* nsWindow = [[NSWindow alloc] initWithContentRect:rect
            styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable)
            backing:NSBackingStoreBuffered
            defer:NO
        ];

        if (!nsWindow)
        {
            NSLog(@"Failed to create NSWindow");
            return false;
        }

        // Set the window title
        [nsWindow setTitle:[NSString stringWithUTF8String:title]];
        // Show the window
        [nsWindow makeKeyAndOrderFront:nil];
        // Ensure app stays in foreground
        [NSApp activateIgnoringOtherApps:YES];
        
        // Setup Pixel Format
        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAAccelerated,
            0
        };
        NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        if (!pixelFormat) 
        {
            NSLog(@"Failed to create OpenGL pixel format");
            return false;
        }

        NSOpenGLContext* nsGlContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
        if (!nsGlContext) 
        {
            NSLog(@"Failed to create OpenGL context");
            return false;
        }
        
        // [glContext NSOpenGLView:[window contextView]];
        [nsGlContext setView:[nsWindow contentView]];
        [nsGlContext makeCurrentContext];

        // Set the global variables
        window = (void*)nsWindow;
        glContext = (void*)nsGlContext;

        // Set ViewPort
        glViewport(0, 0, width, height);

        NSLog(@"OpenGL context created successfully!");
        return true;  // Success
    }
}

// Process macOS events
void platform_update_window()
{
    @autoreleasepool {
        NSEvent* event;
        while ((event = [
                NSApp nextEventMatchingMask:NSEventMaskAny
                untilDate:[NSDate distantPast]
                inMode:NSDefaultRunLoopMode
                dequeue:YES
        ]))
        {
            [NSApp sendEvent:event];
            if ([event type] == NSEventTypeApplicationDefined && [event subtype] == 0) {
                running = false;
            }
        }
    }
}

// Destroy the macOS window
void platform_destroy_window()
{
    @autoreleasepool {
        NSOpenGLContext* nsGlContext = (NSOpenGLContext*)glContext;
        NSWindow* nsWindow = (NSWindow*)window;

        if (nsGlContext)
        {
            [nsGlContext clearDrawable];
            glContext = nil;
        }
        if (nsWindow)
        {
            [nsWindow close];
            window = nil;
        }
    }
    // Terminate the running application
    [NSApp terminate:nil];
}

void* platform_load_gl_function(char* funName) 
{
    return nullptr;
}

void platform_flush_buffers()
{
    @autoreleasepool {
        if (glContext)
        {
            NSOpenGLContext* nsGlContext = (NSOpenGLContext*)glContext;
            [nsGlContext flushBuffer];
        }
    }
}
