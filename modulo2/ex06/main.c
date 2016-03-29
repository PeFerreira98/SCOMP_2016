#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 100
#define NUM_ELEM 10
#define NUM_PROC 5

int main(void){
	
	pid_t p[NUM_PROC];
	int matrix[NUM_PROC][2];
	int fd[2];
	int vec1[NUM_ELEM], vec2[NUM_ELEM], i, j, aux, tot=0, estado;
	
	for(i = 0; i < NUM_ELEM; i++){//Preencher vectores
		vec1[i] = 1;
		vec2[i] = 1;
	}
	for(i = 0; i < NUM_PROC; i++){
		matrix+i = fd;
	}
	for(i = 0; i < NUM_PROC; i++){
		pipe(*(matrix+i));
	}
	
	for(i = 0; i < NUM_PROC; i++){
		
		p[i] = fork();
		if(p[i] == 0){
			int total;
			close(fd[0]);
			for(j = ((NUM_ELEM/NUM_PROC)*i); j < ((NUM_ELEM/NUM_PROC)*i +(NUM_ELEM/NUM_PROC)); j++){
				total += vec1[j] + vec2[j];
			}
			write(matrix+i+1, &total, sizeof(int));
			close(matrix+i+1);
			exit(0);				
		}
		read(matrix+i,&aux,sizeof(int));		
		tot = tot + aux;
	}
	
	for(i = 0; i < NUM_PROC; i++){
		waitpid(p[i], &estado, 0);
	}
	
	printf("%d:\n", tot);
	
	return 0;
}
