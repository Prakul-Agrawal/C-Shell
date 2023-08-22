#include "headers.h"
#include "prompt/prompt.h"

int main()
{
    // Keep accepting commands
    while (1)
    {
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
        if (strcmp(input, "exit\n") == 0)
        {
            exit(EXIT_SUCCESS);
        }
        printf("%s", input);
    }
}
