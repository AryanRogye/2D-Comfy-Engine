#include <Cocoa/Cocoa.h>
#include <OpenGL/OpenGL.h>
#include <AppKit/AppKit.h>
#include <__config>
#include "OpenGL/gl3.h"

#include "platform.h"


// Global Variable for tracking window state
bool running = true;
NSWindow* window;
NSOpenGLContext* glContext; // Open GL Context

// Create the macOS window
bool platform_create_window(int width, int height, const char* title)
{
    @autoreleasepool {
        // Create the shared application instance (macOS equivalent of an event loop)
        [NSApplication sharedApplication];

        // Define the window's dimensions and style
        NSRect rect = NSMakeRect(100, 100, width, height);
        window = [[NSWindow alloc] initWithContentRect:rect
            styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable)
            backing:NSBackingStoreBuffered
            defer:NO
        ];

        if (!window)
        {
            NSLog(@"Failed to create NSWindow");
            return false;
        }

        // Set the window title
        [window setTitle:[NSString stringWithUTF8String:title]];
        // Show the window
        [window makeKeyAndOrderFront:nil];
        // Ensure app stays in foreground
        [NSApp activateIgnoringOtherApps:YES];
        
        // Setup Pixel Format
        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
            // cannot be 32 on a macOS because color buffer is split into components
            // and NSOpenGLPFAColorSize doesnt include alpha channel in its value
            // it only counts the red, green and blue channels 3 x 8 = 24 so stupid
            NSOpenGLPFAColorSize, 24, 
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAAccelerated,
            0
        };
        // Create Pixel Format
        NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        if (!pixelFormat) 
        {
            NSLog(@"Failed to create OpenGL pixel format");
            return false;
        }
        
        // Create OpenGL Context
        glContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
        if (!glContext) 
        {
            NSLog(@"Failed to create OpenGL context");
            return false;
        }
        
        // Create NSOpenGLView
        NSOpenGLView* glView = [[NSOpenGLView alloc] initWithFrame:rect pixelFormat:pixelFormat];
        if (!glView)
        {
            NSLog(@"Failed to create OpenGL view");
            return false;
        }
    
        // Set the OpenGL context for the NSOpenGLView
        [glView setOpenGLContext:glContext];
        // Set the NSOpenGLView as the content view of the NSWindow
        [window setContentView:glView];
        // Make the OpenGL context the current context
        [glContext makeCurrentContext];

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
        if (glContext)
        {
            [glContext clearDrawable];
            glContext = nil;
        }
        if (window)
        {
            [window close];
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
            [glContext flushBuffer];
        }
    }
}
