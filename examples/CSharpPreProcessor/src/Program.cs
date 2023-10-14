// Putting the C back in C# cause it is not C without the preprocessor and header files
#include "System.h"

namespace Test
{
    public static class Program
    {
        public static void Main()
        {
            const int startLine = __LINE__;
            const string file = __FILE__;

            const string compileDate = __DATE__;

            PRINT("Welcome to macro hell C# edition!");
            PRINT($"The starting line is {startLine}");
            PRINT($"The current file is {file}");
            PRINT($"This was compiled on {compileDate}");
        }
    }
}