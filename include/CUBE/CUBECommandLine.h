// CUBE - Header only C/C++/C# build system.
// 
// License at end of file.

#ifndef INCLUDED_HEADER_CUBECOMMANDLINE
#define INCLUDED_HEADER_CUBECOMMANDLINE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct
{
    HANDLE ReadPipe;
    HANDLE WritePipe;
    PROCESS_INFORMATION ProcessInformation;
} CUBEI_WindowsCommandData;
#else
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct
{
    int Pipe;
    int Process;
    int Status;
} CUBEI_UnixCommandData;
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
    void* Data;
} CUBE_CommandLine;

void CUBE_CommandLine_AppendArgumentC(CUBE_CommandLine* a_commandLine, const char* a_argument);
void CUBE_CommandLine_AppendArgumentS(CUBE_CommandLine* a_commandLine, const CUBE_String* a_argument);
void CUBE_CommandLine_AppendArgumentSS(CUBE_CommandLine* a_commandLine, const CUBE_StackString* a_argument);

void CUBE_CommandLine_AppendEnvironmentVariableC(CUBE_CommandLine* a_commandLine, const char* a_variable, const char* a_value);

CBBOOL CUBE_CommandLine_BeginExecution(CUBE_CommandLine* a_commandLine);
CBBOOL CUBE_CommandLine_PollExecution(CUBE_CommandLine* a_commandLine);
int CUBE_CommandLine_EndExecution(CUBE_CommandLine* a_commandLine, CUBE_String** a_lines, CBUINT32* a_lineCount);

int CUBE_CommandLine_Execute(CUBE_CommandLine* a_commandLine, CUBE_String** a_lines, CBUINT32* a_lineCount);

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

CBBOOL CUBE_CommandLine_BeginExecution(CUBE_CommandLine* a_commandLine)
{
#if WIN32
    TCHAR* cmd = NULL;
    LPTSTR workingDir = NULL;
    HANDLE hReadPipe = NULL;
    HANDLE hWritePipe = NULL;
    CUBEI_WindowsCommandData* data = CBNULL;

    TCHAR env[4096];
    LPTSTR curEnv = env;
    for (CBUINT32 i = 0; i < a_commandLine->EnvironmentVariableCount; ++i)
    {
        const CUBE_String variable = a_commandLine->EnvironmentVariables[i];
        const CUBE_String value = a_commandLine->EnvironmentValues[i];

        const LPTSTR end = curEnv + variable.Length + value.Length + 2;
        if (end - env >= sizeof(env))
        {
            return CBFALSE;
        }

        memcpy(curEnv, variable.Data, variable.Length);
        curEnv += variable.Length;
        *curEnv = '=';
        ++curEnv;
        memcpy(curEnv, value.Data, value.Length);
        curEnv += value.Length;
        *curEnv = 0;
        ++curEnv;
    }

    // TODO: Detect environment and use correct path maximum based off enviroment cause Windows plays loose with the limits
    const CBUINT32 cmdBufferSize = 32767;
    if (a_commandLine->Path.Length > 0)
    {
        if (a_commandLine->Path.Length >= cmdBufferSize - 1)
        {
            return CBFALSE;
        }

        workingDir = (LPTSTR)malloc(a_commandLine->Path.Length + 1);
        memcpy(workingDir, a_commandLine->Path.Data, a_commandLine->Path.Length);
        workingDir[a_commandLine->Path.Length] = 0;
    }
    else
    {
        const DWORD res = GetCurrentDirectory(0, NULL);
        workingDir = (LPTSTR)malloc(res);
        GetCurrentDirectory(res, workingDir);
    }

    cmd = (TCHAR*)malloc(sizeof(TCHAR) * cmdBufferSize);
    TCHAR* curCmd = cmd;

    memcpy(curCmd, a_commandLine->Command.Data, a_commandLine->Command.Length);
    curCmd += a_commandLine->Command.Length;

    for (CBUINT32 i = 0; i < a_commandLine->ArgumentCount; ++i)
    {
        const CUBE_String argument = a_commandLine->Arguments[i];

        const LPTSTR end = curCmd + argument.Length + 1;
        if (end - cmd >= cmdBufferSize - 1)
        {
            goto Cleanup;
        }

        *curCmd = ' ';
        ++curCmd;
        memcpy(curCmd, argument.Data, argument.Length);
        curCmd += argument.Length;
    }

    *curCmd = 0;

#ifdef CUBE_PRINT_COMMANDS
    printf("[CMD] %s\n", cmd);
#endif
    
    SECURITY_ATTRIBUTES sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
    {
        free(workingDir);
        free(cmd);

        return CBFALSE;
    }

    if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0))
    {
        goto Cleanup;
    }

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));

    si.cb = sizeof(si);
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, env, workingDir, &si, &pi))
    {
        goto Cleanup;
    }

    free(workingDir);
    free(cmd);

    data = (CUBEI_WindowsCommandData*)malloc(sizeof(CUBEI_WindowsCommandData));

    data->ReadPipe = hReadPipe;
    data->WritePipe = hWritePipe;
    data->ProcessInformation = pi;

    a_commandLine->Data = data;

    return CBTRUE;

