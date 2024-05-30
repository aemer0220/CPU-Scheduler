/* 
 * CS 441/541: CPU Scheduler
 *
 * This program implements different scheduling algorithms, including FCFS,
 * SJF, Priority, and RR.
 *
 * Author: Alexandra Emerson
 * Last Modified Date: 5/1/2024
 */

#include "scheduler.h"

/* MAIN */
int main(int argc, char **argv) {  
  	s_value = 0;
	q_value = 0;
	opt = 0;
	num_procs = 0;
	
	parse_cmdline(argc, argv);  //process command line arguments
	process_file(argc, argv);   //process provided file
	print_header();
	go();                       //performs algorithsm depending on what command line arguments were given.
	print_results();	   //prints and formats the results
	
	return 0;
}

/* Handle the command line arguments */
void parse_cmdline(int argc, char **argv){
	while ((opt = getopt(argc, argv, "s:q:")) != -1){
		switch (opt){
		case 's':
			if(isdigit(optarg[0]) == 0 || strlen(optarg) > 1){
				printf("ERROR: Argument value must be a digit.\n");
				exit(0);
			}
			s_value = atoi(optarg);
			break;    

		case 'q':
			if(strlen(optarg)>2){
				printf("ERROR: Argument value must be a number and under under 100.\n");
				exit(0);
			}
			q_value = atoi(optarg);
			break;
		}
	}

	if (s_value != 4){
		q_value = 0;
	}
}

void process_file(int argc, char** argv){
	int i;
	if (optind >= argc){
		printf("ERROR: File name not detected.");
		exit(0);
	}
	else{
		int i = optind;
		file_name = argv[i];
	}

	input_file = fopen(file_name, "r");

	if(input_file == NULL){
		printf("ERROR: File not found. \n");
		exit(0);
	}
	
	fscanf (input_file, "%d", &num_procs); //process info infile
	if (num_procs < 1){
		printf("ERROR: Check your file. The number of processes must be an integer greater than 0\n");
		exit(0);
	}
  
	p_list = malloc(sizeof(struct process_t) * num_procs);
	//initiate process list attributes
	for(i = 0; i < num_procs; i++){
		p_list[i].pid = 0;
		p_list[i].burst = 0;
		p_list[i].priority = 0;
		p_list[i].waiting_time = 0;
		p_list[i].turnaround_time = 0;
		p_list[i].remaining_time = 0;
	}
}

void go(){
	//fail if something other than 1-4 was entered
	if ((s_value != FCFS) && (s_value != SJF) && (s_value != PRIORITY) && (s_value != RR)){
		printf("ERROR: Argument value for -s should be a digit between 1 and 4 (inclusive");
		exit(0);
	}

	else if ((s_value == FCFS) || (s_value == SJF) || (s_value == PRIORITY)){
		sort(p_list); //only relevent for SJF and PRIORITY.
		              //Won't sort for FCFS, since it is already sorted by arrival order
		run(p_list);
	}
	else{ //round robin has its own algorithm
		if (q_value < 1){ //fail if no argument is provided for -q
			printf("ERROR: Argument value is required for -q for the Round Robin scheduler. \n");
			exit(0);
		}
		round_robin(p_list);
	}
}

void sort(){
	if(s_value == SJF){
		//If shortest job first, compare the burst times and sort with qsort
		qsort(p_list, num_procs, sizeof(struct process_t), compare_burst);
	}
	else if(s_value == PRIORITY){ //PRIORITY
		//If priority, sort by the priority of each process and sort with qsort
		qsort(p_list, num_procs, sizeof(struct process_t), compare_priority);
	}
}

void run(){
	int i = 0;
	int time = 0;
		
	while(i < num_procs){
		p_list[i].turnaround_time = time;
		p_list[i].waiting_time = time;
		p_list[i].remaining_time = p_list[i].burst;

		//while each process still has time left
		while(p_list[i].remaining_time > 0){
			p_list[i].turnaround_time++; //increase the turn around time
			time = time + 1; //and the total time
			
			p_list[i].remaining_time--; //decrease the time left 
		}
		i++;
	}	
}

//aids qsort
int compare_burst(const void * p1, const void * p2){
	int process1 = ((struct process_t *)p1)->burst;
	int process2 = ((struct process_t *)p2)->burst;
	return process1 - process2; //return the difference between the two bursts
}

//aids qsort
int compare_priority(const void * p1, const void * p2){
	int process1 = ((struct process_t *)p1)->priority;
	int process2 = ((struct process_t *)p2)->priority;
	return process1 - process2; //return the difference between the two priorities
}

//aids qsort
int compare_arrival(const void * p1, const void * p2){
	int process1 = ((struct process_t *)p1)->arrival_time;
	int process2 = ((struct process_t *)p2)->arrival_time;
	return process1 - process2; //return the difference between the two arrival times
}

void round_robin(){
	int i, j, k;
	int time = 0;
	int end = 0;

	for(i = 0; i <num_procs; i++){
		end = end + p_list[i].burst; //add up all the bursts to get the time when all processes end
	}
	
	while(time < end){ //while the current time is less than the total time
		for(j = 0; j < num_procs; j++){
			if(p_list[j].remaining_time > 0){ //if the process has remaining time left
				p_list[j].waiting_time = time - (p_list[j].round * q_value);
				p_list[j].turnaround_time = time;

				k = 0;
				while(k < q_value && p_list[j].remaining_time){
					time++;
					p_list[j].turnaround_time++;
					p_list[j].remaining_time--;
					k++;
				}
				
				p_list[j].round++; //go to next cycle
			}
		}
	}
}

