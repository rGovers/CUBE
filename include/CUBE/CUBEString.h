#ifndef INCLUDED_HEADER_CUBESTRING
#define INCLUDED_HEADER_CUBESTRING

#include <stdlib.h>

#include "CUBE/CUBEStackString.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char* Data;
    CBUINT32 Length;
} CUBE_String;

CUBE_String CUBE_String_CreateC(const char* a_string);
CUBE_String CUBE_String_CreateCL(const char* a_string, CBUINT32 a_length);
CUBE_String CUBE_String_CreateSS(const CUBE_StackString* a_string);

void CUBE_String_Destroy(CUBE_String* a_string);

CUBE_String CUBE_String_Copy(const CUBE_String* a_string);

CUBE_String CUBE_String_MergeS(const CUBE_String* a_lhs, const CUBE_String* a_rhs);
CUBE_String CUBE_String_MergeSS(const CUBE_String* a_lhs, const CUBE_StackString* a_rhs);
CUBE_String CUBE_String_MergeC(const CUBE_String* a_lhs, const char* a_rhs);
CUBE_String CUBE_String_MergeCL(const CUBE_String* a_lhs, const char* a_rhs, CBUINT32 a_length);

void CUBE_String_AppendS(CUBE_String* a_lhs, const CUBE_String* a_rhs);
void CUBE_String_AppendSS(CUBE_String* a_lhs, const CUBE_StackString* a_rhs);
void CUBE_String_AppendC(CUBE_String* a_lhs, const char* a_rhs);
void CUBE_String_AppendCL(CUBE_String* a_lhs, const char* a_rhs, CBUINT32 a_length);

#ifdef CUBE_IMPLEMENTATION
// #if 1

CUBE_String CUBE_String_CreateC(const char* a_string)
{
    CUBE_String string;
    string.Length = 0;
    while (a_string[string.Length] != '\0')
    {
        ++string.Length;
    }

    string.Data = (char*)malloc(sizeof(char) * string.Length + 1);
    for (CBUINT32 i = 0; i < string.Length; ++i)
    {
        string.Data[i] = a_string[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}
CUBE_String CUBE_String_CreateCL(const char* a_string, CBUINT32 a_length)
{
    CUBE_String string;
    string.Length = a_length;

    string.Data = (char*)malloc(sizeof(char) * string.Length + 1);
    for (CBUINT32 i = 0; i < string.Length; ++i)
    {
        string.Data[i] = a_string[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}
CUBE_String CUBE_String_CreateSS(const CUBE_StackString* a_string)
{
    CUBE_String string;
    string.Length = a_string->Length;

    string.Data = (char*)malloc(sizeof(char) * string.Length + 1);
    for (CBUINT32 i = 0; i < string.Length; ++i)
    {
        string.Data[i] = a_string->Data[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}
void CUBE_String_Destroy(CUBE_String* a_string)
{
    if (a_string->Data != CBNULL)
    {
        free(a_string->Data);
        a_string->Data = CBNULL;
    }
    
    a_string->Length = 0;
}

CUBE_String CUBE_String_Copy(const CUBE_String* a_string)
{
    CUBE_String string;
    string.Length = a_string->Length;

    string.Data = (char*)malloc(sizeof(char) * string.Length + 1);
    for (CBUINT32 i = 0; i < string.Length; ++i)
    {
        string.Data[i] = a_string->Data[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}

CUBE_String CUBE_String_MergeS(const CUBE_String* a_lhs, const CUBE_String* a_rhs)
{
    CUBE_String string;
    string.Length = a_lhs->Length + a_rhs->Length;

    string.Data = (char*)malloc(sizeof(char) * string.Length + 1);
    CBUINT32 index = 0;
    for (CBUINT32 i = 0; i < a_lhs->Length; ++i)
    {
        string.Data[index++] = a_lhs->Data[i];
    }
    for (CBUINT32 i = 0; i < a_rhs->Length; ++i)
    {
        string.Data[index++] = a_rhs->Data[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}
CUBE_String CUBE_String_MergeSS(const CUBE_String* a_lhs, const CUBE_StackString* a_rhs)
{
    CUBE_String string;
    string.Length = a_lhs->Length + a_rhs->Length;

    string.Data = (char*)malloc(sizeof(char) * string.Length + 1);
    CBUINT32 index = 0;
    for (CBUINT32 i = 0; i < a_lhs->Length; ++i)
    {
        string.Data[index++] = a_lhs->Data[i];
    }
    for (CBUINT32 i = 0; i < a_rhs->Length; ++i)
    {
        string.Data[index++] = a_rhs->Data[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}
CUBE_String CUBE_String_MergeC(const CUBE_String* a_lhs, const char* a_rhs)
{
    CUBE_String string;
    string.Length = a_lhs->Length;

    CBUINT32 rhsLength = 0;
    while (a_rhs[rhsLength] != '\0')
    {
        ++rhsLength;
    }
    string.Length += rhsLength;

    string.Data = (char*)malloc(sizeof(char) * string.Length + 1);
    CBUINT32 index = 0;
    for (CBUINT32 i = 0; i < a_lhs->Length; ++i)
    {
        string.Data[index++] = a_lhs->Data[i];
    }
    for (CBUINT32 i = 0; i < rhsLength; ++i)
    {
        string.Data[index++] = a_rhs[i];
    }
    string.Data[string.Length] = '\0';

    return string;
}
CUBE_String CUBE_String_MergeCL(const CUBE_String* a_lhs, const char* a_rhs, CBUINT32 a_length)
{
    CUBE_String string;
    string.Length = a_lhs->Length + a_length;

    string.Data = (char*)malloc(sizeof(char) * string.Length + 1);
    CBUINT32 index = 0;
    printf("c\n");
    for (CBUINT32 i = 0; i < a_lhs->Length; ++i)
    {
        string.Data[index++] = a_lhs->Data[i];
    }
    printf("d\n");
    for (CBUINT32 i = 0; i < a_length; ++i)
    {
        string.Data[index++] = a_rhs[i];
    }
    printf("e\n");
    string.Data[string.Length] = '\0';

    return string;
}

void CUBE_String_AppendS(CUBE_String* a_lhs, const CUBE_String* a_rhs)
{
    CUBE_String string = CUBE_String_MergeS(a_lhs, a_rhs);
    CUBE_String_Destroy(a_lhs);
    *a_lhs = string;
}
void CUBE_String_AppendSS(CUBE_String* a_lhs, const CUBE_StackString* a_rhs)
{
    CUBE_String string = CUBE_String_MergeSS(a_lhs, a_rhs);
    CUBE_String_Destroy(a_lhs);
    *a_lhs = string;
}
void CUBE_String_AppendC(CUBE_String* a_lhs, const char* a_rhs)
{
    CUBE_String string = CUBE_String_MergeC(a_lhs, a_rhs);
    CUBE_String_Destroy(a_lhs);
    *a_lhs = string;
}
void CUBE_String_AppendCL(CUBE_String* a_lhs, const char* a_rhs, CBUINT32 a_length)
{
    CUBE_String string = CUBE_String_MergeCL(a_lhs, a_rhs, a_length);
    CUBE_String_Destroy(a_lhs);
    *a_lhs = string;
}
#endif

#ifdef __cplusplus
}
#endif

#endif