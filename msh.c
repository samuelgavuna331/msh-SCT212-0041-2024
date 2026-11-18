
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;

    while (1)
    {
        printf("msh> ");
        fflush(stdout);

        /* Read a complete command line */
        length = getline(&line, &capacity, stdin);

        if (length == -1)
        {
            if (feof(stdin))
            {
                printf("\nGoodbye!\n");
                break;
            }

            perror("getline");
            free(line);
            return EXIT_FAILURE;
        }

        /* Allocate memory for argument pointers */
        size_t arg_capacity = 8;
        size_t argc = 0;

        char **argv = malloc(arg_capacity * sizeof(char *));

        if (argv == NULL)
        {
            perror("malloc");
            free(line);
            return EXIT_FAILURE;
        }

        /* Split the command using spaces, tabs and newline */
        char *token = strtok(line, " \t\n");

        while (token != NULL)
        {
            /* Increase argument array when necessary */
            if (argc + 1 >= arg_capacity)
            {
                arg_capacity *= 2;

                char **temp = realloc(
                    argv,
                    arg_capacity * sizeof(char *)
                );

                if (temp == NULL)
                {
                    perror("realloc");
                    free(argv);
                    free(line);
                    return EXIT_FAILURE;
                }

                argv = temp;
            }

            argv[argc] = token;
            argc++;

            token = strtok(NULL, " \t\n");
        }

        /* Terminate argument array with NULL */
        argv[argc] = NULL;

        /* Display parsed command for testing */
        if (argc > 0)
        {
            printf("Command: %s\n", argv[0]);

            for (size_t i = 1; i < argc; i++)
            {
                printf("Argument %zu: %s\n", i, argv[i]);
            }
        }

        /* Release argument pointer array */
        free(argv);
    }

    /* Release input buffer */
    free(line);

    return EXIT_SUCCESS;
}

