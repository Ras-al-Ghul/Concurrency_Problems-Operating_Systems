#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define MAXLANCS 50
#define MAXYORKS 50
#define MAXRAND 3

pthread_t tidL[MAXLANCS],tidY[MAXYORKS];

int lancs,yorks,inncap;
int lcount = 0,ycount = 0;

sem_t mutex1,mutex2,mutex3,mutex4,fullmutex,innmutex;

void *lancaster(void *arg) {
	int lancsid = *((int *) arg);

	sem_wait(&mutex3);

		sem_wait(&mutex1);
	
			lcount++;
	
			if(lcount == 1)
				sem_wait(&innmutex);
	

		sem_post(&mutex1);

		sem_wait(&fullmutex);

		printf("Lancaster %d inside inn\n",lancsid);
		printf("Number of Lancasters inside inn is %d\n",lcount);

	sem_post(&mutex3);

	sleep(rand() % MAXRAND);

	sem_wait(&mutex1);

		if(lcount == 1)
			sem_post(&innmutex);
	
		lcount--;

		printf("Lancaster %d left inn\n",lancsid);

	sem_post(&mutex1);

	sem_post(&fullmutex);

}

void *york(void *arg) {
	int yorksid = *((int *) arg);

	sem_wait(&mutex4);

		sem_wait(&mutex2);
		
			ycount++;
			
			if(ycount == 1)
				sem_wait(&innmutex);
			
		sem_post(&mutex2);

		sem_wait(&fullmutex);

		printf("York %d inside inn\n",yorksid);
		printf("Number of Yorks inside inn is %d\n",ycount);

	sem_post(&mutex4);

	sleep(rand() % MAXRAND);

	sem_wait(&mutex2);

		if(ycount == 1)
			sem_post(&innmutex);

		ycount--;
		printf("York %d left inn\n",yorksid);

	sem_post(&mutex2);

	sem_post(&fullmutex);
}

int main(int argc, char *argv[]){

	int i,j,toss;
	srand(time(NULL));

	int lancs,yorks;

	printf("Enter number of Lancasters\n");
	scanf("%d",&lancs);
	printf("Enter number of Yorks\n");
	scanf("%d",&yorks);
	printf("Enter inn capacity\n");
	scanf("%d",&inncap);

	sem_init(&mutex1, 0, 1);
	sem_init(&mutex2, 0, 1);
	sem_init(&mutex3, 0, 1);
	sem_init(&mutex4, 0, 1);
	sem_init(&fullmutex, 0, inncap);
	sem_init(&innmutex, 0, 1);

	for(i=0,j=0;i<lancs&&j<yorks;){
		int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }

    	sleep(rand() % MAXRAND);

    	toss = rand()%2;

    	if(toss == 0){
    		*arg = i;
    		printf("Lancaster %d created\n",i);
			pthread_create(&tidL[i],NULL,lancaster,arg);
			fflush(stdout);
			i++;
		}
		if(toss == 1){
			*arg = j;
			printf("York %d created\n",j);
			pthread_create(&tidY[j],NULL,york,arg);
			fflush(stdout);
			j++;
		}
	}

	for(;i<lancs;i++){
		int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }
        *arg = i;

    	sleep(rand() % MAXRAND);

    	printf("Lancaster %d created\n",i);
		pthread_create(&tidL[i],NULL,lancaster,arg);
		fflush(stdout);
	}

	for(;j<yorks;j++){
		int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }
        *arg = j;

    	sleep(rand() % MAXRAND);

    	printf("York %d created\n",j);
		pthread_create(&tidY[j],NULL,york,arg);
		fflush(stdout);
	}

	for(i=0;i<lancs;i++)
		pthread_join(tidL[i],NULL);

	for(i=0;i<yorks;i++)
		pthread_join(tidY[i],NULL);

	sem_destroy(&mutex1);
	sem_destroy(&mutex2);
	sem_destroy(&mutex3);
	sem_destroy(&mutex4);
	sem_destroy(&fullmutex);
	sem_destroy(&innmutex);

	return 0;
}