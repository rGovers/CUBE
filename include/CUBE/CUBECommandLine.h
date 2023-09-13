#ifndef INCLUDED_HEADER_CUBECOMMANDLINE
#define INCLUDED_HEADER_CUBECOMMANDLINE

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "CUBE/CUBEString.h"

typedef struct
{
    CUBE_String Command;
    CUBE_String Path;
    CUBE_String* Arguments;
    CBUINT32 ArgumentCount;
} CUBE_CommandLine;

void CUBE_CommandLine_AppendArgumentC(CUBE_CommandLine* a_commandLine, const char* a_argument);
void CUBE_CommandLine_AppendArgumentS(CUBE_CommandLine* a_commandLine, const CUBE_StackString* a_argument);

void CUBE_CommandLine_Execute(CUBE_CommandLine* a_commandLine);

void CUBE_CommandLine_Destroy(CUBE_CommandLine* a_commandLine);

#ifdef CUBE_IMPLEMENTATION
void CUBE_CommandLine_AppendArgumentC(CUBE_CommandLine* a_commandLine, const char* a_argument)
{
    const CBUINT32 argumentCount = a_commandLine->ArgumentCount++;

    const CUBE_String argument = CUBE_String_CreateC(a_argument);

    a_commandLine->Arguments = (CUBE_String*)realloc(a_commandLine->Arguments, sizeof(CUBE_String) * a_commandLine->ArgumentCount);

    a_commandLine->Arguments[argumentCount] = argument;
}
void CUBE_CommandLine_AppendArgumentS(CUBE_CommandLine* a_commandLine, const CUBE_StackString* a_argument)
{
    const CBUINT32 argumentCount = a_commandLine->ArgumentCount++;

    const CUBE_String argument = CUBE_String_CreateSS(a_argument);

    a_commandLine->Arguments = (CUBE_String*)realloc(a_commandLine->Arguments, sizeof(CUBE_String) * a_commandLine->ArgumentCount);

    a_commandLine->Arguments[argumentCount] = argument;
}

void CUBE_CommandLine_Execute(CUBE_CommandLine* a_commandLine)
{
    CUBE_String cmdString = { 0 };

    if (a_commandLine->Path.Length > 0)
    {
        CUBE_String_AppendC(&cmdString, "cd ");
        CUBE_String_AppendS(&cmdString, &a_commandLine->Path);
        CUBE_String_AppendC(&cmdString, " && ");
    }

    CUBE_String_AppendS(&cmdString, &a_commandLine->Command);
    CUBE_String_AppendC(&cmdString, " ");

    for (CBUINT32 i = 0; i < a_commandLine->ArgumentCount; ++i)
    {
        CUBE_String_AppendS(&cmdString, &a_commandLine->Arguments[i]);
        CUBE_String_AppendC(&cmdString, " ");
    }

    system(cmdString.Data);
}

void CUBE_CommandLine_Destroy(CUBE_CommandLine* a_commandLine)
{
    CUBE_String_Destroy(&a_commandLine->Command);
    CUBE_String_Destroy(&a_commandLine->Path);

    for (CBUINT32 i = 0; i < a_commandLine->ArgumentCount; ++i)
    {
        CUBE_String_Destroy(&a_commandLine->Arguments[i]);
    }

    free(a_commandLine->Arguments);

    a_commandLine->ArgumentCount = 0;
    a_commandLine->Arguments = CBNULL;
}
#endif

#ifdef __cplusplus
}
#endif

#endif