//falta apenas inserir as funcoes do jogo
//a comunicacao ja esta sendo feita
//so falta colar as funcoes de computar as matrizes
//as funcoes do jogo devem ser chamadas na hora computar as matrizes
//deve se inserir as matrizes nos codigos de enviar e receber mensagens

#include <hellfire.h>
#include <noc.h>

#define TRUE 1
#define FALSE 0

#define MASTER_PORT 5000
#define SLAVE_PORT 6000

#define MASTER_CPU_ID 0

int number_of_processors;
int number_of_tasks;

void master(void)
{
    printf("\n");
    printf("***CPU ID: %d\n", hf_cpuid());
    printf("\n");

    int processor;
    int work=0;
    int correctness=0;   
    int until = number_of_processors;
    int destination;    
    int must_break = FALSE;    
    int buffer[1];
    int value;
    uint16_t cpu, task, size;
    int16_t error; 
    
    if (hf_comm_create(hf_selfid(), MASTER_PORT, 0))
    {		
       panic(0xff);
    }
    
    //the main processor must send all works
    while(must_break == FALSE)
    {
        //send a work to each processor
        for(processor=1; processor<number_of_processors; processor++)
        {             
            //send the work
            buffer[0]=work;
            error = hf_send(processor, SLAVE_PORT, buffer, sizeof(buffer), 0);
		    if (error)
		    {
			   printf("hf_send(): error %d\n", error);
		    }
            
            //update the next work
            work++;  
            
            //the last work was send
            if(work == number_of_tasks)
            {
                must_break = TRUE;
		        until = processor+1;
		        break;
            }                     
        }        
        
        //receive the works back
		for(processor=1; processor<until; processor++)
		{	
		    error = hf_recv(&cpu, &task, buffer, &size, 0);
		    if (error)
		    {
			    printf("hf_recv(): error %d\n", error);
		    }
		    else
		    {
		        value = buffer[0];
		    
			    printf("VALUE RETURNED: %d\n", value);
		    }
		}
    }
    //terminates the algorithm
	for(processor=1; processor<number_of_processors; processor++)
	{
	    //-1 sinalyzes the end work
        buffer[0]=-1;
        error = hf_send(processor, SLAVE_PORT, buffer, sizeof(buffer), 0);
		if (error)
		{
			   printf("hf_send(): error %d\n", error);
		}
	}
	printf("\n");
    printf("it's done!!!\n");    
    printf("\n");
    
    //just run something
    int i; while(1){i++;i--;}
}

void slave(void)
{
    printf("\n");
    printf("***CPU ID: %d\n", hf_cpuid());
    printf("\n");
    
    int value;
    int buffer[1];
    uint16_t cpu, task, size;
    int16_t error;
    
    if (hf_comm_create(hf_selfid(), SLAVE_PORT, 0))
    {
		panic(0xff);
	}
		
	while (1)
	{
	    //receive the work from the master
		error = hf_recv(&cpu, &task, buffer, &size, 0);
		if (error)
		{
			printf("hf_recv(): error %d\n", error);
		}
		else
		{
		    value = buffer[0];
		    
			printf("VALUE RECEIVED: %d\n", value);
			
			//-1 sinalyzes the end task
			if(value==-1)
			{
			    break;
			}
		}
		
		//compute the work
		
		//send the work back
		buffer[0]=value;
        error = hf_send(MASTER_CPU_ID, MASTER_PORT, buffer, sizeof(buffer), 0);
		if (error)
		{
		    printf("hf_send(): error %d\n", error);
		}
	}
	
	printf("\n");
	printf("it's done!!!\n");
	printf("\n");
	
	//just run something
    int i; while(1){i++;i--;}
}

void app_main(void)
{
    //important data
    number_of_processors = hf_ncores();
    number_of_tasks = 15;
    
    printf("\n\nNUMBER OF PROCESSORS: %d\n\n",number_of_processors);
    
    if(hf_cpuid()==MASTER_CPU_ID)
    {
        hf_spawn(master, 0, 0, 0, "master", 4096);
    }
    else
    {
        hf_spawn(slave, 0, 0, 0, "slave", 4096);
    }
}
