#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);

void scheduler(Process* proc, LinkedQueue** ProcessQueue, int proc_num, int queue_num, int period){
    printf("Process number: %d\n", proc_num);
    for (int i = 0;i < proc_num; i++)
        printf("%d %d %d\n", proc[i].process_id, proc[i].arrival_time, proc[i].execution_time);

    printf("\nQueue number: %d\n", queue_num);
    printf("Period: %d\n", period);
    for (int i = 0;i < queue_num; i++){
        printf("%d %d %d\n", i, ProcessQueue[i]->time_slice, ProcessQueue[i]->allotment_time);
    }

  	int not_done=0;
	int i,j,k,running=0,exist=0, first_priority = queue_num-1, doit=0;
	int real_period = period;
	int move_queue[proc_num],move_process[proc_num];
	//Worked time	number of new process in a time
	int time=0 ;
	//To count how many times process worked
	int count_times[proc_num];
	//The times to move priority
	int queue_time[queue_num];
	//The number new process	The number of finished process	Number to move
	int new_process =0,		fin_process=0,			move=0;
	//The number of process in certain stack
	int stack_process[queue_num];
	//The process number in stack	Array of finished process	Array of new process
	int stack[queue_num][proc_num],finish[proc_num],		new[proc_num+1];
	queue_num--;
	for(i=0;i<proc_num;i++){

	count_times[i]=1;
	finish[i]=-1;
	new[i]=-1;
	move_queue[i]=-1;
	move_process[i]=-1;
	}
	queue_time[0]=-1;
	for(i=0;i<=queue_num;i++){
		queue_time[i] = ProcessQueue[i]->allotment_time/ProcessQueue[i]->time_slice;
		stack_process[i]=0;
		printf("%d ",queue_time[i]);
	}
	

	for(i=0;i<=queue_num;i++){
		for(j=0;j<proc_num;j++){
		stack[i][j] =-1;
		}
	}
	printf("\n");
	//When all processes not done
	while(not_done==0){
		new_process=0;
		exist=0;
		printf("Time: %d \t",time);
		
		//Check if any new process
		for(i=0;i<proc_num;i++){
			if(time>=proc[i].arrival_time){
				//check whether the process exist
				first_priority=queue_num;
				for(j=0;j<=queue_num;j++){
					for(k=0;k<proc_num;k++){
						if(stack[j][k]==i)exist++;
					}
				}
				for(j=0;j<proc_num;j++){
					if(finish[j]==i)exist++;
				}
				//If process has not existed yet
				if(exist==0){
					new[new_process++]=i;}
				
				exist=0;
				
			}
		}
		
		if(new_process==1){
			stack[queue_num][stack_process[queue_num]++] = new[0];
			running++;
		}
		else if(new_process>1){
			for(i=0; i<new_process; ++i){
        			for(j=i+1; j<new_process; ++j){
            				if(proc[new[i]].process_id<proc[new[j]].process_id){
                				int Temp = new[i];
                				new[i] = new[j];
                				new[j] = Temp;
            				}
        			}
        		}
        		for(i=0;i<new_process;i++){
        		
				stack[queue_num][stack_process[queue_num]++] = new[i];
				running++;
        		}
		}
		if(time== period){
			
			for(i=0;i<proc_num;i++){
				count_times[i]=1;
				new[i]=-1;
				
			}
			for(i=0;i<=queue_num;i++){
				for(j=0;j<proc_num;j++){
					stack[i][j]=-1;
				}
				stack_process[i] =0;
			}
			period+=real_period;
			printf("Period time.");
			running=0;
		}
		//To execute process
		doit=0;
		while(doit ==0){
		if(stack_process[first_priority]>0){
			//Check if have process
				
			for(j=0;j<stack_process[first_priority];j++){
				printf("%d in queue %d,\t",proc[stack[first_priority][j]].process_id,first_priority);
			}
				
			if(period > (time+ProcessQueue[first_priority]->time_slice)){
				//If period does not come yet
				if( proc[stack[first_priority][0]].execution_time> ProcessQueue[first_priority]->time_slice){
					//If process will not finish
					printf("\t(Do %d.)",proc[stack[first_priority][0]].process_id);
					count_times[stack[first_priority][0]]++;
					proc[stack[first_priority][0]].execution_time-= ProcessQueue[first_priority]->time_slice;
					outprint(time,time+ProcessQueue[first_priority]->time_slice,proc[stack[first_priority][0]].process_id,proc[stack[first_priority][0]].arrival_time,proc[stack[first_priority][0]].execution_time);
					
					time+=ProcessQueue[first_priority]->time_slice;
						
					if((count_times[stack[first_priority][0]]>queue_time[first_priority])&&(first_priority>0)){
						//If process move queue
						printf("(%d move to queue %d)",proc[stack[first_priority][0]].process_id,first_priority-1);
						move_queue[move] = first_priority;
						move_process[move++] = stack[first_priority][0];
														
					}else{
						//If process still in the same queue
						if(stack_process[first_priority]>1){
							
							int temp = stack[first_priority][0];
							for(j=0;j<(stack_process[first_priority]);j++){
								stack[first_priority][j] = stack[first_priority][j+1];
							}
							stack[first_priority][stack_process[first_priority]-1]=temp;
						}
					}
						
				}else{
					//If process will finish
					outprint(time,time+proc[stack[first_priority][0]].execution_time,proc[stack[first_priority][0]].process_id,proc[stack[first_priority][0]].arrival_time,0);
					time+= proc[stack[first_priority][0]].execution_time;
					proc[stack[first_priority][0]].execution_time=0;
					finish[fin_process++] = stack[first_priority][0];
					printf(" (End %d) ",proc[stack[first_priority][0]].process_id);
				
					if(stack_process[first_priority]>1){
						for(j=0;j<stack_process[first_priority]-1;j++){
							stack[first_priority][j] = stack[first_priority][j+1];
						}
					}
					stack[i][stack_process[first_priority]-1]=-1;
					stack_process[first_priority]--;
					running--;
				}	
			}else{
				//If comes period and process will not finish
				if( (time+proc[stack[first_priority][0]].execution_time)> period){
					proc[stack[first_priority][0]].execution_time -= (period-time);
					outprint(time,period,proc[stack[first_priority][0]].process_id,proc[stack[first_priority][0]].arrival_time,proc[stack[first_priority][0]].execution_time);
					time +=(period-time);
				}else{
					//process will finish
					outprint(time,time+proc[stack[first_priority][0]].execution_time,proc[stack[first_priority][0]].process_id,proc[stack[first_priority][0]].arrival_time,0);
					time +=proc[stack[first_priority][0]].execution_time;
					proc[stack[first_priority][0]].execution_time =0;
					finish[fin_process++] = stack[first_priority][0];
					printf(" (End %d) ",proc[stack[first_priority][0]].process_id);
					
					if(stack_process[first_priority]>1){
						for(j=0;j<stack_process[first_priority]-1;j++){
							stack[first_priority][j] = stack[first_priority][j+1];
						}
					}
					stack[first_priority][stack_process[first_priority]-1]=-1;
					stack_process[first_priority]--;
					running--;
				}
			}
			doit =1;
		}else{
			if(first_priority>0){first_priority--;}else{doit=1; first_priority=queue_num;}
			
		}}
		
		//Move queue afterward
		for(i=0;i<move;i++){
			if((move_queue[i] !=-1) && (move_process[i]!=-1)){
				//If the process to move
				stack[move_queue[i]-1][stack_process[move_queue[i]-1]++]=move_process[i];
				count_times[move_process[i]]=1;
				
				if(stack_process[move_queue[i]]>1){
					for(j=0;j<(stack_process[move_queue[i]]-1);j++){
						stack[move_queue[i]][j] = stack[move_queue[i]][j+1];
					}
				}
				
				stack[move_queue[i]][stack_process[move_queue[i]]-1]=-1;
				stack_process[move_queue[i]]--;
				move_queue[i] =-1;
				move_process[i]=-1;
			}
		}
		
		//Check new process
		for(i=0;i<proc_num;i++){
			if(time>=proc[i].arrival_time){
				//check whether the process esist
				for(j=0;j<=queue_num;j++){
					for(k=0;k<proc_num;k++){
						if(stack[j][k]==i)exist++;
					}
				}
				for(j=0;j<proc_num;j++){
					if(finish[j]==i)exist++;
				}
				if(exist==0){
					running++;}
				
				exist=0;
				first_priority=queue_num;
				
			}
		}

		printf("\tRunning %d,top queue:%d \n",running,first_priority);
		
		if(running ==0) time++;
		
		
		move=0;
		if(fin_process==proc_num) not_done=1;

	}
}

