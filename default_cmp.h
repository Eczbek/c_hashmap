#ifndef DEFAULT_CMP_H
#define DEFAULT_CMP_H

#include <stdbool.h>

bool cmp_sbyte(signed char _1, signed char _2);
bool cmp_byte(unsigned char _1, unsigned char _2);

bool cmp_short(signed short _1, signed short _2);
bool cmp_ushort(unsigned short _1, unsigned short _2);

bool cmp_int(signed int _1, signed int _2);
bool cmp_uint(unsigned int _1, unsigned int _2);

bool cmp_long(signed long _1, signed long _2);
bool cmp_ulong(unsigned long _1, unsigned long _2);

bool cmp_llong(signed long long _1, signed long long _2);
bool cmp_ullong(unsigned long long _1, unsigned long long _2);

bool cmp_float(float _1, float _2);

bool cmp_double(double _1, double _2);
bool cmp_ldouble(long double _1, long double _2);

bool cmp_string(char* _1, char* _2);

#define default_cmp(_ty) _Generic( ( (_ty) {0} ),                    \
/*BYTE*/                                                            \
    signed char: cmp_sbyte,                                         \
    unsigned char: cmp_byte,                                        \
/*SHORT*/                                                           \
    signed short: cmp_short,                                        \
    unsigned short: cmp_ushort,                                     \
/*INT*/                                                             \
    signed int: cmp_int,                                            \
    unsigned int: cmp_uint,                                         \
/*LONG*/                                                            \
    signed long: cmp_long,                                          \
    unsigned long: cmp_ulong,                                       \
/*LONG LONG*/                                                       \
    signed long long: cmp_llong,                                    \
    unsigned long long: cmp_ullong,                                 \
/*FLOAT*/                                                           \
    float: cmp_float,                                               \
/*DOUBLE*/                                                          \
    double: cmp_double,                                             \
    long double: cmp_ldouble,                                       \
/*STRING*/                                                          \
    char*: cmp_string                                               \
)

#endif
