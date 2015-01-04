#include "formatter.h"
#include "fast_math.h"

namespace Formatters{

    constexpr Literal Matrix_proc_f[] = {"[",",\"","\",",",","]"};
    constexpr Literal Matrix_systat_f[] = {
        "[" , "," , "," , "," , "," , "]"
    };

    constexpr Literal Matrix_ip_f[] = {"[\"","\",\"","\"]"};

    Formatter Matrix = {
         "[",  "]" ,",",
         "[",  "]" ,Matrix_proc_f,
         Matrix_systat_f,
         "[",  "]" ,Matrix_ip_f,
         "[",  "]" ,Matrix_ip_f
    };


    constexpr Literal JSON_proc_f[] = {
        R"({"pid":)",
        R"(,"name":")",
        R"(","cpu":)",
        R"(,"residentMem":)",
        "}"};
    constexpr Literal JSON_systat_f[] = {
        R"("sys":{ "uptime":)",
        R"(,"cpu":)",
        R"(,"totalram":)",
        R"(,"freeram":)",
        R"(,"processes":)",
        "}"
    };

    constexpr Literal JSON_ip_f[] = {"\"" , "\":\"" , "\""};

    Formatter JSON = {
         "{",  "}" ,  ",",
        R"("procs":[)",  "]" ,JSON_proc_f,
        JSON_systat_f,
        R"("IPv4 addrs":{)",  "}" ,JSON_ip_f,
        R"("IPv6 addrs":{)",  "}" ,JSON_ip_f
    };


}

Format& Format::nextString(){
    if(fstrIndex < fstr.mLength){
        const Literal &lit = fstr.mStrs[fstrIndex];
        memcpy(pointer,lit.mStr,lit.mLength);
        ++fstrIndex;
        pointer += lit.mLength;
        *pointer = '\0';
    }
    return *this;
}



#define NUM_DIGITS 64		/* enough for 64 bit integer and float */
static char num_buf[NUM_DIGITS];  /* Room for INT_DIGITS digits, - and '\0' */
static char* const num_buf_end =  num_buf + NUM_DIGITS - 1;


char *itoa(long i, unsigned int *len){
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

char *ultoa(unsigned long i, unsigned int *len){
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





