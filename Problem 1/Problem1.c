#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define MAXRAND 100
#define MAXREADER 30
#define MAXBUFFERSIZE 20

pthread_t tidR[MAXREADER],tidW;

int buffersize,loopvalue = 10,rnum;	//rnum is number of readers

int buffer[MAXBUFFERSIZE];

int allread[MAXBUFFERSIZE];
sem_t readsem[MAXREADER][MAXBUFFERSIZE],buffersem[MAXBUFFERSIZE];                        
//Each thread has its own read semaphore for each index
//Each index of buffer has its own semaphore
sem_t mutex;	//Used as lock

int writep = 0;
int readp[MAXREADER];	//To be initialized
//Each reader has its own pointer

void write(int value) {
	buffer[writep] = value;
 	writep = (writep + 1) % buffersize;
}

int read(int rid) {
	int temp = buffer[readp[rid]];
	readp[rid] = (readp[rid] + 1) % buffersize;
	int tmpindex = ((readp[rid] - 1 + buffersize) % buffersize);
	(allread[tmpindex])--;
	if(allread[tmpindex] == 0){
		allread[tmpindex] = rnum;
		sem_post(&buffersem[tmpindex]);
	}
	return temp;
 }

void *writer(void *arg) {
	int writerid = *((int *) arg);
	int i,j;
	for(i=0; i<loopvalue; i++){
		sem_wait(&buffersem[writep]);
		sem_wait(&mutex);
		int temp = rand()%MAXRAND;
		printf("Writer %d has written %d\n",writerid,temp);
		write(temp);
		sem_post(&mutex);
		for(j=0; j<rnum; j++){
			sem_post(&readsem[j][((writep - 1 + buffersize) % buffersize)]);
		}
	}
}

void *reader(void *arg) {
	int readerid = *((int *) arg);
	int i,j;
	for(i=0; i<loopvalue; i++){
		sem_wait(&readsem[readerid][readp[readerid]]);
		sem_wait(&mutex);
		int temp = read(readerid);
		printf("Reader %d has read %d\n",readerid,temp);
		sem_post(&mutex);
	}
}

int main(int argc, char *argv[]) {

	int i,j;
	srand(time(NULL));

	printf("Enter buffer size\n");
	scanf("%d",&buffersize);

	printf("Enter number of readers\n");
	scanf("%d",&rnum);

	//Initializing reader pointers
	for(i=0; i<rnum; i++)
		readp[i] = 0;

	for(i=0; i<buffersize; i++)
		allread[i] = rnum;

	//Initialize the semaphores
	for(i=0; i<rnum; i++){
		for(j=0; j<buffersize; j++){
			sem_init(&readsem[i][j], 0, 0);
		}
	}

	for(i=0; i<buffersize; i++)
		sem_init(&buffersem[i],0,1);
	
	sem_init(&mutex, 0, 1);

	int *arg = malloc(sizeof(*arg));
    if ( arg == NULL ) {
        fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
        exit(EXIT_FAILURE);
    }
    *arg = 0;
	pthread_create(&tidW,NULL,writer,arg);
	
	for(i=0;i<rnum;i++){
		int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }
        *arg = i;

		pthread_create(&tidR[i],NULL,reader,arg);
	}

	
	pthread_join(tidW,NULL);

	for(i=0;i<rnum;i++)
		pthread_join(tidR[i],NULL);

	//Destroy semaphores
	for(i=0; i<rnum; i++){
		for(j=0; j<buffersize; j++){
			sem_destroy(&readsem[i][j]);
		}
	}

	for(i=0; i<buffersize; i++)
		sem_destroy(&buffersem[i]);

	sem_destroy(&mutex);

	return 0;
}

