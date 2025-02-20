// CUBE - Header only C/C++/C# build system.
// 
// License at end of file.

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
    CUBE_CProjectCompiler_MinGW,
    CUBE_CProjectCompiler_Clang,
    CUBE_CProjectCompiler_Zig
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
    CUBE_Path* SystemIncludePaths;
    CBUINT32 SystemIncludePathCount;
    CUBE_Path* Sources;
    CBUINT32 SourceCount;
    CUBE_String* References;
    CBUINT32 ReferenceCount;
    CUBE_Path* Libraries;
    CBUINT32 LibraryCount;
    CUBE_String* CFlags;
    CBUINT32 CFlagCount;
} CUBE_CProject;

void CUBE_CProject_Destroy(CUBE_CProject* a_project);

void CUBE_CProject_PrependPaths(CUBE_CProject* a_project, const char* a_path, CBBOOL a_includeLibraries);

void CUBE_CProject_AppendDefine(CUBE_CProject* a_project, const char* a_define);
void CUBEI_CProject_AppendDefines(CUBE_CProject* a_project, const char* a_defines[]);
#define CUBE_CProject_AppendDefines(project, ...) CUBEI_CProject_AppendDefines(project, (const char*[]){ __VA_ARGS__, CBNULL })
void CUBE_CProject_AppendSource(CUBE_CProject* a_project, const char* a_source);
void CUBEI_CProject_AppendSources(CUBE_CProject* a_project, const char* a_sources[]);
#define CUBE_CProject_AppendSources(project, ...) CUBEI_CProject_AppendSources(project, (const char*[]){ __VA_ARGS__, CBNULL })
void CUBE_CProject_AppendIncludePath(CUBE_CProject* a_project, const char* a_includePath);
void CUBEI_CProject_AppendIncludePaths(CUBE_CProject* a_project, const char* a_includePaths[]);
#define CUBE_CProject_AppendIncludePaths(project, ...) CUBEI_CProject_AppendIncludePaths(project, (const char*[]){ __VA_ARGS__, CBNULL })
void CUBE_CProject_AppendSystemIncludePath(CUBE_CProject* a_project, const char* a_includePath);
void CUBE_CProject_AppendReference(CUBE_CProject* a_project, const char* a_reference);
void CUBE_CProject_AppendLibrary(CUBE_CProject* a_project, const char* a_library);
void CUBEI_CProject_AppendLibraries(CUBE_CProject* a_project, const char* a_libraries[]);
#define CUBE_CProject_AppendLibraries(project, ...) CUBEI_CProject_AppendLibraries(project, (const char*[]){ __VA_ARGS__, CBNULL })
void CUBE_CProject_AppendCFlag(CUBE_CProject* a_project, const char* a_flag);

CBBOOL CUBE_CProject_Compile(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath, const char* a_compilerPath, CUBE_String** a_line, CBUINT32* a_lineCount, CBBOOL a_forceBuild);
CBBOOL CUBE_CProject_MultiCompile(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath, const char* a_compilerPath, CBUINT32 a_jobs, CUBE_String** a_lines, CBUINT32* a_lineCount, CBBOOL a_forceBuild);

CUBE_String CUBE_CProject_GenerateCompileCommands(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath);
CUBE_String CUBE_CProject_GenerateMultiCompileCommands(const CUBE_CProject* a_projects, CBUINT32 a_projectCount, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath);

#ifdef CUBE_IMPLEMENTATION
// #if 1

#ifdef CUBE_PRINT_COLOUR
#define CUBEI_CPROJECT_CONSOLECOLOUR_GREEN "\033[0;32m"
#define CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "\033[0;36m"
#define CUBEI_CPROJECT_CONSOLECOLOUR_RESET "\033[0m"
#else
#define CUBEI_CPROJECT_CONSOLECOLOUR_GREEN
#define CUBEI_CPROJECT_CONSOLECOLOUR_CYAN
#define CUBEI_CPROJECT_CONSOLECOLOUR_RESET
#endif

typedef struct
{
    CUBE_Path Source;
    CUBE_String SourceString;
    CBTIME WriteTime;
} CUBEI_SourceStatus;

typedef struct
{
    // Upon further thought is already unreliable recompiling only on changed files cannot make any assurances about external depedencies so just always re-link
    CUBEI_SourceStatus* Sources;
    CBUINT32 SourceCount;
} CUBEI_CProjectStatus;

CUBEI_CProjectStatus CUBEI_CProject_GetProjectStatus(const CUBE_CProject* a_project, const char* a_workingPath);
void CUBEI_CProject_WriteProjectStatus(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath);
void CUBEI_CProject_AppendStatusSource(CUBEI_CProjectStatus* a_project, const CUBE_Path* a_source, const CUBE_String* a_sourceString, CBTIME a_writeTime);
void CUBEI_CProject_DestroyProjectStatus(CUBEI_CProjectStatus* a_project);

CUBE_CommandLine CUBEI_CProject_CreateObjectCommandLine(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath, const char* a_compilerPath);

CUBE_String CUBEI_CProject_CreateSourceCommandString(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const CUBE_Path* a_workingPath, CBUINT32 a_source);
CUBE_String CUBEI_CProject_CreateProjectCompileCommands(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const CUBE_Path* a_workingPath);

static const char CUBE_GCC_String[] = "gcc";
static const char CUBE_MinGW_String[] = "x86_64-w64-mingw32-gcc";
static const char CUBE_Clang_String[] = "clang";
static const char CUBE_ZigCC_String[] = "zig cc";
static const char CUBE_ZigCPP_String[] = "zig c++";
static const char CUBE_GCCAR_String[] = "ar";
static const char CUBE_MinGWAR_String[] = "x86_64-w64-mingw32-ar";
static const char CUBE_ClangAR_String[] = "llvm-ar";
static const char CUBE_ZigAR_String[] = "zig ar";

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

    for (CBUINT32 i = 0; i < a_project->SystemIncludePathCount; ++i)
    {
        CUBE_Path_Destroy(&a_project->SystemIncludePaths[i]);
    }

    free(a_project->SystemIncludePaths);

    a_project->SystemIncludePathCount = 0;
    a_project->SystemIncludePaths = CBNULL;

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

    for (CBUINT32 i = 0; i < a_project->LibraryCount; ++i)
    {
        CUBE_Path_Destroy(&a_project->Libraries[i]);
    }

    free(a_project->Libraries);

    a_project->LibraryCount = 0;
    a_project->Libraries = CBNULL;

    for (CBUINT32 i = 0; i < a_project->CFlagCount; ++i)
    {
        CUBE_String_Destroy(&a_project->CFlags[i]);
    }

    free(a_project->CFlags);

    a_project->CFlagCount = 0;
    a_project->CFlags = CBNULL;
}

