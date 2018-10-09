#include <hellfire.h>

/*
 * EXEMPLO DE USO DO READ COUNTER PARA CONTABILIZAR TEMPO:
 * hellfireos-master/app/task_create_kill/create_kill.c
 */

void task0(void){
	int i;

	int x=0;
	for(;;){
		for(i = 1; i <= 200; i++)
		{
			x++;
		}
	}
}

void task1(void){
	int i;

	int x=0;
	for(;;){
		for(i = 1; i <= 200; i++)
		{
			x++;
		}
	}
}

void task2(void){
	int i;

	int x=0;
	for(;;){
		for(i = 1; i <= 200; i++)
		{
			x++;
		}
	}
}

void task3(void){
	int i;

	int x=0;
	for(;;){
		for(i = 1; i <= 200; i++)
		{
			x++;
		}
	}
}

void task4(void){
	int i;

	int x=0;
	for(;;){
		for(i = 1; i <= 200; i++)
		{
			x++;
		}
	}
}

void task5(void){
	int i;

	int x=0;
	for(;;){
		for(i = 1; i <= 200; i++)
		{
			x++;
		}
	}
}

void app_main(void){

	hf_spawn(task0, 60, 20, 60, "task a", 1024);
	hf_spawn(task1, 0, 90, 0, "task b", 1024);
	hf_spawn(task2, 0, 0, 0, "task c", 1024);
	hf_spawn(task3, 70, 30, 70, "task d", 1024);
	hf_spawn(task4, 0, 50, 0, "task e", 1024);
	hf_spawn(task5, 0, 0, 0, "task f", 1024);

	delay_ms(10000);
	kprintf("\n\n\n\n\nTESTANDO IMPRESSAO");
	print_jitter();

	return;
}
