#ifndef DISTANCE_H
#define DISTANCE_H

#include "vector.h"

float dot_product(
    const Vector *a,
    const Vector *b
);

float cosine_similarity(
    const Vector *a,
    const Vector *b
);

float euclidean_distance(
    const Vector *a,
    const Vector *b
);

#endif