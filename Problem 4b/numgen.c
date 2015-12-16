#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 4){
		printf("Enter filename, number of integers, max value");
		exit(1);
	}

	srand(time(NULL));

	FILE *fp = fopen(argv[1],"w");

	int i;
	for(i = 0; i < atoi(argv[2]); i++){
		fprintf(fp,"%d\n",rand()%atoi(argv[3]));
	}

	return 0;
}