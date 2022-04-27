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

    //Test outprint function, it will output "Time_slot:1-2, pid:3, arrival-time:4, remaining_time:5" to output.loc file.

	int escape=1;
	int pid=0;
	int time=0;
	int num[queue_num];
	char finish[30]="";
	char stack1[30]="";
	char stack2[30]="";
	char stack3[30]="";
char stack4[30]="";
	char stack5[30]="";
	char stack6[30]="";
	char add[30]="";

	int q=ProcessQueue[queue_num-1]->allotment_time/ProcessQueue[queue_num-1]->time_slice;
	num[0]=q;
	for(int i=1;i<queue_num;i++){
		num[i]=ProcessQueue[queue_num-i-1]->allotment_time/ProcessQueue[queue_num-i-1]->time_slice+num[i-1];
	}



	int count[proc_num];
	for(int i=0;i<proc_num;i++)
		count[i]=0;



	while(escape==1){
//ADD IF NEEDED//

	    for (int i = 0;i < proc_num; i++){
			char tmp= i+'0';
			if(time>=proc[i].arrival_time){

				if(strchr(stack1,tmp)||strchr(finish,tmp)||strchr(stack2,tmp)||strchr(stack3,tmp)||strchr(stack4,tmp)||strchr(stack5,tmp)){

				}else{
    					strncat(add,&tmp,1);
						

				}
			}
		}
	//Add to sack	
	if(strlen(add)==1){	
    		strncat(stack1,add,1);
    	//Add to sack if one or more enter
    	}else if(strlen(add)>1){

    	   for (int i = 0; i < strlen(add)-1; i++){     
	       for (int j = 0; j < strlen(add)-i-1; j++){

	       
		   if (proc[add[j]-'0'].process_id < proc[add[j+1]-'0'].process_id ){
		   	char tmp=add[j+1];
		   	add[j+1]=add[j];
		   	add[j]=tmp;

		   }
		}
		}

    		strcat(stack1,add);

    	}

    	strcpy(add,"");
    	


//First Stack//
	if(strlen(stack1)!=0){
//IF STACK IS NOT FINISH//


	
	
			if( ProcessQueue[queue_num-1]->time_slice < proc[stack1[0]-'0'].execution_time){
				if(period>time+ProcessQueue[queue_num-1]->time_slice){
				
				proc[stack1[0]-'0'].execution_time=proc[stack1[0]-'0'].execution_time - ProcessQueue[queue_num-1]->time_slice;
	   			outprint(time,time+ProcessQueue[queue_num-1]->time_slice,proc[stack1[0]-'0'].process_id,proc[stack1[0]-'0'].arrival_time,proc[stack1[0]-'0'].execution_time);
				time+=ProcessQueue[queue_num-1]->time_slice;
				count[stack1[0]-'0']++;
				char tmp=stack1[0];

//If count is NOT larger than 2//
				if(count[stack1[0]-'0']==num[0]){
						for(int i =0; i<strlen(stack1);i++){
							stack1[i]=stack1[i+1];
						}	
						if(queue_num>1){


							strncat(stack2,&tmp,1);
						}else{
							stack1[strlen(stack1)]=tmp;
						}
					
//If Count is larger than 2//
				}else{
					if(strlen(stack1)>1){
						for(int i =0; i<strlen(stack1);i++){
							stack1[i]=stack1[i+1];
						}
						stack1[strlen(stack1)]=tmp;
					}

	

				}

	

			}else{		//CHECK if trigger finish//
			proc[stack1[0]-'0'].execution_time=proc[stack1[0]-'0'].execution_time - (period-time);
	   		outprint(time,period,proc[stack1[0]-'0'].process_id,proc[stack1[0]-'0'].arrival_time,proc[stack1[0]-'0'].execution_time);
			time=period;
//WHEN FINISH PROCESS//
			}
		}else{

	   			outprint(time,time+proc[stack1[0]-'0'].execution_time,proc[stack1[pid]-'0'].process_id,proc[stack1[0]-'0'].arrival_time,0);
				time+=proc[stack1[0]-'0'].execution_time;
				proc[stack1[0]-'0'].execution_time=0;


		
				char tmp=stack1[0];
				for(int a=0;a<strlen(stack1);a++){
					stack1[a]=stack1[a+1];
				}
	    			strncat(finish,&tmp,1);
		}		
		
	}else if(strlen(stack2)!=0){
//IF STACK IS NOT FINISH//
		if(period>time+ProcessQueue[queue_num-2]->time_slice){
			if( ProcessQueue[queue_num-2]->time_slice < proc[stack2[0]-'0'].execution_time){

				proc[stack2[0]-'0'].execution_time=proc[stack2[0]-'0'].execution_time - ProcessQueue[queue_num-2]->time_slice;

	   			outprint(time,time+ProcessQueue[queue_num-2]->time_slice,proc[stack2[0]-'0'].process_id,proc[stack2[0]-'0'].arrival_time,proc[stack2[0]-'0'].execution_time);
				time+=ProcessQueue[queue_num-2]->time_slice;


				count[stack2[0]-'0']++;
				char tmp=stack2[0];

	

//If count is  larger than 2//

				if(count[stack2[0]-'0']==num[1]){

					for(int i =0; i<strlen(stack2);i++){
						stack2[i]=stack2[i+1];
					}


					if(queue_num>2){
						strncat(stack3,&tmp,1);

					}else{

						stack2[strlen(stack2)]=tmp;
					}


//If Count is NOT larger than 2//
				}else{
					if(strlen(stack2)>1){
						for(int i =0; i<strlen(stack2);i++){
							stack2[i]=stack2[i+1];
						}
						stack2[strlen(stack2)]=tmp;
					}

				}

			}else{
//WHEN FINISH PROCESS//
				outprint(time,time+proc[stack2[0]-'0'].execution_time,proc[stack2[0]-'0'].process_id,proc[stack2[0]-'0'].arrival_time,0);
				time+=proc[stack2[0]-'0'].execution_time;
				proc[stack2[0]-'0'].execution_time=0;
				char tmp=stack2[0];
				strncat(finish,&tmp,1);
				for(int a=0;a<strlen(stack2);a++){
					stack2[a]=stack2[a+1];
					}
			}

		}else{
			//CHECK if trigger finish//

			proc[stack2[0]-'0'].execution_time=proc[stack2[0]-'0'].execution_time - (period-time);

	   		outprint(time,period,proc[stack2[0]-'0'].process_id,proc[stack2[0]-'0'].arrival_time,proc[stack2[0]-'0'].execution_time);
			time=period;
		}
	}else if(strlen(stack3)!=0){
//IF STACK IS NOT FINISH//

		if(period>time+ProcessQueue[queue_num-3]->time_slice){
		
			if( ProcessQueue[queue_num-3]->time_slice < proc[stack3[0]-'0'].execution_time){
				proc[stack3[0]-'0'].execution_time=proc[stack3[0]-'0'].execution_time - ProcessQueue[queue_num-3]->time_slice;
	   			outprint(time,time+ProcessQueue[queue_num-3]->time_slice,proc[stack3[0]-'0'].process_id,proc[stack3[0]-'0'].arrival_time,proc[stack3[0]-'0'].execution_time);
				time+=ProcessQueue[queue_num-3]->time_slice;

				count[stack3[0]-'0']++;
				char tmp=stack3[0];
	

//If count is NOT larger than 2//
			if(count[stack3[0]-'0']==num[2]){

			for(int i =0; i<strlen(stack3);i++){
				stack3[i]=stack3[i+1];
			}
				if(queue_num>3){
					strncat(stack4,&tmp,1);
				}else{
					stack3[strlen(stack3)]=tmp;
				}

//If Count is larger than 2//
			}else{
				if(strlen(stack3)>1){
					for(int i =0; i<strlen(stack3);i++){
						stack3[i]=stack3[i+1];
					}
					stack3[strlen(stack3)]=tmp;
				}

			}

			}else{
//WHEN FINISH PROCESS//
			outprint(time,time+proc[stack3[0]-'0'].execution_time,proc[stack3[0]-'0'].process_id,proc[stack3[0]-'0'].arrival_time,0);
			time+=proc[stack3[0]-'0'].execution_time;
			proc[stack3[0]-'0'].execution_time=0;
			char tmp=stack3[0];
			strncat(finish,&tmp,1);
			for(int a=0;a<strlen(stack3);a++){
				stack3[a]=stack3[a+1];
				}
			}	
		}else{
			//CHECK if trigger finish//
			proc[stack3[0]-'0'].execution_time=proc[stack3[0]-'0'].execution_time -  (period-time);
	   		outprint(time,period,proc[stack3[0]-'0'].process_id,proc[stack3[0]-'0'].arrival_time,proc[stack3[0]-'0'].execution_time);
			time=period;
		}


	}else if(strlen(stack4)!=0){
//IF STACK IS NOT FINISH//


		if(period>time+ProcessQueue[queue_num-4]->time_slice){

			if( ProcessQueue[queue_num-4]->time_slice < proc[stack4[0]-'0'].execution_time){
				proc[stack4[0]-'0'].execution_time=proc[stack4[0]-'0'].execution_time - ProcessQueue[queue_num-4]->time_slice;
	   			outprint(time,time+ProcessQueue[queue_num-4]->time_slice,proc[stack4[0]-'0'].process_id,proc[stack4[0]-'0'].arrival_time,proc[stack4[0]-'0'].execution_time);
				time+=ProcessQueue[queue_num-4]->time_slice;

				count[stack4[0]-'0']++;
				char tmp=stack4[0];
	

//If count is  larger than 2//
			if(count[stack4[0]-'0']==num[4]){

			for(int i =0; i<strlen(stack4);i++){
				stack4[i]=stack4[i+1];
			}
				if(queue_num>4){
					strncat(stack5,&tmp,1);
				}else{
					stack4[strlen(stack4)-1]=tmp;
				}

//If Count is NOT larger than 2//
			}else{
				if(strlen(stack4)>1){
					for(int i =0; i<strlen(stack4);i++){
						stack4[i]=stack4[i+1];
					}
					stack4[strlen(stack4)]=tmp;
				}

			}

		}else{
//WHEN FINISH PROCESS//
				outprint(time,time+proc[stack4[0]-'0'].execution_time,proc[stack4[0]-'0'].process_id,proc[stack4[0]-'0'].arrival_time,0);
				time+=proc[stack4[0]-'0'].execution_time;
				proc[stack4[0]-'0'].execution_time=0;
				char tmp=stack4[0];
				strncat(finish,&tmp,1);
				for(int a=0;a<strlen(stack4);a++){
					stack4[a]=stack4[a+1];
				}
			}
	}else{
		//CHECK if trigger finish//

			proc[stack4[0]-'0'].execution_time=proc[stack4[0]-'0'].execution_time - (period-time);
	   		outprint(time,period,proc[stack4[0]-'0'].process_id,proc[stack4[0]-'0'].arrival_time,proc[stack4[0]-'0'].execution_time);
			time==period;



			time=period;


	}		
}else if(strlen(stack5)!=0){

//IF STACK IS NOT FINISH//
		if(period>time+ProcessQueue[queue_num-5]->time_slice){
			if( ProcessQueue[queue_num-5]->time_slice < proc[stack5[0]-'0'].execution_time){
				proc[stack5[0]-'0'].execution_time=proc[stack5[0]-'0'].execution_time - ProcessQueue[queue_num-5]->time_slice;
	   			outprint(time,time+ProcessQueue[queue_num-5]->time_slice,proc[stack5[0]-'0'].process_id,proc[stack5[0]-'0'].arrival_time,proc[stack5[0]-'0'].execution_time);
				time+=ProcessQueue[queue_num-5]->time_slice;

				count[stack5[0]-'0']++;
				char tmp=stack5[0];
	

//If count is NOT larger than 2//
			if(count[stack5[0]-'0']==num[5]){

			for(int i =0; i<strlen(stack5);i++){
				stack5[i]=stack5[i+1];
			}
				if(queue_num>5){
					strncat(stack6,&tmp,1);
				}else{
					stack5[strlen(stack5)-1]=tmp;
				}

//If Count is larger than 2//
			}else{
				if(strlen(stack5)>1){
					for(int i =0; i<strlen(stack5);i++){
						stack5[i]=stack5[i+1];
					}
					stack5[strlen(stack5)]=tmp;
				}

			}

		}else{
//WHEN FINISH PROCESS//
				outprint(time,time+proc[stack5[0]-'0'].execution_time,proc[stack5[0]-'0'].process_id,proc[stack5[0]-'0'].arrival_time,0);
				time+=proc[stack5[0]-'0'].execution_time;
				proc[stack5[0]-'0'].execution_time=0;
				char tmp=stack5[0];
				strncat(finish,&tmp,1);
				for(int a=0;a<strlen(stack5);a++){
					stack5[a]=stack5[a+1];
				}
			}
	}else{
		//CHECK if trigger finish//
			proc[stack5[0]-'0'].execution_time=proc[stack5[0]-'0'].execution_time -  (period-time);
	   		outprint(time,period,proc[stack5[0]-'0'].process_id,proc[stack5[0]-'0'].arrival_time,proc[stack5[0]-'0'].execution_time);
			time=period;
	}		
}

//IF STACK ARE EMPTY//
	if(strlen(stack1)==0&&strlen(stack2)==0&&strlen(stack3)==0&&strlen(stack4)==0&&strlen(stack5)==0){

		time+=1;
	}

	
if(time==period){

	strcpy(stack1, "");
	strcpy(stack2, "");
	strcpy(stack3, "");
	strcpy(stack4, "");
	strcpy(stack5, "");
	
	strcpy(stack4, "");
	period+=period;


	for(int i=0;i<proc_num;i++){
		count[i]=0;}
		

}
	
//ALL PROCESS IS FINISHED
if(strlen(finish)==proc_num)
escape=0;

}

}

