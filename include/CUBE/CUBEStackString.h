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

CUBE_StackString CUBE_StackString_Copy(CUBE_StackString* a_string);

CUBE_StackString CUBE_StackString_MergeSS(CUBE_StackString* a_lhs, const CUBE_StackString* a_rhs);
CUBE_StackString CUBE_StackString_MergeC(CUBE_StackString* a_lhs, const char* a_rhs);

void CUBE_StackString_AppendSS(CUBE_StackString* a_lhs, const CUBE_StackString* a_rhs);
void CUBE_StackString_AppendC(CUBE_StackString* a_lhs, const char* a_rhs);


#ifdef CUBE_IMPLEMENTATION

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

CUBE_StackString CUBE_StackString_Copy(CUBE_StackString* a_string)
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

CUBE_StackString CUBE_StackString_MergeSS(CUBE_StackString* a_lhs, const CUBE_StackString* a_rhs)
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
CUBE_StackString CUBE_StackString_MergeC(CUBE_StackString* a_lhs, const char* a_rhs)
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