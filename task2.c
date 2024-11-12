/* Logan Kelsch - Lab 8 - Task 2
 *
 * three threads share an integer array and have different jobs.
 * thread 1-- get test scores from keyboard <=20 save into array
 * thread 2-- calculate average and median value and display
 * thread 3-- calculate maximum and minimum	 and display
 * thread 4-- clear buffer and set to 0, display after thread 2,
 * 					thread 3 finishes its job
 *
 * thread 2-3 must wait for 1 to finish its job
 * thread 4 must wait for 2-3 to finish their job
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 3

int *arr;
int length;

typedef struct {
	int arg_c;
	char *arg_v[20];
} CLdata;

//error output function
void err_sys(char *str){
	printf("%s",str);
	exit(1);
}

//first thread function
void *t1_func(void *arg){
	CLdata *data = (CLdata *)arg;
	int scores[data->arg_c];
	arr = (int*)malloc(data->arg_c*sizeof(int));
	for(int i = 0; i < data->arg_c; i++)
		arr[i] = atoi(data->arg_v[i]);
	printf("END OF THREAD 1 REACHED. %d\n", arr[0]);
	pthread_exit(NULL);
}

//second thread function
void *t2_func(void *sum){
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	//command line validation
	if(argc<2)
		err_sys("COMMAND LINE ERROR\n");
	
	//turning command line data into a struct-mobile
	CLdata cl;
	cl.arg_c = argc-1;
	for(int i = 1; i<argc; i++)
		cl.arg_v[i-1] = argv[i];

	//thread declaration
	pthread_t t1, t2, t3, t4;

	pthread_create(&t1, NULL, t1_func, (void *)&cl);
	pthread_join(t1, NULL);
	
	pthread_create(&t2, NULL, t2_func, NULL);
	pthread_join(t2, NULL);
	
	printf("MAIN COMPLETE. PROGRAM TERMINATING..\n");

	pthread_exit(NULL);
	exit(0);
}
