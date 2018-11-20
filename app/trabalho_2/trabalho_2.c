#include <hellfire.h>
#include <noc.h>

#include "labyrinth.h"

#define TRUE 1
#define FALSE 0

#define MASTER_PORT 5000
#define SLAVE_PORT 6000

#define MASTER_CPU_ID 0

int number_of_processors;
int number_of_tasks;
int parallel_run;
uint32_t time_begin;
uint32_t time_end;
uint32_t time_total;

//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//

void show(int *m, int lin, int col)
{    
	int i, j;

	for (i = 0; i < lin; i++) {
		for (j = 0; j < col; j++) {
			printf("%3d ", m[i * col + j]);
		}
		printf("\n");
	}
	printf("\n");
}

void mark(int *m, int l, int c, int lin, int col)
{
	int h, i, j, k = 1;

	for (h = 0; h < lin * col; h++, k++) {
		for (i = 0; i < lin; i++) {
			for (j = 0; j < col; j++) {
				if (m[i * col + j] == k) {
					if (i - 1 >= 0) {
						if (m[(i - 1) * col + j] == 0)
							m[(i - 1) * col + j] = k + 1;
					}
					if (i + 1 < lin) {
						if (m[(i + 1) * col + j] == 0)
							m[(i + 1) * col + j] = k + 1;
					}
					if (j - 1 >= 0) {
						if (m[i * col + (j - 1)] == 0)
							m[i * col + (j - 1)] = k + 1;
					}
					if (j + 1 < col) {
						if (m[i * col + (j + 1)] == 0)
							m[i * col + (j + 1)] = k + 1;
					}
				}
			}
		}
	}
}

int search(int *m, int i, int j, int ei, int ej, int lin, int col)
{
	int k = 2;

	while (k > 1) {
		k = m[i * col + j];
		printf("[%d,%d] ", i, j);
		if (i - 1 >= 0) {
			if (m[(i - 1) * col + j] < k && m[(i - 1) * col + j] > 0) {
				i--;
				continue;
			}
		}
		if (i + 1 < lin) {
			if (m[(i + 1) * col + j] < k && m[(i + 1) * col + j] > 0) {
				i++;
				continue;
			}
		}
		if (j - 1 >= 0) {
			if (m[i * col + (j - 1)] < k && m[i * col + (j - 1)] > 0) {
				j--;
				continue;
			}
		}
		if (j + 1 < col) {
			if (m[i * col + (j + 1)] < k && m[i * col + (j + 1)] > 0) {
				j++;
				continue;
			}
		}
	} printf("\n");
	if (i == ei && j == ej)
		return 1;
	else
		return 0;
}

int solve(int *m, int lin, int col, int si, int sj, int ei, int ej)
{
	m[ei * col + ej] = 1;
	mark(m, ei, ej, lin, col);
	/* show(m, lin, col); */
	return search(m, si, sj, ei, ej, lin, col);
}

void thread(void)
{
	time_begin = _readcounter();

	struct maze_s *m;
	int i, s, k = 0;

	for (i = 0; i < sizeof(mazes) / sizeof(struct maze_s); i++) 
	{
		m = &mazes[i];
		s = solve(m->maze, m->lines, m->columns, m->start_line, m->start_col, m->end_line, m->end_col);
		if (s) 
		{
			printf("\nOK!\n");
			k++;
		} 
		else 
		{
			printf("\nERROR!\n");
		};
	};
	printf("\nsummary: %d of %d solved\n", k, i);

	time_end = _readcounter();
	time_total = time_end - time_begin;
	printf("\n\nEXECUTION TIME: %d\n\n", time_total);

	while (1);
}

//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//

void master(void)
{
	time_begin = _readcounter();

	if (hf_comm_create(hf_selfid(), MASTER_PORT, 0))
	{		
		panic(0xff);
	}

	int processor;
	int work=0;  
	int until = number_of_processors;
	int destination;    
	int must_break = FALSE;	
	uint16_t cpu, task, size;
	int buffer;	
	int16_t error;
	int k=0;

	//the main processor must send all works
	while(must_break == FALSE)
	{
		//send a work to each processor sizeof(struct maze_s)
		for(processor=1; processor<number_of_processors; processor++)
		{         
			//send the main struct
			error = hf_send(processor, SLAVE_PORT, &mazes[work], sizeof(struct maze_s), 0);
			if (error)
			{
				printf("hf_send(): error %d\n", error);
			}

			//receive signal
			error = hf_recv(&cpu, &task, &buffer, &size, 0);
			if (error)
			{
				printf("hf_recv(): error %d\n", error);
			}

			//send the maze
			error = hf_send(processor, SLAVE_PORT, &(mazes[work].maze), sizeof(mazes[work].maze), 0);
			if (error)
			{
				printf("hf_send(): error %d\n", error);
			}	

			//update the work that must be send
			work++;		

			//if the last work was send, break
			if(work == number_of_tasks)
			{
				must_break = TRUE;
				until = processor+1;
				break;
			}                     
		}        

		//receive the answers of the computations of each processor
		for(processor=1; processor<until; processor++)
		{	
			//receive the answer
			error = hf_recv(&cpu, &task, &buffer, &size, 0);
			if (error)
			{
				printf("hf_recv(): error %d\n", error);
			}
			else
			{
				if (buffer) 
				{
					printf("\nOK!\n");
					k++;
				}
			}
		}
	}	

	printf("\nsummary: %d of %d solved\n", k, work); 

	time_end = _readcounter();
	time_total = time_end - time_begin;
	printf("\n\nEXECUTION TIME: %d\n\n", time_total);

	while(1);
}

void slave(void)
{
	if (hf_comm_create(hf_selfid(), SLAVE_PORT, 0))
	{
		panic(0xff);
	}

	uint16_t cpu, task, size;
	int buffer = -1;	
	int16_t error;
	struct maze_s m;
	int* data;

	while (1)
	{	     
		//receive the main struct
		error = hf_recv(&cpu, &task, &m, &size, 0); 
		if (error)
		{
			printf("hf_recv(): error %d\n", error);
		}		

		//send signal
		error = hf_send(MASTER_CPU_ID, MASTER_PORT, &buffer, sizeof(int), 0);
		if (error)
		{
			printf("hf_send(): error %d\n", error);
		}

		//receive the maze
		error = hf_recv(&cpu, &task, &data, &size, 0); 
		if (error)
		{
			printf("hf_recv(): error %d\n", error);
		}			

		//compute the maze
		buffer = solve(mazes[m.position].maze, m.lines, m.columns, m.start_line, m.start_col, m.end_line, m.end_col);

		//send the answer
		error = hf_send(MASTER_CPU_ID, MASTER_PORT, &buffer, sizeof(int), 0);
		if (error)
		{
			printf("hf_send(): error %d\n", error);
		}
	}
}

//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//
//###########################################################################//

void app_main(void)
{
	//important data
	number_of_processors = hf_ncores();
	number_of_tasks = sizeof(mazes) / sizeof(struct maze_s);
	parallel_run = TRUE;

	if(parallel_run == TRUE)
	{
		if(hf_cpuid()==MASTER_CPU_ID)
		{
			hf_spawn(master, 10, 9, 10, "master", 4096);
		}
		else
		{
			hf_spawn(slave, 10, 9, 10, "slave", 4096);
		}
	} 
	else
	{
		hf_spawn(thread, 10, 9, 10, "serial", 4096);    
	}  
}
