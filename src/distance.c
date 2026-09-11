#include "distance.h"
#include <math.h>

float dot_product(
    const Vector *a,
    const Vector *b)
{
    float result = 0.0f;

    for (size_t i = 0; i < a->dimension; i++)
    {
        result += a->data[i] * b->data[i];
    }

    return result;
}

float cosine_similarity(
    const Vector *a,
    const Vector *b)
{
    float dot = dot_product(a, b);

    float norm_a = 0.0f;
    float norm_b = 0.0f;

    for (size_t i = 0; i < a->dimension; i++)
    {
        norm_a += a->data[i] * a->data[i];
        norm_b += b->data[i] * b->data[i];
    }

    norm_a = sqrt(norm_a);
    norm_b = sqrt(norm_b);

    if (norm_a == 0.0f || norm_b == 0.0f){
        return 0.0f;
    }

    return dot / (norm_a * norm_b);
}

float euclidean_distance(
    const Vector *a,
    const Vector *b
){
    float result = 0.0f;

    for (size_t i=0;i < a->dimension;i++){
        float difference = a-> data[i] - b->data[i];

        result += difference * difference;
    }

    return sqrt(result);
}