#include "outputbuffer.h"
#include <stdarg.h>


namespace OutputBuffer {
    char string[capacity]; //512Kb should be enough
    char* const startPointer = string;
    char* const endPointer = &string[capacity-1];
    char* pointer = string;
}


char *ignoreReads(char *r, unsigned int times){
    while(times-- > 0){
        while(*r++ == ' ');
        while(*r++ != ' ');
    }
    return r;
}

char *nextLine(char *r){
    while(*r++ != '\n');
    return r;
}

