#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "vector.h"

typedef struct {
    int key;
    int value;
} int_int_pair_t;

typedef int map_key_type;
typedef int map_key_value;

DEC_VECTOR(bucket, int_int_pair_t)
DEF_VECTOR(bucket, int_int_pair_t)

DEC_VECTOR(bucket_list, struct bucket)
DEF_VECTOR(bucket_list, struct bucket)

typedef struct hash_map {
    struct bucket_list buckets;

    size_t (*hash) (map_key_type);
    bool (*compare) (map_key_type, map_key_type);
    void (*destruct_elements) (int_int_pair_t);
} hash_map_t;

hash_map_t make_map( 
    size_t initial_buckets,
    size_t (*hash) (map_key_type),
    bool (*comparer) (map_key_type, map_key_type),
    void (*dtor) (int_int_pair_t)
) {
    hash_map_t map;

    map.buckets = make_bucket_list(initial_buckets);

    for (size_t i = 0; i < initial_buckets; i++) {
        bucket_list_push(
            &map.buckets, 
            make_bucket(0)
        );
    }

    map.hash = hash;
    map.compare = comparer;
    map.destruct_elements = dtor;

    return map;
}

void rehash_map_bucket(hash_map_t* map, struct bucket* bucket) {
    if (!map || !bucket || map->buckets.count <= 1 || !map->hash || bucket->count == 0) {
        return;
    }

    for (size_t i = bucket->count - 1; i > 0; i--) {
        size_t new_hash = map->hash(
            bucket_get_ptr(bucket, i)->key
        ) % map->buckets.count;

        struct bucket* new_bucket = bucket_list_get_ptr(
            &map->buckets, 
            new_hash
        );

        bucket_push(
            new_bucket,
            bucket_get(bucket, i)
        );

        bucket_pop(bucket);
    }
}

void rehash_map_buckets(hash_map_t* map) {
    if (!map || map->buckets.count == 0 || !map->hash) {
        return;
    }

    for (size_t i = 0; i < map->buckets.count; i++) {
        rehash_map_bucket(
            map,
            bucket_list_get_ptr(&map->buckets, i)
        );
    }
}

void insert_pair(hash_map_t* map, int_int_pair_t pair) {
    if (!map || map->buckets.count == 0 || !map->hash) {
        return;
    }

    size_t pair_hash = map->hash(
        pair.key
    ) % map->buckets.count;

    struct bucket* insert_bucket = bucket_list_get_ptr(
        &map->buckets, 
        pair_hash
    );

    if ( insert_bucket->count >= ( (map->buckets.count / 2 + map->buckets.count / 4) ) ) {
        bucket_list_push(
            &map->buckets, 
            make_bucket(4)
        );

        rehash_map_buckets(map);

        printf("New bucket added!\n");

        pair_hash = map->hash(
            pair.key
        ) % map->buckets.count;

        insert_bucket = bucket_list_get_ptr(
        &map->buckets, 
        pair_hash
        );
    }

    bucket_push(
        insert_bucket, 
        pair
    );
}

int_int_pair_t* find_pair(hash_map_t* map, int key) {
    if (!map || map->buckets.count == 0 || !map->hash || !map->compare) {
        return NULL;
    }

    size_t key_hash = map->hash(
        key
    ) % map->buckets.count;

    struct bucket* retrieve_bucket = bucket_list_get_ptr(
        &map->buckets,
        key_hash
    );

    for (size_t i = 0; i < retrieve_bucket->count; i++) {
        int_int_pair_t* cmp_pair = bucket_get_ptr(
            retrieve_bucket, 
            i
        );

        if (map->compare(cmp_pair->key, key)) {
            return cmp_pair;
        }
    }

    return NULL;
}

void remove_pair(hash_map_t* map, int key) {
    if (!map || map->buckets.count == 0 || !map->hash || !map->compare) {
        return;
    }

    size_t key_hash = map->hash(
        key
    ) % map->buckets.count;

    struct bucket* remove_bucket = bucket_list_get_ptr(
        &map->buckets,
        key_hash
    );

    for (size_t i = 0; i < remove_bucket->count; i++) {
        int_int_pair_t* cmp_pair = bucket_get_ptr(
            remove_bucket, 
            i
        );

        if (map->compare(cmp_pair->key, key)) {
            bucket_remove(
                remove_bucket, 
                i
            );

            break;
        }
    }
}

void free_hashmap(hash_map_t* map) {
    for (size_t i = 0; i < map->buckets.count; i++) {
        struct bucket* current_bucket = bucket_list_get_ptr(
            &map->buckets, 
            i
        );

        if (!map->destruct_elements) {
            goto b_free;
        }

        for (size_t i = 0; i < current_bucket->count; i++) {
            map->destruct_elements(
                bucket_get(
                    current_bucket, 
                    i
                )
            );
        }

        b_free:;
        free_bucket(
            current_bucket
        );
    }

    free_bucket_list(&map->buckets);
}

bool cmp_int(int _1, int _2) {
    return _1 == _2;
}

size_t hash_int(int key) {
    return abs(key);
}

void show_bucket_distribution(hash_map_t* map) {
    if (!map || !map->buckets.count) {
        return;
    }

    for (size_t i = 0; i < map->buckets.count; i++) {
        struct bucket* current_bucket = bucket_list_get_ptr(
            &map->buckets, 
            i
        );

        printf("In bucket %zu: {\n", i);

        for (size_t j = 0; j < current_bucket->count; j++) {
            int_int_pair_t pair = bucket_get(
                current_bucket, 
                j
            );

            printf("    %i: %i,\n", pair.key, pair.value);
        }

        printf("}\n");
    }
}

int main(void) {
    srand(time(NULL));

    hash_map_t map = make_map(
        16, 
        hash_int,
        cmp_int,
        NULL
    );

    for (int i = 0; i < 64; i++) {
        insert_pair(
        &map,
        (int_int_pair_t) { i, rand() }
        );
    }

    show_bucket_distribution(&map);

    free_hashmap(&map);
}