void CUBE_CProject_PrependPaths(CUBE_CProject* a_project, const char* a_path, CBBOOL a_includeLibraries)
{
    CUBE_Path path = CUBE_Path_CreateC(a_path);
    CUBE_Path oldPath = a_project->OutputPath;

    a_project->OutputPath = CUBE_Path_CombineP(&path, &oldPath);

    CUBE_Path_Destroy(&oldPath);

    for (CBUINT32 i = 0; i < a_project->IncludePathCount; ++i)
    {
        oldPath = a_project->IncludePaths[i];

        a_project->IncludePaths[i] = CUBE_Path_CombineP(&path, &oldPath);

        CUBE_Path_Destroy(&oldPath);
    }

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        oldPath = a_project->Sources[i];

        a_project->Sources[i] = CUBE_Path_CombineP(&path, &oldPath);

        CUBE_Path_Destroy(&oldPath);
    }

    if (a_includeLibraries)
    {
        for (CBUINT32 i = 0; i < a_project->LibraryCount; ++i)
        {
            oldPath = a_project->Libraries[i];

            a_project->Libraries[i] = CUBE_Path_CombineP(&path, &oldPath);

            CUBE_Path_Destroy(&oldPath);
        }
    }

    CUBE_Path_Destroy(&path);
}

void CUBE_CProject_AppendDefine(CUBE_CProject* a_project, const char* a_define)
{
    const CBUINT32 defineCount = a_project->DefineCount++;

    a_project->Defines = (CUBE_String*)realloc(a_project->Defines, sizeof(CUBE_String) * a_project->DefineCount);

    a_project->Defines[defineCount] = CUBE_String_CreateC(a_define);;
}
void CUBEI_CProject_AppendDefines(CUBE_CProject* a_project, const char* a_defines[])
{
    const char** end = a_defines;
    while (*end != CBNULL)
    {
        ++end;
    }

    const CBUINT32 projectDefines = a_project->DefineCount;
    const CBUINT32 defineCount = (CBUINT32)(end - a_defines);
    const CBUINT32 finalCount = projectDefines + defineCount;

    a_project->Defines = (CUBE_String*)realloc(a_project->Defines, sizeof(CUBE_String) * finalCount);

    for (CBUINT32 i = 0; i < defineCount; ++i)
    {
        a_project->Defines[projectDefines + i] = CUBE_String_CreateC(a_defines[i]);
    }

    a_project->DefineCount = finalCount;
}

void CUBE_CProject_AppendSource(CUBE_CProject* a_project, const char* a_source)
{
    const CBUINT32 sourceCount = a_project->SourceCount++;

    a_project->Sources = (CUBE_Path*)realloc(a_project->Sources, sizeof(CUBE_Path) * a_project->SourceCount);

    a_project->Sources[sourceCount] = CUBE_Path_CreateC(a_source);
}
void CUBEI_CProject_AppendSources(CUBE_CProject* a_project, const char* a_sources[])
{
    const char** end = a_sources;
    while (*end != CBNULL)
    {
        ++end;
    }

    const CBUINT32 projectSources = a_project->SourceCount;
    const CBUINT32 sourceCount = (CBUINT32)(end - a_sources);
    const CBUINT32 finalCount = projectSources + sourceCount;

    a_project->Sources = (CUBE_Path*)realloc(a_project->Sources, sizeof(CUBE_Path) * finalCount);

    for (CBUINT32 i = 0; i < sourceCount; ++i)
    {
        a_project->Sources[projectSources + i] = CUBE_Path_CreateC(a_sources[i]);
    }

    a_project->SourceCount = finalCount;
}

void CUBE_CProject_AppendIncludePath(CUBE_CProject* a_project, const char* a_includePath)
{
    const CBUINT32 includePathCount = a_project->IncludePathCount++;

    a_project->IncludePaths = (CUBE_Path*)realloc(a_project->IncludePaths, sizeof(CUBE_Path) * a_project->IncludePathCount);

    a_project->IncludePaths[includePathCount] = CUBE_Path_CreateC(a_includePath);
}
void CUBEI_CProject_AppendIncludePaths(CUBE_CProject* a_project, const char* a_includePaths[])
{
    const char** end = a_includePaths;
    while (*end != CBNULL)
    {
        ++end;
    }

    const CBUINT32 projectIncludes = a_project->IncludePathCount;
    const CBUINT32 includeCount = (CBUINT32)(end - a_includePaths);
    const CBUINT32 finalCount = projectIncludes + includeCount;

    a_project->IncludePaths = (CUBE_Path*)realloc(a_project->IncludePaths, sizeof(CUBE_Path) * finalCount);

    for (CBUINT32 i = 0; i < includeCount; ++i)
    {
        a_project->IncludePaths[projectIncludes + i] = CUBE_Path_CreateC(a_includePaths[i]);
    }

    a_project->IncludePathCount = finalCount;
}

void CUBE_CProject_AppendSystemIncludePath(CUBE_CProject* a_project, const char* a_includePath)
{
    const CBUINT32 includePathCount = a_project->SystemIncludePathCount++;

    CUBE_Path includePath = CUBE_Path_CreateC(a_includePath);

    a_project->SystemIncludePaths = (CUBE_Path*)realloc(a_project->SystemIncludePaths, sizeof(CUBE_Path) * a_project->SystemIncludePathCount);

    a_project->SystemIncludePaths[includePathCount] = includePath;
}
void CUBE_CProject_AppendReference(CUBE_CProject* a_project, const char* a_reference)
{
    const CBUINT32 referenceCount = a_project->ReferenceCount++;

    CUBE_String reference = CUBE_String_CreateC(a_reference);

    a_project->References = (CUBE_String*)realloc(a_project->References, sizeof(CUBE_String) * a_project->ReferenceCount);

    a_project->References[referenceCount] = reference;
}
void CUBE_CProject_AppendCFlag(CUBE_CProject* a_project, const char* a_flag)
{
    const CBUINT32 flagCount = a_project->CFlagCount++;

    CUBE_String flag = CUBE_String_CreateC(a_flag);

    a_project->CFlags = (CUBE_String*)realloc(a_project->CFlags, sizeof(CUBE_String) * a_project->CFlagCount);

    a_project->CFlags[flagCount] = flag;
}
void CUBE_CProject_AppendLibrary(CUBE_CProject* a_project, const char* a_library)
{
    const CBUINT32 libraryCount = a_project->LibraryCount++;

    CUBE_Path library = CUBE_Path_CreateC(a_library);

    a_project->Libraries = (CUBE_Path*)realloc(a_project->Libraries, sizeof(CUBE_Path) * a_project->LibraryCount);

    a_project->Libraries[libraryCount] = library;
}
void CUBEI_CProject_AppendLibraries(CUBE_CProject *a_project, const char* a_libraries[])
{
    const char** end = a_libraries;
    while (*end != CBNULL)
    {
        ++end;
    }

    const CBUINT32 projectLibraries = a_project->LibraryCount;
    const CBUINT32 libraryCount = (CBUINT32)(end - a_libraries);
    const CBUINT32 finalCount = projectLibraries + libraryCount;

    a_project->Libraries = (CUBE_Path*)realloc(a_project->Libraries, sizeof(CUBE_Path) * finalCount);

    for (CBUINT32 i = 0; i < libraryCount; ++i)
    {
        a_project->Libraries[projectLibraries + i] = CUBE_Path_CreateC(a_libraries[i]);
    }

    a_project->LibraryCount = finalCount;
}

