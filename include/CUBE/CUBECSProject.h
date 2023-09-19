#ifndef INCLUDED_HEADER_CUBECSPROJ
#define INCLUDED_HEADER_CUBECSPROJ

#include "CUBE/CUBEString.h"
#include "CUBE/CUBECommandLine.h"

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
    CUBE_String* Sources;
    CBUINT32 SourceCount;
    CUBE_String* References;
    CBUINT32 ReferenceCount;
    CBBOOL Optimise;
} CUBE_CSProject;

void CUBE_CSProject_Destroy(CUBE_CSProject* a_project);

void CUBE_CSProject_AppendSource(CUBE_CSProject* a_project, const char* a_source);
void CUBE_CSProject_AppendReference(CUBE_CSProject* a_project, const char* a_reference);

CBBOOL CUBE_CSProject_Compile(CUBE_CSProject* a_project, const char* a_workingPath, const char* a_cscPath, CUBE_String** a_lines, CBUINT32* a_lineCount);

#ifdef CUBE_IMPLEMENTATION
// #if 1

void CUBE_CSProject_Destroy(CUBE_CSProject* a_project)
{
    CUBE_StackString_Destroy(&a_project->Name);
    
    a_project->Target = CUBE_CSProjectTarget_Exe;

    CUBE_Path_Destroy(&a_project->OutputPath);

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        CUBE_String_Destroy(&a_project->Sources[i]);
    }

    free(a_project->Sources);

    a_project->SourceCount = 0;
    a_project->Sources = CBNULL;

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

    CUBE_String source = CUBE_String_CreateC(a_source);

    a_project->Sources = (CUBE_String*)realloc(a_project->Sources, sizeof(CUBE_String) * a_project->SourceCount);

    a_project->Sources[sourceCount] = source;
}

void CUBE_CSProject_AppendReference(CUBE_CSProject* a_project, const char* a_reference)
{
    const CBUINT32 referenceCount = a_project->ReferenceCount++;

    CUBE_String reference = CUBE_String_CreateC(a_reference);

    a_project->References = (CUBE_String*)realloc(a_project->References, sizeof(CUBE_String) * a_project->ReferenceCount);

    a_project->References[referenceCount] = reference;
}

CBBOOL CUBE_CSProject_Compile(CUBE_CSProject* a_project, const char* a_workingPath, const char* a_cscPath, CUBE_String** a_lines, CBUINT32* a_lineCount)
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
        CUBE_CommandLine_AppendArgumentS(&commandLine, &a_project->Sources[i]);
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