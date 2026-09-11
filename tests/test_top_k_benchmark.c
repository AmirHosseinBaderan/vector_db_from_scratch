#include "search.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void fill_random_vector(Vector *vector)
{
    for (size_t i = 0; i < vector->dimension; i++) {
        vector->data[i] =
            (float)rand() / (float)RAND_MAX;
    }
}

static double benchmark_brute_force(
    const Vector *query,
    const Vector *vectors,
    size_t vector_count,
    size_t k
)
{
    SearchResult *results =
        malloc(k * sizeof(SearchResult));

    clock_t start = clock();

    brute_force_search(
        query,
        vectors,
        vector_count,
        results,
        k
    );

    clock_t end = clock();

    free(results);

    return (
        (double)(end - start)
        / CLOCKS_PER_SEC
    ) * 1000.0;
}

static double benchmark_top_k(
    const Vector *query,
    const Vector *vectors,
    size_t vector_count,
    size_t k
)
{
    SearchResult *results =
        malloc(k * sizeof(SearchResult));

    clock_t start = clock();

    top_k_search(
        query,
        vectors,
        vector_count,
        results,
        k
    );

    clock_t end = clock();

    free(results);

    return (
        (double)(end - start)
        / CLOCKS_PER_SEC
    ) * 1000.0;
}

int main(void)
{
    srand(42);

    const size_t vector_count = 1000000;
    const size_t dimension = 128;
    const size_t k = 10;

    Vector query = vector_create(dimension);

    Vector *vectors =
        malloc(vector_count * sizeof(Vector));

    if (
        query.data == NULL ||
        vectors == NULL
    ) {
        free(vectors);
        vector_free(&query);

        return 1;
    }

    fill_random_vector(&query);

    for (size_t i = 0; i < vector_count; i++) {
        vectors[i] = vector_create(dimension);
        fill_random_vector(&vectors[i]);
    }

    double brute_force_ms =
        benchmark_brute_force(
            &query,
            vectors,
            vector_count,
            k
        );

    double top_k_ms =
        benchmark_top_k(
            &query,
            vectors,
            vector_count,
            k
        );

    printf(
        "Top-K Benchmark\n"
        "Vectors: %zu\n"
        "Dimension: %zu\n"
        "K: %zu\n\n",
        vector_count,
        dimension,
        k
    );

    printf(
        "Brute Force : %.3f ms\n",
        brute_force_ms
    );

    printf(
        "Heap Top-K  : %.3f ms\n",
        top_k_ms
    );

    for (size_t i = 0; i < vector_count; i++) {
        vector_free(&vectors[i]);
    }

    free(vectors);
    vector_free(&query);

    return 0;
}