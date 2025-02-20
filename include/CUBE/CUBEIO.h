// CUBE - Header only C/C++/C# build system.
// 
// License at end of file.

#ifndef INCLUDED_HEADER_CUBEIO
#define INCLUDED_HEADER_CUBEIO

#include "CUBE/CUBEPath.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
    CBTIME CreationTime;
    CBTIME AccessTime;
    CBTIME WriteTime;
} CUBE_Stat;

void CUBE_IO_CreateDirectoryP(const CUBE_Path* a_path);
void CUBE_IO_CreateDirectoryNRP(const CUBE_Path* a_path);
void CUBE_IO_CreateDirectoryC(const char* a_path);

void CUBE_IO_CHMODP(const CUBE_Path* a_path, CBUINT32 a_mode);
void CUBE_IO_CHMODC(const char* a_path, CBUINT32 a_mode);

void CUBE_IO_CopyFileP(const CUBE_Path* a_source, const CUBE_Path* a_destination);
void CUBE_IO_CopyFileC(const char* a_source, const char* a_destination);

void CUBE_IO_CopyDirectoryP(const CUBE_Path* a_source, const CUBE_Path* a_destination, CBBOOL a_recursive);
void CUBE_IO_CopyDirectoryC(const char* a_source, const char* a_destination, CBBOOL a_recursive);

CBBOOL CUBE_IO_WriteFileP(const CUBE_Path* a_path, const char* a_data, CBUINT32 a_dataSize);
CBBOOL CUBE_IO_WriteFileC(const char* a_path, const char* a_data, CBUINT32 a_dataSize);

CUBE_Stat CUBE_IO_StatP(const CUBE_Path* a_path);
CUBE_Stat CUBE_IO_StatC(const char* a_path);

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
#include <unistd.h>
#endif

#if WIN32
CBTIME CUBEI_IO_WinFileToUnixTime(FILETIME a_time);
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

void CUBE_IO_CHMODP(const CUBE_Path* a_path, CBUINT32 a_mode)
{
    CUBE_String path = CUBE_Path_ToString(a_path);
    
    CUBE_IO_CHMODC(path.Data, a_mode);

    CUBE_String_Destroy(&path);
}
void CUBE_IO_CHMODC(const char* a_path, CBUINT32 a_mode)
{
#if WIN32
    
#else
    chmod(a_path, a_mode);
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

        fclose(source);
        fclose(destination);
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
            else if (entry->d_type == DT_LNK)
            {
                CUBE_Path destination = CUBE_Path_CreateC(a_destination);
                CUBE_Path fileDestination = CUBE_Path_CombineC(&destination, entry->d_name);
                CUBE_String fileDestinationString = CUBE_Path_ToString(&fileDestination);
                
                CUBE_Path source = CUBE_Path_CreateC(a_source);
                CUBE_Path fileSource = CUBE_Path_CombineC(&source, entry->d_name);
                CUBE_String fileSourceString = CUBE_Path_ToString(&fileSource);

                char link[1024];
                ssize_t linkSize = readlink(fileSourceString.Data, link, sizeof(link));

                if (linkSize > 0)
                {
                    link[linkSize] = '\0';

                    symlink(link, fileDestinationString.Data);
                }

                CUBE_String_Destroy(&fileSourceString);
                CUBE_Path_Destroy(&source);
                CUBE_Path_Destroy(&fileSource);

                CUBE_Path_Destroy(&destination);
                CUBE_Path_Destroy(&fileDestination);
                CUBE_String_Destroy(&fileDestinationString);
            }

            entry = readdir(dir);
        }      

        closedir(dir);  
    }    
#endif
}

CBBOOL CUBE_IO_WriteFileP(const CUBE_Path* a_path, const char* a_data, CBUINT32 a_dataSize)
{
    CUBE_String path = CUBE_Path_ToString(a_path);

    CBBOOL ret = CUBE_IO_WriteFileC(path.Data, a_data, a_dataSize);

    CUBE_String_Destroy(&path);

    return ret;
}
CBBOOL CUBE_IO_WriteFileC(const char* a_path, const char* a_data, CBUINT32 a_dataSize)
{
    FILE* fp = fopen(a_path, "wb");

    if (fp == CBNULL)
    {
        return CBFALSE;
    }

    fwrite(a_data, 1, a_dataSize, fp);

    fclose(fp);

    return CBTRUE;
}

CUBE_Stat CUBE_IO_StatP(const CUBE_Path* a_path)
{
    CUBE_String path = CUBE_Path_ToString(a_path);

    CUBE_Stat stat = CUBE_IO_StatC(path.Data);

    CUBE_String_Destroy(&path);

    return stat;
}

#ifdef WIN32
CBTIME CUBEI_IO_WinFileToUnixTime(FILETIME a_time)
{
    const static CBUINT64 WinTick = 10000000
    const static CBUINT64 UnixEpoch = 11644473600;

    return (CBTIME)(a_time / WinTick - UnixEpoch);
}
#endif

CUBE_Stat CUBE_IO_StatC(const char* a_path)
{
    CUBE_Stat s = { 0 };

#if WIN32
    const char* slider = a_path;
    while (*slider != 0)
    {
        ++slider;
    }

    // WIN32 API so copy on pass because it is a piece of shit that does not respect const
    // *cough* CreateProcess *cough*
    CBUINT64 size = (CBUINT64)(slider - a_path);
    char* str = malloc(size + 1);
    for (CBUINT64 i = 0; i < size; ++i)
    {
        str[i] = a_path[i];
    }
    str[size] = 0;

    HANDLE handle = CreateFileA(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

    free(str);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return s;
    }

    FILETIME creationTime;
    FILETIME lastAccessTime;
    FILETIME lastWriteTime;
    const BOOL success = GetFileTime(handle, &creationTime, &lastAccessTime, &lastWriteTime);

    CloseFile(handle);

    if (success)
    {
        // Want everything in Unix time for consistency
        s.CreationTime = CUBEI_IO_WinFileToUnixTime(creationTime);
        s.AccessTime = CUBEI_IO_WinFileToUnixTime(lastAccessTime);
        s.WriteTime = CUBEI_IO_WinFileToUnixTime(lastWriteTime);
    }
#else
    struct stat us;
    if (stat(a_path, &us) < 0)
    {
        perror("stat");

        return s;
    }

    s.CreationTime = (CBTIME)us.st_ctime;
    s.AccessTime = (CBTIME)us.st_atime;
    s.WriteTime = (CBTIME)us.st_mtime;
#endif

    return s;
}

#endif

#ifdef __cplusplus
}
#endif

#endif

// MIT License
// 
// Copyright (c) 2025 River Govers
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
