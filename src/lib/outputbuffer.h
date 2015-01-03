#ifndef OUTPUTBUFFER_H
#define OUTPUTBUFFER_H

#include <stdio.h>
#include <string.h>
#include "formatter.h"

int mysprintf(char *str, const char *fmt, ...);
char *ignoreReads(char *r, unsigned int times);
char *itoa(int i, unsigned int *len);



namespace OutputBuffer {

    constexpr int capacity = 256*1024;
    extern char string[capacity];
    extern char *pointer;
    inline void clear(){pointer = string;}
    inline int size(){return pointer - string;}
    void appendf(const char *fmt, ...);
    inline void appends(const char *s, unsigned int size){
        memcpy(pointer, s, size);
        pointer += size;
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
