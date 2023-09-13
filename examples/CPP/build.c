#define CUBE_IMPLEMENTATION
#include "CUBE/CUBE.h"

#include <stdio.h>

int main(int a_argc, char** a_argv)
{
    CUBE_CProject proj = { 0 };
    proj.Name = CUBE_StackString_CreateC("CppExample");
    proj.Target = CUBE_CProjectTarget_Exe;
    proj.Language = CUBE_CProjectLanguage_CPP;
#if _WIN32
    proj.OutputPath = CUBE_String_CreateC("CppExample.exe");
#else
    proj.OutputPath = CUBE_String_CreateC("CppExample");
#endif

    CUBE_CProject_AppendSource(&proj, "src/main.cpp");
    CUBE_CProject_Append_Reference(&proj, "stdc++");

    CUBE_CProject_Compile(&proj, CUBE_CProjectCompiler_GCC, CBNULL);

    printf("Done! \n");

    CUBE_CProject_Destroy(&proj);

    return 0;
}