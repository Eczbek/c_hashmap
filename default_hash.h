#ifndef DEFAULT_HASH_H
#define DEFAULT_HASH_H

size_t hash_sbyte(signed char key);
size_t hash_byte(unsigned char key);

size_t hash_short(signed short key);
size_t hash_ushort(unsigned short key);

size_t hash_int(signed int key);
size_t hash_uint(unsigned int key);

size_t hash_long(signed long key);
size_t hash_ulong(unsigned long key);

size_t hash_llong(signed long long key);
size_t hash_ullong(unsigned long long key);

size_t hash_float(float key);

size_t hash_double(double key);
size_t hash_ldouble(long double key);

size_t hash_string(char* key);

#define default_hash(_ty) _Generic( ( (_ty) {0} ),                  \
/*BYTE*/                                                            \
    signed char: hash_sbyte,                                        \
    unsigned char: hash_byte,                                       \
/*SHORT*/                                                           \
    signed short: hash_short,                                       \
    unsigned short: hash_ushort,                                    \
/*INT*/                                                             \
    signed int: hash_int,                                           \
    unsigned int: hash_uint,                                        \
/*LONG*/                                                            \
    signed long: hash_long,                                         \
    unsigned long: hash_ulong,                                      \
/*LONG LONG*/                                                       \
    signed long long: hash_llong,                                   \
    unsigned long long: hash_ullong,                                \
/*FLOAT*/                                                           \
    float: hash_float,                                              \
/*DOUBLE*/                                                          \
    double: hash_double,                                            \
    long double: hash_ldouble,                                      \
/*STRING*/                                                          \
    char*: hash_string                                              \
    )

#endif