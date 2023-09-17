#ifndef INCLUDED_HEADER_CUBEPATH
#define INCLUDED_HEADER_CUBEPATH

#include "CUBE/CUBEString.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CUBE_String* Path;
    CBUINT32 PathCount;
} CUBE_Path;

CUBE_Path CUBE_Path_CreateC(const char* a_path);
CUBE_Path CUBEI_Path_CreateCA(const char* a_strs[]);
#define CUBE_Path_CreateCA(...) CUBEI_Path_CreateCA((const char*[]){__VA_ARGS__, CBNULL})

void CUBE_Path_Destroy(CUBE_Path* a_path);

CUBE_Path CUBE_Path_Copy(const CUBE_Path* a_path);

CUBE_Path CUBE_Path_ParentPath(const CUBE_Path* a_path);

CUBE_Path CUBE_Path_CombineC(const CUBE_Path* a_lhs, const char* a_rhs);
CUBE_Path CUBE_Path_CombineS(const CUBE_Path* a_lhs, const CUBE_String* a_rhs);
CUBE_Path CUBE_Path_CombineSS(const CUBE_Path* a_lhs, const CUBE_StackString* a_rhs);
CUBE_Path CUBE_Path_CombineP(const CUBE_Path* a_lhs, const CUBE_Path* a_rhs);

CUBE_String CUBE_Path_ToString(const CUBE_Path* a_path);
CUBE_String CUBE_Path_ToNRString(const CUBE_Path* a_path);

CUBE_String CUBE_Path_Extension(const CUBE_Path* a_path);
CUBE_String CUBE_Path_Filename(const CUBE_Path* a_path);

#ifdef CUBE_IMPLEMENTATION
// #if 1
CUBE_Path CUBE_Path_CreateC(const char* a_path)
{
    CUBE_Path path = { 0 };

    const char* s = a_path;
    while (*s == '/' || *s == '\\') 
    {
        ++s;
    }
    const char* e = s;

    while (*e != '\0')
    {
        while (*e != '\0' && *e != '/' && *e != '\\')
        {
            ++e;
        }

        const CBUINT32 length = e - s;
        const CBUINT32 index = path.PathCount++;

        path.Path = (CUBE_String*)realloc(path.Path, sizeof(CUBE_String) * path.PathCount);
        path.Path[index] = CUBE_String_CreateCL(s, length);

        if (*e == '\0')
        {
            break;
        }

        s = e + 1;
        e = s;
    }

    return path;
}
CUBE_Path CUBEI_Path_CreateCA(const char* a_strs[])
{
    CUBE_Path path = { 0 };

    while (*a_strs != CBNULL)
    {
        const CBUINT32 index = path.PathCount++;

        path.Path = (CUBE_String*)realloc(path.Path, sizeof(CUBE_String) * path.PathCount);
        path.Path[index] = CUBE_String_CreateC(*a_strs);

        ++a_strs;
    }

    return path;
}

void CUBE_Path_Destroy(CUBE_Path* a_path)
{
    for (CBUINT32 i = 0; i < a_path->PathCount; ++i)
    {
        CUBE_String_Destroy(&a_path->Path[i]);
    }

    free(a_path->Path);

    a_path->Path = NULL;
    a_path->PathCount = 0;
}

CUBE_Path CUBE_Path_Copy(const CUBE_Path* a_path)
{
    CUBE_Path path = { 0 };

    path.PathCount = a_path->PathCount;
    path.Path = (CUBE_String*)malloc(sizeof(CUBE_String) * path.PathCount);

    for (CBUINT32 i = 0; i < path.PathCount; ++i)
    {
        path.Path[i] = CUBE_String_Copy(&a_path->Path[i]);
    }

    return path;
}

