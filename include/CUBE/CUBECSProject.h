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
    CUBE_String OutputPath;
    CUBE_String* Sources;
    CBUINT32 SourceCount;
    CUBE_String* References;
    CBUINT32 ReferenceCount;
    CBBOOL Optimise;
} CUBE_CSProject;

void CUBE_CSProject_Destroy(CUBE_CSProject* a_project);

void CUBE_CSProject_AppendSource(CUBE_CSProject* a_project, const char* a_source);
void CUBE_CSProject_Append_Reference(CUBE_CSProject* a_project, const char* a_reference);

void CUBE_CSProject_Compile(CUBE_CSProject* a_project, const char* a_cscPath);

#ifdef CUBE_IMPLEMENTATION

void CUBE_CSProject_Destroy(CUBE_CSProject* a_project)
{
    CUBE_StackString_Destroy(&a_project->Name);
    
    a_project->Target = CUBE_CSProjectTarget_Exe;

    CUBE_String_Destroy(&a_project->OutputPath);

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

void CUBE_CSProject_Append_Reference(CUBE_CSProject* a_project, const char* a_reference)
{
    const CBUINT32 referenceCount = a_project->ReferenceCount++;

    CUBE_String reference = CUBE_String_CreateC(a_reference);

    a_project->References = (CUBE_String*)realloc(a_project->References, sizeof(CUBE_String) * a_project->ReferenceCount);

    a_project->References[referenceCount] = reference;
}

void CUBE_CSProject_Compile(CUBE_CSProject* a_project, const char* a_cscPath)
{
    CUBE_CommandLine commandLine = { 0 };

    if (a_cscPath != CBNULL)
    {
        CUBE_String_AppendC(&commandLine.Command, a_cscPath);
    }
    else
    {
        CUBE_String_AppendC(&commandLine.Command, "csc");
    }

    CUBE_String outPath = CUBE_String_CreateC("/out:");
    CUBE_String_AppendS(&outPath, &a_project->OutputPath);

    CUBE_CommandLine_AppendArgumentC(&commandLine, outPath.Data);

    CUBE_String_Destroy(&outPath);
    switch (a_project->Target)
    {
    case CUBE_CSProjectTarget_Library:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/target:library");

        break;
    }
    case CUBE_CSProjectTarget_Exe:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/target:exe");

        break;
    }
    case CUBE_CSProjectTarget_WinExe:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/target:winexe");

        break;
    }
    }

    if (a_project->Optimise)
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "/optimize");
    }

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, a_project->Sources[i].Data);
    }

    CUBE_CommandLine_Execute(&commandLine);
}
#endif

#ifdef __cplusplus
}
#endif

#endif