Cleanup:;
    if (workingDir != NULL)
    {
        free(workingDir);
    }
    if (cmd != NULL)
    {
        free(cmd);
    }
    if (hReadPipe != NULL)
    {
        CloseHandle(hReadPipe);
    }
    if (hWritePipe != NULL)
    {
        CloseHandle(hWritePipe);
    }

    return CBFALSE;
#else
    char** arguments = NULL;
    char* arg = NULL;
    char* shell = NULL;

    int pid;

#ifdef CUBE_PRINT_COMMANDS
    printf("[CMD][%s] %s ", a_commandLine->Path.Data, a_commandLine->Command.Data);

    for (CBUINT32 i = 0; i < a_commandLine->ArgumentCount; ++i)
    {
        printf("%s ", a_commandLine->Arguments[i].Data);
    }

    printf("\n");
#endif
    
    int pipes[2];
    if (pipe(pipes) != 0)
    {
        goto Cleanup;
    }

    pid = fork();
    switch (pid)
    {
    case 0:
    {
        const CBBOOL hasPath = a_commandLine->Path.Length > 0;

        static const char CommandArg[] = "-c";
        static const CBUINT32 CommandArgSize = sizeof(CommandArg) - 1;

        static const char ShellArg[] = "/bin/sh";
        static const CBUINT32 ShellArgSize = sizeof(ShellArg) - 1;

        // Technically a leak but crosses process boundaries so memory is tricky to manage safely so depending on OS
        // Risk a SEGFAULT if I attempt to free it
        shell = (char*)malloc(sizeof(ShellArg));
        memcpy(shell, ShellArg, ShellArgSize);
        shell[ShellArgSize] = 0;

        arg = (char*)malloc(sizeof(CommandArg));
        memcpy(arg, CommandArg, CommandArgSize);
        arg[CommandArgSize] = 0;

        const CBUINT32 finalArgCount = 3;
        arguments = (char**)malloc((finalArgCount + 1) * sizeof(char*));

        arguments[0] = shell;
        arguments[1] = arg;

        CUBE_String pArgs = { 0 };
        CUBE_String_AppendS(&pArgs, &a_commandLine->Command);
        CUBE_String_AppendC(&pArgs, " ");
        for (CBUINT32 i = 0; i < a_commandLine->ArgumentCount; ++i)
        {
            CUBE_String_AppendS(&pArgs, &a_commandLine->Arguments[i]);
            CUBE_String_AppendC(&pArgs, " ");
        }

        arguments[2] = pArgs.Data;

        arguments[finalArgCount] = NULL;

        if (hasPath)
        {
            chdir(a_commandLine->Path.Data);
        }

        for (CBUINT32 i = 0; i < a_commandLine->EnvironmentVariableCount; ++i)
        {
            setenv(a_commandLine->EnvironmentVariables[i].Data, a_commandLine->EnvironmentValues[i].Data, 1);
        }

        close(pipes[0]);
        dup2(pipes[1], STDOUT_FILENO);

        execv(ShellArg, arguments);

        // If it returns something has gone wrong
        // Cannot trust application state as forked and failed to start
        exit(1);
    }
    // Should not happen unless something is burning
    case -1:
    {
        close(pipes[0]);
        close(pipes[1]);

        goto Cleanup;
    }
    default:
    {
        CUBEI_UnixCommandData* data = (CUBEI_UnixCommandData*)malloc(sizeof(CUBEI_UnixCommandData));

        data->Status = -1;

        close(pipes[1]);
        data->Pipe = pipes[0];
        data->Process = pid;

        a_commandLine->Data = data;

        return CBTRUE;
    }
    }

Cleanup:;
    if (shell != NULL)
    {
        free(shell);
    }
    if (arg != NULL)
    {
        free(arg);
    }

    if (arguments != NULL)
    {
        free(arguments);
    }

    return CBFALSE;
