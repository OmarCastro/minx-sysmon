#ifndef NUMERIC_PARSER_H
#define NUMERIC_PARSER_H
#include <stdint.h>
#include <type_traits>
#include "fast_math.h"


char *itoa (int64_t i, unsigned int *len);
char *ftoa (double f,  unsigned int precision, unsigned int *len);
char *ultoa(uint64_t i, unsigned int *len);

char* stof (char* pointer, double &f);



template<class T ,
         typename std::enable_if<std::is_integral<T>::value
                              && std::is_signed<T>::value>::type* = nullptr >
char* stoi(char* p, T &i)
{
    while (*p == ' ') p++;
    int s = 1;
    if (*p == '-') {
        s = -1; p++;
    }
    T acc = 0;
    while (isDigit(*p))
        acc = acc * 10 + *p++ - '0';
    i = s * acc;
    return p;
}

template<class T ,
         typename std::enable_if<std::is_integral<T>::value
                              && std::is_unsigned<T>::value>::type* = nullptr >
char* stoul(char* p, T &i)
{
    while (*p == ' ') p++;
    T acc = 0;
    while (isDigit(*p))
        acc = acc * 10 + *p++ - '0';
    i = acc;
    return p;
}

#endif // NUMERIC_PARSER_H
