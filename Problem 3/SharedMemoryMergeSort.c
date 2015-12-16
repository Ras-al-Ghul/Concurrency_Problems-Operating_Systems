#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


void printarray(int *array, int len){
	int i;
	for(i=0;i < len;i++)
		printf("%d\t",array[i]);
	printf("\n");
}

void selectionsort(int array[], int len){
	int i,j,min,temp;
	for(i = 0; i < len-1; i++){
		min = i;
		for(j = i+1; j < len; j++){
			if(array[j] < array[min])
				min = j;
		}
		temp = array[i];
		array[i] = array[min];
		array[min] = temp;
	}
}

void merge(int *l, int llen, int *r, int rlen){
	
	int larray[llen],rarray[rlen];
	
	int i;
	int *templ = l,*tempr = r;
	for(i = 0; i < llen; i++){
		larray[i] = *templ;
		templ++;
	}
	
	for(i = 0; i < rlen; i++){
		rarray[i] = *tempr;
		tempr++;
	}

	int j = 0,*k = l;
	i = 0;
	while(i < llen && j < rlen){
		if(larray[i] < rarray[j])
			*k = larray[i++];
		else
			*k = rarray[j++];
		k++;
	}
	while(i < llen){
		*k = larray[i++];
		k++;
	}
	while(j < rlen){
		*k = rarray[j++];
		k++;
	}

}

void mergesort(int share_id, int *array, int len){
	
	int status;

	if(len < 5){
		selectionsort(array, len);
		return;
	}

	int mid = len/2;

	int *l,*r ;
	int lpid,rpid;
	
	l = array;
	r = array + mid;

	lpid = fork();
	if(lpid < 0){
		perror("Unable to fork");
		exit(1);
	}
	else if(lpid == 0){
		mergesort(share_id, l, mid);
		exit(0);
	}
	else{
		//printf("pid of left is %d\n",lpid);
		rpid = fork();

		if(rpid < 0){
			perror("Unable to fork");
			exit(1);
		}
		else if(rpid == 0){
			mergesort(share_id, r, len-mid);
			exit(0);
		}
		else{
			//printf("pid of right is %d\n",rpid);
		}
		
	}

	waitpid(lpid, &status, 0);
	waitpid(rpid, &status, 0);	
	merge(l, mid, r, len-mid);
	
	return;
}

int main(int argc, char *argv[]){

	int *sharedarray;
	int len,count,data;

	FILE *fp;

	if(argc != 2){
		printf("Enter file to be sorted\n");
		return 1;
	}

	count = 0;
	fp = fopen(argv[1],"r");
	while(fscanf(fp, "%d", &data) != EOF){
		count++;
	}
	fclose(fp);

	len = count;

	int sharememsize = len * sizeof(int);
	int share_id;

	if((share_id = shmget(IPC_PRIVATE, sharememsize, IPC_CREAT | 0666)) == -1){
		perror("shmget error");
		exit(1);
	}

	if((sharedarray = shmat(share_id, NULL, SHM_RND)) == (int *) -1){
		perror("shmat error");
		exit(1);
	}

	fp = fopen(argv[1],"r");
	if (fp == NULL) {
		printf("error opening file\n");
		return 0;
	}

	count = 0;
	while(fscanf(fp, "%d", &data) != EOF){
		sharedarray[count++] = data;
	}
	fclose(fp);

	mergesort(share_id, sharedarray, len);

	//printarray(sharedarray,len);

	if (shmdt(sharedarray) == -1) {
		perror("shmdt error");
		exit(1);
	}

	if (shmctl(share_id, IPC_RMID, NULL) == -1) {
		perror("shmctl error");
		exit(1);
	}

	return 0;
}