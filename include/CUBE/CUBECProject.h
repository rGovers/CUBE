#ifndef INCLUDED_HEADER_CUBECPROJECT
#define INCLUDED_HEADER_CUBECPROJECT

#include "CUBE/CUBECommandLine.h"
#include "CUBE/CUBEIO.h"
#include "CUBE/CUBEPath.h"
#include "CUBE/CUBEString.h"

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
    CUBE_Path OutputPath;
    CUBE_String* Defines;
    CBUINT32 DefineCount;
    CUBE_Path* IncludePaths;
    CBUINT32 IncludePathCount;
    CUBE_Path* Sources;
    CBUINT32 SourceCount;
    CUBE_String* References;
    CBUINT32 ReferenceCount;
} CUBE_CProject;

void CUBE_CProject_Destroy(CUBE_CProject* a_project);

void CUBE_CProject_AppendDefine(CUBE_CProject* a_project, const char* a_define);
void CUBE_CProject_AppendSource(CUBE_CProject* a_project, const char* a_source);
void CUBE_CProject_AppendIncludePath(CUBE_CProject* a_project, const char* a_includePath);
void CUBE_CProject_AppendReference(CUBE_CProject* a_project, const char* a_reference);

CUBE_CommandLine CUBEI_CProject_CreateObjectCommandLine(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_compilerPath);

void CUBE_CProject_Compile(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_compilerPath);

#ifdef CUBE_IMPLEMENTATION
// #if 1

void CUBE_CProject_Destroy(CUBE_CProject* a_project)
{
    CUBE_StackString_Destroy(&a_project->Name);
    
    a_project->Target = CUBE_CProjectTarget_Exe;
    a_project->Language = CUBE_CProjectLanguage_C;

    CUBE_Path_Destroy(&a_project->OutputPath);

    for (CBUINT32 i = 0; i < a_project->DefineCount; ++i)
    {
        CUBE_String_Destroy(&a_project->Defines[i]);
    }

    free(a_project->Defines);

    a_project->DefineCount = 0;
    a_project->Defines = CBNULL;

    for (CBUINT32 i = 0; i < a_project->IncludePathCount; ++i)
    {
        CUBE_Path_Destroy(&a_project->IncludePaths[i]);
    }

    free(a_project->IncludePaths);

    a_project->IncludePathCount = 0;
    a_project->IncludePaths = CBNULL;

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        CUBE_Path_Destroy(&a_project->Sources[i]);
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

void CUBE_CProject_AppendDefine(CUBE_CProject* a_project, const char* a_define)
{
    const CBUINT32 defineCount = a_project->DefineCount++;

    CUBE_String define = CUBE_String_CreateC(a_define);

    a_project->Defines = (CUBE_String*)realloc(a_project->Defines, sizeof(CUBE_String) * a_project->DefineCount);

    a_project->Defines[defineCount] = define;
}
void CUBE_CProject_AppendSource(CUBE_CProject* a_project, const char* a_source)
{
    const CBUINT32 sourceCount = a_project->SourceCount++;

    CUBE_Path source = CUBE_Path_CreateC(a_source);

    a_project->Sources = (CUBE_Path*)realloc(a_project->Sources, sizeof(CUBE_Path) * a_project->SourceCount);

    a_project->Sources[sourceCount] = source;
}
void CUBE_CProject_AppendIncludePath(CUBE_CProject* a_project, const char* a_includePath)
{
    const CBUINT32 includePathCount = a_project->IncludePathCount++;

    CUBE_Path includePath = CUBE_Path_CreateC(a_includePath);

    a_project->IncludePaths = (CUBE_Path*)realloc(a_project->IncludePaths, sizeof(CUBE_Path) * a_project->IncludePathCount);

    a_project->IncludePaths[includePathCount] = includePath;
}
void CUBE_CProject_AppendReference(CUBE_CProject* a_project, const char* a_reference)
{
    const CBUINT32 referenceCount = a_project->ReferenceCount++;

    CUBE_String reference = CUBE_String_CreateC(a_reference);

    a_project->References = (CUBE_String*)realloc(a_project->References, sizeof(CUBE_String) * a_project->ReferenceCount);

    a_project->References[referenceCount] = reference;
}

CUBE_CommandLine CUBEI_CProject_CreateObjectCommandLine(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_compilerPath)
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

    CUBE_CommandLine_AppendArgumentC(&commandLine, "-c");
    switch (a_project->Target)
    {
    case CUBE_CProjectTarget_Library:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-fPIC");

        break;
    }
    }

    for (CBUINT32 i = 0; i < a_project->DefineCount; ++i)
    {
        CUBE_String define = CUBE_String_CreateC("-D");
        CUBE_String_AppendS(&define, &a_project->Defines[i]);

        CUBE_CommandLine_AppendArgumentC(&commandLine, define.Data);

        CUBE_String_Destroy(&define);
    }

    for (CBUINT32 i = 0; i < a_project->IncludePathCount; ++i)
    {
        CUBE_String includePath = CUBE_String_CreateC("-I");
        CUBE_String includeStr = CUBE_Path_ToString(&a_project->IncludePaths[i]);

        CUBE_String_AppendS(&includePath, &includeStr);

        CUBE_CommandLine_AppendArgumentC(&commandLine, includePath.Data);

        CUBE_String_Destroy(&includePath);
        CUBE_String_Destroy(&includeStr);
    }

    return commandLine;
}

