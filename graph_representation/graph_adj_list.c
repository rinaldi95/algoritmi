#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdint.h>
#include<limits.h>

#include"graph_adj_list.h"

/*==========================helpers=============================*/


typedef struct {
	uint64_t state;
	uint64_t inc;
} Pcg32Rng;

static uint32_t pcg32_random(Pcg32Rng *rng){
	uint64_t oldstate=rng->state;
	rng->state=oldstate * 6364136223846793005ULL + rng->inc;
	uint32_t xorshifted=(uint32_t)(((oldstate >> 18u)^oldstate) >> 27u);
	uint32_t rot=(uint32_t)(oldstate >> 59u);
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static void pcg32_srandom(Pcg32Rng *rng, uint64_t initstate, uint64_t initseq){
	rng->state=0u;
	rng->inc=(initseq << 1u) | 1u;
	(void)pcg32_random(rng);
	rng->state+=initstate;
	(void)pcg32_random(rng);
}

static uint32_t pcg32_boundedrand(Pcg32Rng *rng, uint32_t bound){
	uint32_t threshold=(uint32_t)(-bound) % bound;
	for (;;){
		uint32_t value=pcg32_random(rng);
		if (value>=threshold) return value % bound;
	}
}

static int node_add_edge(AdjNode **n, int v){
	AdjNode *node=*n;
	while(node!=NULL){
		if (node->v==v) return 1;
		node=node->next;
	}
	node=malloc(sizeof(AdjNode));
	if (node==NULL){
		fprintf(stderr,"Error: malloc failure. node_add_edge not performed.\n");
		return -2;
	}
	node->v=v;
	node->next=*n;
	*n=node;
	return 0;
}


static int node_remove_edge(AdjNode **n, int v){
	if (*n==NULL)return 1;
	AdjNode *prev=*n;
	if (prev->v==v){
		*n=prev->next;
		free(prev);
		return 0;
	}
	AdjNode *node=prev->next;
	while(node!=NULL){
		if (node->v==v){
			prev->next=node->next;
			free(node);
			return 0;
		}
		prev=node;
		node=node->next;
	}
	return 1;
}

static void print_node(AdjNode *node){
	while(node!=NULL){
		printf("%d->",node->v);
		node=node->next;
	}
	printf("\n");
}	

static void print_graph(GraphAdjList *g){
	for (int i=0; i<g->n; i++){
		printf("%p: ",(void*)g->heads[i]);
		printf("%d-> ",i);
		print_node(g->heads[i]);
	}
}

static void list_destroy(AdjNode *node){
	AdjNode *temp;
	while(node!=NULL){
		temp=node;
		node=node->next;
		free(temp);
	}
}
	

/*==========================non static==========================*/

int graph_adj_list_init(GraphAdjList *g, int n){
	if (g==NULL){
		fprintf(stderr,"Error: pointer to GraphAdjList not valid. "
				"graph_adj_list_init not performed.\n");
		return -1;
	}
	if (n<0){
		fprintf(stderr,"Error: number of vertices is negative. "
				"graph_adj_list_init not performed.\n");
		return -1;
	}
	if (n==0)g->heads=NULL;
	else{
		g->heads=malloc(n*sizeof(AdjNode *));
		if (g->heads==NULL){
			fprintf(stderr,"Error: malloc failure. "
					"graph_adj_list_init not performed.\n");
			return -2;
		}
		for (int i=0; i<n;i++){
			g->heads[i]=NULL;
		}
	}
	g->n=n;
	g->m=0;
	return 0;		
}

int graph_adj_list_add_edge(GraphAdjList *g, int u, int v){
	if (g==NULL){
		fprintf(stderr,"Error: pointer to GraphAdjList not valid. "
				"graph_adj_list_add_edge not performed.\n");
		return -1;
	}
	if (u<0 ||v<0||g->n<=u||g->n<=v){
		fprintf(stderr,"Error: invalid index: "
				"graph_adj_list_add_edge not performed\n");
		return -1;
	}
	if (u==v){	
		fprintf(stderr,"Error: indices must be different. "
				"graph_adj_list_add_edge not performed.\n");
		return -1;
	}
	switch (node_add_edge(&g->heads[u],v)){
		case 1:
			return 1;
		case -2:
			return -2;
		case 0:
			if (node_add_edge(&g->heads[v],u)==0){	
				g->m++;
				return 0;
			}
			else {
				node_remove_edge(&g->heads[u],v);
				return -2;
			}
	}
	return -1;
}

void graph_adj_list_destroy(GraphAdjList *g){
	if (g==NULL) return;
	for (int i=0;i<g->n;i++){
		list_destroy(g->heads[i]);
	}
	free(g->heads);
	g->heads=NULL;
	g->n=0;
	g->m=0;
}


int graph_adj_list_remove_edge(GraphAdjList *g, int u, int v){
	if (g==NULL){
		fprintf(stderr,"Error: pointer to GraphAdjList not valid. "
				"graph_adj_list_remove_edge not performed.\n");
		return -1;
	}
	if (u<0 ||v<0||g->n<=u||g->n<=v){
		fprintf(stderr,"Error: invalid index: "
				"graph_adj_list_remove_edge not performed\n");
		return -1;
	}
	if (u==v){	
		fprintf(stderr,"Error: indices must be different. "
				"graph_adj_list_remove_edge not performed.\n");
		return -1;
	}
	switch (node_remove_edge(&g->heads[u],v)){
		case 1:
			return 1;
		case 0:
			g->m--;
			return node_remove_edge(&g->heads[v],u);	
	}
	return -1;
}

int graph_adj_list_is_adjacent(const GraphAdjList *g, int u, int v){
	if (g==NULL){
		fprintf(stderr,"Error: pointer to GraphAdjList not valid. "
				"graph_adj_list_is_adjacent not performed.\n");
		return -1;
	}
	if (u<0 ||v<0||g->n<=u||g->n<=v){
		fprintf(stderr,"Error: invalid index: "
				"graph_adj_list_is_adjacent not performed\n");
		return -1;
	}
	if (u==v){	
		fprintf(stderr,"Error: indices must be different. "
				"graph_adj_list_is_adjacent not performed.\n");
		return -1;
	}
	AdjNode *node=g->heads[u];
	while(node!=NULL){
		if (node->v==v)	return 1;
		node=node->next;
	}
	return 0;
}	

int graph_adj_list_neighbors(
	    const GraphAdjList *g, int u, int **out_neighbors, int *out_count) {
	if (g==NULL){
		fprintf(stderr,"Error: pointer to GraphAdjList not valid. "
				"graph_adj_list_neighbors not performed.\n");
		return -1;
	}
	if (out_neighbors==NULL){
		fprintf(stderr,"Error: pointer to out_neighbors not valid. "
				"graph_adj_list_neighbors not performed.\n");
		return -1;
	}
	if (out_count==NULL){
		fprintf(stderr,"Error: pointer to out_count not valid. "
				"graph_adj_list_neighbors not performed.\n");
		return -1;
	}
	if (u<0 ||g->n<=u){
		fprintf(stderr,"Error: invalid index: "
				"graph_adj_list_neighbors not performed\n");
		return -1;
	}
	AdjNode *node=g->heads[u];
	int i=0;
	while(node!=NULL){
		i++;
		node=node->next;
	}
	*out_count=i;
	if (i==0){
		*out_neighbors=NULL;
		return 0;
	}
	*out_neighbors=malloc(i * sizeof(int));
	if (*out_neighbors==NULL){
		fprintf(stderr,"Error: malloc failure. "
				"graph_adj_list_neighbors not performed");
		return -2;
	}
	i=0;
	node = g->heads[u];
	while(node!=NULL){
		(*out_neighbors)[i]=node->v;
		i++;
		node=node->next;
	}
	int min;
	int min_idx;
	int swap;
	for (int i=0;i<*out_count;i++){
		min=INT_MAX;
		for (int j=i;j<*out_count;j++){
			if (min> (*out_neighbors)[j]){
				min=(*out_neighbors)[j];
				min_idx=j;
			}
		}
		swap=(*out_neighbors)[i];
		(*out_neighbors)[i]=min;
		(*out_neighbors)[min_idx]=swap;
	}
	return 0;
}
	


void run_tests(GraphAdjList *g){
	assert(graph_adj_list_init(g,5)==0);
	assert(graph_adj_list_add_edge(g,1,2)==0);
	assert(graph_adj_list_add_edge(g,1,3)==0);
	print_graph(g);
	assert(graph_adj_list_add_edge(g,1,3)==1);
	assert(graph_adj_list_add_edge(g,1,2)==1);
	assert(graph_adj_list_add_edge(g,2,1)==1);

	assert(graph_adj_list_is_adjacent(g,1,2));
	assert(graph_adj_list_remove_edge(g,1,2)==0);
	assert(!graph_adj_list_is_adjacent(g,1,2));
	assert(graph_adj_list_remove_edge(g,1,2)==1);
	assert(!graph_adj_list_is_adjacent(g,1,2));
	assert(graph_adj_list_add_edge(g,1,0)==0);
	assert(graph_adj_list_add_edge(g,1,2)==0);

	assert(graph_adj_list_add_edge(g,1,4)==0);
	int out_count;
	int	*out_neighbors;
	assert(graph_adj_list_neighbors(g,1,&out_neighbors,&out_count)==0);
	for (int i=0;i<out_count;i++){
		printf("%d, ",out_neighbors[i]);
	}
	printf("\n");
	print_graph(g);
	graph_adj_list_destroy(g);

	assert(graph_adj_list_init(g,0)==0);
	assert(g->n==0);
	assert(g->m==0);
	assert(graph_adj_list_add_edge(g,0,0)==-1);
	assert(graph_adj_list_is_adjacent(g,0,0)==-1);
	assert(graph_adj_list_neighbors(g,0,&out_neighbors,&out_count)==-1);
	graph_adj_list_destroy(g);

	graph_adj_list_init(g,10);
	int u;
	int v;
	Pcg32Rng rng;
	pcg32_srandom(&rng,0xC0FFEEu,0x1234u);
	for (int i=0; i<20;i++){
		u=(int)pcg32_boundedrand(&rng,(uint32_t)g->n);
		v=(int)pcg32_boundedrand(&rng,(uint32_t)g->n);
		if (u==v){
			assert(graph_adj_list_add_edge(g,u,v)==-1);
		}
		else if (!graph_adj_list_is_adjacent(g,u,v)){
			assert(graph_adj_list_add_edge(g,u,v)==0);
		}
		else{
			assert(graph_adj_list_add_edge(g,u,v));
		}
	}
	print_graph(g);

	free(out_neighbors);
	assert(graph_adj_list_neighbors(g,1,&out_neighbors,&out_count)==0);
	for (int i=0;i<out_count;i++){
		printf("%d, ",out_neighbors[i]);
	}
	printf("\n");
	

	
	for (int i=0; i<20;i++){
		u=(int)pcg32_boundedrand(&rng,(uint32_t)g->n);
		v=(int)pcg32_boundedrand(&rng,(uint32_t)g->n);
		if (u==v){
			assert(graph_adj_list_remove_edge(g,u,v)==-1);
		}
		else if (graph_adj_list_is_adjacent(g,u,v)){
			assert(graph_adj_list_remove_edge(g,u,v)==0);
		}
		else{
			assert(graph_adj_list_remove_edge(g,u,v));
		}
	}
	print_graph(g);
	
	printf("All tests passed\n");
	graph_adj_list_destroy(g);
	free(out_neighbors);
}

int main(void){
	GraphAdjList g;
	run_tests(&g);
	return 0;
}
