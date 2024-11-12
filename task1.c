/* Logan Kelsch - Lab 8 - Task 1
 *
 * program accepts int arg and calculates sum up and factorial.
 * instead of calling two functions, two threads will work on their own functions.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int *nums;
	int size;
} Data;

//error output function
void err_sys(char *str, int num){
	printf("%s %d\n",str, num);
	exit(1);
}

//first thread function
void *t1_sum(void *arg){
	Data *data = (Data*)arg;
	int tmp = 0;
	for(int i = 0; i < data->size; i++)
		tmp+=data->nums[i];
	int *sum = malloc(sizeof(int));
	*sum = tmp;
	printf("Sum from Thread:\t%d\n",*sum);
	pthread_exit(sum);
}

//second thread function
void *t2_fact(void *sum){
	int fact = *((int *) sum);
	int tmp = 1;
	for(int i = fact; i > 0; i--)
		tmp*=i;
	int *factorial = malloc(sizeof(int));
	*factorial = tmp;
	printf("Factorial from Thread:\t%d\n",*factorial);
	pthread_exit(factorial);
}

int main(int argc, char *argv[]){
	//command line validation
	if(argc<2)
		err_sys("COMMAND LINE ERROR\n", 0);

	//thread declaration
	pthread_t t1, t2;

	//prep command line numbers for use
	int nums[argc-1];
	for(int i = 0; i < argc-1; i++){
		nums[i] = atoi(argv[i+1]);
	}
	Data data;
	data.nums = nums;
	data.size = argc-1;
	//variablue used for join
	int *passedValue = malloc(sizeof(int));
	*passedValue = 0;
	
	int rc;

	rc = pthread_create(&t1, NULL, t1_sum, (void *)&data);
	if(rc)
		err_sys("ERROR; Return code from pthread_create() is %d\n",rc);
	pthread_join(t1, (void **)&passedValue);
	printf("Sum from Main:\t\t%d\n",*passedValue);
	
	rc = pthread_create(&t2, NULL, t2_fact, (void *)passedValue);
	if (rc)
		err_sys("ERROR; Return code from pthread_create() is %d\n",rc);
	pthread_join(t2, (void **)&passedValue);
	printf("Factorial from Main:\t%d\n",*passedValue);
	
	pthread_exit(NULL);
	exit(0);
}