CUBE_CommandLine CUBEI_CProject_CreateObjectCommandLine(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath, const char* a_compilerPath)
{
    CUBE_CommandLine commandLine = { 0 };
    if (a_workingPath != CBNULL)
    {
        CUBE_String_AppendC(&commandLine.Path, a_workingPath);
    }

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
            CUBE_String_AppendC(&commandLine.Command, CUBE_GCC_String);

            break;
        } 
        case CUBE_CProjectCompiler_MinGW:
        {
            CUBE_String_AppendC(&commandLine.Command, CUBE_MinGW_String);

            break;
        }
        case CUBE_CProjectCompiler_Clang:
        {
            CUBE_String_AppendC(&commandLine.Command, CUBE_Clang_String);

            break;
        }
        case CUBE_CProjectCompiler_Zig:
        {
            // Language arguments do not work properly in zig so have to determine compiler ourselves
            switch (a_project->Language)
            {
            case CUBE_CProjectLanguage_C:
            {
                CUBE_String_AppendC(&commandLine.Command, CUBE_ZigCC_String);

                break;
            }
            case CUBE_CProjectLanguage_CPP:
            {
                CUBE_String_AppendC(&commandLine.Command, CUBE_ZigCPP_String);

                break;
            }
            }
        }
        }
    }

    switch (a_project->Language)
    {
    case CUBE_CProjectLanguage_C:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-x c");
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-DCUBE_LANGUAGE_C");

        break;
    }
    case CUBE_CProjectLanguage_CPP:
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-x c++");
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-DCUBE_LANGUAGE_CPP");

        break;
    }
    }

    CUBE_CommandLine_AppendArgumentC(&commandLine, "-c");
    if (a_project->Target == CUBE_CProjectTarget_Library)
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, "-fPIC");
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

    for (CBUINT32 i = 0; i < a_project->CFlagCount; ++i)
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, a_project->CFlags[i].Data);
    }

    if (a_project->SystemIncludePathCount > 0)
    {
        CUBE_String value = { 0 };

        for (CBUINT32 i = 0; i < a_project->SystemIncludePathCount; ++i)
        {
            CUBE_String pathValue = CUBE_Path_ToString(&a_project->SystemIncludePaths[i]);
            
            CUBE_String_AppendS(&value, &pathValue);
            if (i < a_project->SystemIncludePathCount - 1)
            {
                CUBE_String_AppendC(&value, ":");
            }

            CUBE_String_Destroy(&pathValue);
        }

        CUBE_CommandLine_AppendEnvironmentVariableC(&commandLine, "CPATH", value.Data);

        CUBE_String_Destroy(&value);
    }

    return commandLine;
}

