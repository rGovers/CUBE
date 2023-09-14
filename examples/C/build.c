#define CUBE_IMPLEMENTATION
#include "CUBE/CUBE.h"

#include <stdio.h>

int main(int a_argc, char* a_argv[])
{
    CUBE_CProject proj = { 0 };
    proj.Name = CUBE_StackString_CreateC("CExample");
    proj.Target = CUBE_CProjectTarget_Exe;
    proj.Language = CUBE_CProjectLanguage_C;
#if _WIN32
    proj.OutputPath = CUBE_Path_CreateC("CExample.exe");
#else
    proj.OutputPath = CUBE_Path_CreateC("CExample");
#endif

    CUBE_CProject_AppendSource(&proj, "src/main.c");

    CUBE_CProject_Compile(&proj, CUBE_CProjectCompiler_GCC, CBNULL);

    printf("Done! \n");

    CUBE_CProject_Destroy(&proj);

    return 0;
}