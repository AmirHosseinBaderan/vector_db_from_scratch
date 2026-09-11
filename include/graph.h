#ifndef GRAPH_H
#define GRAPH_H

#include "vector.h"
#include <stddef.h>

typedef struct {
    size_t *neighbors;
    size_t count;
} GraphNode;


typedef struct{
    Vector *vectors;
    GraphNode *nodes;

    size_t size;
    size_t max_neighbors;
} Graph;


Graph graph_create(
    Vector *vectors,
    size_t size,
    size_t max_neighbors
);

void graph_free(Graph *graph);

int graph_add_edge(
    Graph *graph,
    size_t from,
    size_t to
);

int graph_build(Graph *graph);

#endif