CUBE_CommandLine CUBEI_CProject_CreateObjectFileCommandLine(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath, const char* a_compilerPath, const CUBE_Path* a_source, const CUBE_Path* a_objOut)
{
    CUBE_CommandLine commandLine = CUBEI_CProject_CreateObjectCommandLine(a_project, a_compiler, a_workingPath, a_compilerPath);

    CUBE_String sourceStr = CUBE_Path_ToString(a_source);

    CUBE_Path parentPath = CUBE_Path_ParentPath(a_source);
    CUBE_String parentPathStr = CUBE_Path_ToString(&parentPath);

    CUBE_String fileName = CUBE_Path_Filename(a_source);
    CUBE_String extension = CUBE_Path_Extension(a_source);

    CUBE_String filenameObj = CUBE_String_MergeC(&fileName, ".o");

    CUBE_Path outPath = CUBE_Path_CombineS(a_objOut, &parentPathStr);

    if (a_workingPath != CBNULL)
    {
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

    CUBE_Path objectPath = CUBE_Path_CombineS(&outPath, &filenameObj);
    CUBE_String objectPathStr = CUBE_Path_ToNRString(&objectPath);

    CUBE_String outArg = CUBE_String_CreateC("-o ");
    CUBE_String_AppendS(&outArg, &objectPathStr);

    CUBE_CommandLine_AppendArgumentC(&commandLine, outArg.Data);
    CUBE_CommandLine_AppendArgumentC(&commandLine, sourceStr.Data);

    CUBE_String_Destroy(&outArg);
    CUBE_String_Destroy(&sourceStr);

    CUBE_Path_Destroy(&parentPath);
    CUBE_String_Destroy(&parentPathStr);

    CUBE_String_Destroy(&fileName);
    CUBE_String_Destroy(&extension);

    CUBE_String_Destroy(&filenameObj);

    CUBE_Path_Destroy(&outPath);

    CUBE_Path_Destroy(&objectPath);
    CUBE_String_Destroy(&objectPathStr);

    return commandLine;
}

CBBOOL CUBE_CProject_LinkProject(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath, const char* a_compilerPath, const CUBE_Path* a_objOut, CUBE_String** a_lines, CBUINT32* a_lineCount)
{
    CUBE_CommandLine commandLine = { 0 };
    if (a_workingPath != CBNULL)
    {
        CUBE_String_AppendC(&commandLine.Path, a_workingPath);
    }

    switch (a_project->Target)
    {
    case CUBE_CProjectTarget_StaticLibrary:
    {
        switch (a_compiler)
        {
        case CUBE_CProjectCompiler_GCC:
        {
            CUBE_String_AppendC(&commandLine.Command, CUBE_GCCAR_String);

            break;
        }
        case CUBE_CProjectCompiler_MinGW:
        {
            CUBE_String_AppendC(&commandLine.Command, CUBE_MinGWAR_String);

            break;
        }
        case CUBE_CProjectCompiler_Clang:
        {
            CUBE_String_AppendC(&commandLine.Command, CUBE_ClangAR_String);

            break;
        }
        case CUBE_CProjectCompiler_Zig:
        {
            CUBE_String_AppendC(&commandLine.Command, CUBE_ZigAR_String);
        }
        }

        CUBE_CommandLine_AppendArgumentC(&commandLine, "rcs");

        CUBE_StackString libName = { 0 };

        switch (a_compiler)
        {
        case CUBE_CProjectCompiler_GCC:
        case CUBE_CProjectCompiler_Clang:
        case CUBE_CProjectCompiler_Zig:
        {
            CUBE_StackString_AppendC(&libName, "lib");
            CUBE_StackString_AppendSS(&libName, &a_project->Name);
            CUBE_StackString_AppendC(&libName, ".a");

            break;
        }
        case CUBE_CProjectCompiler_MinGW:
        {
            CUBE_StackString_AppendSS(&libName, &a_project->Name);
            CUBE_StackString_AppendC(&libName, ".lib");

            break;
        }
        }

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

            CUBE_Path objDir = CUBE_Path_CombineP(a_objOut, &parentPath);
            CUBE_Path objPath = CUBE_Path_CombineS(&objDir, &objName);

            CUBE_String objPathStr = CUBE_Path_ToNRString(&objPath);

            CUBE_CommandLine_AppendArgumentC(&commandLine, objPathStr.Data);

            CUBE_Path_Destroy(&parentPath);

            CUBE_String_Destroy(&fileName);
            CUBE_String_Destroy(&objName);

            CUBE_Path_Destroy(&objDir);
            CUBE_Path_Destroy(&objPath);

            CUBE_String_Destroy(&objPathStr);
        }

        const int ret = CUBE_CommandLine_Execute(&commandLine, a_lines, a_lineCount);

        CUBE_CommandLine_Destroy(&commandLine);

        return ret == 0;
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
                CUBE_String_AppendC(&commandLine.Command, CUBE_GCC_String);

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
            case CUBE_CProjectCompiler_MinGW:
            {
                CUBE_String_AppendC(&commandLine.Command, CUBE_MinGW_String);

                CUBE_Path exePath = CUBE_Path_CombineSS(&a_project->OutputPath, &a_project->Name);
                CUBE_String exePathStr = CUBE_Path_ToString(&exePath);
                CUBE_String_AppendC(&exePathStr, ".exe");

                CUBE_String outArg = CUBE_String_CreateC("-o ");
                CUBE_String_AppendS(&outArg, &exePathStr);

                CUBE_CommandLine_AppendArgumentC(&commandLine, outArg.Data);

                CUBE_String_Destroy(&outArg);

                CUBE_Path_Destroy(&exePath);
                CUBE_String_Destroy(&exePathStr);

                break;
            }
            case CUBE_CProjectCompiler_Clang:
            {
                CUBE_String_AppendC(&commandLine.Command, CUBE_Clang_String);

                CUBE_Path exePath = CUBE_Path_CombineSS(&a_project->OutputPath, &a_project->Name);
                CUBE_String exePathStr = CUBE_Path_ToString(&exePath);

                CUBE_String outArg = CUBE_String_CreateC("-o ");
                CUBE_String_AppendS(&outArg, &exePathStr);

                CUBE_CommandLine_AppendArgumentS(&commandLine, &outArg);

                CUBE_String_Destroy(&outArg);

                CUBE_Path_Destroy(&exePath);
                CUBE_String_Destroy(&exePathStr);

                break;
            }
            case CUBE_CProjectCompiler_Zig:
            {
                switch (a_project->Language)
                {
                case CUBE_CProjectLanguage_C:
                {
                    CUBE_String_AppendC(&commandLine.Command, CUBE_ZigCC_String);

                    break;
                }
                case CUBE_CProjectLanguage_CPP:
                {
                    CUBE_String_AppendC(&commandLine.Command, CUBE_ZigCPP_String);

                    break;
                }
                }

                CUBE_Path exePath = CUBE_Path_CombineSS(&a_project->OutputPath, &a_project->Name);
                CUBE_String exePathStr = CUBE_Path_ToString(&exePath);

                CUBE_String outArg = CUBE_String_CreateC("-o ");
                CUBE_String_AppendS(&outArg, &exePathStr);

                CUBE_CommandLine_AppendArgumentS(&commandLine, &outArg);

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
            CUBE_String_AppendC(&commandLine.Command, CUBE_GCC_String);

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
        case CUBE_CProjectCompiler_MinGW:
        {
            CUBE_String_AppendC(&commandLine.Command, CUBE_MinGW_String);

            CUBE_CommandLine_AppendArgumentC(&commandLine, "-shared");

            CUBE_StackString libName = CUBE_StackString_MergeC(&a_project->Name, ".dll");
            CUBE_Path libPath = CUBE_Path_CombineSS(&a_project->OutputPath, &libName);
            CUBE_String libPathStr = CUBE_Path_ToString(&libPath);

            CUBE_String outArg = CUBE_String_CreateC("-o ");
            CUBE_String_AppendS(&outArg, &libPathStr);

            CUBE_CommandLine_AppendArgumentC(&commandLine, outArg.Data);

            CUBE_String_Destroy(&outArg);

            CUBE_Path_Destroy(&libPath);
            CUBE_String_Destroy(&libPathStr);
        }
        case CUBE_CProjectCompiler_Clang:
        {
            CUBE_String_AppendC(&commandLine.Command, CUBE_Clang_String);

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
        case CUBE_CProjectCompiler_Zig:
        {
            switch (a_project->Language)
            {
            case CUBE_CProjectLanguage_C:
            {
                CUBE_String_AppendC(&commandLine.Command, CUBE_ZigCC_String);

                break;
            }
            case CUBE_CProjectLanguage_CPP:
            {
                CUBE_String_AppendC(&commandLine.Command, CUBE_ZigCPP_String);

                break;
            }
            }

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

        CUBE_Path objDir = CUBE_Path_CombineP(a_objOut, &parentPath);
        CUBE_Path objPath = CUBE_Path_CombineS(&objDir, &objName);

        CUBE_String objPathStr = CUBE_Path_ToNRString(&objPath);

        CUBE_CommandLine_AppendArgumentC(&commandLine, objPathStr.Data);

        CUBE_Path_Destroy(&parentPath);

        CUBE_String_Destroy(&fileName);
        CUBE_String_Destroy(&objName);

        CUBE_Path_Destroy(&objDir);
        CUBE_Path_Destroy(&objPath);

        CUBE_String_Destroy(&objPathStr);
    }

    if (a_project->LibraryCount > 0)
    {
        // Not all compilers support groups
        // Got sick of rearranging dependencies
        // Should also resolve circular depedencies
        // Naive but ehh it works
        for (CBUINT32 i = 0; i < 2; ++i)
        {
            for (CBUINT32 j = 0; j < a_project->LibraryCount - i; ++j)
            {
                CUBE_String libraryStr = CUBE_Path_ToString(&a_project->Libraries[j]);

                CUBE_CommandLine_AppendArgumentC(&commandLine, libraryStr.Data);

                CUBE_String_Destroy(&libraryStr);
            }
        }
    }

    // Should be pretty safe to include normally as they are system libraries and are often pretty self contained
    // Not always but has not been an issue so far
    for (CBUINT32 i = 0; i < a_project->ReferenceCount; ++i)
    {
        CUBE_String reference = CUBE_String_CreateC("-l");
        CUBE_String_AppendS(&reference, &a_project->References[i]);

        CUBE_CommandLine_AppendArgumentC(&commandLine, reference.Data);

        CUBE_String_Destroy(&reference);
    }

    for (CBUINT32 i = 0; i < a_project->CFlagCount; ++i)
    {
        CUBE_CommandLine_AppendArgumentC(&commandLine, a_project->CFlags[i].Data);
    }

    const int ret = CUBE_CommandLine_Execute(&commandLine, a_lines, a_lineCount);

    CUBE_CommandLine_Destroy(&commandLine);

    return ret == 0;
}

CUBEI_CProjectStatus CUBEI_CProject_GetProjectStatus(const CUBE_CProject* a_project, const char* a_workingPath)
{
    CUBEI_CProjectStatus status = { 0 };

    CUBE_Path workingPath = { 0 };
    if (a_workingPath != CBNULL)
    {
        workingPath = CUBE_Path_CreateC(a_workingPath);
    }

    CUBE_Path outPath = CUBE_Path_CombineP(&workingPath, &a_project->OutputPath);

    CUBE_String fileName = CUBE_String_CreateSS(&a_project->Name);
    CUBE_String_AppendC(&fileName, ".cube");
    
    CUBE_Path filePath = CUBE_Path_CombineS(&outPath, &fileName);
    CUBE_String pathStr = CUBE_Path_ToString(&filePath);

    FILE* file = fopen(pathStr.Data, "r");
    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        const CBUINT64 size = (CBUINT64)ftell(file);
        rewind(file);

        char* fileStr = (char*)malloc(size + 1);
        fread(fileStr, size, 1, file);
        fileStr[size] = 0;

        fclose(file);

        typedef enum 
        {
            ReadMode_Null,
            ReadMode_Path,
            ReadMode_Write,
            ReadMode_CmdStr
        } e_ReadMode;

        e_ReadMode readMode = ReadMode_Null;

        CUBE_String cmdString = { 0 };
        CUBE_Path srcPath = { 0 };
        CBTIME writeTime = 0;

        const char* strStart = fileStr;
        const char* slider = fileStr;
        while (*slider != 0)
        {
            const char chr = *slider;

            switch (chr)
            {
            case '[':
            {
                readMode = ReadMode_Null;

                CUBE_String_Destroy(&cmdString);
                CUBE_Path_Destroy(&srcPath);
                writeTime = 0;

                break;
            }
            case ' ':
            {
                if (readMode == ReadMode_Null)
                {
                    strStart = slider + 1;
                }

                break;
            }
            case ':':
            {
                const CBUINT32 len = (CBUINT32)(slider - strStart);

                if (strncmp(strStart, "PATH", len) == 0)
                {
                    readMode = ReadMode_Path;
                }
                else if (strncmp(strStart, "WRITE", len) == 0)
                {
                    readMode = ReadMode_Write;
                }
                else if (strncmp(strStart, "CMDSTR", len) == 0)
                {
                    readMode = ReadMode_CmdStr;
                }

                strStart = slider + 1;

                break;
            }
            case '\n':
            {
                switch (readMode)
                {
                case ReadMode_Path:
                {
                    CUBE_Path_Destroy(&srcPath);

                    srcPath = CUBE_Path_CreateCL(strStart, (CBUINT32)(slider - strStart));

                    break;
                }
                case ReadMode_Write:
                {
                    writeTime = (CBTIME)strtoul(strStart, NULL, 10);

                    break;
                }
                case ReadMode_CmdStr:
                {
                    CUBE_String_Destroy(&cmdString);

                    cmdString = CUBE_String_CreateCL(strStart, (CBUINT32)(slider - strStart));

                    break;
                }
                default:
                {
                    break;
                }
                }

                strStart = slider + 1;
                readMode = ReadMode_Null;

                break;
            }
            case ']':
            {
                CUBEI_CProject_AppendStatusSource(&status, &srcPath, &cmdString, writeTime);

                break;
            }
            }

            ++slider;
        }

        free(fileStr);
    }

    CUBE_Path_Destroy(&workingPath);
    CUBE_Path_Destroy(&outPath);
    CUBE_String_Destroy(&fileName);
    CUBE_Path_Destroy(&filePath);
    CUBE_String_Destroy(&pathStr);

    return status;
}
void CUBEI_CProject_WriteProjectStatus(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath)
{
    CUBE_Path workingPath = { 0 };

    if (a_workingPath != CBNULL)
    {
        workingPath = CUBE_Path_CreateC(a_workingPath);
    }

    CUBE_Path outPath = CUBE_Path_CombineP(&workingPath, &a_project->OutputPath);

    CUBE_String fileName = CUBE_String_CreateC(a_project->Name.Data);
    CUBE_String_AppendC(&fileName, ".cube");

    CUBE_Path filePath = CUBE_Path_CombineS(&outPath, &fileName);
    CUBE_String filePathStr = CUBE_Path_ToString(&filePath);

    FILE* file = fopen(filePathStr.Data, "w+");

    CUBE_String_Destroy(&fileName);
    CUBE_String_Destroy(&filePathStr);
    CUBE_Path_Destroy(&filePath);

    if (file == NULL)
    {
        CUBE_Path_Destroy(&workingPath);
        CUBE_Path_Destroy(&outPath);

        return;
    }

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        fprintf(file, "[\n");

        CUBE_String sourceStr = CUBE_Path_ToString(&(a_project->Sources[i]));

        fprintf(file, "   PATH:%s\n", sourceStr.Data);
        
        CUBE_Path sourcePath = CUBE_Path_CombineP(&workingPath, &(a_project->Sources[i]));

        CUBE_Stat stat = CUBE_IO_StatP(&sourcePath);
        fprintf(file, "   WRITE:%u\n", stat.WriteTime);

        CUBE_String cmdStr = CUBEI_CProject_CreateSourceCommandString(a_project, a_compiler, &workingPath, i);

        fprintf(file, "   CMDSTR:%s\n", cmdStr.Data);
        
        CUBE_Path_Destroy(&sourcePath);
        
        CUBE_String_Destroy(&sourceStr);

        CUBE_String_Destroy(&cmdStr);

        fprintf(file, "]\n");
    }

    fclose(file);

    CUBE_Path_Destroy(&workingPath);
    CUBE_Path_Destroy(&outPath);
}
void CUBEI_CProject_AppendStatusSource(CUBEI_CProjectStatus* a_project, const CUBE_Path* a_source, const CUBE_String* a_sourceString, CBTIME a_writeTime)
{
    CBUINT32 newSize = a_project->SourceCount + 1;

    a_project->Sources = (CUBEI_SourceStatus*)realloc(a_project->Sources, sizeof(CUBEI_SourceStatus) * newSize);

    a_project->Sources[a_project->SourceCount].Source = CUBE_Path_Copy(a_source);
    a_project->Sources[a_project->SourceCount].SourceString = CUBE_String_Copy(a_sourceString);
    a_project->Sources[a_project->SourceCount].WriteTime = a_writeTime;

    a_project->SourceCount = newSize;
}
void CUBEI_CProject_DestroyProjectStatus(CUBEI_CProjectStatus* a_project)
{
    if (a_project->Sources != CBNULL)
    {
        for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
        {
            CUBE_Path_Destroy(&a_project->Sources[i].Source);
            CUBE_String_Destroy(&a_project->Sources[i].SourceString);
        }

        free(a_project->Sources);
        a_project->Sources = CBNULL;
    }

    a_project->SourceCount = 0;
}

CBBOOL CUBE_CProject_MultiCompile(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath, const char* a_compilerPath, CBUINT32 a_jobs, CUBE_String** a_lines, CBUINT32* a_lineCount, CBBOOL a_forceBuild)
{
    CUBE_Path objectOutpath = CUBE_Path_CombineC(&a_project->OutputPath, "obj");

    CUBEI_CProjectStatus oldStatus = { 0 };

#ifdef CUBE_PRETTY_PRINT        
    printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ] " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "Reading CUBE Data " CUBEI_CPROJECT_CONSOLECOLOUR_RESET "\n", a_project->Name.Data);
#endif
    if (!a_forceBuild)
    {
        oldStatus = CUBEI_CProject_GetProjectStatus(a_project, a_workingPath);
    }

    typedef struct
    {
        CBBOOL State;
        CUBE_CommandLine CommandLine;
    } ObjectData;

    CBUINT32 input = 0;
    if (a_jobs < 1)
    {
        a_jobs = 1;
    }

    ObjectData* commandObjects = (ObjectData*)malloc(sizeof(ObjectData) * a_jobs);
    memset(commandObjects, 0, sizeof(ObjectData) * a_jobs);

    CUBE_Path workingPath = { 0 };
    if (a_workingPath != CBNULL)
    {
        workingPath = CUBE_Path_CreateC(a_workingPath);
    }

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
#ifdef CUBE_PRETTY_PRINT
        const CBUINT32 printPercentage = (CBUINT32)(((double)i / a_project->SourceCount) * 100);
        
        CUBE_String printPath = CUBE_Path_ToString(&(a_project->Sources[i]));
        
        printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ][ %d% ] " CUBEI_CPROJECT_CONSOLECOLOUR_GREEN "Compiling \"%s\" " CUBEI_CPROJECT_CONSOLECOLOUR_RESET " \n", a_project->Name.Data, printPercentage, printPath.Data);
        
        CUBE_String_Destroy(&printPath);
