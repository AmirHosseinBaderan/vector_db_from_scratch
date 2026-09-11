#include "distance.h"
#include "vector.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    Vector a = vector_create(3);
    Vector b = vector_create(3);

    a.data[0] = 1.0f;
    a.data[1] = 2.0f;
    a.data[2] = 3.0f;

    b.data[0] = 4.0f;
    b.data[1] = 5.0f;
    b.data[2] = 6.0f;

    assert(fabsf(dot_product(&a, &b) - 32.0f) < 0.0001f);

    assert(
        fabsf(
            euclidean_distance(&a, &b)
            - sqrtf(27.0f)
        ) < 0.0001f
    );

    Vector x = vector_create(3);
    Vector y = vector_create(3);

    x.data[0] = 1.0f;
    x.data[1] = 0.0f;
    x.data[2] = 0.0f;

    y.data[0] = 1.0f;
    y.data[1] = 0.0f;
    y.data[2] = 0.0f;

    assert(
        fabsf(cosine_similarity(&x, &y) - 1.0f)
        < 0.0001f
    );

    vector_free(&a);
    vector_free(&b);
    vector_free(&x);
    vector_free(&y);

    printf("All distance tests passed.\n");

    return 0;
}