# Käyttöjärjestelmät ja systeemiohjelmointi: project 3
Unix shell - a part of my systems programming course project</br></br>
This is a basic shell "wish".

## Compilation
The repository includes a Makefile, and the program can be compiled by runnning "make" on the command line.

## Running
Wish has 2 modes: basic mode and batch mode. It can be invoked on command line either with 0 arguments or 1 argument. When run with 0 arguments, the program reads commands from user input. This is the so-called basic mode.<br></br>

When invoked with one argument, the argument specifies a batch file where the wish shell will look for commands. This is the so-called batch mode.
Example: ./wish batch.txt

## Built-in commands

Wish has 3 built-in commands.</br></br>
**exit**: exit wish</br></br>
**cd**: change directory. This command takes exactly one argument, which specifies the directory where the user wants to move.</br></br>
**path**: change the path where to look for executable files. This command takes up to 10 arguments, so the user can give up to 10 paths where to look for executables at a time.

## Paths

The initial shell path is /bin. When user changes the path(s), the previous paths are overwritten, not appended.