#endif
        CUBE_CommandLine commandLine = { 0 };

        for (CBUINT32 j = 0; j < oldStatus.SourceCount; ++j)
        {
            if (CUBE_Path_Equals(&(oldStatus.Sources[j].Source), &(a_project->Sources[i])))
            {
                CUBE_Path srcPath = CUBE_Path_CombineP(&workingPath, &(a_project->Sources[i]));

                CUBE_Stat stat = CUBE_IO_StatP(&srcPath);

                CUBE_Path_Destroy(&srcPath);

                if (stat.WriteTime != oldStatus.Sources[j].WriteTime)
                {
                    break;
                }

                CUBE_String str = CUBEI_CProject_CreateSourceCommandString(a_project, a_compiler, &workingPath, i);

                if (!CUBE_String_Equals(&str, &oldStatus.Sources[j].SourceString))
                {
                    CUBE_String_Destroy(&str);

                    break;
                }

                CUBE_String_Destroy(&str);

                goto NextMultiSource;
            }
        }

        commandLine = CUBEI_CProject_CreateObjectFileCommandLine(a_project, a_compiler, a_workingPath, a_compilerPath, &a_project->Sources[i], &objectOutpath);

        while (1)
        {
            input = (input + 1) % a_jobs;

            if (!commandObjects[input].State)
            {
                CUBE_CommandLine_BeginExecution(&commandLine);

                commandObjects[input].State = CBTRUE;
                commandObjects[input].CommandLine = commandLine;

                break;
            }

            CUBE_CommandLine prevCmdLine = commandObjects[input].CommandLine;

            if (!CUBE_CommandLine_PollExecution(&prevCmdLine))
            {
                const int ret = CUBE_CommandLine_EndExecution(&prevCmdLine, a_lines, a_lineCount);

                commandObjects[input].State = CBFALSE;
                CUBE_CommandLine_Destroy(&prevCmdLine);

                if (ret == 0)
                {
                    CUBE_CommandLine_BeginExecution(&commandLine);

                    commandObjects[input].State = CBTRUE;
                    commandObjects[input].CommandLine = commandLine;
                }
                else
                {
                    for (CBUINT32 i = 0; i < a_jobs; ++i)
                    {
                        if (commandObjects[i].State)
                        {
                            CUBE_CommandLine cmdLine = commandObjects[i].CommandLine;

                            while (CUBE_CommandLine_PollExecution(&cmdLine)) { }

                            CUBE_CommandLine_EndExecution(&cmdLine, a_lines, a_lineCount);
                            CUBE_CommandLine_Destroy(&cmdLine);

                            commandObjects[i].State = CBFALSE;
                        }
                    }

                    free(commandObjects);
                    CUBE_Path_Destroy(&objectOutpath);

                    return CBFALSE;
                }

                break;
            }
        }

