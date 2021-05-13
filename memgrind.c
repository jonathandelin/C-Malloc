#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mymalloc.h"
#include <time.h>
#include <sys/time.h>
#include <math.h>
void WorkloadA(){ //malloc 1 byte and immediately free it 120x
	int i; 
	for(i=0;i<120;i++){
			void* ptr = malloc(1);
			free(ptr);
		}
}

void WorkloadB(){ // malloc 1 byte and then free ptrs one by one
		int i;
		char* runtime[120];
		for(i=0;i<120;i++){
			runtime[i]=malloc(1);
		}
		for(i=0;i<120;i++){
			free(runtime[i]);
			runtime[i]=NULL;
		}
}

void WorkloadC(){
	
	int i;
	int temp;
	int mallocs=0;
	int currMalloc=0;
	int currFree=0;
	srand(time(NULL));
	char *runtime[240];


for(i=0; i<240; i++){
		//while(mallocs<120){
			temp = rand()%2;
			if(temp==1){
				if(runtime[i]){
					free(runtime[i]);
					runtime[i]==NULL;
					
				} else {	
					runtime[i]=malloc(1);
					
					mallocs++;
				}	
			}
			if(temp==0){
				if(!runtime[i]){
					runtime[i]=malloc(1);
					mallocs++;
				} else{
					free(runtime[i]);
					runtime[i]=NULL;
					
				}
				
			} 
			if(mallocs=120-1){
				break;
			}
			
		}		
	//}
	for(i=0;i<240;i++){
		if(!runtime[i]){
			continue;
		} else {
			free(runtime[i]);
			runtime[i]=NULL;
		}
		
	}

}

void WorkloadD(){
    int i;
    for(i=0;i<120;i++){
			//printf("\nWorkload D malloc\n");
			void* ptr = malloc(5000);
		}
    
}

void WorkloadE(){
    int i;
    char* validpointer[120];
    char* invalidpointer;
    for(i = 0; i < 120; i++){
        //printf("Workload E malloc\n");
        validpointer[i] = malloc(1);
    }
    
    for(i=0;i<120;i++){
		//printf("Workload E freeing VALIDpointer\n");
		free(validpointer[i]);
		validpointer[i]=NULL;
	}
    
    for(i=0;i<120;i++){
		//printf("Workload E freeing INvalidpointer\n");
		free(invalidpointer);
    }
    
    
    
}


int main (){
	int totaltime=0;
	int i=0;
	int arr[120];
	struct timeval startime, endtime;
	int l;

//run 50x
	for(i=0; i<50;i++){
		//workload A: malloc 1 byte and immediately free it 120x
		gettimeofday(&startime, NULL);
		WorkloadA();
		gettimeofday(&endtime, NULL);
		totaltime=totaltime+((endtime.tv_sec-startime.tv_sec)*1000000+(endtime.tv_usec-startime.tv_usec));
		arr[i]=totaltime;
		
	}

	int aRuntime=0;
	for(l=0;l<50;l++){
		aRuntime+=arr[l];
	}
    
    totaltime = 0;
	//workload B: malloc 1 byte and then free ptrs one by one
	for(i=0; i<50;i++){
		gettimeofday(&startime, NULL);
		WorkloadB();
		gettimeofday(&endtime, NULL);
		totaltime=totaltime+((endtime.tv_sec-startime.tv_sec)*1000000+(endtime.tv_usec-startime.tv_usec));
		arr[i]=totaltime;
	}

	int bRuntime=0;
	for(l=0;l<50;l++){
		bRuntime+=arr[l];
	}


	totaltime =0;
	//workload C: randomly choose between a 1 byte malloc() or free()ing one of the malloc()ed pointers
	for(i=0;i<50;i++){
		gettimeofday(&startime, NULL);
		WorkloadC();
		gettimeofday(&endtime, NULL);
		totaltime=totaltime+((endtime.tv_sec-startime.tv_sec)*1000000+(endtime.tv_usec-startime.tv_usec));
		arr[i]=totaltime;
	}

	int cRuntime=0;
	for(l=0;l<50;l++){
		cRuntime+=arr[l];
	}
	
	totaltime = 0;
	//workload D: will check if malloc() handles cases in which the user will try to allocate more memory than we can handle.
	for(i=0; i<50;i++){
		//
		gettimeofday(&startime, NULL);
		WorkloadD();
		gettimeofday(&endtime, NULL);
		totaltime=totaltime+((endtime.tv_sec-startime.tv_sec)*1000000+(endtime.tv_usec-startime.tv_usec));
		arr[i]=totaltime;
		
	}
	

	int dRuntime=0;
	for(l=0;l<50;l++){
		dRuntime+=arr[l];
	}
	
	
	totaltime = 0;
	//Workload E: will free a valid pointer and attempt to free non valid pointers each 120 times
	for(i=0; i<50;i++){
		//
		gettimeofday(&startime, NULL);
		WorkloadE();
		gettimeofday(&endtime, NULL);
		totaltime=totaltime+((endtime.tv_sec-startime.tv_sec)*1000000+(endtime.tv_usec-startime.tv_usec));
		arr[i]=totaltime;
		
	}
	
	int eRuntime=0;
	for(l=0;l<50;l++){
		eRuntime+=arr[l];
	}
	
	float avgA = ((float)aRuntime/10000)/50;
	float avgB = ((float)bRuntime/10000)/50;
	float avgC = ((float)cRuntime/10000)/50;
	float avgD = ((float)dRuntime/10000)/50;
	float avgE = ((float)eRuntime/10000)/50;
	
	printf("\nWorkload A average time is %f milliseconds\n", avgA);
	printf("Workload B average time is %f milliseconds\n", avgB);
	printf("Workload C average time is %f milliseconds\n", avgC);
	printf("Workload D average time is %f milliseconds\n", avgD);
	printf("Workload E average time is %f milliseconds\n", avgE);
	    
}
