/* Logan Kelsch - Lab 8 - Task 3
 *
 * create two threads that run in different part of program
 * original program runs forever printing 'in the original thread'
 * first thread terminates original thread
 * original thread needs prepare a clean up handler function Bye1()
 * 		which will display "END OF PROGRAM" when terminated
 *
 * 888 NO SIGNALS NO GLOBAL VARIABLES 888
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//error output function
void err_sys(char *str){
	printf("%s",str);
	exit(1);
}

void Bye(void *arg){
	pthread_t *tid_orig = (pthread_t*)arg;
	sleep(1);
	printf("BYE!\n");
	pthread_cancel(*tid_orig);
	pthread_exit(NULL);
}

void Bye1(){
	sleep(1);
	printf("END OF PROGRAM\n");
	pthread_exit(NULL);
}

void *thread1(void *arg){
	pthread_t *tid_orig = (pthread_t*)arg;
	pthread_cleanup_push(Bye, tid_orig);
	while(1){
		printf("In the first thread\n");
		sleep(1);
		pthread_testcancel();
	}
	pthread_cleanup_pop(1);
	pthread_exit(NULL);
}

void *thread2(void *arg){
	pthread_t *tid_1 = (pthread_t*)arg;
	int iter = 0;
	while (1){
		if(iter==10){
			pthread_cancel(*tid_1);
			break;
		}
		printf("In the second thread\n");
		sleep(1);
		iter++;
	}
	pthread_exit(NULL);
}


int main(){
	pthread_t self = pthread_self();
	pthread_t t1, t2;
	int rc;
	pthread_create(&t1, NULL, thread1, &self);
	pthread_create(&t2, NULL, thread2, &t1);

	pthread_cleanup_push(Bye1, NULL);
	while(1){
		printf("In the original thread\n");
		sleep(1);
		pthread_testcancel();
	}
	printf("threadO - loop exited\n");
	pthread_cleanup_pop(1);
	printf("threadO - post pop\n");

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);


	pthread_exit(NULL);
	exit(0);
}
