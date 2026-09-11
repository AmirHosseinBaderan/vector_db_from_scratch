#include "graph.h"

#include <stdlib.h>

Graph graph_create(
    Vector *vectors,
    size_t size,
    size_t max_neighbors
)
{
    Graph graph = {
        .vectors = vectors,
        .nodes = NULL,
        .size = size,
        .max_neighbors = max_neighbors
    };

    if (size == 0 || max_neighbors == 0) {
        return graph;
    }

    graph.nodes =
        calloc(size, sizeof(GraphNode));

    if (graph.nodes == NULL) {
        return graph;
    }

    for (size_t i = 0; i < size; i++) {
        graph.nodes[i].neighbors =
            malloc(
                max_neighbors *
                sizeof(size_t)
            );

        if (graph.nodes[i].neighbors == NULL) {
            graph_free(&graph);
            return graph;
        }

        graph.nodes[i].count = 0;
    }

    return graph;
}

void graph_free(Graph *graph)
{
    if (graph == NULL) {
        return;
    }

    if (graph->nodes != NULL) {
        for (size_t i = 0; i < graph->size; i++) {
            free(graph->nodes[i].neighbors);
            graph->nodes[i].neighbors = NULL;
            graph->nodes[i].count = 0;
        }

        free(graph->nodes);
        graph->nodes = NULL;
    }

    graph->vectors = NULL;
    graph->size = 0;
    graph->max_neighbors = 0;
}

int graph_add_edge(
    Graph *graph,
    size_t from,
    size_t to
)
{
    if (graph == NULL || graph->nodes == NULL) {
        return -1;
    }

    if (from >= graph->size || to >= graph->size) {
        return -1;
    }

    GraphNode *node = &graph->nodes[from];

    if (node->count >= graph->max_neighbors) {
        return -1;
    }

    node->neighbors[node->count] = to;
    node->count++;

    return 0;
}