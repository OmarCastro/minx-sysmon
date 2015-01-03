#include "outputbuffer.h"
#include <stdarg.h>
#include "fast_math.h"

namespace OutputBuffer {
    char string[capacity]; //512Kb should be enough
    char* const startPointer = string;
    char* const endPointer = &string[capacity-1];
    char* pointer = string;
    void appendf(const char *fmt, ...)
    {
    va_list argp;
    va_start(argp, fmt);
    pointer += vsprintf(pointer,fmt,argp);
    va_end(argp);
    }
}


#define NUM_DIGITS 30		/* enough for 64 bit integer and float */
static char num_buf[NUM_DIGITS + 2];  /* Room for INT_DIGITS digits, - and '\0' */
static char* const num_buf_end =  num_buf + NUM_DIGITS + 1;

char *itoa(int i, unsigned int *len){
  char *p = num_buf_end;	/* points to terminating '\0' */
  bool isNegative = (i < 0);
  do {
    *--p = '0' + (i % 10);
    i *= 0.1;
  } while (i != 0);
  if(isNegative) *--p = '-';
  *len = num_buf_end - p;
  return p;
}


char *ftoa(float f, unsigned int precision, unsigned int *len){
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
  *len = num_buf_end - p;
  return p;
}

char *ignoreReads(char *r, unsigned int times){
    while(times-- > 0){
        while(*r++ == ' ');
        while(*r++ != ' ');
    }
    return r;
}

int mysprintf(char *str, const char *fmt, ...)
{
va_list argp;
va_start(argp, fmt);
unsigned int it = vsprintf(str,fmt,argp);
va_end(argp);
return it;
}

int vsprintf(char *str, const char *fmt, va_list args){
    int i;
    unsigned int len;
    char *s;
    float f;
    unsigned int it = 0;
    for(const char *p = fmt; *p != '\0'; p++)
        {
        if(*p != '%')
            {
            str[it++] = *p;
            continue;
        }

        switch(*++p)
            {
            case 'c': i = va_arg(args, int);
            str[it++] = i; break;
            case 'd':
                i = va_arg(args, int);
                s = itoa(i, &len);
                memcpy(&str[it],s,len);
                it += len;
                break;
            case 'f':
                f = va_arg(args, double);
                s = ftoa(f,2,&len);
                memcpy(&str[it],s,len);
                it += len;
                break;
            case 's':
                s = va_arg(args, char *);
                len = strlen(s);
                memcpy(&str[it],s,len);
                it += len;
                break;
            case '%':
                str[it++] = '%';
                break;
            }
        }
    str[it] = '\0';
    return it;
}
