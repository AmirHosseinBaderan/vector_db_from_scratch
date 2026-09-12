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

size_t graph_search(
    const Graph *graph,
    const Vector *query,
    size_t entry_point
)
{
    if (
        graph == NULL ||
        graph->nodes == NULL ||
        graph->vectors == NULL ||
        query == NULL
    ) {
        return 0;
    }

    if (entry_point >= graph->size) {
        return 0;
    }

    size_t current = entry_point;

    float current_score =
        cosine_similarity(
            query,
            &graph->vectors[current]
        );

    while (1) {
        size_t best = current;
        float best_score = current_score;

        const GraphNode *node =
            &graph->nodes[current];

        for (size_t i = 0; i < node->count; i++) {
            size_t neighbor =
                node->neighbors[i];

            float score =
                cosine_similarity(
                    query,
                    &graph->vectors[neighbor]
                );

            if (score > best_score) {
                best = neighbor;
                best_score = score;
            }
        }

        if (best == current) {
            break;
        }

        current = best;
        current_score = best_score;
    }

    return current;
}

size_t graph_search_k(
    const Graph *graph,
    const Vector *query,
    size_t entry_point,
    size_t k,
    size_t *results
)
{
    if (
        graph == NULL ||
        graph->nodes == NULL ||
        graph->vectors == NULL ||
        query == NULL ||
        results == NULL ||
        k == 0 ||
        entry_point >= graph->size
    ) {
        return 0;
    }

    if (k > graph->size) {
        k = graph->size;
    }

    size_t *visited =
        calloc(graph->size, sizeof(size_t));

    size_t *candidates =
        malloc(graph->size * sizeof(size_t));

    if (
        visited == NULL ||
        candidates == NULL
    ) {
        free(visited);
        free(candidates);

        return 0;
    }

    size_t candidate_count = 1;

    candidates[0] = entry_point;
    visited[entry_point] = 1;

    while (candidate_count > 0) {
        size_t best_position = 0;

        float best_score =
            cosine_similarity(
                query,
                &graph->vectors[candidates[0]]
            );

        for (size_t i = 1; i < candidate_count; i++) {
            float score =
                cosine_similarity(
                    query,
                    &graph->vectors[candidates[i]]
                );

            if (score > best_score) {
                best_position = i;
                best_score = score;
            }
        }

        size_t current =
            candidates[best_position];

        for (
            size_t i = best_position + 1;
            i < candidate_count;
            i++
        ) {
            candidates[i - 1] = candidates[i];
        }

        candidate_count--;

        const GraphNode *node =
            &graph->nodes[current];

        for (size_t i = 0; i < node->count; i++) {
            size_t neighbor =
                node->neighbors[i];

            if (visited[neighbor]) {
                continue;
            }

            visited[neighbor] = 1;

            candidates[candidate_count] =
                neighbor;

            candidate_count++;
        }
    }

    /*
     * Build Top-K directly.
     * Do not write more than k elements
     * into the caller's results array.
     */

    size_t result_count = 0;

    for (size_t i = 0; i < graph->size; i++) {
        if (!visited[i]) {
            continue;
        }

        float score_i =
            cosine_similarity(
                query,
                &graph->vectors[i]
            );

        size_t position = result_count;

        if (result_count < k) {
            results[result_count] = i;
            result_count++;
        } else {
            float worst_score =
                cosine_similarity(
                    query,
                    &graph->vectors[results[k - 1]]
                );

            if (score_i <= worst_score) {
                continue;
            }

            results[k - 1] = i;
            position = k - 1;
        }

        while (position > 0) {
            float current_score =
                cosine_similarity(
                    query,
                    &graph->vectors[results[position]]
                );

            float previous_score =
                cosine_similarity(
                    query,
                    &graph->vectors[results[position - 1]]
                );

            if (current_score <= previous_score) {
                break;
            }

            size_t temp =
                results[position];

            results[position] =
                results[position - 1];

            results[position - 1] =
                temp;

            position--;
        }
    }

    free(visited);
    free(candidates);

    return result_count;
}