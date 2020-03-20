
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t readtime = PTHREAD_COND_INITIALIZER;
pthread_cond_t writetime = PTHREAD_COND_INITIALIZER;
int var = -1;
int readers = 0;
int writers = 0;

void *read (void *param){
	for(int i=0; i<5;i++){
		pthread_mutex_lock(&m);
		if(readers<0||writers<0){
			printf("number of readers or writers is <0");
			exit(1);
		}
		if(writers>0||var==-1){
			pthread_cond_wait(&readtime, &m);
		}
		readers = readers+1;
		printf("Value read by thread = %d numeber of reader = %d\n", var, readers);
		pthread_mutex_unlock(&m);
		pthread_cond_broadcast(&readtime);
		readers=readers-1;
		pthread_cond_signal(&writetime);
	}
}
void *write (void *param){
        for(int i=0; i<5;i++){
                pthread_mutex_lock(&m);
                if(readers<0||writers<0){
                        printf("number of readers or writers is <0");
                        exit(1);
                }
                if(writers>0||readers>0){
                        pthread_cond_wait(&writetime, &m);
                }
               writers = writers+1;
	       var = i;
                printf("Value written by thread = %d numeber of reader = %d\n", var, readers);
                writers = writers-1;
                pthread_mutex_unlock(&m);
		pthread_cond_broadcast(&readtime);
        	pthread_cond_signal(&writetime);
        }
}
int main(){
	pthread_t reader[5];
	pthread_t writer[5];
	for(int i = 0 ; i < 5 ; i++){
		if(pthread_create(&reader[i], NULL, read, NULL)!=0){
			printf("Error in creating reader\n");
			exit(1);
		}	
	}
	 for(int i = 0 ; i < 5 ; i++){
                if(pthread_create(&writer[i], NULL, write, NULL)!=0){
                        printf("Error in creating writer\n");
                        exit(1);
                }
        }
	  for(int i = 0 ; i < 5 ; i++){
                pthread_join(reader[i], NULL);
        }
	  for(int i = 0 ; i < 5 ; i++){
                pthread_join(writer[i], NULL);
        }
	  printf("Execution of code has ended\n");
}



