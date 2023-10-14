// CUBE - Header only C/C++/C# build system.
// 
// License at end of file.

#ifndef INCLUDED_HEADER_CUBECSPROJ
#define INCLUDED_HEADER_CUBECSPROJ

#include "CUBE/CUBECommandLine.h"
#include "CUBE/CUBECProject.h"
#include "CUBE/CUBEString.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CUBE_CSProjectTarget_Exe,
    CUBE_CSProjectTarget_WinExe,
    CUBE_CSProjectTarget_Library
} e_CUBE_CSProjectTarget;

typedef struct 
{
    CUBE_StackString Name;
    e_CUBE_CSProjectTarget Target;
    CUBE_Path OutputPath;
    CUBE_Path* IncludePaths;
    CBUINT32 IncludePathCount;
    CUBE_Path* Sources;
    CBUINT32 SourceCount;
    CUBE_String* References;
    CBUINT32 ReferenceCount;
    CBBOOL Optimise;
} CUBE_CSProject;

void CUBE_CSProject_Destroy(CUBE_CSProject* a_project);

void CUBE_CSProject_AppendSource(CUBE_CSProject* a_project, const char* a_source);
void CUBE_CSProject_AppendIncludePath(CUBE_CSProject* a_project, const char* a_includePath);
void CUBE_CSProject_AppendReference(CUBE_CSProject* a_project, const char* a_reference);

CBBOOL CUBE_CSProject_PreProcessCompile(const CUBE_CSProject* a_project, const char* a_workingPath, const char* a_cscPath, e_CUBE_CProjectCompiler a_preProcessor, const char* a_preprocessorPath, CUBE_String** a_lines, CBUINT32* a_lineCount);
CBBOOL CUBE_CSProject_Compile(const CUBE_CSProject* a_project, const char* a_workingPath, const char* a_cscPath, CUBE_String** a_lines, CBUINT32* a_lineCount);

#ifdef CUBE_IMPLEMENTATION
// #if 1

void CUBE_CSProject_Destroy(CUBE_CSProject* a_project)
{
    CUBE_StackString_Destroy(&a_project->Name);
    
    a_project->Target = CUBE_CSProjectTarget_Exe;

    CUBE_Path_Destroy(&a_project->OutputPath);

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        CUBE_Path_Destroy(&a_project->Sources[i]);
    }

    free(a_project->Sources);

    a_project->SourceCount = 0;
    a_project->Sources = CBNULL;

    for (CBUINT32 i = 0; i < a_project->IncludePathCount; ++i)
    {
        CUBE_Path_Destroy(&a_project->IncludePaths[i]);
    }

    free(a_project->IncludePaths);

    a_project->IncludePathCount = 0;
    a_project->IncludePaths = CBNULL;

    for (CBUINT32 i = 0; i < a_project->ReferenceCount; ++i)
    {
        CUBE_String_Destroy(&a_project->References[i]);
    }

    free(a_project->References);

    a_project->ReferenceCount = 0;
    a_project->References = CBNULL;
}

void CUBE_CSProject_AppendSource(CUBE_CSProject* a_project, const char* a_source)
{
    const CBUINT32 sourceCount = a_project->SourceCount++;

    CUBE_Path source = CUBE_Path_CreateC(a_source);

    a_project->Sources = (CUBE_Path*)realloc(a_project->Sources, sizeof(CUBE_Path) * a_project->SourceCount);

    a_project->Sources[sourceCount] = source;
}
void CUBE_CSProject_AppendIncludePath(CUBE_CSProject* a_project, const char* a_includePath)
{
    const CBUINT32 includePathCount = a_project->IncludePathCount++;

    CUBE_Path includePath = CUBE_Path_CreateC(a_includePath);

    a_project->IncludePaths = (CUBE_Path*)realloc(a_project->IncludePaths, sizeof(CUBE_Path) * a_project->IncludePathCount);

    a_project->IncludePaths[includePathCount] = includePath;
}
void CUBE_CSProject_AppendReference(CUBE_CSProject* a_project, const char* a_reference)
{
    const CBUINT32 referenceCount = a_project->ReferenceCount++;

    CUBE_String reference = CUBE_String_CreateC(a_reference);

    a_project->References = (CUBE_String*)realloc(a_project->References, sizeof(CUBE_String) * a_project->ReferenceCount);

    a_project->References[referenceCount] = reference;
}

