#include "numeric_parser.h"

#define NUM_DIGITS 64		/* enough for 64 bit integer and floats */
static char num_buf[NUM_DIGITS];  /* Room for INT_DIGITS digits, - and '\0' */
static char* const num_buf_end =  num_buf + NUM_DIGITS - 1;


char *itoa(int64_t i, unsigned int *len){
  char *p = num_buf_end;	/* points to terminating '\0' */
  bool isNegative = (i < 0);
  do {
    *--p = '0' + (i % 10);
    i *= 0.1;
  } while (i != 0);
  if(isNegative) *--p = '-';
  if(len != nullptr) *len = num_buf_end - p;
  return p;
}



char *ultoa(uint64_t i, unsigned int *len){
  char *p = num_buf_end;	/* points to terminating '\0' */
  do {
    *--p = '0' + (i % 10);
    i *= 0.1;
  } while (i != 0);
  if(len != nullptr) *len = num_buf_end - p;
  return p;
}




char *ftoa(double f, unsigned int precision, unsigned int *len){
  char *p = num_buf_end;	/* points to terminating '\0' */
  bool isNegative = (f < 0);
  long i = f; // extracting integer part
  f -= i;      // extracting decimal part
  long d = f * quick_pow<10>(precision);
  do {
    *--p = '0' + (d % 10);
    d *= 0.1;
  } while (d != 0);
  *--p = '.';
  do {
    *--p = '0' + (i % 10);
    i *= 0.1;
  } while (i != 0);

  if(isNegative) *--p = '-';
  if(len != nullptr) *len = num_buf_end - p;
  return p;
}





char* stoul(char * p, uint64_t &i)
{
    while (*p == ' ') p++;
    uint64_t acc = 0;
    while (isDigit(*p))
        acc = acc * 10 + *p++ - '0';
    i = acc;
    return p;
}

char* stof(char *p, double &f)
{
    while (*p == ' ') p++;
    int s = 1;
    if (*p == '-') {
        s = -1; p++;
    }

    double acc = 0;
    while (isDigit(*p))
        acc = acc * 10 + *p++ - '0';

    if (*p == '.') {
        double k = 0.1;
        p++;
        while (*p >= '0' && *p <= '9') {
            acc += (*p++ - '0') * k;
            k *= 0.1;
        }
    }
    f = s * acc;
    return p;
}

