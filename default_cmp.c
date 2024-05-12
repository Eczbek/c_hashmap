#include <string.h>

#include "default_cmp.h"

bool cmp_sbyte(signed char _1, signed char _2) {
    return _1 == _2;
}

bool cmp_byte(unsigned char _1, unsigned char _2) {
    return _1 == _2;
}

bool cmp_short(signed short _1, signed short _2) {
    return _1 == _2;
}

bool cmp_ushort(unsigned short _1, unsigned short _2) {
    return _1 == _2;
}

bool cmp_int(signed int _1, signed int _2) {
    return _1 == _2;
}

bool cmp_uint(unsigned int _1, unsigned int _2) {
    return _1 == _2;
}

bool cmp_long(signed long _1, signed long _2) {
    return _1 == _2;
}

bool cmp_ulong(unsigned long _1, unsigned long _2) {
    return _1 == _2;
}

bool cmp_llong(signed long long _1, signed long long _2) {
    return _1 == _2;
}

bool cmp_ullong(unsigned long long _1, unsigned long long _2) {
    return _1 == _2;
}

bool cmp_float(float _1, float _2) {
    return _1 == _2;
}

bool cmp_double(double _1, double _2) {
    return _1 == _2;
}

bool cmp_ldouble(long double _1, long double _2) {
    return _1 == _2;
}

bool cmp_string(char* _1, char* _2) {
    return !strcmp(_1, _2);
}