CBBOOL CUBE_CSProject_PreProcessCompile(const CUBE_CSProject* a_project, const char* a_workingPath, const char* a_cscPath, e_CUBE_CProjectCompiler a_preProcessor, const char* a_preprocessorPath, CUBE_String** a_lines, CBUINT32* a_lineCount)
{
    CUBE_Path preprocOutPath = CUBE_Path_CombineC(&a_project->OutputPath, "pp");

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        CUBE_CommandLine commandLine = { 0 };

        const CUBE_Path source = a_project->Sources[i];

        CUBE_Path parentPath = CUBE_Path_ParentPath(&source);
        CUBE_Path outPath = CUBE_Path_CombineP(&preprocOutPath, &parentPath);

        if (a_workingPath != CBNULL)
        {
            CUBE_String_AppendC(&commandLine.Path, a_workingPath);

            CUBE_Path workingPath = CUBE_Path_CreateC(a_workingPath);
            CUBE_Path workingOutPath = CUBE_Path_CombineP(&workingPath, &outPath);

            CUBE_IO_CreateDirectoryNRP(&workingOutPath);

            CUBE_Path_Destroy(&workingPath);
            CUBE_Path_Destroy(&workingOutPath);
        }
        else
        {
            CUBE_IO_CreateDirectoryP(&outPath);
        }

        if (a_preprocessorPath != CBNULL)
        {
            CUBE_String_AppendC(&commandLine.Command, a_preprocessorPath);
        }
        else
        {
            switch (a_preProcessor)
            {
            case CUBE_CProjectCompiler_GCC:
            {
                CUBE_String_AppendC(&commandLine.Command, CUBE_GCC_String);

                break;
            }
            case CUBE_CProjectCompiler_MinGW:
            {
                CUBE_String_AppendC(&commandLine.Command, CUBE_MinGW_String);

                break;
            }
            }
        }

        CUBE_CommandLine_AppendArgumentC(&commandLine, "-DCUBE_LANGUAGE_CSHARP");

        CUBE_CommandLine_AppendArgumentC(&commandLine, "-E");
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-P");
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-x c");
        
        for (CBUINT32 i = 0; i < a_project->IncludePathCount; ++i)
        {
            CUBE_String includePath = CUBE_String_CreateC("-I");
            CUBE_String includePathStr = CUBE_Path_ToString(&a_project->IncludePaths[i]);

            CUBE_String_AppendS(&includePath, &includePathStr);

            CUBE_CommandLine_AppendArgumentS(&commandLine, &includePath);

            CUBE_String_Destroy(&includePathStr);
            CUBE_String_Destroy(&includePath);
        }

        CUBE_String sourceStr = CUBE_Path_ToString(&source);

        CUBE_CommandLine_AppendArgumentS(&commandLine, &sourceStr);
        
        CUBE_String fileName = CUBE_Path_Filename(&source);
        CUBE_String fileNameCSP = CUBE_String_MergeC(&fileName, ".cs");

        CUBE_Path preprocOutFilePath = CUBE_Path_CombineC(&outPath, fileNameCSP.Data);
        CUBE_String preprocOutFilePathStr = CUBE_Path_ToNRString(&preprocOutFilePath);

        CUBE_CommandLine_AppendArgumentC(&commandLine, "-o ");
        CUBE_CommandLine_AppendArgumentS(&commandLine, &preprocOutFilePathStr);

        const int ret = CUBE_CommandLine_Execute(&commandLine, a_lines, a_lineCount);

        CUBE_String_Destroy(&preprocOutFilePathStr);
        CUBE_Path_Destroy(&preprocOutFilePath);

        CUBE_String_Destroy(&fileNameCSP);
        CUBE_String_Destroy(&fileName);

        CUBE_String_Destroy(&sourceStr);

        CUBE_Path_Destroy(&parentPath);
        CUBE_Path_Destroy(&outPath);

        CUBE_CommandLine_Destroy(&commandLine);

        if (ret != 0)
        {
            CUBE_Path_Destroy(&preprocOutPath);

            return CBFALSE;
        }
    }

    CUBE_CommandLine commandLine = { 0 };
    if (a_workingPath != CBNULL)
    {
        CUBE_String_AppendC(&commandLine.Path, a_workingPath);
    }

    if (a_cscPath != CBNULL)
    {
        CUBE_String_AppendC(&commandLine.Command, a_cscPath);
    }
    else
    {
        CUBE_String_AppendC(&commandLine.Command, "csc");
    }

    CUBE_StackString outName = CUBE_StackString_Copy(&a_project->Name);

    switch (a_project->Target)
    {
    case CUBE_CSProjectTarget_Library:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/target:library");

        CUBE_StackString_AppendC(&outName, ".dll");

        break;
    }
    case CUBE_CSProjectTarget_Exe:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/target:exe");

        CUBE_StackString_AppendC(&outName, ".exe");

        break;
    }
    case CUBE_CSProjectTarget_WinExe:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/target:winexe");

        CUBE_StackString_AppendC(&outName, ".exe");

        break;
    }
    }

    CUBE_Path outPath = CUBE_Path_CombineSS(&a_project->OutputPath, &outName);
    CUBE_String outPathStr = CUBE_Path_ToString(&outPath);

    CUBE_Path workingPath = CUBE_Path_CreateC(a_workingPath);
    CUBE_Path workingOutPath = CUBE_Path_CombineP(&workingPath, &a_project->OutputPath);

    CUBE_IO_CreateDirectoryP(&workingOutPath);

    CUBE_Path_Destroy(&workingPath);
    CUBE_Path_Destroy(&workingOutPath);

    CUBE_String outArg = CUBE_String_CreateC("/out:");
    CUBE_String_AppendS(&outArg, &outPathStr);

    CUBE_CommandLine_AppendArgumentS(&commandLine, &outArg);

    CUBE_Path_Destroy(&outPath);
    CUBE_String_Destroy(&outPathStr);
    CUBE_String_Destroy(&outArg);

    if (a_project->Optimise)
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/optimize");
    }

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        const CUBE_Path source = a_project->Sources[i];

        CUBE_Path parentPath = CUBE_Path_ParentPath(&source);

        CUBE_String fileName = CUBE_Path_Filename(&source);
        CUBE_String fileNameCSP = CUBE_String_MergeC(&fileName, ".cs");

        CUBE_Path preprocOutFilePath = CUBE_Path_CombineP(&preprocOutPath, &parentPath);

        CUBE_Path preprocOutFile = CUBE_Path_CombineS(&preprocOutFilePath, &fileNameCSP);
        CUBE_String preprocOutFileStr = CUBE_Path_ToNRString(&preprocOutFile);

        CUBE_CommandLine_AppendArgumentS(&commandLine, &preprocOutFileStr);

        CUBE_String_Destroy(&preprocOutFileStr);
        CUBE_Path_Destroy(&preprocOutFile);

        CUBE_String_Destroy(&fileNameCSP);
        CUBE_String_Destroy(&fileName);

        CUBE_Path_Destroy(&parentPath);
        CUBE_Path_Destroy(&preprocOutFilePath);
    }

    CUBE_Path_Destroy(&preprocOutPath);

    for (CBUINT32 i = 0; i < a_project->ReferenceCount; ++i)
    {
        CUBE_String reference = CUBE_String_CreateC("/reference:");
        CUBE_String_AppendS(&reference, &a_project->References[i]);

        CUBE_CommandLine_AppendArgumentS(&commandLine, &reference);

        CUBE_String_Destroy(&reference);
    }

    const int ret = CUBE_CommandLine_Execute(&commandLine, a_lines, a_lineCount);

    CUBE_CommandLine_Destroy(&commandLine);

    return ret == 0;
}
CBBOOL CUBE_CSProject_Compile(const CUBE_CSProject* a_project, const char* a_workingPath, const char* a_cscPath, CUBE_String** a_lines, CBUINT32* a_lineCount)
{
    CUBE_CommandLine commandLine = { 0 };
    if (a_workingPath != CBNULL)
    {
        CUBE_String_AppendC(&commandLine.Path, a_workingPath);
    }

    if (a_cscPath != CBNULL)
    {
        CUBE_String_AppendC(&commandLine.Command, a_cscPath);
    }
    else
    {
        CUBE_String_AppendC(&commandLine.Command, "csc");
    }
    
    CUBE_StackString outName = CUBE_StackString_Copy(&a_project->Name);

    switch (a_project->Target)
    {
    case CUBE_CSProjectTarget_Library:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/target:library");

        CUBE_StackString_AppendC(&outName, ".dll");

        break;
    }
    case CUBE_CSProjectTarget_Exe:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/target:exe");

        CUBE_StackString_AppendC(&outName, ".exe");

        break;
    }
    case CUBE_CSProjectTarget_WinExe:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/target:winexe");

        CUBE_StackString_AppendC(&outName, ".exe");

        break;
    }
    }

    CUBE_Path outPath = CUBE_Path_CombineSS(&a_project->OutputPath, &outName);
    CUBE_String outPathStr = CUBE_Path_ToString(&outPath);

    CUBE_Path workingPath = CUBE_Path_CreateC(a_workingPath);
    CUBE_Path workingOutPath = CUBE_Path_CombineP(&workingPath, &a_project->OutputPath);

    CUBE_IO_CreateDirectoryP(&workingOutPath); 

    CUBE_Path_Destroy(&workingPath);
    CUBE_Path_Destroy(&workingOutPath);

    CUBE_String outArg = CUBE_String_CreateC("/out:");
    CUBE_String_AppendS(&outArg, &outPathStr);

    CUBE_CommandLine_AppendArgumentS(&commandLine, &outArg);

    CUBE_Path_Destroy(&outPath);
    CUBE_String_Destroy(&outPathStr);
    CUBE_String_Destroy(&outArg);

    if (a_project->Optimise)
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/optimize");
    }

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        CUBE_String source = CUBE_Path_ToString(&a_project->Sources[i]);

        CUBE_CommandLine_AppendArgumentS(&commandLine, &source);

        CUBE_String_Destroy(&source);
    }

    for (CBUINT32 i = 0; i < a_project->ReferenceCount; ++i)
    {
        CUBE_String reference = CUBE_String_CreateC("/reference:");
        CUBE_String_AppendS(&reference, &a_project->References[i]);

        CUBE_CommandLine_AppendArgumentS(&commandLine, &reference);

        CUBE_String_Destroy(&reference);
    }

    const int ret = CUBE_CommandLine_Execute(&commandLine, a_lines, a_lineCount);

    CUBE_CommandLine_Destroy(&commandLine);

    return ret == 0;
}
#endif

#ifdef __cplusplus
}
#endif

#endif

// MIT License
// 
// Copyright (c) 2023 River Govers
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
