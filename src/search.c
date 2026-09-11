#include "search.h"
#include "distance.h"

#include <stdlib.h>

static int compare_results(
    const void *a,
    const void *b
)
{
    const SearchResult *result_a = a;
    const SearchResult *result_b = b;

    if (result_a->score < result_b->score) {
        return 1;
    }

    if (result_a->score > result_b->score) {
        return -1;
    }

    return 0;
}

static void heap_swap(
    SearchResult *a,
    SearchResult *b
)
{
    SearchResult temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify_up(
    SearchResult *heap,
    size_t index
)
{
    while (index > 0) {
        size_t parent = (index - 1) / 2;

        if (heap[parent].score <= heap[index].score) {
            break;
        }

        heap_swap(
            &heap[parent],
            &heap[index]
        );

        index = parent;
    }
}

static void heapify_down(
    SearchResult *heap,
    size_t size,
    size_t index
)
{
    while (1) {
        size_t left = index * 2 + 1;
        size_t right = index * 2 + 2;
        size_t smallest = index;

        if (
            left < size &&
            heap[left].score < heap[smallest].score
        ) {
            smallest = left;
        }

        if (
            right < size &&
            heap[right].score < heap[smallest].score
        ) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        heap_swap(
            &heap[index],
            &heap[smallest]
        );

        index = smallest;
    }
}

size_t top_k_search(
    const Vector *query,
    const Vector *vectors,
    size_t vector_count,
    SearchResult *results,
    size_t k
)
{
    if (
        vector_count == 0 ||
        k == 0
    ) {
        return 0;
    }

    size_t heap_capacity =
        k < vector_count ? k : vector_count;

    SearchResult *heap =
        malloc(
            heap_capacity *
            sizeof(SearchResult)
        );

    if (heap == NULL) {
        return 0;
    }

    size_t heap_size = 0;

    for (size_t i = 0; i < vector_count; i++) {
        SearchResult result = {
            .index = i,
            .score = cosine_similarity(
                query,
                &vectors[i]
            )
        };

        if (heap_size < heap_capacity) {
            heap[heap_size] = result;
            heapify_up(heap, heap_size);
            heap_size++;

            continue;
        }

        if (result.score <= heap[0].score) {
            continue;
        }

        heap[0] = result;

        heapify_down(
            heap,
            heap_size,
            0
        );
    }

    for (size_t i = 0; i < heap_size; i++) {
        results[i] = heap[i];
    }

    qsort(
        results,
        heap_size,
        sizeof(SearchResult),
        compare_results
    );

    free(heap);

    return heap_size;
}

size_t brute_force_search(
    const Vector *query,
    const Vector *vectors,
    size_t vector_count,
    SearchResult *results,
    size_t k
)
{
    if (
        vector_count == 0 ||
        k == 0
    ) {
        return 0;
    }

    SearchResult *all_results =
        malloc(
            vector_count *
            sizeof(SearchResult)
        );

    if (all_results == NULL) {
        return 0;
    }

    for (size_t i = 0; i < vector_count; i++) {
        all_results[i].index = i;
        all_results[i].score =
            cosine_similarity(
                query,
                &vectors[i]
            );
    }

    qsort(
        all_results,
        vector_count,
        sizeof(SearchResult),
        compare_results
    );

    size_t result_count =
        k < vector_count
            ? k
            : vector_count;

    for (size_t i = 0; i < result_count; i++) {
        results[i] = all_results[i];
    }

    free(all_results);

    return result_count;
}