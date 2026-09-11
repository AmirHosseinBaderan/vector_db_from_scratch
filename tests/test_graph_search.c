#include "graph.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    const size_t vector_count = 5;
    const size_t dimension = 2;
    const size_t max_neighbors = 2;

    Vector vectors[vector_count];

    vectors[0] = vector_create(dimension);
    vectors[1] = vector_create(dimension);
    vectors[2] = vector_create(dimension);
    vectors[3] = vector_create(dimension);
    vectors[4] = vector_create(dimension);

    vectors[0].data[0] = 0.0f;
    vectors[0].data[1] = 1.0f;

    vectors[1].data[0] = 0.2f;
    vectors[1].data[1] = 0.98f;

    vectors[2].data[0] = 0.4f;
    vectors[2].data[1] = 0.92f;

    vectors[3].data[0] = 0.7f;
    vectors[3].data[1] = 0.7f;

    vectors[4].data[0] = 1.0f;
    vectors[4].data[1] = 0.0f;

    Graph graph = graph_create(
        vectors,
        vector_count,
        max_neighbors
    );

    assert(graph.nodes != NULL);
    assert(graph_build(&graph) == 0);

    Vector query = vector_create(dimension);

    query.data[0] = 0.65f;
    query.data[1] = 0.76f;

    size_t result =
        graph_search(
            &graph,
            &query,
            0
        );

    /*
     * Vector 3 is the closest vector
     * to the query.
     */
    assert(result == 3);

    vector_free(&query);

    graph_free(&graph);

    for (size_t i = 0; i < vector_count; i++) {
        vector_free(&vectors[i]);
    }

    printf("All graph search tests passed.\n");

    return 0;
}