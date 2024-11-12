/* Logan Kelsch - Lab 8 - Task 1
 *
 * program accepts int arg and calculates sum up and factorial.
 * instead of calling two functions, two threads will work on their own functions.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//error output function
void err_sys(char *str){
	printf("%s",str);
	exit(1);
}

//function return sum of all inputted number char strings
void *sum(void *vals){
	int *nums = (int*)vals;
	int num_count = argc-1;
	printf("Sizeof(nums): %i\n",num_count);
	int sum = 0;
	//for all number arguments from command line
	for(int i = 0; i < num_count; i++){
		printf("nums[%i]: %i\n",i,nums[i]);
		sum+=nums[i];	
	}
	printf("Sum of values: %i\n",sum);
	pthread_exit(NULL);
}

//function returns factorial value of sum of inputted number char strings
void *factorial(void *vals){
	int *nums = (int*)vals;
	int num_count = sizeof(nums);
	int factorial = 0;
	//for all number arguments from command line
	for(int i = 0; i < num_count; i++){
	
	}
	printf("Factorial of sum value: %i\n",factorial);
	pthread_exit(NULL);
}

//main function described in lines [3-4]
int main(int argc, char *argv[]){
	//validation check that values were entered
	if(argc<2)
		err_sys("COMMAND LINE ERROR");
	pthread_t thr_sum;
	pthread_t thr_fac;
	int rc;
	printf("argc: %i\n",argc);
	int nums[(argc-1)];
	
	//for loop for changing all command line arguments 
	//to integers with less computational time
	for(int i = 1; i < argc; i++){
		int off = i-1;
		nums[off] = atoi(argv[i]);
		printf("atoi[%i]: %i\n",i-1,nums[i-1]);
	}

	rc = pthread_create(&thr_sum, NULL, sum, (void *)nums);

	rc = pthread_create(&thr_fac, NULL, factorial, (void *)nums);

	pthread_exit(NULL);
	exit(0);
}