NextMultiSource:;
    }

    CUBE_Path_Destroy(&workingPath);

    CBBOOL ret = CBTRUE;

    for (CBUINT32 i = 0; i < a_jobs; ++i)
    {
        if (commandObjects[i].State)
        {
            CUBE_CommandLine cmdLine = commandObjects[i].CommandLine;

            while (CUBE_CommandLine_PollExecution(&cmdLine)) { }

            if (CUBE_CommandLine_EndExecution(&cmdLine, a_lines, a_lineCount) != 0)
            {
                ret = CBFALSE;
            }
            
            CUBE_CommandLine_Destroy(&cmdLine);

            commandObjects[i].State = CBFALSE;
        }
    }

    free(commandObjects);

    if (!ret)
    {
        return CBFALSE;
    }

#ifdef CUBE_PRETTY_PRINT
    printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ][ 100% ] Linking Project\n", a_project->Name.Data);
#endif
    ret = CUBE_CProject_LinkProject(a_project, a_compiler, a_workingPath, a_compilerPath, &objectOutpath, a_lines, a_lineCount);

    CUBE_Path_Destroy(&objectOutpath);
    CUBEI_CProject_DestroyProjectStatus(&oldStatus);

    if (ret)
    {
#ifdef CUBE_PRETTY_PRINT        
        printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ] " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "Writing CUBE Data " CUBEI_CPROJECT_CONSOLECOLOUR_RESET "\n", a_project->Name.Data);
