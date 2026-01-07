#include<stdio.h>
#include<stdlib.h>

#include"sort.h"

//-------------------------helpers--------------------//


void swap(TaggedInt *a, int i, int j){
	TaggedInt tmp=a[i];
	a[i]=a[j];
	a[j]=tmp;
}



//-----------------------main functions--------------//


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

//it doesn't use lomuto partition. sould be improved by adding it.
void quick_sort(TaggedInt *a, int n){
	if (n<2) return;
	int pivot;
	int mid=(int)(n/2);
	if ((a[0].key<a[mid].key)!=(a[0].key<a[n-1].key)) pivot=0;
	else if ((a[0].key<=a[mid].key)!=(a[n-1].key<=a[mid].key)) pivot=mid;
	else pivot=n-1;		
	int p0=pivot;
	int val=a[pivot].key;
	for (int i=p0-1;i>=0;i--){
		if (a[i].key>val){
			TaggedInt tmp=a[pivot];
			a[pivot]=a[i];
			a[i]=a[pivot-1];
			a[pivot-1]=tmp;
			pivot--;
		}
	}
	for (int i=p0+1;i<n;i++){
		if (a[i].key<val){
			TaggedInt tmp=a[pivot];
			a[pivot]=a[i];
			a[i]=a[pivot+1];
			a[pivot+1]=tmp;
			pivot++;
		}
	}
	quick_sort(a,pivot);
	quick_sort(a+pivot+1,n-pivot-1);
}

//---------------------------heap sort---------------------------//

//doesn't check for i. it must be i<n/2 
void heapify(TaggedInt *a, int n, int i){
	int max_idx;
	if (((i*2+2)==n)||(a[i*2+1].key>a[i*2+2].key)){ 
		max_idx=i*2+1;
	}
	else {
		max_idx=i*2+2;
	}
	if (a[i].key<a[max_idx].key){
		swap(a,i,max_idx);
		if (max_idx<n/2){
			heapify(a,n,max_idx);
		}
	}
	
}

//doesn't check the correctness of the max_heap
void max_extraction(TaggedInt *a, int n){
	for (int i=n-1;i>1;i--){
		swap(a,0,i);
		heapify(a,i,0);		
	}
	swap(a,0,1);
}


void heap_sort(TaggedInt *a, int n){
	if (n<2) return;
	for(int i =n/2-1; i>=0;i--){
		heapify(a,n,i);
	} 
	max_extraction(a,n);
}








void print_array(TaggedInt *a, int n){
	for (int i=0;i<n; i++){
		printf("[%d, %d], ",a[i].key,a[i].id);
	}
	printf("\n");
}




			
