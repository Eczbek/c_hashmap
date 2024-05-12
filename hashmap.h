#include "vector.h"

#define DEC_HASHMAP(map_name, pair_name, key_type, val_type)                                                    \
                                                                                                                \
typedef typeof(key_type) map_name##_key_type;                                                                   \
typedef typeof(val_type) map_name##_val_type;                                                                   \
                                                                                                                \
struct pair_name {                                                                                              \
    map_name##_key_type key;                                                                                    \
    map_name##_val_type value;                                                                                  \
};                                                                                                              \
                                                                                                                \
DEC_VECTOR(map_name##_bucket, struct pair_name)                                                                 \
                                                                                                                \
DEC_VECTOR(map_name##_bucket_list, struct map_name##_bucket)                                                    \
                                                                                                                \
struct map_name {                                                                                               \
    struct map_name##_bucket_list buckets;                                                                      \
                                                                                                                \
    size_t (*hash) (map_name##_key_type);                                                                       \
    bool (*compare) (map_name##_key_type, map_name##_key_type);                                                 \
    void (*element_dtor) (struct pair_name);                                                                    \
};                                                                                                              \
                                                                                                                \
struct map_name make_##map_name(                                                                                \
    size_t initial_buckets,                                                                                     \
    size_t (*) (map_name##_key_type),                                                                           \
    bool (*) (map_name##_key_type, map_name##_key_type),                                                        \
    void (*) (struct pair_name)                                                                                 \
);                                                                                                              \
                                                                                                                \
void rehash_##map_name##_bucket(struct map_name*, struct map_name##_bucket*);                                   \
                                                                                                                \
void rehash_##map_name##_buckets(struct map_name*);                                                             \
                                                                                                                \
void insert_##pair_name(struct map_name*, struct pair_name);                                                    \
                                                                                                                \
struct pair_name* find_##pair_name(struct map_name*, map_name##_key_type);                                      \
                                                                                                                \
void remove_##pair_name(struct map_name*, map_name##_key_type);                                                 \
                                                                                                                \
void free_hashmap(struct map_name*);


#define DEF_HASHMAP(map_name, pair_name, key_type, val_type)                                                    \
                                                                                                                \
DEF_VECTOR(map_name##_bucket, struct pair_name)                                                                 \
                                                                                                                \
DEF_VECTOR(map_name##_bucket_list, struct map_name##_bucket)                                                    \
                                                                                                                \
struct map_name make_##map_name(                                                                                \
    size_t initial_buckets,                                                                                     \
    size_t (*hash) (map_name##_key_type),                                                                       \
    bool (*comparer) (map_name##_key_type, map_name##_key_type),                                                \
    void (*dtor) (struct pair_name)                                                                             \
) {                                                                                                             \
    struct map_name map;                                                                                        \
                                                                                                                \
    map.buckets = make_##map_name##_bucket_list(initial_buckets);                                               \
                                                                                                                \
    for (size_t i = 0; i < initial_buckets; i++) {                                                              \
        struct map_name##_bucket bucket = make_##map_name##_bucket(initial_buckets);                            \
                                                                                                                \
        bucket.dtor = dtor;                                                                                     \
        map_name##_bucket_list_push(                                                                            \
            &map.buckets,                                                                                       \
            bucket                                                                                              \
        );                                                                                                      \
    }                                                                                                           \
                                                                                                                \
    map.hash = hash;                                                                                            \
    map.compare = comparer;                                                                                     \
    map.element_dtor = dtor;                                                                                    \
                                                                                                                \
    return map;                                                                                                 \
}                                                                                                               \
                                                                                                                \
void rehash_##map_name##_bucket(struct map_name* map, struct map_name##_bucket* bucket) {                       \
    if (!map || !bucket || map->buckets.count <= 1 || !map->hash || bucket->count == 0) {                       \
        return;                                                                                                 \
    }                                                                                                           \
                                                                                                                \
    struct map_name##_bucket replace_bucket = *bucket;                                                          \
                                                                                                                \
    map_name##_bucket_clear(bucket, false);                                                                     \
                                                                                                                \
    for (size_t i = 0; i < replace_bucket.count; i++) {                                                         \
        struct pair_name pair = map_name##_bucket_get(&replace_bucket, i);                                      \
                                                                                                                \
        size_t new_hash = map->hash(                                                                            \
            pair.key                                                                                            \
        ) % map->buckets.count;                                                                                 \
                                                                                                                \
        struct map_name##_bucket* insert_bucket = map_name##_bucket_list_get_ptr(                               \
            &map->buckets,                                                                                      \
            new_hash                                                                                            \
        );                                                                                                      \
                                                                                                                \
        map_name##_bucket_push(insert_bucket, pair);                                                            \
    }                                                                                                           \
}                                                                                                               \
                                                                                                                \
void rehash_##map_name##_buckets(struct map_name* map) {                                                        \
    if (!map || map->buckets.count == 0 || !map->hash) {                                                        \
        return;                                                                                                 \
    }                                                                                                           \
                                                                                                                \
    for (size_t i = 0; i < map->buckets.count; i++) {                                                           \
        rehash_##map_name##_bucket(                                                                             \
            map,                                                                                                \
            map_name##_bucket_list_get_ptr(&map->buckets, i)                                                    \
        );                                                                                                      \
    }                                                                                                           \
}                                                                                                               \
                                                                                                                \
void insert_##pair_name(struct map_name* map, struct pair_name pair) {                                          \
    if (!map || map->buckets.count == 0 || !map->hash) {                                                        \
        return;                                                                                                 \
    }                                                                                                           \
                                                                                                                \
    size_t pair_hash = map->hash(                                                                               \
        pair.key                                                                                                \
    ) % map->buckets.count;                                                                                     \
                                                                                                                \
    struct map_name##_bucket* insert_bucket = map_name##_bucket_list_get_ptr(                                   \
        &map->buckets,                                                                                          \
        pair_hash                                                                                               \
    );                                                                                                          \
                                                                                                                \
    if ( insert_bucket->count >= ( (map->buckets.count / 2 + map->buckets.count / 4) ) ) {                      \
        map_name##_bucket_list_push(                                                                            \
            &map->buckets,                                                                                      \
            make_##map_name##_bucket(4)                                                                         \
        );                                                                                                      \
                                                                                                                \
        rehash_##map_name##_buckets(map);                                                                       \
                                                                                                                \
        pair_hash = map->hash(                                                                                  \
            pair.key                                                                                            \
        ) % map->buckets.count;                                                                                 \
                                                                                                                \
        insert_bucket = map_name##_bucket_list_get_ptr(                                                         \
            &map->buckets,                                                                                      \
            pair_hash                                                                                           \
        );                                                                                                      \
    }                                                                                                           \
                                                                                                                \
    map_name##_bucket_push(                                                                                     \
        insert_bucket,                                                                                          \
        pair                                                                                                    \
    );                                                                                                          \
}                                                                                                               \
                                                                                                                \
