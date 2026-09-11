#include "graph.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    const size_t vector_count = 4;
    const size_t dimension = 2;
    const size_t max_neighbors = 2;

    Vector vectors[vector_count];

    vectors[0] = vector_create(dimension);
    vectors[1] = vector_create(dimension);
    vectors[2] = vector_create(dimension);
    vectors[3] = vector_create(dimension);

    vectors[0].data[0] = 1.0f;
    vectors[0].data[1] = 0.0f;

    vectors[1].data[0] = 0.9f;
    vectors[1].data[1] = 0.1f;

    vectors[2].data[0] = 0.0f;
    vectors[2].data[1] = 1.0f;

    vectors[3].data[0] = 0.8f;
    vectors[3].data[1] = 0.2f;

    Graph graph = graph_create(
        vectors,
        vector_count,
        max_neighbors);

    assert(graph.nodes != NULL);

    assert(graph_build(&graph) == 0);

    for (size_t i = 0; i < vector_count; i++)
    {
        assert(graph.nodes[i].count == max_neighbors);
    }

    /*
     * Vector 0 is closest to:
     * Vector 1
     * Vector 3
     */
    assert(graph.nodes[0].neighbors[0] == 1);
    assert(graph.nodes[0].neighbors[1] == 3);

    /*
     * Vector 2 is closest to:
     * Vector 1
     * Vector 3
     */
    assert(graph.nodes[2].neighbors[0] == 3);
    assert(graph.nodes[2].neighbors[1] == 1);

    graph_free(&graph);

    for (size_t i = 0; i < vector_count; i++)
    {
        vector_free(&vectors[i]);
    }

    printf("All graph build tests passed.\n");

    return 0;
}