#endif

        CUBEI_CProject_WriteProjectStatus(a_project, a_compiler, a_workingPath);

#ifdef CUBE_PRETTY_PRINT        
        printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ] Done\n", a_project->Name.Data);
#endif
    }

    return ret;
}
CBBOOL CUBE_CProject_Compile(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath, const char* a_compilerPath, CUBE_String** a_lines, CBUINT32* a_lineCount, CBBOOL a_forceBuild)
{
    CUBE_Path objectOutpath = CUBE_Path_CombineC(&a_project->OutputPath, "obj");

    CUBEI_CProjectStatus oldStatus = { 0 };

#ifdef CUBE_PRETTY_PRINT        
    printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ] " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "Reading CUBE Data " CUBEI_CPROJECT_CONSOLECOLOUR_RESET "\n", a_project->Name.Data);
#endif
    if (!a_forceBuild)
    {
        oldStatus = CUBEI_CProject_GetProjectStatus(a_project, a_workingPath);
    }

    CUBE_Path workingPath = { 0 };
    if (a_workingPath != CBNULL)
    {
        workingPath = CUBE_Path_CreateC(a_workingPath);
    }

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
#ifdef CUBE_PRETTY_PRINT
        const CBUINT32 printPercentage = (CBUINT32)(((double)i / a_project->SourceCount) * 100);
        
        CUBE_String printPath = CUBE_Path_ToString(&(a_project->Sources[i]));
        
        printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ][ %d% ] " CUBEI_CPROJECT_CONSOLECOLOUR_GREEN "Compiling \"%s\" " CUBEI_CPROJECT_CONSOLECOLOUR_RESET " \n", a_project->Name.Data, printPercentage, printPath.Data);
        
        CUBE_String_Destroy(&printPath);
#endif
        CUBE_CommandLine commandLine = { 0 };
        int ret;

        for (CBUINT32 j = 0; j < oldStatus.SourceCount; ++j)
        {
            if (CUBE_Path_Equals(&(oldStatus.Sources[j].Source), &(a_project->Sources[i])))
            {
                CUBE_Path srcPath = CUBE_Path_CombineP(&workingPath, &(a_project->Sources[i]));
            
                CUBE_Stat stat = CUBE_IO_StatP(&srcPath);
            
                CUBE_Path_Destroy(&srcPath);
            
                if (stat.WriteTime != oldStatus.Sources[j].WriteTime)
                {
                    break;
                }
            
                CUBE_String str = CUBEI_CProject_CreateSourceCommandString(a_project, a_compiler, &workingPath, i);
            
                if (!CUBE_String_Equals(&str, &oldStatus.Sources[j].SourceString))
                {
                    CUBE_String_Destroy(&str);
                
                    break;
                }
            
                CUBE_String_Destroy(&str);
            
                goto NextSource;
            }
        }


        commandLine = CUBEI_CProject_CreateObjectFileCommandLine(a_project, a_compiler, a_workingPath, a_compilerPath, &a_project->Sources[i], &objectOutpath);
        ret = CUBE_CommandLine_Execute(&commandLine, a_lines, a_lineCount);

        CUBE_CommandLine_Destroy(&commandLine);

        if (ret != 0)
        {
            CUBE_Path_Destroy(&objectOutpath);

            return CBFALSE;
        }

NextSource:;
    }

    CUBE_Path_Destroy(&workingPath);

#ifdef CUBE_PRETTY_PRINT
    printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ][ 100% ] Linking Project\n", a_project->Name.Data);
#endif
    const CBBOOL ret = CUBE_CProject_LinkProject(a_project, a_compiler, a_workingPath, a_compilerPath, &objectOutpath, a_lines, a_lineCount);

    CUBE_Path_Destroy(&objectOutpath);

    if (ret)
    {
#ifdef CUBE_PRETTY_PRINT        
        printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ] " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "Writing CUBE Data " CUBEI_CPROJECT_CONSOLECOLOUR_RESET "\n", a_project->Name.Data);
#endif

        CUBEI_CProject_WriteProjectStatus(a_project, a_compiler, a_workingPath);

#ifdef CUBE_PRETTY_PRINT        
        printf("[ " CUBEI_CPROJECT_CONSOLECOLOUR_CYAN "%s" CUBEI_CPROJECT_CONSOLECOLOUR_RESET " ] Done\n", a_project->Name.Data);
#endif
    }

    return ret;
}