#endif

    return CBFALSE;
}
CBBOOL CUBE_CommandLine_PollExecution(CUBE_CommandLine* a_commandLine)
{
#ifdef WIN32
    CUBEI_WindowsCommandData* data = (CUBEI_WindowsCommandData*)a_commandLine->Data;
    if (data == NULL)
    {
        return CBFALSE;
    }

    switch (WaitForSingleObject(data->ProcessInformation.hProcess, 0))
    {
    case WAIT_OBJECT_0:
    {
        DWORD exitCode;

        GetExitCodeProcess(data->ProcessInformation.hProcess, &exitCode);
        if (exitCode != STILL_ACTIVE)
        {
            return CBFALSE;
        }

        return CBTRUE;
    }
    case WAIT_TIMEOUT:
    {
        return CBTRUE;
    }
    default:
    {
        return CBFALSE;
    }
    }
#else
    CUBEI_UnixCommandData* data = (CUBEI_UnixCommandData*)a_commandLine->Data;
    if (data == NULL)
    {
        return CBFALSE;
    }

    int status;
    const pid_t r = waitpid(data->Process, &status, WNOHANG);

    if (WIFEXITED(status)) 
    {
        // Took me a second to figure out but have to re wait for the exitcode for some reason
        // Guess I have to throw Linux under the bus for once
        // Weird that by what I can see the man pages make no reference to that
        // After digging through source goes into syscall in the kernel and there is different paths based on how it exited 
        // I am not debugging kernel code to find the problem and find whos problem it is so leaving it at that with second call
        // Kernel code is hic sunt dracones
        // For those that want a rabbit hole syscall wait4 waitpid is just a glibc wrapper, I am tapping out
        // Probably just I am stupid
        waitpid(data->Process, &status, 0);
        data->Status = WEXITSTATUS(status);

        return CBFALSE;
    }

    if (r < 0)
    {
        return CBFALSE;
    }

    if (r == data->Process)
    {
        return CBTRUE;
    }
    if (r == 0)
    {
        return CBTRUE;
    }
    if (errno == EINTR)
    {
        return CBTRUE;
    }
#endif

    return CBFALSE;
}
int CUBE_CommandLine_EndExecution(CUBE_CommandLine* a_commandLine, CUBE_String** a_lines, CBUINT32* a_lineCount)
{
#ifdef WIN32
    CUBEI_WindowsCommandData* data = (CUBEI_WindowsCommandData*)a_commandLine->Data;
    a_commandLine->Data = CBNULL;
    if (data == CBNULL)
    {
        return -1;
    }

    DWORD dwExitCode;
    if (!GetExitCodeProcess(data->ProcessInformation.hProcess, &dwExitCode))
    {
        return -1;
    }

    CloseHandle(data->ProcessInformation.hProcess);
    CloseHandle(data->ProcessInformation.hThread);

    CloseHandle(data->WritePipe);

    DWORD dwRead;
    CHAR chBuf[4096];
    while (ReadFile(data->ReadPipe, chBuf, sizeof(chBuf), &dwRead, NULL))
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
    
    CloseHandle(data->ReadPipe);

    free(data);

    return (int)dwExitCode;
#else
    CUBEI_UnixCommandData* data = (CUBEI_UnixCommandData*)a_commandLine->Data;
    a_commandLine->Data = NULL;
    if (data == NULL)
    {
        return -1;
    }    

    const int ret = data->Status;

    char buffer[4096];
    while (read(data->Pipe, buffer, sizeof(buffer) - 1) > 0)
    {
        const char* s = buffer;
        const char* e = s;
        while (*s != 0 && s - buffer < sizeof(buffer) - 1)
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

    close(data->Pipe);

    free(data);

    return ret;
#endif

    return -1;
}

int CUBE_CommandLine_Execute(CUBE_CommandLine* a_commandLine, CUBE_String** a_lines, CBUINT32* a_lineCount)
{
    if (!CUBE_CommandLine_BeginExecution(a_commandLine))
    {
        return -1;
    }

    while (CBTRUE) 
    {
        if (!CUBE_CommandLine_PollExecution(a_commandLine))
        {
            break;
        }
    }

    return CUBE_CommandLine_EndExecution(a_commandLine, a_lines, a_lineCount);
}

void CUBE_CommandLine_Destroy(CUBE_CommandLine* a_commandLine)
{
    if (a_commandLine->Data != CBNULL)
    {
        free(a_commandLine->Data);
        a_commandLine->Data = CBNULL;
    }
    
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
// Copyright (c) 2024 River Govers
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
