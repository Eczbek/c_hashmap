#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "default_hash.h"

static size_t identity(int key) {
    return (size_t)abs(key);
}

size_t hash_sbyte(signed char key) {
    return identity(key);
}

size_t hash_byte(unsigned char key) {
    return identity(key);
}

size_t hash_short(signed short key) {
    return identity(key);
}

size_t hash_ushort(unsigned short key) {
    return identity(key);
}

size_t hash_int(signed int key) {
    return identity(key);
}

size_t hash_uint(unsigned int key) {
    return identity(key);
}

size_t hash_long(signed long key) {
    return identity(key);
}

size_t hash_ulong(unsigned long key) {
    return identity(key);
}

size_t hash_llong(signed long long key) {
    return hash_ullong( (unsigned long long)llabs(key)  );
}

size_t hash_ullong(unsigned long long key) {
    const size_t ull_size = sizeof(unsigned long long);

    size_t final = 0;

    unsigned char* bytes = malloc(ull_size);
    memcpy(bytes, &key, ull_size);

    for (size_t i = 0; i < ull_size; i++) {
        final += (size_t)bytes[i];
    }

    free(bytes);

    return final;
}

size_t hash_float(float key) {
    const float phi = 1.618033988749f;

    float fibonacci = powf(2.f, key) / phi;

    return (size_t)floorf(fibonacci);
}

size_t hash_double(double key) {
    const float phi = 1.618033988749;

    double fibonacci = pow(2.0, key) / phi;

    return (size_t)floor(fibonacci);
}

size_t hash_ldouble(long double key) {
    const long double phi = 1.618033988749L;

    float fibonacci = powl(2.0L, key) / phi;

    return (size_t)floorl(fibonacci);
}

size_t hash_string(char* key) {
    size_t hash = 2166136261u;

    for (char* it = key; *it != '\0'; ++it) {
        hash ^= (unsigned char)*it;
        hash *= 16777619u;
    }

    return hash;
}