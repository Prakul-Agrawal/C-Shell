[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# **SHELL** 

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
- The shell supports the following commands:
  - `warp` - change working directory to a directory specified by the user
  - `proclore` - print information about a process specified by the user
- Type `exit` to exit the shell.

# Assumptions