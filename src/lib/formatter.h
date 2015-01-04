#ifndef FORMATTER_H
#define FORMATTER_H
#include <cstdint>
#include <string.h>
#include <initializer_list>
#include <type_traits>

char *itoa(long i, unsigned int *len);
char *ftoa(double f, unsigned int precision, unsigned int *len);
char *ultoa(unsigned long i, unsigned int *len);


struct Literal
{
    template <std::size_t N> constexpr
    Literal(const char (&str)[N])
    : mStr(str),
      mLength(N-1)
    {}
    template <std::size_t N> Literal(char (&str)[N]) = delete;
    const char* const mStr;
    const std::size_t mLength;
};

struct FormatString
{
    template <std::size_t N> constexpr
    FormatString(const Literal (&strs)[N])
    : mStrs(strs),
      mLength(N){}

    const Literal* const mStrs;
    const std::size_t mLength;
};

class Format{
    FormatString fstr;
    char * (&pointer);
    unsigned int fstrIndex;
    unsigned int len;
    char *cnvStr;
    Format& nextString();

    template<typename T>
    Format& nextInt(T val){
        cnvStr = itoa(val, &len);
        memcpy(pointer,cnvStr,len);
        pointer += len;
        return nextString();
    }

    template<typename T>
    Format& nextUInt(T val){
        cnvStr = ultoa(val, &len);
        memcpy(pointer,cnvStr,len);
        pointer += len;
        return nextString();
    }

    template<typename T>
    Format& nextFP(T val){
        cnvStr = ftoa(val, 2, &len);
        memcpy(pointer,cnvStr,len);
        pointer += len;
        return nextString();
    }

    Format& nextStr(const char *val){
        len = strlen(val);
        memcpy(pointer,val,len);
        pointer += len;
        return nextString();
    }

public:
    Format(const FormatString &fstr,char * (&pointer))
        :fstr(fstr)
        ,pointer(pointer)
        ,fstrIndex(0)
    {nextString();}

    static Format format(const FormatString &fstr,char * (&pointer)){
        return Format(fstr,pointer);
    }
    template <std::size_t N>
    static Format format(const FormatString &fstr,char (&pointer)[N]){
        char * pointer_ref =  &pointer[0];
        return Format(fstr,pointer_ref);
    }

    static Format format(const FormatString &fstr,char * pointer){
        char * pointer_ref =  pointer;
        return Format(fstr,pointer_ref);
    }

     template<typename T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type* = nullptr >
      Format& operator %(T val){
         return nextInt(val);
     }

      template<typename T, typename std::enable_if<std::is_integral<T>::value && !std::is_signed<T>::value>::type* = nullptr >
       Format& operator %(T val){
          return nextUInt(val);
      }

    template<typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr >
     Format& operator %(T val){
        return nextFP(val);
    }


    template <std::size_t N>
    Format& operator %(const char (&val)[N]){ return nextStr(val);}
    Format& operator %(const char *val){ return nextStr(val);}
    Format& operator %(char *val){ return nextStr(val);}
};


struct Formatter
{
    constexpr Formatter(const Literal &start,const Literal &end, const Literal &separator,
              const Literal &p_start,const Literal &p_end,const FormatString &process_format,
              const FormatString &systat_format,
              const Literal &ipv4_start,const Literal &ipv4_end,const FormatString &ipv4_format,
              const Literal &ipv6_start,const Literal &ipv6_end,const FormatString &ipv6_format)
        :start(start) ,end(end) ,separator(separator)
        ,process_start(p_start),process_end(p_end),process_format(process_format)
        ,systat_format(systat_format)
        ,ipv4_start(ipv4_start),ipv4_end(ipv4_end),ipv4_format(ipv4_format)
        ,ipv6_start(ipv6_start),ipv6_end(ipv6_end),ipv6_format(ipv6_format)
    {}
    const Literal start;
    const Literal end;
    const Literal separator;

    const Literal process_start;
    const Literal process_end;
    const FormatString process_format;

    const FormatString systat_format;

    const Literal ipv4_start;
    const Literal ipv4_end;
    const FormatString ipv4_format;

    const Literal ipv6_start;
    const Literal ipv6_end;
    const FormatString ipv6_format;
};

namespace Formatters{
    extern Formatter Matrix;
    extern Formatter JSON;
}

#endif // FORMATTER_H