CUBE_String CUBEI_CProject_CreateSourceCommandString(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const CUBE_Path* a_workingPath, CBUINT32 a_source)
{
    CUBE_String str = { 0 };

    switch (a_compiler)
    {
    case CUBE_CProjectCompiler_GCC:
    {
        CUBE_String_AppendC(&str, CUBE_GCC_String);

        break;
    }
    case CUBE_CProjectCompiler_MinGW:
    {
        CUBE_String_AppendC(&str, CUBE_MinGW_String);

        break;
    }
    case CUBE_CProjectCompiler_Clang:
    {
        CUBE_String_AppendC(&str, CUBE_Clang_String);

        break;
    }
    case CUBE_CProjectCompiler_Zig:
    {
        switch (a_project->Language) 
        {
        case CUBE_CProjectLanguage_C:
        {
            CUBE_String_AppendC(&str, CUBE_ZigCC_String);

            break;
        }
        case CUBE_CProjectLanguage_CPP:
        {
            CUBE_String_AppendC(&str, CUBE_ZigCPP_String);

            break;
        }
        }

        break;
    }
    }

    switch (a_project->Language)
    {
    case CUBE_CProjectLanguage_C:
    {
        CUBE_String_AppendC(&str, " -x c");

        break;
    }
    case CUBE_CProjectLanguage_CPP:
    {
        CUBE_String_AppendC(&str, " -x c++");

        break;
    }
    }

    for (CBUINT32 i = 0; i < a_project->DefineCount; ++i)
    {
        CUBE_String_AppendC(&str, " -D");
        CUBE_String_AppendS(&str, &a_project->Defines[i]);
    }

    if (a_project->Language == CUBE_CProjectLanguage_C)
    {
        CUBE_String_AppendC(&str, " -DCUBE_LANGUAGE_C");
    }
    else
    {
        CUBE_String_AppendC(&str, " -DCUBE_LANGUAGE_CPP");
    }

    for (CBUINT32 i = 0; i < a_project->IncludePathCount; ++i)
    {
        CUBE_String_AppendC(&str, " -I");
        
        CUBE_Path includePath = CUBE_Path_CombineP(a_workingPath, &a_project->IncludePaths[i]);
        CUBE_String includePathStr = CUBE_Path_ToString(&includePath);

        CUBE_String_AppendS(&str, &includePathStr);

        CUBE_Path_Destroy(&includePath);
        CUBE_String_Destroy(&includePathStr);
    }

    for (CBUINT32 i = 0; i < a_project->CFlagCount; ++i)
    {
        CUBE_String_AppendC(&str, " ");
        CUBE_String_AppendS(&str, &a_project->CFlags[i]);
    }

    CUBE_Path outPath = CUBE_Path_CombineP(a_workingPath, &a_project->OutputPath);

    CUBE_Path sourcePath = CUBE_Path_CombineP(a_workingPath, &a_project->Sources[a_source]);
    CUBE_String sourcePathStr = CUBE_Path_ToString(&sourcePath);

    CUBE_Path parentPath = CUBE_Path_ParentPath(&a_project->Sources[a_source]);
    CUBE_String fileName = CUBE_Path_Filename(&sourcePath);
    CUBE_Path objOutPath = CUBE_Path_CombineP(&outPath, &parentPath);
    CUBE_Path objPath = CUBE_Path_CombineS(&objOutPath, &fileName);
    CUBE_String objPathStr = CUBE_Path_ToNRString(&objPath);

    CUBE_String_AppendC(&str, " -o ");
    CUBE_String_AppendS(&str, &objPathStr);
    CUBE_String_AppendC(&str, ".o");

    CUBE_String_AppendC(&str, " -c ");

    CUBE_String_AppendS(&str, &sourcePathStr);

    CUBE_Path_Destroy(&parentPath);
    CUBE_String_Destroy(&fileName);
    CUBE_Path_Destroy(&objOutPath);
    CUBE_Path_Destroy(&objPath);
    CUBE_String_Destroy(&objPathStr);

    CUBE_Path_Destroy(&sourcePath);
    CUBE_String_Destroy(&sourcePathStr);

    CUBE_Path_Destroy(&outPath);

    return str;
}
CUBE_String CUBEI_CProject_CreateProjectCompileCommands(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const CUBE_Path* a_workingPath)
{
    CUBE_String compileCommands = { 0 };
    CUBE_Path outPath = CUBE_Path_CombineP(a_workingPath, &a_project->OutputPath);
    CUBE_String outPathStr = CUBE_Path_ToString(&outPath);

    CUBE_String directory = CUBE_String_CreateC("    \"directory\": \"");
    CUBE_String_AppendS(&directory, &outPathStr);
    CUBE_String_AppendC(&directory, "\",\n");

    for (CBUINT32 i = 0; i < a_project->SourceCount; ++i)
    {
        CUBE_String_AppendC(&compileCommands, "  {\n");

        CUBE_String_AppendS(&compileCommands, &directory);

        CUBE_String_AppendC(&compileCommands, "    \"command\": \"");

        CUBE_String cmdStr = CUBEI_CProject_CreateSourceCommandString(a_project, a_compiler, a_workingPath, i);

        CUBE_String_AppendS(&compileCommands, &cmdStr);

        CUBE_String_Destroy(&cmdStr);

        CUBE_Path sourcePath = CUBE_Path_CombineP(a_workingPath, &(a_project->Sources[i]));
        CUBE_String sourcePathStr = CUBE_Path_ToString(&sourcePath);

        CUBE_String_AppendC(&compileCommands, "\",\n");

        CUBE_String_AppendC(&compileCommands, "    \"file\": \"");
        CUBE_String_AppendS(&compileCommands, &sourcePathStr);
        CUBE_String_AppendC(&compileCommands, "\",\n");

        CUBE_Path parentPath = CUBE_Path_ParentPath(&a_project->Sources[i]);
        CUBE_String fileName = CUBE_Path_Filename(&sourcePath);
        CUBE_Path objOutPath = CUBE_Path_CombineP(&outPath, &parentPath);
        CUBE_Path objPath = CUBE_Path_CombineS(&objOutPath, &fileName);
        CUBE_String objPathStr = CUBE_Path_ToNRString(&objPath);

        CUBE_String_AppendC(&compileCommands, "    \"output\": \"");
        CUBE_String_AppendS(&compileCommands, &objPathStr);
        CUBE_String_AppendC(&compileCommands, ".o\"\n");

        CUBE_Path_Destroy(&sourcePath);
        CUBE_String_Destroy(&sourcePathStr);

        CUBE_Path_Destroy(&parentPath);
        CUBE_String_Destroy(&fileName);
        CUBE_Path_Destroy(&objOutPath);
        CUBE_Path_Destroy(&objPath);
        CUBE_String_Destroy(&objPathStr);

        CUBE_String_AppendC(&compileCommands, "  }");

        if (i < a_project->SourceCount - 1)
        {
            CUBE_String_AppendC(&compileCommands, ",\n");
        }
        else 
        {
            CUBE_String_AppendC(&compileCommands, "\n");
        }
    }

    CUBE_String_Destroy(&directory);
    CUBE_Path_Destroy(&outPath);
    CUBE_String_Destroy(&outPathStr);

    return compileCommands;
}

CUBE_String CUBE_CProject_GenerateCompileCommands(const CUBE_CProject* a_project, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath)
{
    CUBE_String compileCommands = { 0 };
    CUBE_String_AppendC(&compileCommands, "[\n");

    CUBE_Path workingPath = CUBE_Path_CreateC(a_workingPath);
    
    CUBE_String str = CUBEI_CProject_CreateProjectCompileCommands(a_project, a_compiler, &workingPath);
    CUBE_String_AppendS(&compileCommands, &str);
    CUBE_String_Destroy(&str);

    CUBE_Path_Destroy(&workingPath);
    
    CUBE_String_AppendC(&compileCommands, "]\n");

    return compileCommands;
}
CUBE_String CUBE_CProject_GenerateMultiCompileCommands(const CUBE_CProject* a_projects, CBUINT32 a_projectCount, e_CUBE_CProjectCompiler a_compiler, const char* a_workingPath)
{
    CUBE_String compileCommands = { 0 };
    CUBE_String_AppendC(&compileCommands, "[\n");

    CUBE_Path workingPath = CUBE_Path_CreateC(a_workingPath);

    const CBUINT32 lastIndex = a_projectCount - 1;
    for (CBUINT32 i = 0; i < a_projectCount; ++i)
    {
        CUBE_String str = CUBEI_CProject_CreateProjectCompileCommands(&(a_projects[i]), a_compiler, &workingPath);
        CUBE_String_AppendS(&compileCommands, &str);
        CUBE_String_Destroy(&str);

        if (i < lastIndex)
        {
            CUBE_String_AppendC(&compileCommands, ",\n");
        }
    }

    CUBE_Path_Destroy(&workingPath);

    CUBE_String_AppendC(&compileCommands, "]\n");

    return compileCommands;
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
