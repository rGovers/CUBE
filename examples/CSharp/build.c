#define CUBE_IMPLEMENTATION
#include "CUBE/CUBE.h"

#include <stdio.h>

int main(int a_argc, char* a_argv[])
{
    CUBE_CSProject proj = { 0 };
    proj.Name = CUBE_StackString_CreateC("CSharpExample");
    proj.Target = CUBE_CSProjectTarget_Exe;
    proj.OutputPath = CUBE_String_CreateC("CSharpExample.exe");
    CUBE_CSProject_AppendSource(&proj, "src/Program.cs");

    CUBE_CSProject_Compile(&proj, CBNULL);

    printf("Done!\n");

    CUBE_CSProject_Destroy(&proj);

    return 0;
}