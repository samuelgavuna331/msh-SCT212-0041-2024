# MSH Test Plan

**Course:** ICS 2305 - Systems Programming  
**Student:** Samuel Gavuna  
**Registration Number:** SCT212-0041/2024

## Required Tests

| No. | Test | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| 1 | Press Enter on a blank line | Shell displays another prompt without an error. | Shell displayed another prompt without an error. | PASS |
| 2 | Run `echo hello world` | Prints hello world; prompt displays status 0. | Printed hello world; prompt displayed msh[0]>. | PASS |
| 3 | Run `ls /nonexistent` | Displays an error and returns a nonzero status. | Displayed an error; prompt displayed status 2. | PASS |
| 4 | Run `nosuchcommand` | Displays an error and returns status 127. | Displayed an error; prompt displayed status 127. | PASS |
| 5 | Run `cd /tmp`, followed by `pwd` | Displays /tmp. | pwd displayed /tmp. | PASS |
| 6 | Run `sleep 3 &` | Prompt returns immediately and background PID is displayed. | Started PID 12957 and immediately returned to the prompt. | PASS |
| 7 | Run `echo hi > f.txt`, followed by `cat f.txt` | File contains hi. | File was created and cat displayed hi. | PASS |
| 8 | Run `sleep 100`, then press Ctrl+C | Command stops, shell survives, and prompt displays status 130. | sleep terminated; shell survived and displayed status 130. | PASS |
| 9 | Press Ctrl+D | Shell exits cleanly. | Displayed Goodbye! and returned to Ubuntu terminal. | PASS |

## Additional Tests

| No. | Test | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| 10 | Run `help` | Displays available built-in commands. | Displayed the built-in commands and usage information. | PASS |
| 11 | Run `cd` without an argument | Changes to the HOME directory. | Changed to /home/samgavuna. | PASS |
| 12 | Run `false` | Next prompt displays status 1. | Next prompt displayed msh[1]>. | PASS |

## Test Environment

- Operating system: Ubuntu Linux
- Compiler: GCC
- Build tool: GNU Make
- Language: C

## Test Summary

All 9 required tests and 3 additional tests were executed successfully. All 12 tests passed in the test environment.
