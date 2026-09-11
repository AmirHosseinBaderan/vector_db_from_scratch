#include "search.h"
#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void fill_vector(Vector *vector, float base)
{
    for (size_t i = 0; i < vector->dimension; i++) {
        vector->data[i] = base + (float)i * 0.1f;
    }
}

int main(void)
{
    const size_t vector_count = 100;
    const size_t dimension = 16;
    const size_t k = 10;

    Vector query = vector_create(dimension);

    Vector *vectors =
        malloc(vector_count * sizeof(Vector));

    assert(query.data != NULL);
    assert(vectors != NULL);

    fill_vector(&query, 1.0f);

    for (size_t i = 0; i < vector_count; i++) {
        vectors[i] = vector_create(dimension);

        assert(vectors[i].data != NULL);

        fill_vector(
            &vectors[i],
            1.0f + (float)i * 0.01f
        );
    }

    SearchResult brute_force_results[10];
    SearchResult top_k_results[10];

    size_t brute_force_count =
        brute_force_search(
            &query,
            vectors,
            vector_count,
            brute_force_results,
            k
        );

    size_t top_k_count =
        top_k_search(
            &query,
            vectors,
            vector_count,
            top_k_results,
            k
        );

    assert(brute_force_count == k);
    assert(top_k_count == k);

    for (size_t i = 0; i < k; i++) {
        assert(
            brute_force_results[i].index ==
            top_k_results[i].index
        );

        assert(
            brute_force_results[i].score ==
            top_k_results[i].score
        );
    }

    for (size_t i = 0; i < vector_count; i++) {
        vector_free(&vectors[i]);
    }

    free(vectors);
    vector_free(&query);

    printf("Top-K correctness test passed.\n");

    return 0;
}