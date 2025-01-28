#pragma once

#include <cstdarg>
#include <cstdio>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/_types/_filesec_t.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <format>

// #########################################################
// Defines
// #########################################################
#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#elif __linux__
#define DEBUG_BREAK() __builtin_debugtrap()
#elif __APPLE__
#define DEBUG_BREAK() __builtin_trap()
#endif

// #########################################################
// Logging
// #########################################################

enum TextColor
{
    TEXT_COLOR_BLACK,
    TEXT_COLOR_RED,
    TEXT_COLOR_GREEN,
    TEXT_COLOR_YELLOW,
    TEXT_COLOR_BLUE,
    TEXT_COLOR_MAGENTA,
    TEXT_COLOR_CYAN,
    TEXT_COLOR_WHITE,
    TEXT_COLOR_BRIGHT_BLACK,
    TEXT_COLOR_BRIGHT_RED,
    TEXT_COLOR_BRIGHT_GREEN,
    TEXT_COLOR_BRIGHT_YELLOW,
    TEXT_COLOR_BRIGHT_BLUE,
    TEXT_COLOR_BRIGHT_MAGENTA,
    TEXT_COLOR_BRIGHT_CYAN,
    TEXT_COLOR_BRIGHT_WHITE,
    TEXT_COLOR_COUNT
};

template <typename... Args>
void _log(const std::string& prefix,const std::string& msg,TextColor textColor,Args... args)
{
    static const std::string TextColorTable[TEXT_COLOR_COUNT] =
    {
        "\033[0;30m", // TEXT_COLOR_BLACK
        "\033[0;31m", // TEXT_COLOR_RED
        "\033[0;32m", // TEXT_COLOR_GREEN
        "\033[0;33m", // TEXT_COLOR_YELLOW
        "\033[0;34m", // TEXT_COLOR_BLUE
        "\033[0;35m", // TEXT_COLOR_MAGENTA
        "\033[0;36m", // TEXT_COLOR_CYAN
        "\033[0;37m", // TEXT_COLOR_WHITE
        "\033[1;30m", // TEXT_COLOR_BRIGHT_BLACK
        "\033[1;31m", // TEXT_COLOR_BRIGHT_RED
        "\033[1;32m", // TEXT_COLOR_BRIGHT_GREEN
        "\033[1;33m", // TEXT_COLOR_BRIGHT_YELLOW
        "\033[1;34m", // TEXT_COLOR_BRIGHT_BLUE
        "\033[1;35m", // TEXT_COLOR_BRIGHT_MAGENTA
        "\033[1;36m", // TEXT_COLOR_BRIGHT_CYAN
        "\033[1;37m"  // TEXT_COLOR_BRIGHT_WHITE
    };
    // Prepare Format String
    std::ostringstream formatBuffer;
    formatBuffer << TextColorTable[textColor] << " " << prefix + " " << msg << " \033[0m\n";
    // Format the String to use variadic arguments
    char textBuffer[8192];
    snprintf(textBuffer, sizeof(textBuffer), formatBuffer.str().c_str(), args ...);
    // Print the formatted string
    puts(textBuffer);
}

