#include "time.h"

#include "hashmap.h"
#include "default_hash.h"
#include "default_cmp.h"

DEC_HASHMAP(hash_map, str_ull_pair, char*, unsigned long long)
DEF_HASHMAP(hash_map, str_ull_pair, char*, unsigned long long)

typedef struct hash_map hash_map_t;
typedef struct str_ull_pair str_ull_pair_t;

void show_bucket_distribution(hash_map_t* map) {
    for (size_t i = 0; i < map->buckets.count; i++) {
        printf("Bucket %zu: {\n", i);

        struct hash_map_bucket* bucket = hash_map_bucket_list_get_ptr(
            &map->buckets, 
            i
        );

        for (size_t j = 0; j < bucket->count; j++) {
            str_ull_pair_t* pair = hash_map_bucket_get_ptr(
                bucket, 
                j
            );

            printf("    '%s': %llu\n", pair->key, pair->value);
        }

        printf("}, count: %zu\n\n", bucket->count);
    }
}

char* random_str(size_t len) {
    char* str = malloc(len + 1);

    for (size_t i = 0; i < len; i++) {
        str[i] = (rand() + 33) % 126;
    }

    str[len] = '\0';

    return str;
}

void free_str(str_ull_pair_t pair) {
    free(pair.key);
}

int main(void) {
    srand(time(NULL));

    hash_map_t map = make_hash_map(
        16, 
        default_hash( char* ),
        default_cmp( char* ),
        free_str
    );

    for (unsigned long long i = 0; i < 256; i++) {
        insert_str_ull_pair(
            &map,
            (str_ull_pair_t) { random_str( (i % 10) + 1), i*10/3 }
        );
    }

    show_bucket_distribution(&map);

    free_hashmap(&map);
}
