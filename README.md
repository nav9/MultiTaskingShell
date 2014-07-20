The MIT License (MIT)

Copyright (c) 2014 Navin Ipe

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.



MultiTaskingShell
=================

Multi-tasking Shell on Unix

Project specification :
-----------------------
The shell was to support the following features: 

* Internal and External commands 
* Command Line Interface 
* Back grounding 
* Redirection 
* Pipes 
* Job control 
* Listing and timing of jobs 
* Shell script programming 
* Wild-cards 
* Signal handling 

Input/Output Specification: 
--------------------------- 

Input:  suspend [-f] 
Output: Suspend the execution of this shell until it receives a SIGCONT signal. 

Input:  kill [process id] 
Output: Terminates the process specified by process id. 
 
Input: times 
Output: Print the accumulated user and system times for the shell and for processes run from the shell 

Input: ulimit 
Output: Provides control over the resources available to the shell and to processes started by it, on systems that allow 
such control. 

Input: wait 
Output: Wait for each specified process and return its termination status. 

Input: alias [NAME[=VALUE] ...] 
Output: Create a name for another command or long command string. 

Input: trap [[arg] sigspec ...] 
Output: The command  arg is to be read and executed when the shell receives signal(s) sigspec. 

Input: cd [dir] 
Output: Change the current directory to dir 

Input: dirs 
Output: displays the list  of  currently remembered directories. 

Input: echo [arg ...] 
Output: Output the args, separated by spaces, followed by a newline. 

Input: pwd 
Output: Print the absolute pathname of the current working directory. 

Input: jobs [ jobspec ... ] 
Output: If jobspec is given, output is restricted to information about that job. 


Interaction Specification: 
-------------------------- 
Piping with redirection (N no of pipes and redirections will be supported) 

Signal Handling: 
---------------- 
Signal name: SIGEXIT 
Keyboard Shortcut: Ctrl-d 
Function: Exits the shell 

Signal name: SIGINT 
Keyboard Shortcut: Ctrl-c 
Function: Kills foreground process 

Signal name: SIGQUIT 
Keyboard Shortcut: Ctrl-\ 
Function: Causes the process to terminate 

Signal name: SIGTSTP 
Keyboard Shortcut: Ctrl-z 
Function: causes the process to suspend execution. 

Wild cards to be supported: 
-------------------------- 
* An asterisk matches any number of characters in a filename. 
? The question mark matches any single character. 
[ ] Brackets enclose a set of characters, any one of which may match a single character at that position. 
- A hyphen used within [ ] denotes a range of characters. 


Summary of what was implemented:
----------------------------------------

1. Command Line Interface
2. External commands
3. Internal commands (echo, pwd, jobs)
4. Single Pipe (Supports only external Commands)
5. Back grounding
6. Signal Handling (ctrl-c, ctrl-z, ctrl-\ )
7. Process control


Differences between the original specification and the final software, and reasons for it :
-------------------------------------------------------------------------------------------
Tasks we couldn't implement due to time constraint:

 1. Shell script programming
 2. Wild cards
 3. Job control
 4. Multiple pipes
 5. Redirection
 6. Few Internal commands



Problems and what was done to overcome them?
--------------------------------------------------------------------------
1. Infinite forking problems when working with signal handling.
   Found the bug in the execvp command error handling section where the child had to be exited.

2. Segmentation faults
   Appropriately parsing the user input string solved the problem.
