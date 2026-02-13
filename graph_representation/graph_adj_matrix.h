#ifndef GRAPH_ADJ_MATRIX_H
#define GRAPH_ADJ_MATRIX_H

/*
 * Graph Representation - Adjacency Matrix (undirected, unweighted)
 *
 * This header defines a detailed contract to guide incremental implementation.
 * The corresponding source file should implement all behaviors described here.
 *
 * ----------------------------
 * MODEL AND POLICIES (FROZEN)
 * ----------------------------
 * - Graph is UNDIRECTED.
 * - Graph is UNWEIGHTED.
 * - Valid vertices are integers in [0, n-1].
 * - Self-loops are NOT allowed: (u, u) is invalid input.
 * - Duplicate edges are not inserted:
 *   add_edge on an existing edge returns NO_CHANGE and keeps graph unchanged.
 * - remove_edge on a missing edge returns NO_CHANGE and keeps graph unchanged.
 *
 * ----------------------------
 * RETURN CODE CONVENTION
 * ----------------------------
 * Base status policy (used by mutating functions and most queries):
 * -  0: success with state change (or successful query execution).
 * -  1: valid request, but no state change needed (idempotent no-op).
 * - -1: invalid input (NULL pointers, out-of-range vertex, self-loop, n < 0).
 * - -2: memory allocation failure.
 *
 * Special case:
 * - graph_adj_matrix_is_adjacent returns 1/0/-1 directly:
 *   1 = edge exists, 0 = edge does not exist, -1 = invalid input.
 */

#include <stddef.h>

/*
 * Graph adjacency-matrix container.
 *
 * Fields:
 * - matrix: contiguous n*n buffer in row-major order.
 *   Cell (u, v) is matrix[u*n + v], value in {0,1}.
 * - n: number of vertices.
 * - m: number of undirected edges currently stored.
 *
 * Invariants:
 * - If n == 0, matrix may be NULL.
 * - matrix[u*n + v] is always 0 or 1.
 * - Symmetry for undirected graph:
 *   matrix[u*n + v] == matrix[v*n + u] for all valid u,v.
 * - No self-loop: matrix[u*n + u] == 0 for all valid u.
 * - m counts unique undirected edges (not matrix cells).
 */
typedef struct {
    unsigned char *matrix;
    int n;
    int m;
} GraphAdjMatrix;

/*
 * graph_adj_matrix_init
 * ---------------------
 * Initialize an empty graph with n vertices and zero edges.
 *
 * Input:
 * - g != NULL
 * - n >= 0
 *
 * Output/Postconditions on success:
 * - g->n == n
 * - g->m == 0
 * - g->matrix allocated for n*n cells (or NULL if n == 0, implementation choice)
 * - every matrix cell initialized to 0
 *
 * Returns:
 * -  0 on success
 * - -1 on invalid input
 * - -2 on allocation failure
 */
int graph_adj_matrix_init(GraphAdjMatrix *g, int n);

/*
 * graph_adj_matrix_destroy
 * ------------------------
 * Free all memory owned by the graph and reset it to a safe empty state.
 *
 * Required behavior:
 * - Safe on already-empty graph.
 * - After call: g->matrix == NULL, g->n == 0, g->m == 0.
 * - If g == NULL: no-op.
 */
void graph_adj_matrix_destroy(GraphAdjMatrix *g);

/*
 * graph_adj_matrix_add_edge
 * -------------------------
 * Add undirected edge {u, v}.
 *
 * Input constraints:
 * - g != NULL
 * - 0 <= u,v < g->n
 * - u != v
 *
 * Behavior:
 * - If edge does not exist, set both matrix[u*n+v] and matrix[v*n+u] to 1,
 *   increment m.
 * - If edge already exists, leave graph unchanged.
 *
 * Returns:
 * -  0 on insertion
 * -  1 if edge already present
 * - -1 on invalid input
 */
int graph_adj_matrix_add_edge(GraphAdjMatrix *g, int u, int v);

/*
 * graph_adj_matrix_remove_edge
 * ----------------------------
 * Remove undirected edge {u, v}.
 *
 * Input constraints:
 * - g != NULL
 * - 0 <= u,v < g->n
 * - u != v
 *
 * Behavior:
 * - If edge exists, set both matrix[u*n+v] and matrix[v*n+u] to 0, decrement m.
 * - If edge does not exist, leave graph unchanged.
 *
 * Returns:
 * -  0 on successful removal
 * -  1 if edge was not present
 * - -1 on invalid input
 */
int graph_adj_matrix_remove_edge(GraphAdjMatrix *g, int u, int v);

/*
 * graph_adj_matrix_is_adjacent
 * ----------------------------
 * Query whether edge {u, v} exists.
 *
 * Input constraints:
 * - g != NULL
 * - 0 <= u,v < g->n
 * - u != v
 *
 * Returns:
 * -  1 if edge exists
 * -  0 if edge does not exist
 * - -1 on invalid input
 *
 * Complexity:
 * - O(1) in adjacency-matrix model
 */
int graph_adj_matrix_is_adjacent(const GraphAdjMatrix *g, int u, int v);

/*
 * graph_adj_matrix_neighbors
 * --------------------------
 * Return a newly allocated array with all neighbors of u.
 *
 * Input constraints:
 * - g != NULL
 * - out_neighbors != NULL
 * - out_count != NULL
 * - 0 <= u < g->n
 *
 * Output on success:
 * - *out_count = degree(u)
 * - If degree(u) == 0: *out_neighbors = NULL
 * - Else: *out_neighbors points to malloc-allocated int array of size degree(u)
 *
 * Ownership:
 * - Caller owns *out_neighbors and must free(*out_neighbors) when non-NULL.
 *
 * Order guarantee:
 * - Neighbors must be returned in ascending order (deterministic tests).
 *   (Natural with a scan v=0..n-1.)
 *
 * Returns:
 * -  0 on success
 * - -1 on invalid input
 * - -2 on allocation failure
 *
 * Complexity:
 * - O(n)
 */
int graph_adj_matrix_neighbors(
    const GraphAdjMatrix *g, int u, int **out_neighbors, int *out_count
);

/*
 * graph_adj_matrix_vertex_count
 * -----------------------------
 * Read current vertex count (n).
 *
 * Input:
 * - g != NULL
 * - out_n != NULL
 *
 * Output:
 * - *out_n = g->n
 *
 * Returns:
 * -  0 on success
 * - -1 on invalid input
 */
int graph_adj_matrix_vertex_count(const GraphAdjMatrix *g, int *out_n);

/*
 * graph_adj_matrix_edge_count
 * ---------------------------
 * Read current undirected edge count (m).
 *
 * Input:
 * - g != NULL
 * - out_m != NULL
 *
 * Output:
 * - *out_m = g->m
 *
 * Returns:
 * -  0 on success
 * - -1 on invalid input
 */
int graph_adj_matrix_edge_count(const GraphAdjMatrix *g, int *out_m);

#endif
