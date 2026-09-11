#include "graph.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    const size_t vector_count = 5;
    const size_t dimension = 2;
    const size_t max_neighbors = 3;

    Vector vectors[vector_count];

    for (size_t i = 0; i < vector_count; i++) {
        vectors[i] = vector_create(dimension);

        assert(vectors[i].data != NULL);
    }

    Graph graph = graph_create(
        vectors,
        vector_count,
        max_neighbors
    );

    assert(graph.size == vector_count);
    assert(graph.max_neighbors == max_neighbors);
    assert(graph.nodes != NULL);

    for (size_t i = 0; i < vector_count; i++) {
        assert(graph.nodes[i].neighbors != NULL);
        assert(graph.nodes[i].count == 0);
    }

    assert(graph_add_edge(&graph, 0, 1) == 0);
    assert(graph_add_edge(&graph, 0, 2) == 0);
    assert(graph_add_edge(&graph, 0, 3) == 0);

    assert(graph.nodes[0].count == 3);

    assert(graph.nodes[0].neighbors[0] == 1);
    assert(graph.nodes[0].neighbors[1] == 2);
    assert(graph.nodes[0].neighbors[2] == 3);

    assert(graph_add_edge(&graph, 0, 4) != 0);
    assert(graph.nodes[0].count == 3);

    graph_free(&graph);

    for (size_t i = 0; i < vector_count; i++) {
        vector_free(&vectors[i]);
    }

    printf("All graph tests passed.\n");

    return 0;
}