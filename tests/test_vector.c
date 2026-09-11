#include "vector.h"

#include <assert.h>
#include <stdio.h>

int main(void){
    Vector vector = vector_create(3);

    assert(vector.dimension == 3);
    assert(vector.data != NULL);

    vector.data[0] = 1.0f;
    vector.data[1] = 2.0f;
    vector.data[2] = 3.0f;

    assert(vector.data[0] == 1.0f);
    assert(vector.data[1] == 2.0f);
    assert(vector.data[2] == 3.0f);

    vector_free(&vector);

    assert(vector.data == NULL);
    assert(vector.dimension == 0);

    printf("All vector tests passed \n");

    return 0;
}