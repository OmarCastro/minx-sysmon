#ifndef NUMERIC_PARSER_H
#define NUMERIC_PARSER_H
#include "stdint.h"

char *itoa (int64_t i, unsigned int *len);
char *ftoa (double f,  unsigned int precision, unsigned int *len);
char *ultoa(uint64_t i, unsigned int *len);

char* stoi (char *pointer, int64_t &i);
char* stof (char* pointer, double &f);
char* stoul(char *pointer, uint64_t &ul);

#endif // NUMERIC_PARSER_H
