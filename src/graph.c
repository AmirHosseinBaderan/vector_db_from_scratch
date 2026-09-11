#include "graph.h"

#include <stdlib.h>
#include <stdio.h>
#include "distance.h"
#include "search.h"

Graph graph_create(
    Vector *vectors,
    size_t size,
    size_t max_neighbors)
{
    Graph graph = {
        .vectors = vectors,
        .nodes = NULL,
        .size = size,
        .max_neighbors = max_neighbors};

    if (size == 0 || max_neighbors == 0)
    {
        return graph;
    }

    graph.nodes =
        calloc(size, sizeof(GraphNode));

    if (graph.nodes == NULL)
    {
        return graph;
    }

    for (size_t i = 0; i < size; i++)
    {
        graph.nodes[i].neighbors =
            malloc(
                max_neighbors *
                sizeof(size_t));

        if (graph.nodes[i].neighbors == NULL)
        {
            graph_free(&graph);
            return graph;
        }

        graph.nodes[i].count = 0;
    }

    return graph;
}

void graph_free(Graph *graph)
{
    if (graph == NULL)
    {
        return;
    }

    if (graph->nodes != NULL)
    {
        for (size_t i = 0; i < graph->size; i++)
        {
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
    size_t to)
{
    if (graph == NULL || graph->nodes == NULL)
    {
        return -1;
    }

    if (from >= graph->size || to >= graph->size)
    {
        return -1;
    }

    GraphNode *node = &graph->nodes[from];

    if (node->count >= graph->max_neighbors)
    {
        return -1;
    }

    node->neighbors[node->count] = to;
    node->count++;

    return 0;
}

static void swap_results(
    size_t *indices,
    float *scores,
    size_t a,
    size_t b
)
{
    size_t index_temp = indices[a];
    indices[a] = indices[b];
    indices[b] = index_temp;

    float score_temp = scores[a];
    scores[a] = scores[b];
    scores[b] = score_temp;
}

static void sort_candidates(
    size_t *indices,
    float *scores,
    size_t count
)
{
    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            if (scores[j] > scores[i]) {
                swap_results(
                    indices,
                    scores,
                    i,
                    j
                );
            }
        }
    }
}

int graph_build(Graph *graph)
{
    if (
        graph == NULL ||
        graph->nodes == NULL ||
        graph->vectors == NULL
    ) {
        return -1;
    }

    for (size_t i = 0; i < graph->size; i++) {
        size_t candidate_count = graph->size - 1;

        size_t *indices =
            malloc(candidate_count * sizeof(size_t));

        float *scores =
            malloc(candidate_count * sizeof(float));

        if (indices == NULL || scores == NULL) {
            free(indices);
            free(scores);

            return -1;
        }

        size_t count = 0;

        for (size_t j = 0; j < graph->size; j++) {
            if (i == j) {
                continue;
            }

            indices[count] = j;

            scores[count] =
                cosine_similarity(
                    &graph->vectors[i],
                    &graph->vectors[j]
                );

            count++;
        }

        sort_candidates(
            indices,
            scores,
            count
        );

        size_t neighbor_count =
            graph->max_neighbors < count
                ? graph->max_neighbors
                : count;

        for (size_t j = 0; j < neighbor_count; j++) {
            graph_add_edge(
                graph,
                i,
                indices[j]
            );
        }

        free(indices);
        free(scores);
    }

    return 0;
}