#define SM_TRACE(msg, ...) _log("TRACE: ", msg, TEXT_COLOR_GREEN, ##__VA_ARGS__)
#define SM_WARN(msg, ...) _log("WARN: ", msg, TEXT_COLOR_YELLOW, ##__VA_ARGS__)
#define SM_ERROR(msg, ...) _log("ERROR: ", msg, TEXT_COLOR_RED, ##__VA_ARGS__)
#define SM_ASSERT(x, msg, ...)              \
{                                           \
    if(!(x))                                \
    {                                       \
        SM_ERROR(msg, ##__VA_ARGS__);       \
        DEBUG_BREAK();                      \
        SM_ERROR("Asserting HIT!");         \
    }                                       \
}


// #########################################################
// Bump Allocator
// #########################################################
struct BumpAllocator
{
    size_t capacity;
    size_t used;
    char* memory;
};

BumpAllocator make_bump_allocator(size_t size)
{
    BumpAllocator ba = {};
    ba.memory = (char*)malloc(size);
    if(ba.memory)
    {
        ba.capacity = size;
        memset(ba.memory, 0, size);
    } else {
        SM_ASSERT(false, "Failed to allocate memory for BumpAllocator");
    }
    return ba;
}

char* bump_alloc(BumpAllocator* bumpAllocator, size_t size)
{
    char* result = nullptr;
    size_t allignedSize = (size + 7) & ~7;
    if(bumpAllocator->used + allignedSize <= bumpAllocator->capacity)
    {
        result = bumpAllocator->memory + bumpAllocator->used;
        bumpAllocator->used += allignedSize;
    }
    else
    {
        SM_ASSERT(false, "BumpAllocator out of memory");
    }

    return result;
}

// #########################################################
// File I/O
// #########################################################
long long get_timestamp(char *file)
{
    struct stat file_stat = {};
    stat(file, &file_stat);
    return file_stat.st_mtime;
}

bool file_exists(char* filepath)
{
    SM_ASSERT(filepath, "NO FILE PATH PROVIDED");

    FILE* file = fopen(filepath, "rb");
    if(!file)
    {
        return false;
    }
    fclose(file);
    return true;
}

long get_file_size(char* filePath)
{
    SM_ASSERT(filePath, "NO FILE PATH PROVIDED");

    long fileSize = 0;
    FILE* file = fopen(filePath, "rb");
    if (!file)
    {
        SM_ERROR("Failed to open file: %s", filePath);
        return 0;
    }
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    fclose(file);

    return fileSize;
}

char* read_file(char* filePath, int* fileSize, char* buffer)
{
    SM_ASSERT(filePath, "NO FILE PATH PROVIDED");
    SM_ASSERT(fileSize, "NO FILE SIZE PROVIDED");
    SM_ASSERT(buffer, "NO BUFFER PROVIDED");

    *fileSize = 0;
    FILE* file = fopen(filePath, "rb");
    if(!file)
    {
        SM_ERROR("Failed to open file: %s", filePath);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    *fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    memset(buffer, 0, *fileSize + 1);
    fread(buffer, sizeof(char), * fileSize, file);
    fclose(file);

    return buffer;
}

void write_file(char* filePath, char* buffer, int size)
{
    SM_ASSERT(filePath, "NO FILE PATH PROVIDED");
    SM_ASSERT(buffer, "NO BUFFER PROVIDED");

    FILE* file = fopen(filePath, "wb");
    if(!file)
    {
        SM_ERROR("Failed to open file: %s", filePath);
        return;
    }

    fwrite(buffer, sizeof(char), size, file);
    fclose(file);
}

bool copy_file(char* filename, char* outputName, char* buffer)
{
    SM_ASSERT(filename, "NO FILE PATH PROVIDED");
    SM_ASSERT(outputName, "NO OUTPUT FILE PATH PROVIDED");
    SM_ASSERT(buffer, "NO BUFFER PROVIDED");

    int fileSize = 0;
    char* data = read_file(filename, &fileSize, buffer);
    FILE* outputFile = fopen(outputName, "wb");
    if(!outputFile)
    {
        SM_ERROR("Failed to open file: %s", outputName);
        return false;
    }
    int result = fwrite(data, sizeof(char), fileSize, outputFile);
    if (!result)
    {
        SM_ERROR("Failed to write to file: %s", outputName);
        return false;
    }
    fclose(outputFile);
    return true;
}
bool copy_file(char* fileName, char*outputName, BumpAllocator* bumpAllocator)
{
    SM_ASSERT(fileName, "NO FILE PATH PROVIDED");
    SM_ASSERT(outputName, "NO OUTPUT FILE PATH PROVIDED");
    SM_ASSERT(bumpAllocator, "NO BUMP ALLOCATOR PROVIDED");

    int fileSize = 0;
    long fileSize2 = get_file_size(fileName);

    if (fileSize2)
    {
        char* buffer = bump_alloc(bumpAllocator, fileSize2 + 1);
        return copy_file(fileName, outputName, buffer);
    }

    return false;
}

