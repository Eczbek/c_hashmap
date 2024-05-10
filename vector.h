#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define DEC_VECTOR(container_name, ty)                                                                      \
                                                                                                            \
typedef typeof(ty) container_name##_element;                                                                \
                                                                                                            \
struct container_name {                                                                                     \
    size_t count;                                                                                           \
    size_t capacity;                                                                                        \
    size_t typesize;                                                                                        \
                                                                                                            \
    container_name##_element* data;                                                                         \
                                                                                                            \
    void (*dtor)(container_name##_element*);                                                                \
};                                                                                                          \
                                                                                                            \
struct container_name make_##container_name(size_t);                                                        \
                                                                                                            \
void free_##container_name(struct container_name*);                                                         \
                                                                                                            \
void container_name##_check_valid(struct container_name*);                                                  \
                                                                                                            \
void container_name##_reserve(struct container_name*, size_t);                                              \
                                                                                                            \
container_name##_element container_name##_get(struct container_name*, size_t);                              \
                                                                                                            \
container_name##_element* container_name##_get_ptr(struct container_name*, size_t);                         \
                                                                                                            \
void container_name##_set(struct container_name*, container_name##_element, size_t);                        \
                                                                                                            \
void container_name##_push(struct container_name*, container_name##_element);                               \
                                                                                                            \
void container_name##_pop(struct container_name*);                                                          \
                                                                                                            \
void container_name##_insert(struct container_name*,                                                        \
    container_name##_element,                                                                               \
    size_t);                                                                                                \
                                                                                                            \
void container_name##_insert_range(struct container_name*,                                                  \
    size_t,                                                                                                 \
    container_name##_element*,                                                                              \
    container_name##_element*);                                                                             \
                                                                                                            \
void container_name##_remove(struct container_name*, size_t);                                               \
                                                                                                            \
void container_name##_remove_range(struct container_name*,                                                  \
    size_t,                                                                                                 \
    size_t);                                                                                                \
                                                                                                            \
bool is_##container_name##_empty(struct container_name const*);                                          

#define DEF_VECTOR(container_name, ty)                                                                      \
                                                                                                            \
struct container_name make_##container_name(size_t initial_cap) {                                           \
    size_t elem_size = sizeof(container_name##_element);                                                    \
                                                                                                            \
    struct container_name v = {                                                                             \
        0,                                                                                                  \
        initial_cap,                                                                                        \
        elem_size,                                                                                          \
        calloc(initial_cap, elem_size),                                                                     \
        NULL                                                                                                \
    };                                                                                                      \
                                                                                                            \
    if (!v.data) {                                                                                          \
        fprintf(stderr, "Vector allocation failed: program out of memory");                                 \
    }                                                                                                       \
                                                                                                            \
    return v;                                                                                               \
}                                                                                                           \
                                                                                                            \
void free_##container_name(struct container_name* vector) {                                                 \
    if (!vector->data) {                                                                                    \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    if (vector->dtor) {                                                                                     \
        for (size_t i = 0; i < vector->count; i++) {                                                        \
            vector->dtor(&vector->data[i]);                                                                 \
        }                                                                                                   \
    }                                                                                                       \
                                                                                                            \
    free(vector->data);                                                                                     \
    vector->data = NULL;                                                                                    \
}                                                                                                           \
                                                                                                            \
void container_name##_check_valid(struct container_name* vector) {                                          \
    assert(vector && vector->data);                                                                         \
}                                                                                                           \
                                                                                                            \
