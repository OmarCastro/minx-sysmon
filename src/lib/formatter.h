#ifndef FORMATTER_H
#define FORMATTER_H
#include <cstdint>

class Literal
{
  public:

    template <std::size_t N> constexpr
    Literal(const char (&str)[N])
    : mStr(str),
      mLength(N-1)
    {
    }

    template <std::size_t N> Literal(char (&str)[N]) = delete;
    const char* const mStr;
    const std::size_t mLength;
};


struct Formatter
{
    template <std::size_t M,std::size_t N,std::size_t O
              ,std::size_t P,std::size_t Q
              ,std::size_t T,std::size_t U
              ,std::size_t V,std::size_t W>constexpr
    Formatter(const char (&str1)[M],const char (&str2)[N],const char (&str3)[O],
              const char (&str4)[P],const char (&str5)[Q],const char* process_format,
              const char* systat_format,
              const char (&str8)[T],const char (&str9)[U],const char* ipv4_format,
              const char(&str10)[V],const char(&str11)[W],const char* ipv6_format)
        :start(str1)
        ,end(str2)
        ,separator(str3)
        ,process_start(str4)
        ,process_end(str5)
        ,process_format(process_format)
        ,systat_format(systat_format)
        ,ipv4_start(str8)
        ,ipv4_end(str9)
        ,ipv4_format(ipv4_format)
        ,ipv6_start(str10)
        ,ipv6_end(str11)
        ,ipv6_format(ipv6_format)
    {}
    const Literal start;
    const Literal end;
    const Literal separator;

    const Literal process_start;
    const Literal process_end;
    const char* process_format;

    const char* systat_format;

    const Literal ipv4_start;
    const Literal ipv4_end;
    const char* ipv4_format;

    const Literal ipv6_start;
    const Literal ipv6_end;
    const char* ipv6_format;
};

namespace Formatters{
    static constexpr Formatter Matrix = {
         "[",  "]" ,",",
         "[",  "]" ,"[%d,\"%s\",%f]",
         "[%f, %f]",
         "[",  "]" ,"[%d,\"%s\",%f]",
         "[",  "]" ,"[%d,\"%s\",%f]"
    };

    static constexpr Formatter JSON = {
         "{",  "}" ,",",
        "\"procs\":{",  "}" ,"%d:{name:\"%s\",cpu:%f}",
        "\"sys\":{ cpu: %f, ram: %f }",
         "[",  "]" ,"[%d,\"%s\",%f]",
         "[",  "]" ,"[%d,\"%s\",%f]"
    };
}

#endif // FORMATTER_H
