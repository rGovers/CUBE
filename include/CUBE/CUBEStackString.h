// CUBE - Header only C/C++/C# build system.
// 
// License at end of file.

#ifndef INCLUDED_HEADER_CUBESTACKSTRING
#define INCLUDED_HEADER_CUBESTACKSTRING

#include "CUBE/CUBE.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char Data[1024];
    CBUINT32 Length;
} CUBE_StackString;

CUBE_StackString CUBE_StackString_CreateC(const char* a_string);
void CUBE_StackString_Destroy(CUBE_StackString* a_string);

CUBE_StackString CUBE_StackString_Copy(const CUBE_StackString* a_string);

CUBE_StackString CUBE_StackString_MergeSS(const CUBE_StackString* a_lhs, const CUBE_StackString* a_rhs);
CUBE_StackString CUBE_StackString_MergeC(const CUBE_StackString* a_lhs, const char* a_rhs);

void CUBE_StackString_AppendSS(CUBE_StackString* a_lhs, const CUBE_StackString* a_rhs);
void CUBE_StackString_AppendC(CUBE_StackString* a_lhs, const char* a_rhs);


#ifdef CUBE_IMPLEMENTATION
// #if 1

CUBE_StackString CUBE_StackString_CreateC(const char* a_string)
{
    CUBE_StackString string;
    string.Length = 0;
    while (a_string[string.Length] != '\0')
    {
        ++string.Length;
    }

    for (CBUINT32 i = 0; i < string.Length; ++i)
    {
        string.Data[i] = a_string[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}
void CUBE_StackString_Destroy(CUBE_StackString* a_string)
{
    a_string->Length = 0;
}

CUBE_StackString CUBE_StackString_Copy(const CUBE_StackString* a_string)
{
    CUBE_StackString string;
    string.Length = a_string->Length;

    for (CBUINT32 i = 0; i < string.Length; ++i)
    {
        string.Data[i] = a_string->Data[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}

CUBE_StackString CUBE_StackString_MergeSS(const CUBE_StackString* a_lhs, const CUBE_StackString* a_rhs)
{
    CUBE_StackString string;
    string.Length = a_lhs->Length + a_rhs->Length;

    for (CBUINT32 i = 0; i < a_lhs->Length; ++i)
    {
        string.Data[i] = a_lhs->Data[i];
    }

    for (CBUINT32 i = 0; i < a_rhs->Length; ++i)
    {
        string.Data[i + a_lhs->Length] = a_rhs->Data[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}
CUBE_StackString CUBE_StackString_MergeC(const CUBE_StackString* a_lhs, const char* a_rhs)
{
    CUBE_StackString string;
    string.Length = a_lhs->Length;

    for (CBUINT32 i = 0; i < a_lhs->Length; ++i)
    {
        string.Data[i] = a_lhs->Data[i];
    }

    CBUINT32 index = 0;
    while (a_rhs[index] != '\0')
    {
        string.Data[index + a_lhs->Length] = a_rhs[index];
        ++index;
    }
    string.Length += index;
    string.Data[string.Length] = '\0';

    return string;
}

void CUBE_StackString_AppendSS(CUBE_StackString* a_lhs, const CUBE_StackString* a_rhs)
{
    const CBUINT32 length = a_lhs->Length + a_rhs->Length;

    for (CBUINT32 i = 0; i < a_rhs->Length; ++i)
    {
        a_lhs->Data[i + a_lhs->Length] = a_rhs->Data[i];
    }
    a_lhs->Data[length] = '\0';
    a_lhs->Length = length;
}
void CUBE_StackString_AppendC(CUBE_StackString* a_lhs, const char* a_rhs)
{
    const CBUINT32 length = a_lhs->Length;

    CBUINT32 index = 0;
    while (a_rhs[index] != '\0')
    {
        a_lhs->Data[index + length] = a_rhs[index];
        ++index;
    }
    
    a_lhs->Data[length + index] = '\0';
    a_lhs->Length = length + index;
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
