# CS441/541 Scheduling Project

## Author(s):

Alexandra Emerson

## Date:

May 1, 2024

## Description:

This program is designed to simulate different scheduling algorithms, including First-Come, First-Served (FCFS); Shortest-Job First (SJF); Priority; and Round-Robin (RR). The outcome of the program is dependent on the input the user provides, including the file name and which scheduling algorithm they would like to use (as well as the quantum for Round-Robin scheduling).


## How to build the software

The included Makefile is used to build (compile and link) this software. It can be used as follows:
 - To build the software, run "make" from the command line.
 - To build the software and run the given tests, run "make check" from the command line.
 - To clean the directory, run "make clean" from the command line.


## How to use the software

To use the scheduler, users must first have a text file populated with integers in the following format:  

4  
4 3 7  
2 3 10  
3 5 7  
1 7 1

The integers used above are merely an example; any integers can be used. The first line identifies the number of processes to schedule. Each subsequent line describes each of those processes, including the process identifier, CPU burst length, and the process' priority, respectively.  

To run the program after building it, users must provide the name for the file created in the format specified above, along with the option specifying which algorithm they would like to use. The order of these arguments does not matter.  

* **Required:** -s # - See below for which numbers are associated with which scheduling algorithm.  

Option  | Algorithm
------------- | -------------
-s 1   | First-Come, First-Served (FCFS)
-s 2  | Shortest-Job First (SJF)
-s 3  | Priority
-s 4  | Round-Robin (RR)

* Required for **RR only:** -q # - This must be specified if the -s 4 option is entered.

__All other inputs will be ignored.__

__Example Input:__ ./scheduler -s 4 -q 5 example-file.txt  

## How the software was tested
The software was tested utilizing the command
```
make check
```
as well as running the given tests and my own tests. All three testing methods provided a solid understanding of how my program behaves.

## Test Suite

* tests/test1.txt - tests what happens when there are less processes (rows) in the file than the number of processes provided at the top of the file.
* tests/test2.txt - tests very large burst times.
* tests/test3.txt - tests that FCFS occurs with processes with the same priorities.
* tests/test4.txt - tests what happens if a negative number of processes is provided.
* tests/test5.txt - tests what happens when there are more processes (rows) in the file than the number of processes provided at the top of the file.

## Known bugs and problem areas
* No known bugs. I gave up on formatting after the level 5 tests, so the output looks a little wonky beyond level 5, but it still provides the correct results.
