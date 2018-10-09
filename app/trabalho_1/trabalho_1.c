#include <hellfire.h>

#define TOTAL_SIMULATION_TIME 1000

int RUN_REPORT = 1;

void task1(void){
	int i;

	int x=0;
	while(1)
    {		
	    x++;
	}
}

void task2(void){
	int i;

	int x=0;
	while(1)
    {		
	    x++;
	}
}

void task3(void){
	int i;

	int x=0;
	while(1)
    {		
	    x++;
	}
}

void task4(void){
	int i;

	int x=0;
	while(1)
    {		
	    x++;
	}
}

void task5(void){
	int i;

	int x=0;
	while(1)
    {		
	    x++;
	}
}

void simulation_control(void){
    if(RUN_REPORT == 1)
    {
        delay_ms(TOTAL_SIMULATION_TIME);
        kprintf("\n\n\n\n\n");
        kprintf("RELATORIO DA SIMULACAO\n");
	    print_jitter();
        RUN_REPORT=0;    
    }  
}

void app_main(void){

	hf_spawn(task1, 60, 20, 60, "task a", 1024);
	hf_spawn(task2, 0, 90, 0, "task b", 1024);
	hf_spawn(task3, 0, 0, 0, "task c", 1024);
	hf_spawn(task4, 70, 30, 70, "task d", 1024);
	hf_spawn(task5, 0, 50, 0, "task e", 1024);
	hf_spawn(simulation_control, 0, 0, 0, "simulation control", 1024);

	return;
}
