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

int *arr;
int length;

typedef struct {
	int arg_c;
	char *arg_v[20];
} CLdata;

//error output function
void err_sys(char *str, int num){
	printf("%s %d\n",str, num);
	exit(1);
}

void arrPrint(){
	for(int i = 0; i < length; i++){
		printf("arr[%d]==%d\n",i,arr[i]);
	}
}

void arrSort(){
	int min = 0;
	int swpTmp = 0;
	for(int i = 0; i < length; i++){
		min = i;
		for(int j = i+1; j < length; j++){
			if(arr[j]<arr[min])
				min=j;
		}
		swpTmp = arr[i];
		arr[i] = arr[min];
		arr[min] = swpTmp;
	}
}

//first thread function\
//moves all collected scores into global array
void *t1_func(void *arg){
	CLdata *data = (CLdata *)arg;
	arr = (int*)malloc(data->arg_c*sizeof(int));
	for(int i = 0; i < data->arg_c; i++)
		arr[i] = atoi(data->arg_v[i]);
	length = data->arg_c;
	printf("Score data gathered.\n");
	arrPrint();
	arrSort();
	printf("Score data sorted.\n");
	arrPrint();
	pthread_exit(NULL);
}

//second thread function
//calculates average and median score
void *t2_func(void *null){
	int average = 0;
	int median = 0;
	//one line median calculation
	median = (arr[(length-1)/2] + arr[length/2]) / 2;
	//find and grab average value
	for(int i = 0; i < length; i++)
		average+=arr[i];
	average/=length;
	printf("Average of Scores: %d\n",average);
	printf("Median of Scores: %d\n",median);
	pthread_exit(NULL);
}

//third thread function
//calculates min and max score
void *t3_func(void *null){
	int min, max;
	min = arr[0];
	max = arr[0];
	//find and grab min and max values
	for(int i = 1; i < length; i++){
		if(arr[i]<min)
			min=arr[i];
		if(arr[i]>max)
			max=arr[i];
	}
	printf("Maximum of Scores: %d\n",max);
	printf("Minimum of Scores: %d\n",min);
	pthread_exit(NULL);
}

//fourth thread function
//clear array and call thread 2,3 functions??????
void *t4_func(void *null){
	for(int i = 0; i < length; i++)
		arr[i] = 0;
	printf("Buffer has been cleared.\n");

	pthread_t t2_2, t3_2;

	int rc;

	rc = pthread_create(&t2_2, NULL, t2_func, NULL);
	if (rc)
		err_sys("ERROR; return code for pthread_create() is %d\n",rc);
	rc = pthread_create(&t3_2, NULL, t3_func, NULL);
	if (rc)
		err_sys("ERROR; return code for pthread_create() is %d\n",rc);
	pthread_join(t2_2, NULL);
	pthread_join(t3_2, NULL);

	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	//command line validation
	if(argc<2)
		err_sys("COMMAND LINE ERROR\n",0);
	
	//turning command line data into a struct-mobile
	CLdata cl;
	cl.arg_c = argc-1;
	for(int i = 1; i<argc; i++)
		cl.arg_v[i-1] = argv[i];

	//thread declaration
	pthread_t t1, t2, t3, t4;
	
	//thread 1 actions
	pthread_create(&t1, NULL, t1_func, (void *)&cl);	
	
	pthread_join(t1, NULL);
	
	//thread 2-3 actions
	pthread_create(&t2, NULL, t2_func, NULL);
	pthread_create(&t3, NULL, t3_func, NULL);

	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	
	//thread 4 actions
	pthread_create(&t4, NULL, t4_func, NULL);

	pthread_join(t4, NULL);
	
	//main program complete, terminate
	pthread_exit(NULL);
	exit(0);
}
