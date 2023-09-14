#ifndef INCLUDED_HEADER_CUBEIO
#define INCLUDED_HEADER_CUBEIO

#include "CUBE/CUBEPath.h"

#ifdef __cplusplus
extern "C" {
#endif

void CUBE_IO_CreateDirectoryP(const CUBE_Path* a_path);
void CUBE_IO_CreateDirectoryC(const char* a_path);

#ifdef CUBE_IMPLEMENTATION
// #if 1
#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

void CUBE_IO_CreateDirectoryP(const CUBE_Path* a_path)
{
    for (CBUINT32 i = 0; i < a_path->PathCount; ++i)
    {
        CUBE_String path = { 0 };

        for (CBUINT32 j = 0; j <= i; ++j)
        {
            CUBE_String_AppendS(&path, &a_path->Path[j]);
            CUBE_String_AppendC(&path, "/");
        }

        CUBE_IO_CreateDirectoryC(path.Data);

        CUBE_String_Destroy(&path);
    }
}
void CUBE_IO_CreateDirectoryC(const char* a_path)
{
#if WIN32
    CreateDirectoryA(a_path, NULL);
#else
    struct stat st = { 0 };
    if (stat(a_path, &st) == -1)
    {
        mkdir(a_path, 0700);
    }
#endif
}

#endif

#ifdef __cplusplus
}
#endif

#endif