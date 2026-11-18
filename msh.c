
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

        length = getline(&line, &capacity, stdin);

        if (length == -1)
        {
            if (feof(stdin))
            {
                printf("\nGoodbye!\n");
                break;
            }

            perror("getline");
            break;
        }

        size_t arg_capacity = 8;
        size_t argc = 0;

        char **argv = malloc(arg_capacity * sizeof(char *));

        if (argv == NULL)
        {
            perror("malloc");
            break;
        }

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

            argv[argc++] = token;
            token = strtok(NULL, " \t\n");
        }

        argv[argc] = NULL;

        if (argc == 0)
        {
            free(argv);
            continue;
        }

        /* TASK 4: cd built-in */
        if (strcmp(argv[0], "cd") == 0)
        {
            if (argc > 2)
            {
                fprintf(stderr, "msh: cd: too many arguments\n");
            }
            else
            {
                const char *directory;

                if (argc == 1)
                {
                    directory = getenv("HOME");

                    if (directory == NULL)
                    {
                        fprintf(stderr, "msh: cd: HOME not set\n");
                        free(argv);
                        continue;
                    }
                }
                else
                {
                    directory = argv[1];
                }

                if (chdir(directory) == -1)
                {
                    perror("msh: cd");
                }
            }

            free(argv);
            continue;
        }

        /* TASK 4: help built-in */
        if (strcmp(argv[0], "help") == 0)
        {
            printf("\n");
            printf("========== MSH HELP ==========\n");
            printf("Built-in commands:\n");
            printf("  cd [dir]  Change directory\n");
            printf("  help      Display help\n");
            printf("  exit      Exit the shell\n");
            printf("\nExternal commands use fork(), execvp(), and waitpid().\n");
            printf("==============================\n\n");

            free(argv);
            continue;
        }

        /* TASK 4: exit built-in */
        if (strcmp(argv[0], "exit") == 0)
        {
            printf("Goodbye!\n");
            free(argv);
            free(line);
            return EXIT_SUCCESS;
        }

        /* TASK 3: Execute external commands */
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            free(argv);
            continue;
        }

        if (pid == 0)
        {
            execvp(argv[0], argv);

            perror(argv[0]);
            _exit(127);
        }
        else
        {
            int status;
            pid_t result;

            do
            {
                result = waitpid(pid, &status, 0);
            }
            while (result == -1 && errno == EINTR);

            if (result == -1)
            {
                perror("waitpid");
            }
        }

        free(argv);
    }

    free(line);
    return EXIT_SUCCESS;
}

