#ifndef SEARCH_H
#define SEARCH_H

#include "vector.h"

#include <stddef.h>

typedef struct {
    size_t index;
    float score;
} SearchResult;

size_t brute_force_search(
    const Vector *query,
    const Vector *vectors,
    size_t vector_count,
    SearchResult *results,
    size_t k
);

size_t top_k_search(
    const Vector *query,
    const Vector *vectors,
    size_t vector_count,
    SearchResult *results,
    size_t k
);

#endif