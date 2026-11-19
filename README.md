# MSH - Mini Shell

**Course:** ICS 2305 - Systems Programming  
**Student:** Samuel Gavuna  
**Registration Number:** SCT212-0041/2024

## Project Description

MSH is a simple command-line shell written in C. It accepts user commands, executes external programs, and provides basic shell functionality.

## Requirements Implemented

- [x] Task 1: Dynamic command input using getline().
- [x] Task 2: Parse commands into NULL-terminated argument arrays.
- [x] Task 3: Execute external commands using fork(), execvp(), and waitpid().
- [x] Task 4: Implement cd, help, and exit built-ins.
- [x] Task 5: Display the previous command's exit status.
- [x] Task 6: Support background execution and reap completed processes.
- [x] Task 7: Support output redirection using >.
- [x] Task 8: Handle Ctrl+C without terminating the shell.

## Building and Running

Requirements: GCC, GNU Make, and Linux.

Build: `make`

Run: `./msh`

Clean compiled files: `make clean`

## Design Explanation

MSH uses a loop to display its prompt, read input, process commands, and execute programs. The getline() function reads input dynamically, avoiding a fixed-size input buffer. The strtok() function separates commands into arguments using spaces and tabs. The argument array is terminated with NULL for use by execvp().

For external commands, the shell creates a child process using fork(). The child calls execvp() to execute the requested program, while the parent uses waitpid() to wait for foreground commands and obtain their exit status. If execution fails, the child reports the error and exits with status 127.

The cd and exit commands are built-ins because they must affect the shell process itself. If cd ran in a child process, only that child would change its working directory; the parent shell would remain in its original directory. Similarly, a child process cannot terminate its parent shell. The help command is also implemented internally.

Commands ending in & run in the background. The shell displays their process IDs and continues accepting commands. The waitpid() function with WNOHANG checks for completed background processes without blocking. This reaps finished children and prevents zombie processes.

Output redirection uses open() and dup2() in the child process. The shell ignores SIGINT so Ctrl+C does not terminate it, while child processes restore the default signal handling so commands can be interrupted.

## Known Limitations

- No support for quoted arguments.
- No pipes or command chaining.
- No input redirection or append redirection.
- No advanced job-control commands.

## Sources and Acknowledgements

- ICS 2305 Systems Programming practical assignment.
- Linux manual pages for getline(), strtok(), fork(), execvp(), waitpid(), open(), dup2(), and sigaction().
- GCC and GNU Make documentation.
- ChatGPT (OpenAI) was used for programming assistance, explanations, and debugging guidance.
