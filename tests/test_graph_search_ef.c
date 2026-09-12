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
    }

    vectors[0].data[0] = 0.0f;
    vectors[0].data[1] = 1.0f;

    vectors[1].data[0] = 0.2f;
    vectors[1].data[1] = 0.98f;

    vectors[2].data[0] = 0.5f;
    vectors[2].data[1] = 0.866f;

    vectors[3].data[0] = 0.8f;
    vectors[3].data[1] = 0.6f;

    vectors[4].data[0] = 1.0f;
    vectors[4].data[1] = 0.0f;

    Graph graph = graph_create(
        vectors,
        vector_count,
        max_neighbors
    );

    assert(graph.nodes != NULL);

    /*
     * Build a navigable graph manually.
     *
     * 0 -> 1, 2
     * 1 -> 0, 2, 3
     * 2 -> 1, 3, 4
     * 3 -> 1, 2, 4
     * 4 -> 2, 3
     */

    assert(graph_add_edge(&graph, 0, 1) == 0);
    assert(graph_add_edge(&graph, 0, 2) == 0);

    assert(graph_add_edge(&graph, 1, 0) == 0);
    assert(graph_add_edge(&graph, 1, 2) == 0);
    assert(graph_add_edge(&graph, 1, 3) == 0);

    assert(graph_add_edge(&graph, 2, 1) == 0);
    assert(graph_add_edge(&graph, 2, 3) == 0);
    assert(graph_add_edge(&graph, 2, 4) == 0);

    assert(graph_add_edge(&graph, 3, 1) == 0);
    assert(graph_add_edge(&graph, 3, 2) == 0);
    assert(graph_add_edge(&graph, 3, 4) == 0);

    assert(graph_add_edge(&graph, 4, 2) == 0);
    assert(graph_add_edge(&graph, 4, 3) == 0);

    Vector query = vector_create(dimension);

    query.data[0] = 1.0f;
    query.data[1] = 0.0f;

    size_t results[2];

    size_t count =
        graph_search_ef(
            &graph,
            &query,
            0,
            3,
            results,
            2
        );

    assert(count == 2);

    /*
     * Vector 4 is the exact nearest neighbor.
     */
    assert(results[0] == 4);

    /*
     * Vector 3 is the second nearest neighbor.
     */
    assert(results[1] == 3);

    vector_free(&query);

    graph_free(&graph);

    for (size_t i = 0; i < vector_count; i++) {
        vector_free(&vectors[i]);
    }

    printf("All graph ef search tests passed.\n");

    return 0;
}