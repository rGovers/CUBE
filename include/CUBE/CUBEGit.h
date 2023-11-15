// CUBE - Header only C/C++/C# build system.
// 
// License at end of file.

#ifndef INCLUDED_HEADER_CUBEGIT
#define INCLUDED_HEADER_CUBEGIT

#ifdef __cplusplus
extern "C" {
#endif

#include "CUBE/CUBECommandLine.h"
#include "CUBE/CUBEStackString.h"
#include "CUBE/CUBEString.h"

CUBE_StackString CUBE_Git_GetCommitHash();
CUBE_StackString CUBE_Git_GetCommitHashShort();

CUBE_StackString CUBE_Git_GetCommitHashP(const CUBE_Path* a_workingPath);
CUBE_StackString CUBE_Git_GetCommitHashShortP(const CUBE_Path* a_workingPath);

#ifdef CUBE_IMPLEMENTATION

CUBE_StackString CUBE_Git_GetCommitHash()
{
    CUBE_CommandLine cmd = { 0 };

    CUBE_String_AppendC(&cmd.Command, "git");

    CUBE_CommandLine_AppendArgumentC(&cmd, "rev-parse");
    CUBE_CommandLine_AppendArgumentC(&cmd, "HEAD");

    CUBE_String* output = CBNULL;
    CBUINT32 outputSize = 0;

    CUBE_CommandLine_Execute(&cmd, &output, &outputSize);

    CUBE_StackString commitHash = CUBE_StackString_CreateC(output->Data);

    CUBE_CommandLine_Destroy(&cmd);
    for (CBUINT32 i = 0; i < outputSize; ++i)
    {
        CUBE_String_Destroy(&output[i]);
    }

    return commitHash;
}
CUBE_StackString CUBE_Git_GetCommitHashShort()
{
    CUBE_CommandLine cmd = { 0 };

    CUBE_String_AppendC(&cmd.Command, "git");

    CUBE_CommandLine_AppendArgumentC(&cmd, "rev-parse");
    CUBE_CommandLine_AppendArgumentC(&cmd, "--short");
    CUBE_CommandLine_AppendArgumentC(&cmd, "HEAD");

    CUBE_String* output = CBNULL;
    CBUINT32 outputSize = 0;

    CUBE_CommandLine_Execute(&cmd, &output, &outputSize);

    CUBE_StackString commitHash = CUBE_StackString_CreateC(output->Data);

    CUBE_CommandLine_Destroy(&cmd);
    for (CBUINT32 i = 0; i < outputSize; ++i)
    {
        CUBE_String_Destroy(&output[i]);
    }

    return commitHash;
}

CUBE_StackString CUBE_Git_GetCommitHashP(const CUBE_Path* a_workingPath)
{
    CUBE_CommandLine cmd = { 0 };

    cmd.Path = CUBE_Path_ToString(a_workingPath);
    CUBE_String_AppendC(&cmd.Command, "git");

    CUBE_CommandLine_AppendArgumentC(&cmd, "rev-parse");
    CUBE_CommandLine_AppendArgumentC(&cmd, "HEAD");

    CUBE_String* output = CBNULL;
    CBUINT32 outputSize = 0;

    CUBE_CommandLine_Execute(&cmd, &output, &outputSize);

    CUBE_StackString commitHash = CUBE_StackString_CreateC(output->Data);

    CUBE_CommandLine_Destroy(&cmd);
    for (CBUINT32 i = 0; i < outputSize; ++i)
    {
        CUBE_String_Destroy(&output[i]);
    }

    return commitHash;
}
CUBE_StackString CUBE_Git_GetCommitHashShortP(const CUBE_Path* a_workingPath)
{
    CUBE_CommandLine cmd = { 0 };

    cmd.Path = CUBE_Path_ToString(a_workingPath);
    CUBE_String_AppendC(&cmd.Command, "git");

    CUBE_CommandLine_AppendArgumentC(&cmd, "rev-parse");
    CUBE_CommandLine_AppendArgumentC(&cmd, "--short");
    CUBE_CommandLine_AppendArgumentC(&cmd, "HEAD");

    CUBE_String* output = CBNULL;
    CBUINT32 outputSize = 0;

    CUBE_CommandLine_Execute(&cmd, &output, &outputSize);

    CUBE_StackString commitHash = CUBE_StackString_CreateC(output->Data);

    CUBE_CommandLine_Destroy(&cmd);
    for (CBUINT32 i = 0; i < outputSize; ++i)
    {
        CUBE_String_Destroy(&output[i]);
    }

    return commitHash;
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