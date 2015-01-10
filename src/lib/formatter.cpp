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




