#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include"sort.h"

long ns_diff(struct timespec a, struct timespec b){
	return (b.tv_sec - a.tv_sec) * 1000000000L + (b.tv_nsec - a.tv_nsec);
}

typedef void (*sort_fn)(TaggedInt *, int);


int main(void){
	srand((unsigned)time(NULL));
	int reps=100;
	printf("n,case,algorithm,ns_per_call\n");
	int n_max=14;
	for (int i=2;i<n_max;i++){
		int n=1<<i;
		int maxv=n;
		TaggedInt *arr=malloc(n*sizeof(TaggedInt));
		if (arr==NULL){
			fprintf(stderr,"malloc failure");
			return 1;
		}
		struct {
			const char *name;
			sort_fn fn;
		} algs[] = {
			{"insertion_sort", insertion_sort},
			{"selection_sort", selection_sort},
			{"merge_sort", merge_sort},
			{"quick_sort", quick_sort},
			{"heap_sort", heap_sort},
		};
		int algs_count = (int)(sizeof(algs) / sizeof(algs[0]));
		for (int a=0;a<algs_count;a++){
			long ns=0;
			struct timespec t0,t1;
			for (int j=0;j<reps;j++){
				for (int k=0;k<n;k++){
					arr[k].key = (maxv > 0) ? (rand() % maxv) : 0;
					arr[k].id = k;
				}
				clock_gettime(CLOCK_MONOTONIC, &t0);
				algs[a].fn(arr,n);
				clock_gettime(CLOCK_MONOTONIC, &t1);
				ns += ns_diff(t0, t1);
			}
			double per_call = (double)ns / reps;
			printf("%d,random,%s,%.2f\n", n, algs[a].name, per_call);
		}
		free(arr);
	}
	return 0;
}
		
