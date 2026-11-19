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
- Course notes from piazza and google classroom.
- The instructios from ICS 2305 Systems Programming assignment. 
- ChatGPT (OpenAI) was used for explanations, and debugging guidance.


Required Tests
No.	Test	Expected Result	Actual Result	Status
1	Press Enter on a blank line	Shell displays another prompt without an error.	Shell displayed another prompt without an error.	PASS
2	Run echo hello world	Prints hello world; prompt displays status 0.	Printed hello world; prompt displayed msh[0]>.	PASS
3	Run ls /nonexistent	Displays an error and returns a nonzero status.	Displayed an error; prompt displayed status 2.	PASS
4	Run nosuchcommand	Displays an error and returns status 127.	Displayed an error; prompt displayed status 127.	PASS
5	Run cd /tmp, followed by pwd	Displays /tmp.	pwd displayed /tmp.	PASS
6	Run sleep 3 &	Prompt returns immediately and background PID is displayed.	Started PID 12957 and immediately returned to the prompt.	PASS
7	Run echo hi > f.txt, followed by cat f.txt	File contains hi.	File was created and cat displayed hi.	PASS
8	Run sleep 100, then press Ctrl+C	Command stops, shell survives, and prompt displays status 130.	sleep terminated; shell survived and displayed status 130.	PASS
9	Press Ctrl+D	Shell exits cleanly.	Displayed Goodbye! and returned to Ubuntu terminal.	PASS
Additional Tests
No.	Test	Expected Result	Actual Result	Status
10	Run help	Displays available built-in commands.	Displayed the built-in commands and usage information.	PASS
11	Run cd without an argument	Changes to the HOME directory.	Changed to /home/samgavuna.	PASS
12	Run false	Next prompt displays status 1.	Next prompt displayed msh[1]>.	PASS
Test Environment
Operating system: Ubuntu Linux
Compiler: GCC
Build tool: GNU Make
Language: C
Test Summary
All 9 required tests and 3 additional tests were executed successfully. All 12 tests passed in the test environment.
