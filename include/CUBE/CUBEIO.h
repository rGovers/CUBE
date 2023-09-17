#ifndef INCLUDED_HEADER_CUBEIO
#define INCLUDED_HEADER_CUBEIO

#include "CUBE/CUBEPath.h"

#ifdef __cplusplus
extern "C" {
#endif

void CUBE_IO_CreateDirectoryP(const CUBE_Path* a_path);
void CUBE_IO_CreateDirectoryNRP(const CUBE_Path* a_path);
void CUBE_IO_CreateDirectoryC(const char* a_path);

void CUBE_IO_CopyFileP(const CUBE_Path* a_source, const CUBE_Path* a_destination);
void CUBE_IO_CopyFileC(const char* a_source, const char* a_destination);

void CUBE_IO_CopyDirectoryP(const CUBE_Path* a_source, const CUBE_Path* a_destination, CBBOOL a_recursive);
void CUBE_IO_CopyDirectoryC(const char* a_source, const char* a_destination, CBBOOL a_recursive);

#ifdef CUBE_IMPLEMENTATION
// #if 1
#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dirent.h>
#include <string.h>
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
void CUBE_IO_CreateDirectoryNRP(const CUBE_Path* a_path)
{
    for (CBUINT32 i = 0; i < a_path->PathCount; ++i)
    {
        CUBE_String path = { 0 };

        for (CBUINT32 j = 0; j <= i; ++j)
        {
            const CUBE_String pathPart = a_path->Path[j];
            if (pathPart.Length == 2 && pathPart.Data[0] == '.' && pathPart.Data[1] == '.')
            {
                CUBE_String_AppendC(&path, "NR/");
            }
            else
            {
                CUBE_String_AppendS(&path, &a_path->Path[j]);
                CUBE_String_AppendC(&path, "/");
            }
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

void CUBE_IO_CopyFileP(const CUBE_Path* a_source, const CUBE_Path* a_destination)
{
    CUBE_String source = CUBE_Path_ToString(a_source);
    CUBE_String destination = CUBE_Path_ToString(a_destination);

    CUBE_IO_CopyFileC(source.Data, destination.Data);

    CUBE_String_Destroy(&source);
    CUBE_String_Destroy(&destination);
}
void CUBE_IO_CopyFileC(const char* a_source, const char* a_destination)
{
    FILE* source = fopen(a_source, "rb");
    FILE* destination = fopen(a_destination, "wb");

    if (source && destination)
    {
        char buffer[4096];
        size_t read = 0;
        while ((read = fread(buffer, 1, sizeof(buffer), source)) > 0)
        {
            fwrite(buffer, 1, read, destination);
        }
    }   
}

void CUBE_IO_CopyDirectoryP(const CUBE_Path* a_source, const CUBE_Path* a_destination, CBBOOL a_recursive)
{
    CUBE_String source = CUBE_Path_ToString(a_source);
    CUBE_String destination = CUBE_Path_ToString(a_destination);

    CUBE_IO_CopyDirectoryC(source.Data, destination.Data, a_recursive);

    CUBE_String_Destroy(&source);
    CUBE_String_Destroy(&destination);
}
void CUBE_IO_CopyDirectoryC(const char* a_source, const char* a_destination, CBBOOL a_recursive)
{
#if WIN32

#else
    DIR* dir = opendir(a_source);

    if (dir != CBNULL)
    {
        struct dirent* entry = readdir(dir);
        while (entry != CBNULL)
        {
            if (entry->d_type == DT_DIR)
            {
                if (a_recursive)
                {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                    {
                        CUBE_Path source = CUBE_Path_CreateC(a_source);
                        CUBE_Path nextSource = CUBE_Path_CombineC(&source, entry->d_name);
                        CUBE_Path destination = CUBE_Path_CreateC(a_destination);
                        CUBE_Path nextDestination = CUBE_Path_CombineC(&destination, entry->d_name);

                        CUBE_IO_CreateDirectoryP(&nextDestination);
                        CUBE_IO_CopyDirectoryP(&nextSource, &nextDestination, CBTRUE);    

                        CUBE_Path_Destroy(&source);
                        CUBE_Path_Destroy(&nextSource);
                        CUBE_Path_Destroy(&destination); 
                        CUBE_Path_Destroy(&nextDestination);                        
                    }                
                }
            }
            else if (entry->d_type == DT_REG)
            {
                CUBE_Path source = CUBE_Path_CreateC(a_source);
                CUBE_Path fileSource = CUBE_Path_CombineC(&source, entry->d_name);
                CUBE_Path destination = CUBE_Path_CreateC(a_destination);
                CUBE_Path fileDestination = CUBE_Path_CombineC(&destination, entry->d_name);

                CUBE_IO_CopyFileP(&fileSource, &fileDestination);

                CUBE_Path_Destroy(&source);
                CUBE_Path_Destroy(&fileSource);
                CUBE_Path_Destroy(&destination);
                CUBE_Path_Destroy(&fileDestination);
            }

            entry = readdir(dir);
        }      

        closedir(dir);  
    }    
#endif
}

#endif

#ifdef __cplusplus
}
#endif

#endif