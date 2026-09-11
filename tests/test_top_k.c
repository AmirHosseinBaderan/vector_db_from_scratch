#include "search.h"
#include "vector.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    Vector query = vector_create(2);

    query.data[0] = 1.0f;
    query.data[1] = 0.0f;

    Vector vectors[5];

    for (int i = 0; i < 5; i++) {
        vectors[i] = vector_create(2);
    }

    vectors[0].data[0] = 1.0f;
    vectors[0].data[1] = 0.0f;

    vectors[1].data[0] = 0.9f;
    vectors[1].data[1] = 0.1f;

    vectors[2].data[0] = 0.5f;
    vectors[2].data[1] = 0.5f;

    vectors[3].data[0] = 0.0f;
    vectors[3].data[1] = 1.0f;

    vectors[4].data[0] = -1.0f;
    vectors[4].data[1] = 0.0f;

    SearchResult results[3];

    size_t count = top_k_search(
        &query,
        vectors,
        5,
        results,
        3
    );

    assert(count == 3);

    assert(results[0].index == 0);
    assert(results[1].index == 1);
    assert(results[2].index == 2);

    for (int i = 0; i < 5; i++) {
        vector_free(&vectors[i]);
    }

    vector_free(&query);

    printf("All top-k tests passed.\n");

    return 0;
}