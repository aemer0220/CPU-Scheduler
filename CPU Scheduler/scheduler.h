/*
 * 
 *
 * CS 441/541: CPU Scheduler
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

/******************************
 * Defines
 ******************************/


/******************************
 * Structures
 ******************************/
struct process_t{
	int pid;
	int burst;
    int priority;
	int waiting_time;
	int turnaround_time;
	int remaining_time;
	int round;
	int arrival_time;
};

enum algorithm{
        FCFS = 1,
        SJF,
        PRIORITY,
        RR
};

/******************************
 * Global Variables
 ******************************/
FILE* input_file;
char *file_name;
struct process_t *p_list; //used to create list of processes
int s_value;
int q_value;
int opt;
int num_procs;


/******************************
 * Function declarations
 ******************************/

/* Parse the command-line arguments
 *
 * Parameters:
 *    argc: number of arguments provided
 *    argv: argument strings
 * Returns:
 *    None
 */
void parse_cmdline(int argc, char **argv);


/* Calls sort functions or round robin algorithm,
 * depending on what command line arguments were
 * entered.
 *
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void go();
	
/* Prints the preliminary header
 *
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void print_header();

/* Prints the results of the program
 *
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void print_results();

/* Opens provided file, and instantiates
 * process_t struct
 *
 * Parameters:
 *    argc: number of arguments provided
 *    argv: argument strings
 * Returns:
 *    None
 */
void process_file(int argc, char **argv);

/* Compares the burst times of two processes
 *
 * Parameters:
 *    process1: first process
 *    process2: second process
 * Returns:
 *    The difference between the two processes, indicating
 *    which order they can be sorted in, via qsort
 */
int compare_burst(const void * process1, const void * process2);


/* Compares the arrivals of two processes
 *
 * Parameters:
 *    process1: first process
 *    process2: second process
 * Returns:
 *    The difference between the two processes, indicating
 *    which order they can be sorted in, via qsort
 */
int compare_arrival(const void * process1, const void * process2);


/* Compares the priorities of two processes
 *
 * Parameters:
 *    process1: first process
 *    process2: second process
 * Returns:
 *    The difference between the two processes, indicating
 *    which order they can be sorted in, via qsort
 */
int compare_priority(const void * process1, const void * process2);

/* Calls qsort - sorts processes by their burst times
 * or by their priority, depending on which algorithm
 * was called
 *
 * Parameters:
 *    processes: process_t struct
 * Returns:
 *    None
 */
void sort();

/* Runs the processes based on which algorithm was called,
 * controlled by the qsort calls.
 *
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void run();

/* Computes the average wait and turnaround times
 * of each process.
 *
 * Parameters:
 *    None
 * Returns:
 *    double array
 */
double * average();
/* Does the Round Robin Algorithm
 *
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void round_robin();