double * average(){ //find average waiting and turnaround times
	int i;
	double average_wait, average_turn;
	double *a_list = malloc(sizeof(double) * 2);
	double wait = 0;
	double turn = 0;

	for(i = 0; i < num_procs; i++){
		wait = wait + p_list[i].waiting_time; //add up all of the wait times
		turn = turn + p_list[i].turnaround_time; //add up all of the turnaround times
	}
	
	average_wait = wait/num_procs; //get average wait time
	a_list[0] = average_wait; //put into double array

	average_turn = turn/num_procs; //get average turnaround time
	a_list[1] = average_turn;      //put into double array

	return a_list;
}

void print_header(){
	char* alg;
	int i;
   
	if(s_value == 1){
		alg = "FCFS";
	}
	else if(s_value == 2){
		alg = "SJF";
	}
	else if(s_value == 3){
		alg = "Priority";
	}
	else{
		alg = "RR";
	}

	printf("Scheduler   :  %d %s\n", s_value, alg);
	printf("Quantum     : %2d\n", q_value);
	printf("Sch. File   : %s\n", file_name);
	printf("-------------------------------\n");

	printf("Arrival Order:");

	for(i = 0; i < num_procs - 1; i++){
		fscanf(input_file, "%i %i %i", &p_list[i].pid, &p_list[i].burst, &p_list[i].priority);
		if (p_list[i].pid >= 1000){
			printf("%5i,", p_list[i].pid);
		}

		else if(p_list[i].pid >= 100){
			printf("%4i,", p_list[i].pid);
		}
		else{  
			printf("%3i,", p_list[i].pid);
		}
		p_list[i].arrival_time = i;
		p_list[i].remaining_time = p_list[i].burst;
		p_list[i].round = 0;
	}
	fscanf(input_file, "%i %i %i", &p_list[i].pid, &p_list[i].burst, &p_list[i].priority);
	if (p_list[i].pid >= 1000){
		printf("%5i\n", p_list[i].pid);
	}

	else if(p_list[i].pid >= 100){
		printf("%4i\n", p_list[i].pid);
	}
	else{
		printf("%3i\n", p_list[i].pid);
	}
		p_list[i].arrival_time = i;
		p_list[i].remaining_time = p_list[i].burst;
		p_list[i].round = 0;
	
	printf("Process Information:\n");
	for(i = 0; i < num_procs; i++){
		if(p_list[i].pid >= 1000){
			printf("%4d%7d%4d\n", p_list[i].pid, p_list[i].burst, p_list[i].priority);
		}

		else if(p_list[i].pid >= 100){
			printf("%3d%4d%4d\n", p_list[i].pid, p_list[i].burst, p_list[i].priority);
		}
		else{
			printf("%2d%5d%4d\n", p_list[i].pid, p_list[i].burst, p_list[i].priority);
		}
	}
	printf("-------------------------------\n");
	printf("Running...\n");

}

void print_results(){
	int i;
	double *averages;
	qsort(p_list, num_procs, sizeof(struct process_t), compare_arrival);

	printf("##################################################\n");
	printf("#  #    CPU Pri   W   T\n");

	for(i = 0; i < num_procs; i++){
		
		if((p_list[i].waiting_time < 1000) && (p_list[i].turnaround_time >= 1000)){
			if (p_list[i].pid >=100){
				printf("# %2d%6d%4d%4d%5d\n", p_list[i].pid, p_list[i].burst, p_list[i].priority, p_list[i].waiting_time, p_list[i].turnaround_time);
			}
			else{
				printf("# %2d%7d%4d%4d%5d\n", p_list[i].pid, p_list[i].burst, p_list[i].priority, p_list[i].waiting_time, p_list[i].turnaround_time);
			}
		}
		else if(p_list[i].waiting_time < 1000){
			if(p_list[i].pid >=1000){
				printf("# %2d%6d%4d%4d%4d\n", p_list[i].pid, p_list[i].burst, p_list[i].priority, p_list[i].waiting_time, p_list[i].turnaround_time);
			}
			else{
				printf("# %2d%7d%4d%4d%4d\n", p_list[i].pid, p_list[i].burst, p_list[i].priority, p_list[i].waiting_time, p_list[i].turnaround_time);
			}
		}

		else{
			if(p_list[i].pid >=1000){
				printf("# %2d%6d%4d%5d%8d\n", p_list[i].pid, p_list[i].burst, p_list[i].priority, p_list[i].waiting_time, p_list[i].turnaround_time);
				
			}
			else{
				printf("# %2d%7d%4d%5d%8d\n", p_list[i].pid, p_list[i].burst, p_list[i].priority, p_list[i].waiting_time, p_list[i].turnaround_time);
			}
		}	
	}
	
	averages = average(p_list); //compute average waiting time and turnaround time
	printf("##################################################\n");
	
	if(averages[0] >= 1000){
		printf("# Avg. Waiting Time   : %7.2f\n", averages[0]);
	}
	else{
		printf("# Avg. Waiting Time   :%7.2f\n", averages[0]);
	}
	
	if(averages[1] >= 1000){
		printf("# Avg. Turnaround Time: %7.2f\n", averages[1]);
	}
	else{
		printf("# Avg. Turnaround Time:%7.2f\n", averages[1]);
	}
	printf("##################################################\n");
}