void CUBE_CProject_Compile(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_compilerPath)
{
    CUBE_Path objectOutpath = CUBE_Path_CombineC(&a_project->OutputPath, "obj");

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        CUBE_CommandLine commandLine = CUBEI_CProject_CreateObjectCommandLine(a_project, a_compiler, a_compilerPath);

        const CUBE_Path source = a_project->Sources[i];
        CUBE_String sourceStr = CUBE_Path_ToString(&source);

        CUBE_Path parentPath = CUBE_Path_ParentPath(&source);
        CUBE_String parentPathStr = CUBE_Path_ToString(&parentPath);

        CUBE_String fileName = CUBE_Path_Filename(&source);
        CUBE_String extension = CUBE_Path_Extension(&source);

        CUBE_String filenameExt = CUBE_String_MergeS(&fileName, &extension);
        CUBE_String filenameObj = CUBE_String_MergeC(&fileName, ".o");

        CUBE_Path outPath = CUBE_Path_CombineS(&objectOutpath, &parentPathStr);

        CUBE_IO_CreateDirectoryP(&outPath);

        CUBE_Path objectPath = CUBE_Path_CombineS(&outPath, &filenameObj);
        CUBE_String objectPathStr = CUBE_Path_ToString(&objectPath);

        CUBE_String outArg = CUBE_String_CreateC("-o ");
        CUBE_String_AppendS(&outArg, &objectPathStr);

        CUBE_CommandLine_AppendArgumentC(&commandLine, outArg.Data);
        CUBE_CommandLine_AppendArgumentC(&commandLine, sourceStr.Data);

        CUBE_CommandLine_Execute(&commandLine);

        CUBE_String_Destroy(&outArg);

        CUBE_Path_Destroy(&objectPath);
        CUBE_String_Destroy(&objectPathStr);

        CUBE_CommandLine_Destroy(&commandLine);

        CUBE_String_Destroy(&sourceStr);

        CUBE_Path_Destroy(&parentPath);
        CUBE_String_Destroy(&parentPathStr);

        CUBE_String_Destroy(&fileName);
        CUBE_String_Destroy(&extension);

        CUBE_String_Destroy(&filenameExt);
        CUBE_String_Destroy(&filenameObj);

        CUBE_Path_Destroy(&outPath);
    }

    CUBE_CommandLine commandLine = { 0 };

    switch (a_project->Target)
    {
    case CUBE_CProjectTarget_StaticLibrary:
    {
        CUBE_String_AppendC(&commandLine.Command, "ar");

        CUBE_CommandLine_AppendArgumentC(&commandLine, "rcs");

        CUBE_StackString libName = CUBE_StackString_MergeC(&a_project->Name, ".a");
        CUBE_Path libPath = CUBE_Path_CombineSS(&a_project->OutputPath, &libName);
        CUBE_String libPathStr = CUBE_Path_ToString(&libPath);

        CUBE_CommandLine_AppendArgumentC(&commandLine, libPathStr.Data);

        CUBE_Path_Destroy(&libPath);
        CUBE_String_Destroy(&libPathStr);

        for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
        {
            const CUBE_Path source = a_project->Sources[i];

            CUBE_Path parentPath = CUBE_Path_ParentPath(&source);

            CUBE_String fileName = CUBE_Path_Filename(&source);
            CUBE_String objName = CUBE_String_MergeC(&fileName, ".o");

            CUBE_Path objDir = CUBE_Path_CombineP(&objectOutpath, &parentPath);
            CUBE_Path objPath = CUBE_Path_CombineS(&objDir, &objName);

            CUBE_String objPathStr = CUBE_Path_ToString(&objPath);

            CUBE_CommandLine_AppendArgumentC(&commandLine, objPathStr.Data);

            CUBE_Path_Destroy(&parentPath);

            CUBE_String_Destroy(&fileName);
            CUBE_String_Destroy(&objName);

            CUBE_Path_Destroy(&objDir);
            CUBE_Path_Destroy(&objPath);

            CUBE_String_Destroy(&objPathStr);
        }

        CUBE_Path_Destroy(&objectOutpath);

        CUBE_CommandLine_Execute(&commandLine);

        CUBE_CommandLine_Destroy(&commandLine);

        return;
    }
    case CUBE_CProjectTarget_Exe:
    {
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

                CUBE_Path exePath = CUBE_Path_CombineSS(&a_project->OutputPath, &a_project->Name);
                CUBE_String exePathStr = CUBE_Path_ToString(&exePath);

                CUBE_String outArg = CUBE_String_CreateC("-o ");
                CUBE_String_AppendS(&outArg, &exePathStr);

                CUBE_CommandLine_AppendArgumentC(&commandLine, outArg.Data);

                CUBE_String_Destroy(&outArg);

                CUBE_Path_Destroy(&exePath);
                CUBE_String_Destroy(&exePathStr);

                break;
            }
            }
        }

        break;
    }
    case CUBE_CProjectTarget_Library:
    {
        switch (a_compiler)
        {
        case CUBE_CProjectCompiler_GCC:
        {
            CUBE_String_AppendC(&commandLine.Command, "gcc");

            CUBE_CommandLine_AppendArgumentC(&commandLine, "-shared");

            CUBE_StackString libName = CUBE_StackString_MergeC(&a_project->Name, ".so");
            CUBE_Path libPath = CUBE_Path_CombineSS(&a_project->OutputPath, &libName);
            CUBE_String libPathStr = CUBE_Path_ToString(&libPath);

            CUBE_String outArg = CUBE_String_CreateC("-o ");
            CUBE_String_AppendS(&outArg, &libPathStr);

            CUBE_CommandLine_AppendArgumentC(&commandLine, outArg.Data);

            CUBE_String_Destroy(&outArg);

            CUBE_Path_Destroy(&libPath);
            CUBE_String_Destroy(&libPathStr);

            break;
        }
        }

        break;
    }
    }

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        const CUBE_Path source = a_project->Sources[i];

        CUBE_Path parentPath = CUBE_Path_ParentPath(&source);

        CUBE_String fileName = CUBE_Path_Filename(&source);
        CUBE_String objName = CUBE_String_MergeC(&fileName, ".o");

        CUBE_Path objDir = CUBE_Path_CombineP(&objectOutpath, &parentPath);
        CUBE_Path objPath = CUBE_Path_CombineS(&objDir, &objName);

        CUBE_String objPathStr = CUBE_Path_ToString(&objPath);

        CUBE_CommandLine_AppendArgumentC(&commandLine, objPathStr.Data);
        
        CUBE_Path_Destroy(&parentPath);

        CUBE_String_Destroy(&fileName);
        CUBE_String_Destroy(&objName);

        CUBE_Path_Destroy(&objDir);
        CUBE_Path_Destroy(&objPath);

        CUBE_String_Destroy(&objPathStr);
    }

    CUBE_Path_Destroy(&objectOutpath);

    CUBEI_CProject_AppendLinkerFlags(&commandLine, a_project, a_compiler);

    CUBE_CommandLine_Execute(&commandLine);

    CUBE_CommandLine_Destroy(&commandLine);

    // CUBE_CommandLine_AppendArgumentC(&commandLine, outPath.Data);

    // CUBE_String_Destroy(&outPath);

    // for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    // {
    //     CUBE_CommandLine_AppendArgumentC(&commandLine, a_project->Sources[i].Data);
    // }

    // for (CBUINT32 i = 0; i < a_project->ReferenceCount; ++i)
    // {
    //     CUBE_String reference = CUBE_String_CreateC("-l");
    //     CUBE_String_AppendS(&reference, &a_project->References[i]);

    //     CUBE_CommandLine_AppendArgumentC(&commandLine, reference.Data);

    //     CUBE_String_Destroy(&reference);
    // }

    // CUBE_CommandLine_Execute(&commandLine);
}
#endif

#ifdef __cplusplus
}
#endif

#endif