void container_name##_reserve(struct container_name* vector, size_t new_size) {                             \
    if (!vector) {                                                                                          \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    if (!vector->data) {                                                                                    \
        vector->data = calloc(new_size, sizeof(container_name##_element));                                     \
                                                                                                            \
        if (!vector->data) {                                                                                \
            fprintf(stderr, "Vector allocation failed: program out of memory");                             \
                                                                                                            \
            return;                                                                                         \
        }                                                                                                   \
                                                                                                            \
        vector->capacity = new_size;                                                                        \
                                                                                                            \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    void* new_mem = realloc(vector->data, new_size * sizeof(container_name##_element));                     \
                                                                                                            \
    if (!new_mem) {                                                                                         \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    vector->data = new_mem;                                                                                 \
    vector->capacity = new_size;                                                                            \
}                                                                                                           \
                                                                                                            \
void container_name##_set(struct container_name* vector, container_name##_element elem, size_t pos) {       \
    container_name##_check_valid(vector);                                                                   \
                                                                                                            \
    if (pos >= vector->count) {                                                                             \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    vector->data[pos] = elem;                                                                               \
}                                                                                                           \
container_name##_element container_name##_get(struct container_name* vector, size_t element_position) {     \
    container_name##_check_valid(vector);                                                                   \
                                                                                                            \
    if (element_position >= vector->count) {                                                                \
        return vector->data[vector->count - 1];                                                             \
    }                                                                                                       \
                                                                                                            \
    return vector->data[element_position];                                                                  \
}                                                                                                           \
                                                                                                            \
container_name##_element* container_name##_get_ptr(struct container_name* vector, size_t element_position){ \
    container_name##_check_valid(vector);                                                                   \
                                                                                                            \
    if (element_position >= vector->count) {                                                                \
        return &vector->data[vector->count - 1];                                                            \
    }                                                                                                       \
                                                                                                            \
    return &(vector->data[element_position]);                                                               \
}                                                                                                           \
                                                                                                            \
void container_name##_push(struct container_name* vector, container_name##_element element) {               \
    container_name##_check_valid(vector);                                                                   \
                                                                                                            \
    if (vector->count == vector->capacity) {                                                                \
        container_name##_reserve(vector, vector->capacity * 2 + 1);                                         \
    }                                                                                                       \
                                                                                                            \
    vector->data[vector->count++] = element;                                                                \
}                                                                                                           \
                                                                                                            \
void container_name##_pop(struct container_name* vector) {                                                  \
    if (!vector || !vector->data || !vector->count) {                                                       \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    if (vector->dtor) {                                                                                     \
        vector->dtor(&vector->data[vector->count - 1]);                                                     \
    }                                                                                                       \
                                                                                                            \
    vector->count--;                                                                                        \
}                                                                                                           \
                                                                                                            \
void container_name##_insert(struct container_name* vector,                                                 \
    container_name##_element element,                                                                       \
    size_t insert_position) {                                                                               \
                                                                                                            \
    container_name##_check_valid(vector);                                                                      \
                                                                                                            \
    if (vector->count == vector->capacity) {                                                                \
        container_name##_reserve(vector, vector->capacity * 2 + 1);                                            \
    }                                                                                                       \
                                                                                                            \
    if (insert_position > vector->count) {                                                                  \
        container_name##_insert(vector, element, 0);                                                             \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    for (size_t i = vector->count; i > insert_position - 1; i--) {                                          \
        vector->data[i] = vector->data[i+1];                                                                \
    }                                                                                                       \
                                                                                                            \
    vector->data[insert_position] = element;                                                                \
}                                                                                                           \
                                                                                                            \
void container_name##_insert_range(struct container_name* vector,                                           \
    size_t insert_pos,                                                                                      \
    container_name##_element* insert_begin,                                                                 \
    container_name##_element* insert_end) {                                                                 \
                                                                                                            \
    container_name##_check_valid(vector);                                                                      \
                                                                                                            \
    ptrdiff_t range_size = insert_end - insert_begin;                                                       \
                                                                                                            \
    if ( range_size <= 0 ) {                                                                                \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    if (vector->count + range_size >= vector->capacity) {                                                   \
        container_name##_reserve(vector, vector->capacity * 2 + range_size);                                   \
    }                                                                                                       \
                                                                                                            \
    for (size_t i = vector->count + range_size; i > insert_pos; i--) {                                      \
        vector->data[i] = vector->data[i - range_size];                                                     \
    }                                                                                                       \
                                                                                                            \
    container_name##_element* iter = insert_begin;                                                          \
                                                                                                            \
    while (iter != insert_end) {                                                                            \
        vector->data[insert_pos + (iter - insert_begin)] = *iter;                                           \
        iter++;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    vector->count += range_size;                                                                            \
}                                                                                                           \
                                                                                                            \
void container_name##_remove(struct container_name* vector, size_t remove_pos) {                            \
    container_name##_check_valid(vector);                                                                      \
                                                                                                            \
    if (vector->count == 0 || remove_pos >= vector->count) {                                                \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    if (vector->dtor) {                                                                                     \
        vector->dtor(&vector->data[remove_pos]);                                                            \
    }                                                                                                       \
                                                                                                            \
    for (size_t i = remove_pos; i < vector->count - 1; i++) {                                               \
        vector->data[i] = vector->data[i+1];                                                                \
    }                                                                                                       \
}                                                                                                           \
                                                                                                            \
void container_name##_remove_range(struct container_name* vector,                                           \
    size_t removeStart,                                                                                     \
    size_t count) {                                                                                         \
    container_name##_check_valid(vector);                                                                      \
                                                                                                            \
    if (vector->capacity < removeStart + count || vector->count < count) {                                  \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    if (vector->dtor) {                                                                                     \
        for (size_t i = removeStart; i < removeStart + count; i++) {                                        \
            vector->dtor(&vector->data[i]);                                                                 \
        }                                                                                                   \
    }                                                                                                       \
                                                                                                            \
    for (size_t i = removeStart; i < vector->count - count; i++) {                                          \
        vector->data[i] = vector->data[i+count];                                                            \
    }                                                                                                       \
                                                                                                            \
    vector->count -= count;                                                                                 \
}                                                                                                           \
                                                                                                            \
bool is_##container_name##_empty(struct container_name const* vector) {                                     \
    if (!vector) {                                                                                          \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    return vector->count == 0;                                                                              \
}

#endif
