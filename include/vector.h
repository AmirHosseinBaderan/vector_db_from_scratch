#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct{
    float *data;
    size_t dimension;
} Vector;

Vector vector_create(size_t dimension);
void vector_free(Vector *vector);

#endif