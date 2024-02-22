// CUBE - Header only C/C++/C# build system.
// 
// License at end of file.

#ifndef INCLUDED_HEADER_CUBECOMMANDLINE
#define INCLUDED_HEADER_CUBECOMMANDLINE

#include <stdio.h>
#include <stdlib.h>

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

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
void CUBE_CommandLine_AppendArgumentS(CUBE_CommandLine* a_commandLine, const CUBE_String* a_argument);
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
void CUBE_CommandLine_AppendArgumentS(CUBE_CommandLine* a_commandLine, const CUBE_String* a_argument)
{
    const CBUINT32 argumentCount = a_commandLine->ArgumentCount++;

    const CUBE_String argument = CUBE_String_Copy(a_argument);

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
#if WIN32
    HANDLE hReadPipe = NULL;
    HANDLE hWritePipe = NULL;
    SECURITY_ATTRIBUTES sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
    {
        return -1;
    }

    if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0))
    {
        return -1;
    }

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));

    si.cb = sizeof(si);
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
    
    TCHAR env[4096];
    LPTSTR curEnv = env;
    for (CBUINT32 i = 0; i < a_commandLine->EnvironmentVariableCount; ++i)
    {
        const CUBE_String variable = a_commandLine->EnvironmentVariables[i];
        const CUBE_String value = a_commandLine->EnvironmentValues[i];

        memcpy(curEnv, variable.Data, variable.Length);
        curEnv += variable.Length;
        *curEnv = '=';
        ++curEnv;
        memcpy(curEnv, value.Data, value.Length);
        curEnv += value.Length;
        *curEnv = '\0';
        ++curEnv;
    }

    TCHAR workingDir[4096];
    if (a_commandLine->Path.Length > 0)
    {
        memcpy(workingDir, a_commandLine->Path.Data, a_commandLine->Path.Length);
        workingDir[a_commandLine->Path.Length] = '\0';
    }
    else
    {
        GetCurrentDirectory(4096, workingDir);
    }

    TCHAR* cmd = (TCHAR*)malloc(sizeof(TCHAR) * 16384);
    TCHAR* curCmd = cmd;

    memcpy(curCmd, a_commandLine->Command.Data, a_commandLine->Command.Length);
    curCmd += a_commandLine->Command.Length;

    for (CBUINT32 i = 0; i < a_commandLine->ArgumentCount; ++i)
    {
        const CUBE_String argument = a_commandLine->Arguments[i];

        *curCmd = ' ';
        ++curCmd;
        memcpy(curCmd, argument.Data, argument.Length);
        curCmd += argument.Length;
    }

    *curCmd = '\0';

#ifdef CUBE_PRINT_COMMANDS
    printf("[CMD] %s\n", cmd);
#endif

    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, env, workingDir, &si, &pi))
    {
        free(cmd);

        return -1;
    }

    free(cmd);

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD dwExitCode;
    if (!GetExitCodeProcess(pi.hProcess, &dwExitCode))
    {
        return -1;
    }

    printf("Exit code: %d\n", dwExitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    CloseHandle(hWritePipe);

    DWORD dwRead;
    CHAR chBuf[4096];

    while (ReadFile(hReadPipe, chBuf, 4096, &dwRead, NULL))
    {
        if (dwRead <= 0)
        {
            break;
        }

        const CHAR* s = chBuf;
        const CHAR* e = s;

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
    
    CloseHandle(hReadPipe);

    return (int)dwExitCode;
#else
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

    CUBE_String_Destroy(&cmdString);

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

    free(a_commandLine->EnvironmentVariables);
    free(a_commandLine->EnvironmentValues);

    a_commandLine->EnvironmentVariableCount = 0;

    a_commandLine->EnvironmentVariables = CBNULL;
    a_commandLine->EnvironmentValues = CBNULL;
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
