#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include"sort.h"

int main(void){
	int a1[]={1,3,5,2,0,4,6,7,8,9};
	int a2[]={4,5,6,5,4};
	int a3[]={0};
	int a4[]={1};
	int a5[]={2,1};
	int a6[]={1,2,3,4,5};
	int a7[]={5,4,3,2,1};
	int a8[]={7,7,7,7};
	int a9[]={3,1,3,2,3,1,2};
	int a10[]={0,-1,5,-3,2};
	int a11[]={9,1,8,2,7};
	int a12[]={16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
	int a13[]={17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
	int a14[]={18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
	int a15[]={21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
	int *tests[]={a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15};
	int n[]={10,5,0,1,2,5,5,4,7,5,5,16,17,18,21};

	int num_tests=(int)(sizeof (n)/sizeof(int));
	for (int i=0;i<num_tests;i++){
		TaggedInt *a=malloc(n[i]*sizeof(TaggedInt));
		if(a==NULL){
			fprintf(stderr,"Error: malloc failure");
			return -1;
		}
/*		for (int j=0;j<n[i];j++){
			a[j].key=tests[i][j];
			a[j].id=j;
		}
		printf("\n ---------------------insertion_sort------------------\n");
		print_array(a,n[i]);
		insertion_sort(a, n[i]);
		print_array(a,n[i]);
		for (int j=1;j<n[i];j++){
			assert(a[j-1].key<=a[j].key);
			if (a[j-1].key==a[j].key){
				assert(a[j-1].id<a[j].id);
			}
		}
		for (int j=0;j<n[i];j++){
			a[j].key=tests[i][j];
			a[j].id=j;
		}
		printf("\n ---------------------selection_sort------------------\n");
		print_array(a,n[i]);
		selection_sort(a, n[i]);
		print_array(a,n[i]);
		for (int j=1;j<n[i];j++){
			assert(a[j-1].key<=a[j].key);
			if (a[j-1].key==a[j].key){
//				assert(a[j-1].id<a[j].id);
			}
		}*/
		for (int j=0;j<n[i];j++){
			a[j].key=tests[i][j];
			a[j].id=j;
		}
		printf("\n ---------------------merge_sort------------------\n");
		print_array(a,n[i]);
		merge_sort(a, n[i]);
		print_array(a,n[i]);
		for (int j=1;j<n[i];j++){
			assert(a[j-1].key<=a[j].key);
			if (a[j-1].key==a[j].key){
				assert(a[j-1].id<a[j].id);
			}
		}
	free(a);
	}

	
	return 0;
}
