#ifndef INCLUDED_HEADER_CUBECPROJECT
#define INCLUDED_HEADER_CUBECPROJECT

#include "CUBE/CUBEString.h"
#include "CUBE/CUBECommandLine.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
    CUBE_CProjectTarget_Exe,
    CUBE_CProjectTarget_Library,
    CUBE_CProjectTarget_StaticLibrary
} e_CUBE_CProjectTarget;

typedef enum
{
    CUBE_CProjectLanguage_C,
    CUBE_CProjectLanguage_CPP
} e_CUBE_CProjectLanguage;

typedef enum 
{
    CUBE_CProjectCompiler_GCC,
} e_CUBE_CProjectCompiler;

typedef struct
{
    CUBE_StackString Name;
    e_CUBE_CProjectTarget Target;
    e_CUBE_CProjectLanguage Language;
    CUBE_String OutputPath;
    CUBE_String* IncludePaths;
    CBUINT32 IncludePathCount;
    CUBE_String* Sources;
    CBUINT32 SourceCount;
    CUBE_String* References;
    CBUINT32 ReferenceCount;
} CUBE_CProject;

void CUBE_CProject_Destroy(CUBE_CProject* a_project);

void CUBE_CProject_AppendSource(CUBE_CProject* a_project, const char* a_source);
void CUBE_CProject_Append_Reference(CUBE_CProject* a_project, const char* a_reference);

void CUBE_CProject_Compile(CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_compilerPath);

// #ifdef CUBE_IMPLEMENTATION
#if 1

void CUBE_CProject_Destroy(CUBE_CProject* a_project)
{
    CUBE_StackString_Destroy(&a_project->Name);
    
    a_project->Target = CUBE_CProjectTarget_Exe;
    a_project->Language = CUBE_CProjectLanguage_C;

    CUBE_String_Destroy(&a_project->OutputPath);

    for (CBUINT32 i = 0; i < a_project->IncludePathCount; ++i)
    {
        CUBE_String_Destroy(&a_project->IncludePaths[i]);
    }

    free(a_project->IncludePaths);

    a_project->IncludePathCount = 0;
    a_project->IncludePaths = CBNULL;

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

void CUBE_CProject_AppendSource(CUBE_CProject* a_project, const char* a_source)
{
    const CBUINT32 sourceCount = a_project->SourceCount++;

    CUBE_String source = CUBE_String_CreateC(a_source);

    a_project->Sources = (CUBE_String*)realloc(a_project->Sources, sizeof(CUBE_String) * a_project->SourceCount);

    a_project->Sources[sourceCount] = source;
}
void CUBE_CProject_Append_Reference(CUBE_CProject* a_project, const char* a_reference)
{
    const CBUINT32 referenceCount = a_project->ReferenceCount++;

    CUBE_String reference = CUBE_String_CreateC(a_reference);

    a_project->References = (CUBE_String*)realloc(a_project->References, sizeof(CUBE_String) * a_project->ReferenceCount);

    a_project->References[referenceCount] = reference;
}

void CUBE_CProject_Compile(CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_compilerPath)
{
    CUBE_CommandLine commandLine = { 0 };

    if (a_compilerPath != CBNULL)
    {
        CUBE_String_AppendC(&commandLine.Command, a_compilerPath);
    }
    else 
    {
        switch (a_compiler)
        {
        case CUBE_CProjectCompiler_GCC:
        {
            CUBE_String_AppendC(&commandLine.Command, "gcc");

            break;
        } 
        }
    }

    switch (a_project->Language)
    {
    case CUBE_CProjectLanguage_C:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-x c");

        break;
    }
    case CUBE_CProjectLanguage_CPP:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-x c++");

        break;
    }
    }

    switch (a_project->Target)
    {
    case CUBE_CProjectTarget_Library:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-shared");

        break;
    }
    case CUBE_CProjectTarget_StaticLibrary:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-static");

        break;
    }
    }

    for (CBUINT32 i = 0; i < a_project->ReferenceCount; ++i)
    {
        CUBE_String reference = CUBE_String_CreateC("-l");
        CUBE_String_AppendS(&reference, &a_project->References[i]);

        CUBE_CommandLine_AppendArgumentC(&commandLine, reference.Data);

        CUBE_String_Destroy(&reference);
    }

    for (CBUINT32 i = 0; i < a_project->IncludePathCount; ++i)
    {
        CUBE_String includePath = CUBE_String_CreateC("-I");
        CUBE_String_AppendS(&includePath, &a_project->IncludePaths[i]);

        CUBE_CommandLine_AppendArgumentC(&commandLine, includePath.Data);

        CUBE_String_Destroy(&includePath);
    }

    CUBE_String outPath = CUBE_String_CreateC("-o");
    CUBE_String_AppendS(&outPath, &a_project->OutputPath);

    CUBE_CommandLine_AppendArgumentC(&commandLine, outPath.Data);
    

    CUBE_String_Destroy(&outPath);

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