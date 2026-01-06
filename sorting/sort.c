#include<stdio.h>
#include<stdlib.h>

#include"sort.h"


void print_array(TaggedInt *a, int n){
	for (int i=0;i<n; i++){
		printf("[%d, %d], ",a[i].key,a[i].id);
	}
	printf("\n");
}




void insertion_sort(TaggedInt *a, int n){
	int j;
	for (int i=1;i<n;i++){
		//print_array(a,n);
		TaggedInt value=a[i];
		for(j=i;(j>0)&&(a[j-1].key>value.key);j--){
			//printf("%d, ",j);
			a[j]=a[j-1];
		}
		a[j]=value;
		//printf("   i=%d\n",i);
	}
}



void selection_sort(TaggedInt *a, int n){
	for (int i =0; i<n; i++){
		TaggedInt min=a[i];
		int min_idx=i;
		for (int j=i; j<n; j++){
			if(a[j].key<min.key){
				min=a[j];
				min_idx=j;
			}
		}
		a[min_idx]=a[i];
		a[i]=min;
	}
}



void merge_sort(TaggedInt *a, int n){
	if (n<=1) return;
	TaggedInt *src=a;
	TaggedInt *tmp=malloc(n*sizeof(TaggedInt));
	if (tmp==NULL){
		fprintf(stderr,"Error: malloc failure\n");
		return;
	}
	int len=1;
	while(len<n){

		int k=0;
		while(k+len<n){
			int i=0;
			int j=0;
			int len_i=len;
			int len_j=len;
		 	if (n-k<2*len){
				len_j=(n-k)-len_i;
			}		
			while((i<len_i)&&(j<len_j)){
				if (a[k+i].key>a[k+len_i+j].key){
					tmp[k+i+j]=a[k+len_i+j];
					j++;
				}
				else {
					tmp[k+i+j]=a[k+i];
					i++;
				}
			}
			while (i<len_i){
				tmp[k+i+j]=a[k+i];
				i++;
			}
			while (j<len_j){
				tmp[k+i+j]=a[k+len_i+j];
				j++;
			}
			k=k+len_i+len_j;
		}
		while(k<n){
			tmp[k]=a[k];
			k++;
		}
//		print_array(tmp,n);		
		TaggedInt *swap=tmp;
		tmp=a;
		a=swap;		
		len*=2;
	}
	if (src==tmp){
		for (int i=0;i<n;i++){
			src[i]=a[i];
		}
		free(a);
	}
	else	free(tmp);
}















			
