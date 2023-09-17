#ifndef INCLUDED_HEADER_CUBECOMMANDLINE
#define INCLUDED_HEADER_CUBECOMMANDLINE

#include <stdio.h>
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
    CUBE_String* EnvironmentVariables;
    CUBE_String* EnvironmentValues;
    CBUINT32 EnvironmentVariableCount;
} CUBE_CommandLine;

void CUBE_CommandLine_AppendArgumentC(CUBE_CommandLine* a_commandLine, const char* a_argument);
void CUBE_CommandLine_AppendArgumentSS(CUBE_CommandLine* a_commandLine, const CUBE_StackString* a_argument);

void CUBE_CommandLine_AppendEnvironmentVariableC(CUBE_CommandLine* a_commandLine, const char* a_variable, const char* a_value);

int CUBE_CommandLine_Execute(const CUBE_CommandLine* a_commandLine, CUBE_String** a_lines, CBUINT32* a_lineCount);

void CUBE_CommandLine_Destroy(CUBE_CommandLine* a_commandLine);

#ifdef CUBE_IMPLEMENTATION
// #if 1

void CUBE_CommandLine_AppendArgumentC(CUBE_CommandLine* a_commandLine, const char* a_argument)
{
    const CBUINT32 argumentCount = a_commandLine->ArgumentCount++;

    const CUBE_String argument = CUBE_String_CreateC(a_argument);

    a_commandLine->Arguments = (CUBE_String*)realloc(a_commandLine->Arguments, sizeof(CUBE_String) * a_commandLine->ArgumentCount);

    a_commandLine->Arguments[argumentCount] = argument;
}
void CUBE_CommandLine_AppendArgumentSS(CUBE_CommandLine* a_commandLine, const CUBE_StackString* a_argument)
{
    const CBUINT32 argumentCount = a_commandLine->ArgumentCount++;

    const CUBE_String argument = CUBE_String_CreateSS(a_argument);

    a_commandLine->Arguments = (CUBE_String*)realloc(a_commandLine->Arguments, sizeof(CUBE_String) * a_commandLine->ArgumentCount);

    a_commandLine->Arguments[argumentCount] = argument;
}

void CUBE_CommandLine_AppendEnvironmentVariableC(CUBE_CommandLine* a_commandLine, const char* a_variable, const char* a_value)
{
    const CBUINT32 variableCount = a_commandLine->EnvironmentVariableCount++;

    const CUBE_String variable = CUBE_String_CreateC(a_variable);
    const CUBE_String value = CUBE_String_CreateC(a_value);

    a_commandLine->EnvironmentVariables = (CUBE_String*)realloc(a_commandLine->EnvironmentVariables, sizeof(CUBE_String) * a_commandLine->EnvironmentVariableCount);
    a_commandLine->EnvironmentValues = (CUBE_String*)realloc(a_commandLine->EnvironmentValues, sizeof(CUBE_String) * a_commandLine->EnvironmentVariableCount);

    a_commandLine->EnvironmentVariables[variableCount] = variable;
    a_commandLine->EnvironmentValues[variableCount] = value;
}

int CUBE_CommandLine_Execute(const CUBE_CommandLine* a_commandLine, CUBE_String** a_lines, CBUINT32* a_lineCount)
{
    CUBE_String cmdString = { 0 };

    for (CBUINT32 i = 0; i < a_commandLine->EnvironmentVariableCount; ++i)
    {
        CUBE_String_AppendC(&cmdString, "export ");
        CUBE_String_AppendS(&cmdString, &a_commandLine->EnvironmentVariables[i]);
        CUBE_String_AppendC(&cmdString, "=\"");
        CUBE_String_AppendS(&cmdString, &a_commandLine->EnvironmentValues[i]);
        CUBE_String_AppendC(&cmdString, "\" && ");
    }

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

#ifdef CUBE_PRINT_COMMANDS
    printf("[CMD] %s\n", cmdString.Data);
#endif

#if WIN32
    // TODO: Implement this for windows
#else
    FILE* fp = popen(cmdString.Data, "r");
    if (fp == NULL)
    {
        return -1;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        const char* s = buffer;
        const char* e = s;

        while (*s != '\0')
        {
            if (*s == '\n')
            {
                CUBE_String str = CUBE_String_CreateCL(e, s - e);

                e = s + 1;

                *a_lines = (CUBE_String*)realloc(*a_lines, sizeof(CUBE_String) * (*a_lineCount + 1));
                (*a_lines)[*a_lineCount].Length = str.Length;
                (*a_lines)[*a_lineCount].Data = str.Data;
                *a_lineCount += 1;
            }

            ++s;
        }

        if (e != s)
        {
            CUBE_String str = CUBE_String_CreateCL(e, s - e);

            *a_lines = (CUBE_String*)realloc(*a_lines, sizeof(CUBE_String) * (*a_lineCount + 1));
            (*a_lines)[*a_lineCount].Length = str.Length;
            (*a_lines)[*a_lineCount].Data = str.Data;
            *a_lineCount += 1;
        }
    }

    return pclose(fp);
#endif
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

    for (CBUINT32 i = 0; i < a_commandLine->EnvironmentVariableCount; ++i)
    {
        CUBE_String_Destroy(&a_commandLine->EnvironmentVariables[i]);
        CUBE_String_Destroy(&a_commandLine->EnvironmentValues[i]);
    }
}
#endif

#ifdef __cplusplus
}
#endif

#endif