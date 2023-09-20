#define CUBE_IMPLEMENTATION
#include "CUBE/CUBE.h"

#include <stdio.h>

int main(int a_argc, char* a_argv[])
{   
    // Create the project
    CUBE_CProject proj = { 0 };
    proj.Name = CUBE_StackString_CreateC("CExample");
    proj.Target = CUBE_CProjectTarget_Exe;
    proj.Language = CUBE_CProjectLanguage_C;
    proj.OutputPath = CUBE_Path_CreateC("build");

    CUBE_CProject_AppendSource(&proj, "src/main.c");

    CUBE_String* lines = CBNULL;
    CBUINT32 lineCount = 0;

    // Compile the project
    const CBBOOL ret = CUBE_CProject_Compile(&proj, CUBE_CProjectCompiler_GCC, ".", CBNULL, &lines, &lineCount);

    // Print the output of the compiler
    if (lines != CBNULL)
    {
        for (CBUINT32 i = 0; i < lineCount; ++i)
        {
            printf("%s\n", lines[i].Data);

            CUBE_String_Destroy(&lines[i]);
        }

        free(lines);
        lineCount = 0;
    }
    
    // Cleanup
    CUBE_CProject_Destroy(&proj);

    // Check if the compilation was successful
    if (!ret)
    {
        printf("Failed to compile project!\n");

        return 1;
    }

    printf("Done! \n");

    return 0;
}