struct pair_name* find_##pair_name(struct map_name* map, map_name##_key_type key) {                             \
    if (!map || map->buckets.count == 0 || !map->hash || !map->compare) {                                       \
        return NULL;                                                                                            \
    }                                                                                                           \
                                                                                                                \
    size_t key_hash = map->hash(                                                                                \
        key                                                                                                     \
    ) % map->buckets.count;                                                                                     \
                                                                                                                \
    struct map_name##_bucket* retrieve_bucket = map_name##_bucket_list_get_ptr(                                 \
        &map->buckets,                                                                                          \
        key_hash                                                                                                \
    );                                                                                                          \
                                                                                                                \
    for (size_t i = 0; i < retrieve_bucket->count; i++) {                                                       \
        struct pair_name* cmp_pair = map_name##_bucket_get_ptr(                                                 \
            retrieve_bucket,                                                                                    \
            i                                                                                                   \
        );                                                                                                      \
                                                                                                                \
        if (map->compare(cmp_pair->key, key)) {                                                                 \
            return cmp_pair;                                                                                    \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    return NULL;                                                                                                \
}                                                                                                               \
                                                                                                                \
void remove_##pair_name(struct map_name* map, map_name##_key_type key) {                                        \
    if (!map || map->buckets.count == 0 || !map->hash || !map->compare) {                                       \
        return;                                                                                                 \
    }                                                                                                           \
                                                                                                                \
    size_t key_hash = map->hash(                                                                                \
        key                                                                                                     \
    ) % map->buckets.count;                                                                                     \
                                                                                                                \
    struct map_name##_bucket* remove_bucket = map_name##_bucket_list_get_ptr(                                   \
        &map->buckets,                                                                                          \
        key_hash                                                                                                \
    );                                                                                                          \
                                                                                                                \
    for (size_t i = 0; i < remove_bucket->count; i++) {                                                         \
        struct pair_name* cmp_pair = map_name##_bucket_get_ptr(                                                 \
            remove_bucket,                                                                                      \
            i                                                                                                   \
        );                                                                                                      \
                                                                                                                \
        if (map->compare(cmp_pair->key, key)) {                                                                 \
            map_name##_bucket_remove(                                                                           \
                remove_bucket,                                                                                  \
                i                                                                                               \
            );                                                                                                  \
                                                                                                                \
            break;                                                                                              \
        }                                                                                                       \
    }                                                                                                           \
}                                                                                                               \
                                                                                                                \
void free_hashmap(struct map_name* map) {                                                                       \
    for (size_t i = 0; i < map->buckets.count; i++) {                                                           \
        struct map_name##_bucket* current_bucket = map_name##_bucket_list_get_ptr(                              \
            &map->buckets,                                                                                      \
            i                                                                                                   \
        );                                                                                                      \
                                                                                                                \
        free_##map_name##_bucket(                                                                               \
            current_bucket                                                                                      \
        );                                                                                                      \
    }                                                                                                           \
                                                                                                                \
    free_##map_name##_bucket_list(&map->buckets);                                                               \
}



                    