#include "search.h"
#include "vector.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    Vector query = vector_create(2);
    query.data[0] = 1.0f;
    query.data[1] = 0.0f;

    Vector vectors[4];

    for (int i = 0; i < 4; i++) {
        vectors[i] = vector_create(2);
    }

    vectors[0].data[0] = 1.0f;
    vectors[0].data[1] = 0.0f;

    vectors[1].data[0] = 0.9f;
    vectors[1].data[1] = 0.1f;

    vectors[2].data[0] = 0.0f;
    vectors[2].data[1] = 1.0f;

    vectors[3].data[0] = -1.0f;
    vectors[3].data[1] = 0.0f;

    SearchResult results[2];

    size_t count = brute_force_search(
        &query,
        vectors,
        4,
        results,
        2
    );

    assert(count == 2);

    assert(results[0].index == 0);
    assert(results[1].index == 1);

    assert(
        fabsf(results[0].score - 1.0f)
        < 0.0001f
    );

    assert(
        results[1].score > 0.9f
    );

    for (int i = 0; i < 4; i++) {
        vector_free(&vectors[i]);
    }

    vector_free(&query);

    printf("All search tests passed.\n");

    return 0;
}