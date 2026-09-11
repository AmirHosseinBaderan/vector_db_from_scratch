#include "vector.h"
#include <stdlib.h>

Vector vector_create(size_t dimension){
    Vector vector;

    vector.dimension = dimension;
    vector.data = malloc(
        dimension * sizeof(float)
    );

    return vector;
}

void vector_free(Vector *vector){
    if(vector == NULL){
        return;
    }

    free(vector-> data);

    vector -> data = NULL;
    vector -> dimension = 0;
}