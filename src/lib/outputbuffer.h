#ifndef OUTPUTBUFFER_H
#define OUTPUTBUFFER_H

#include <stdio.h>
#include <string.h>
#include "formatter.h"

char *ignoreReads(char *r, unsigned int times);

namespace OutputBuffer {

    constexpr int capacity = 256*1024;
    extern char string[capacity];
    extern char *pointer;
    inline void clear(){pointer = string;}
    inline int size(){return pointer - string;}
    template<typename... Args>
    inline Format appendf(const FormatString &format){
        return Format(format, pointer);
    }

    inline void appends(const char *s, unsigned int size){
        if(size > 0){
            memcpy(pointer, s, size);
            pointer += size;
        }
    }
    inline void appends(const char *s){
        return appends(s,strlen(s));
    }
    inline void appends(const Literal &s){
        return appends(s.mStr,s.mLength);
    }
    inline void print(){
        *pointer++='\n';
        fwrite (string , sizeof(char), size(), stdout);
    }
}



#endif // OUTPUTBUFFER_H
