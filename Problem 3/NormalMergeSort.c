 #include<stdio.h>
#define MAX 1000000

void mergeSort(int arr[],int low,int mid,int high);
void partition(int arr[],int low,int high);

int main(int argc, char *argv[]){
   
    int merge[MAX],i,n,data;

    FILE *fp;

	if(argc != 2){
		printf("Enter file to be sorted\n");
		return 1;
	}

	i = 0;
	fp = fopen(argv[1],"r");
	while(fscanf(fp, "%d ", &data) != EOF){
		i++;
	}
	fclose(fp);

	n=i;

	fp = fopen(argv[1],"r");
	i = 0;
	while(fscanf(fp, "%d", &data) != EOF){
		merge[i++] = data;
	}
	fclose(fp);

	/*
	printf("Before merge sorting elements are: ");
    for(i=0;i<n;i++){
         printf("%d ",merge[i]);
    }
	*/
    partition(merge,0,n-1);

    /*
    printf("After merge sorting elements are: ");
    for(i=0;i<n;i++){
         printf("%d ",merge[i]);
    }
	*/
   return 0;
}

void partition(int arr[],int low,int high){

    int mid;

    if(low<high){
         mid=(low+high)/2;
         partition(arr,low,mid);
         partition(arr,mid+1,high);
         mergeSort(arr,low,mid,high);
    }
}

void mergeSort(int arr[],int low,int mid,int high){

    int i,m,k,l,temp[MAX];

    l=low;
    i=low;
    m=mid+1;

    while((l<=mid)&&(m<=high)){

         if(arr[l]<=arr[m]){
             temp[i]=arr[l];
             l++;
         }
         else{
             temp[i]=arr[m];
             m++;
         }
         i++;
    }

    if(l>mid){
         for(k=m;k<=high;k++){
             temp[i]=arr[k];
             i++;
         }
    }
    else{
         for(k=l;k<=mid;k++){
             temp[i]=arr[k];
             i++;
         }
    }
   
    for(k=low;k<=high;k++){
         arr[k]=temp[k];
    }
}