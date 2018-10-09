#include <hellfire.h>

#define TOTAL_SIMULATION_TIME 5000

int RUN_REPORT=1;
int static count_id=0;
int IS_RUNNING=1;

void caso_de_teste_1(void);
void caso_de_teste_2(void);
void simple_task(void);
void lancar_tarefas_aperiodicas(void);

void caso_de_teste_1(void)
{
    hf_spawn(lancar_tarefas_aperiodicas, 0, 0, 0, "aperiodic tasks creator", 1024);
}

void caso_de_teste_2(void)
{
    hf_spawn(lancar_tarefas_aperiodicas, 0, 0, 0, "aperiodic tasks creator", 1024);
    hf_spawn(simple_task, 60, 20, 60, "task a", 1024);
	hf_spawn(simple_task, 0, 90, 0, "task b", 1024);
	hf_spawn(simple_task, 0, 0, 0, "task c", 1024);
	hf_spawn(simple_task, 70, 30, 70, "task d", 1024);
	hf_spawn(simple_task, 0, 50, 0, "task e", 1024);
}

void lancar_tarefas_aperiodicas(void){
    int time_to_wait;
    while(IS_RUNNING==1)
    {
        time_to_wait = (random()%450)+50;
        delay_ms(time_to_wait);

        hf_spawn(simple_task, 0, (random()%100)+10, 0, "aperiodic simple_task", 1024);
    }
}

void simple_task(void){
	int i, x = 0;
	while(IS_RUNNING==1)
    {		
	    for(i=0; i<1000; i++)
        {
            x++;
        }
        for(i=1000; i>0; i--)
        {
            x--;
        }
	}
}

void simulation_control(void){    
    if(RUN_REPORT == 1)
    {
        delay_ms(TOTAL_SIMULATION_TIME);
        kprintf("\n\n\n\n\n");
        kprintf("RELATORIO DA SIMULACAO ---- INICIO\n");
	    print_jitter();
        kprintf("RELATORIO DA SIMULACAO ---- FIM\n");
        RUN_REPORT=0;  
        IS_RUNNING=0;  
    }  
}

void app_main(void){
    //IMPRIME RELATORIO APOS ESGOTAR O TEMPO DE SIMULACAO
    hf_spawn(simulation_control, 0, 0, 0, "simulation control", 1024);	

    //caso_de_teste_1();
    caso_de_teste_2();	

	return;
}
