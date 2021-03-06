#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 10
#define NUM_ELEM 10000
#define NUM_NR 50

int chaves[NUM_ELEM];
int contador[NUM_NR];
pthread_mutex_t mutex[NUM_NR];

//-------------------------------------------------------------------------

void error_message(char* a){
	printf("Error >> %s\n", a);
}

void init_array_chaves(){
	for (int i = 0; i < NUM_ELEM; ++i)
		chaves[i] = 1 + rand() % 50;
}

void init_array_contador(){
	for (int i = 0; i < NUM_NR; ++i)
		contador[i] = 0;
}

void init_array_mutex(){
	for (int i = 0; i < NUM_NR; ++i)
		if (pthread_mutex_init(&mutex[i], NULL) != 0) error_message("pthread_mutex_init");
}

void delete_array_mutex(){
	for (int i = 0; i < NUM_NR; ++i)
		if (pthread_mutex_destroy(&mutex[i]) != 0) error_message("pthread_mutex_destroy");
}

//-------------------------------------------------------------------------

void *thread_func(void *arg){
	printf("Thread %lu Executing...\n", pthread_self());
	int value = *((int*) arg);

	int amp = NUM_ELEM/NUM_THREADS;

	int start = value * amp;
	int end = start + amp;
	
	printf("\tstart >> %d , end >> %d\n", start, end);

	for (int i = start; i < end; ++i)
	{
		int valor = chaves[i];
		valor--;

		if (pthread_mutex_lock(&mutex[valor]) != 0) error_message("pthread_mutex_lock");
        contador[valor]++;
        if (pthread_mutex_unlock(&mutex[valor]) != 0) error_message("pthread_mutex_unlock");
		
	}

	pthread_exit((void*)NULL);
}

//-------------------------------------------------------------------------

int main(){
	pthread_t thread_id[NUM_THREADS];
	int args[NUM_THREADS];
	int i, total = 0;


	//Initialization Arrays
	init_array_chaves();
	init_array_contador();
	init_array_mutex();


	//Thread Creation
	for (i = 0; i < NUM_THREADS; i++){
		args[i] = i;
		if (pthread_create(&thread_id[i], NULL, thread_func, &args[i]) != 0) error_message("pthread_create");
	}


	//Thread Join
	for (i = 0; i < NUM_THREADS; i++)
		if (pthread_join(thread_id[i], NULL) != 0) error_message("pthread_join");
		


	//Delete Mutex
	delete_array_mutex();


	//Print Contador
	for (int i = 0; i < NUM_NR; ++i){
		printf("%d >> %d\n", i+1, contador[i]);
		total += contador[i];
	}

	printf("Nr de valores contados >> %d\n", total);
	
	return 0;
}
