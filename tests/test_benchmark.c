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

static double benchmark(
    size_t vector_count,
    size_t dimension
)
{
    Vector query = vector_create(dimension);

    Vector *vectors =
        malloc(vector_count * sizeof(Vector));

    SearchResult *results =
        malloc(10 * sizeof(SearchResult));

    if (
        query.data == NULL ||
        vectors == NULL ||
        results == NULL
    ) {
        free(vectors);
        free(results);
        vector_free(&query);

        return -1.0;
    }

    fill_random_vector(&query);

    for (size_t i = 0; i < vector_count; i++) {
        vectors[i] = vector_create(dimension);
        fill_random_vector(&vectors[i]);
    }

    clock_t start = clock();

    brute_force_search(
        &query,
        vectors,
        vector_count,
        results,
        10
    );

    clock_t end = clock();

    for (size_t i = 0; i < vector_count; i++) {
        vector_free(&vectors[i]);
    }

    free(vectors);
    free(results);
    vector_free(&query);

    return (
        (double)(end - start)
        / CLOCKS_PER_SEC
    ) * 1000.0;
}

int main(void)
{
    srand(42);

    const size_t dimension = 128;

    size_t sizes[] = {
        1000,
        10000,
        100000,
        1000000
    };

    size_t count =
        sizeof(sizes) / sizeof(sizes[0]);

    printf(
        "Brute Force Benchmark\n"
        "Dimension: %zu\n\n",
        dimension
    );

    for (size_t i = 0; i < count; i++) {
        double elapsed =
            benchmark(sizes[i], dimension);

        printf(
            "%10zu vectors -> %.3f ms\n",
            sizes[i],
            elapsed
        );
    }

    return 0;
}