[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# ***C - SHELL***

**Mini Project 1** - Operating Systems and Networks, Monsoon 2023 

## Building and running

Run `make` in the root directory to build. Run `./main` to run the shell.

# Description

## Directory structure

- `main.c` is the main file which contains the main function and is the entry point of the program.
- `headers.h` is the header file which contains all the general function declarations and global variables.
- `prompt` folder contains the source code for the prompt of the shell.
- `utils` folder contains the source code for some utility functions required by other files.
- `commands` folder contains the source code for the commands of the shell.
  
## Usage

- The working directory of the shell is the directory where it was executed from.
- Commands can be chained with `;` or `&`. For now, background processes are not supported.
- Type `exit` to exit the shell.
<!-- - The shell supports the following commands:
  - `warp` - change working directory to a directory specified by the user
  - `proclore` - print information about a process specified by the user -->

# Commands
The shell supports the following commands:

| Directory | Description | Usage |
| --- | --- | --- |
| commands/warp/ | Change working directory to a directory specified by the user | `warp <path>` |
| commands/proclore/ | Print information about a process specified by the user | `proclore <pid>` |

# Utilities
The following utility functions are used by other files:

| Directory | Description |
| --- | --- |
| utils/delimiter/ | Achieve tokenization of input string based on a certain delimiter, and return number of tokens, the tokens themselves, etc. |
| utils/cwd/ | Return the current working directory |
| utils/str_to_num/ | Can check if a string is a number, and convert a string to a number |
| utils/common/ | Contains some common useful functions used by other files, such as a safe malloc, error handling, etc. |

# Assumptions

- If the word `pastevents` comes anywhere in the command (without the correct usage of `pastevents execute <number>`), the command will not be included in the history.