[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# ***C - SHELL***

**Mini Project 1** - Operating Systems and Networks, Monsoon 2023 

## Building and running

Run `make` in the root directory to build. Run `./a.out` to run the shell.

# Description

## Directory structure

- `main.c` is the main file which initializes the shell and is the entry point of the program.
- `headers.h` is the header file which contains all the general function declarations and global variables.
- `prompt` folder contains the source code for the prompt of the shell.
- `utils` folder contains the source code for some utility functions required by other files.
- `commands` folder contains the source code for the commands of the shell.
  
## Usage

- The working directory of the shell is the directory where it was executed from.
- Commands can be chained with `;` or `&`. For now, background processes are not supported.
- Type `exit` to exit the shell.

# Commands
The shell supports the following commands:

| Directory | Description | Usage |
| --- | --- | --- |
| commands/warp/ | Change working directory to a directory specified by the user | `warp <path>` |
| commands/proclore/ | Print information about a process specified by the user | `proclore <pid>` |
| commands/pastevents/ | Print the last 15 (at most) commands executed by the user. Also used to clear history or run some previous command | `pastevents <purge> / <execute index>` |
| commands/peek/ | Print the files and directories present in the given directory | `peek <-l> <-a> <path>` |
| commands/activities/ | Print a sorted list of all the processes spawed by the shell | `activities` |
| commands/neonate/ | Continuously prints the pid of the most recently created process of the system at fixed intervals | `neonate -n <time_arg>` |
| commands/iMan/ | Print the manual for the command given, by using sockets to get the information from the web | `iMan <command>` |

# Utilities
The following utility functions are used by other files:

| Directory | Description |
| --- | --- |
| utils/delimiter/ | Achieve tokenization of input string based on a certain delimiter, and return number of tokens, the tokens themselves, etc. |
| utils/cwd/ | Return the current working directory |
| utils/str_to_num/ | Can check if a string is a number, and convert a string to a number |
| utils/common/ | Contains some common useful functions used by other files, such as a safe malloc, error handling, etc. |
| utils/execute/ | The main part of the code which executes all the other commands. |
| utils/history/ | Contains commands to handle storing and retrieving history from the persistent storage. |
| utils/process/ | Contains functions to handle the process list for processes spawed by the shell. |
| utils/rawmode/ | It allows us to toggle back and forth between raw mode and cooked mode. |

# Assumptions

- If the word `pastevents` comes anywhere in the command (without the correct usage of `pastevents execute <number>`), the command will not be included in the history.
- The command `peek` *can* include more than one path as well, and will just display the contents of all the paths given one after another. The flags can also come anywhere in the command, and will be handled accordingly.
- The way the input is parsed doesn't take into account quotation marks. Therefore commands like `echo` print the text along with the quotes.
- `iMan` only prints NAME, SYNOPSIS and DESCRIPTION for the command given. It doesn't print the other sections. Also, if the length of these exceeds the buffer length of 4096, it truncates it.
- `neonate` command only takes integer values are valid argument for time.
- After a background process terminates, the display will display the main command name, not the entire command.
- Pipes works with I/O redirection as well. However, custom commands are not involved in the pipe.