CUBE_String CUBE_Path_ToString(const CUBE_Path* a_path)
{
    CUBE_String string = { 0 };

    for (CBUINT32 i = 0; i < a_path->PathCount; ++i)
    {
        CUBE_String_AppendS(&string, &a_path->Path[i]);
        if (i < a_path->PathCount - 1)
        {
            CUBE_String_AppendC(&string, "/");
        }
    }

    return string;
}
CUBE_String CUBE_Path_ToNRString(const CUBE_Path* a_path)
{
    CUBE_String string = { 0 };

    for (CBUINT32 i = 0; i < a_path->PathCount; ++i)
    {
        const CUBE_String path = a_path->Path[i];
        if (path.Length == 2 && path.Data[0] == '.' && path.Data[1] == '.')
        {
            CUBE_String_AppendC(&string, "NR");
        }
        else
        {
            CUBE_String_AppendS(&string, &path);
        }

        if (i < a_path->PathCount - 1)
        {
            CUBE_String_AppendC(&string, "/");
        }
    }

    return string;
}

CUBE_Path CUBE_Path_CombineC(const CUBE_Path* a_lhs, const char* a_rhs)
{
    CUBE_Path rhs = CUBE_Path_CreateC(a_rhs);

    CUBE_Path path = CUBE_Path_CombineP(a_lhs, &rhs);

    CUBE_Path_Destroy(&rhs);

    return path;
}
CUBE_Path CUBE_Path_CombineS(const CUBE_Path* a_lhs, const CUBE_String* a_rhs)
{
    return CUBE_Path_CombineC(a_lhs, a_rhs->Data);
}
CUBE_Path CUBE_Path_CombineSS(const CUBE_Path* a_lhs, const CUBE_StackString* a_rhs)
{
    return CUBE_Path_CombineC(a_lhs, a_rhs->Data);
}
CUBE_Path CUBE_Path_CombineP(const CUBE_Path* a_lhs, const CUBE_Path* a_rhs)
{
    CUBE_Path path = { 0 };

    path.PathCount = a_lhs->PathCount + a_rhs->PathCount;
    path.Path = (CUBE_String*)malloc(sizeof(CUBE_String) * path.PathCount);

    for (CBUINT32 i = 0; i < a_lhs->PathCount; ++i)
    {
        path.Path[i] = CUBE_String_Copy(&a_lhs->Path[i]);
    }

    CBUINT32 index = 0;
    for (CBUINT32 i = 0; i < a_rhs->PathCount; ++i)
    {
        if (a_rhs->Path[i].Length == 1 && a_rhs->Path[i].Data[0] == '.')
        {
            --path.PathCount;

            continue;
        }
        
        path.Path[index++ + a_lhs->PathCount] = CUBE_String_Copy(&a_rhs->Path[i]);
    }

    return path;
}

CUBE_Path CUBE_Path_ParentPath(const CUBE_Path* a_path)
{
    CUBE_Path path;

    path.PathCount = a_path->PathCount - 1;
    path.Path = (CUBE_String*)malloc(sizeof(CUBE_String) * path.PathCount);

    for (CBUINT32 i = 0; i < path.PathCount; ++i)
    {
        path.Path[i] = CUBE_String_Copy(&a_path->Path[i]);
    }

    return path;
}

CUBE_String CUBE_Path_Extension(const CUBE_Path* a_path)
{
    CUBE_String str = { 0 };

    if (a_path->PathCount > 0)
    {
        const CUBE_String last = a_path->Path[a_path->PathCount - 1];

        const char* s = last.Data;
        const char* e = s + last.Length;

        while (e != s && *e != '.')
        {
            --e;
        }

        if (e != s)
        {
            str = CUBE_String_CreateCL(e, last.Length - (e - s));
        }
    }

    return str;
}

CUBE_String CUBE_Path_Filename(const CUBE_Path* a_path)
{
    CUBE_String str = { 0 };

    if (a_path->PathCount > 0)
    {
        const CUBE_String last = a_path->Path[a_path->PathCount - 1];

        const char* s = last.Data;
        const char* e = s + last.Length;

        while (e != s && *e != '.')
        {
            --e;
        }

        if (e != s)
        {
            str = CUBE_String_CreateCL(s, e - s);
        }
    }

    return str;
}

#endif

#ifdef __cplusplus
}
#endif

#endif