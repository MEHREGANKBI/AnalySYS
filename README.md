# AnalySYS

<img src="https://drive.usercontent.google.com/uc?id=1LNA72Ylezvov0XEjAsKquIPPqM705qb6&export=download">

## Table of contents
- [Introduction](#introduction)
- [Installation](#installation)
- [Uninstallation](#uninstallation)
- [Usage](#usage)
- [Notes](#notes)


## Introduction
AnalySYS is a simple, easy-to-use tool suitable for those who need statistics on the number of times each syscall is called by a thread. Under the hood, AnalySYS uses ptrace to trace a child process' main thread and when the child process exits, AnalySYS prints a comprehensive yet brief report to STDOUT.


## Installation
1- On your linux system, clone/download this repository

2- If you chose to download the repository in step 1, extract the zip file. Otherwise go to step 3

3- Use the "cd" built-in shell command to change directory to the root directory of the project

4- Inside your terminal, run the following command: "make install"

5- The binary will be built and located inside the "~/.local/bin/" directory which is usually part included in the $PATH variable


## Usage
SYNOPSIS: analySYS your-cmd [your-cmd-arguments]

For instance, if you want to inspect the syscalls made by the main thread of the infamous ls binary, you should type and execute the following command: analySYS ls

AnalySYS supports searching the $PATH variable so that you don't have to enter the entire binary path when trying to assess a common command like ls or sed.


## Uninstallation
If you wish to delete the app, follow the steps below:

1- Use "cd" to move to the project's root directory

2- Run the following command: "make uninstall"

3- After deleting the binary, you can safely remove the source code directory


## Notes
*This app **"should"** run on all relatively modern x64 linux systems.*

*This app acts like a shell intsance. Users should be aware of the permissions granted to the AnalySYS binary.*

*Normal usage is not likely to cause security concerns. However, the possibility of shell injection attacks in edge-cases cannot be ruled out. Proceed with caution.*

*This app has no guarantees. Consult the license file of this project for more information.*

*If you want to contribute to the project or report any bugs or issues, you're welcome to use the github issues tab.