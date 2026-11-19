
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

/* TASK 6: Reap completed background processes */
void reap_background_processes(void)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("\n[Background] Process %d finished with status %d\n",
                   pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("\n[Background] Process %d terminated by signal %d\n",
                   pid, WTERMSIG(status));
        }
    }
}

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;

    /* TASK 5: Previous command exit status */
    int last_status = 0;

    while (1)
    {
        /* TASK 6: Check completed background processes */
        reap_background_processes();

        printf("msh[%d]> ", last_status);
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
            free(line);
            return EXIT_FAILURE;
        }

        /* TASK 2: Parse command arguments */
        size_t arg_capacity = 8;
        size_t argc = 0;

        char **argv = malloc(arg_capacity * sizeof(char *));

        if (argv == NULL)
        {
            perror("malloc");
            free(line);
            return EXIT_FAILURE;
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

        /* TASK 6: Detect background execution */
        int background = 0;

        if (strcmp(argv[argc - 1], "&") == 0)
        {
            background = 1;
            argc--;
            argv[argc] = NULL;
        }

        if (argc == 0)
        {
            free(argv);
            continue;
        }

        /* TASK 7: Detect output redirection */
        int redirect_index = -1;
        int redirect_count = 0;

        for (size_t i = 0; i < argc; i++)
        {
            if (strcmp(argv[i], ">") == 0)
            {
                redirect_index = (int)i;
                redirect_count++;
            }
        }

        char *output_file = NULL;

        if (redirect_count > 0)
        {
            if (redirect_count != 1 ||
                redirect_index == 0 ||
                redirect_index + 2 != (int)argc)
            {
                fprintf(stderr, "msh: invalid output redirection syntax\n");
                last_status = 2;
                free(argv);
                continue;
            }

            output_file = argv[redirect_index + 1];

            /* Remove > and filename from command arguments */
            argv[redirect_index] = NULL;
            argc = (size_t)redirect_index;
        }

        /* TASK 4: cd built-in */
        if (strcmp(argv[0], "cd") == 0)
        {
            if (argc > 2)
            {
                fprintf(stderr, "msh: cd: too many arguments\n");
                last_status = 1;
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
                        last_status = 1;
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
                    last_status = 1;
                }
                else
                {
                    last_status = 0;
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

            last_status = 0;
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

        /* TASK 3: Create child process */
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            last_status = 1;
            free(argv);
            continue;
        }

        if (pid == 0)
        {
            /* TASK 7: Redirect child standard output */
            if (output_file != NULL)
            {
                int fd = open(
                    output_file,
                    O_WRONLY | O_CREAT | O_TRUNC,
                    0644
                );

                if (fd == -1)
                {
                    perror("msh: open");
                    _exit(1);
                }

                if (fchmod(fd, 0644) == -1)
                {
                    perror("msh: fchmod");
                    close(fd);
                    _exit(1);
                }

                if (dup2(fd, STDOUT_FILENO) == -1)
                {
                    perror("msh: dup2");
                    close(fd);
                    _exit(1);
                }

                close(fd);
            }

            /* TASK 3: Execute external command */
            execvp(argv[0], argv);

            perror(argv[0]);
            _exit(127);
        }
        else if (background)
        {
            /* TASK 6: Do not wait for background process */
            printf("[Background] Started process with PID %d\n", pid);
            last_status = 0;
        }
        else
        {
            /* TASK 3 and TASK 5: Wait for foreground process */
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
                last_status = 1;
            }
            else if (WIFEXITED(status))
            {
                last_status = WEXITSTATUS(status);
            }
            else if (WIFSIGNALED(status))
            {
                last_status = 128 + WTERMSIG(status);
            }
        }

        free(argv);
    }

    free(line);
    return EXIT_SUCCESS;
}
