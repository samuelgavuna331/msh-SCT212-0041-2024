
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;

    while (1)
    {
        printf("msh> ");
        fflush(stdout);

        /* Task 1: Read unlimited input */
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

        /* Task 2: Prepare argument array */
        size_t arg_capacity = 8;
        size_t argc = 0;

        char **argv = malloc(arg_capacity * sizeof(char *));

        if (argv == NULL)
        {
            perror("malloc");
            free(line);
            return EXIT_FAILURE;
        }

        /* Split input into arguments */
        char *token = strtok(line, " \t\n");

        while (token != NULL)
        {
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

        /* NULL terminate argv */
        argv[argc] = NULL;

        /* Ignore blank lines */
        if (argc == 0)
        {
            free(argv);
            continue;
        }

        /* Task 3: Create a child process */
        pid_t pid = fork();

        if (pid < 0)
        {
            /* Fork failed */
            perror("fork");
            free(argv);
            continue;
        }

        if (pid == 0)
        {
            /* Child process: execute command */
            execvp(argv[0], argv);

            /* Reached only if execvp fails */
            perror(argv[0]);

            _exit(127);
        }
        else
        {
            /* Parent process: wait for child */
            int status;

            while (waitpid(pid, &status, 0) == -1)
            {
                if (errno == EINTR)
                {
                    continue;
                }

                perror("waitpid");
                break;
            }
        }

        free(argv);
    }

    /* Release input memory */
    free(line);

    return EXIT_SUCCESS;
}

