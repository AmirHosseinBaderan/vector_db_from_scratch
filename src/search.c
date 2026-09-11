#include "search.h"
#include "distance.h"

#include <stdlib.h>

static int compare_results(
    const void *a,
    const void *b
){
    const SearchResult *result_a = a;
    const SearchResult *result_b = b;

    if(result_a->score < result_b-> score){
        return 1;
    }

    if(result_a-> score > result_b-> score){
        return -1;
    }

    return 0;
}

size_t brute_force_search(
    const Vector *query,
    const Vector *vectors,
    size_t vector_count,
    SearchResult *results,
    size_t k
){
    if(vector_count == 0 || k == 0){
        return 0;
    }

    SearchResult *all_results = malloc(vector_count * sizeof(SearchResult));

    if(all_results == NULL){
        return 0;
    }

    for (size_t i=0;i< vector_count;i++){
        all_results[i].index = i;
        all_results[i].score = cosine_similarity(query,&vectors[i]);
    }

    qsort(
        all_results,
        vector_count,
        sizeof(SearchResult),
        compare_results
    );

    size_t  result_count = k < vector_count ? k : vector_count;

    for (size_t i =0;i < result_count;i++){
        results[i] = all_results[i];
    }

    free(all_results);

    return result_count;
}
