#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "lib/procs.h"
#include "lib/inet.h"
#include "lib/outputbuffer.h"
#include "lib/systemstat.h"

void init(const Formatter &form){
    OutputBuffer::clear();
    SysInfo.init();
    calc_cpu_usage_pct_all_procs(form);
}

void nextFrame(const Formatter &form){
    OutputBuffer::clear();
    OutputBuffer::appends(form.start);
    SysInfo.print(form);
    OutputBuffer::appends(form.separator);
    printIP(form);
    OutputBuffer::appends(form.separator);
    calc_cpu_usage_pct_all_procs(form);
    OutputBuffer::appends(form.end);
    OutputBuffer::print();
}

void makeFrames(const Formatter &form){
    init(form);
    while( 1 ){
        sleep( INTERVAL );
        nextFrame(form);
    }
}

int main( int argc, char* argv[] )
{

    if(argc > 1){
        if (strEquals(argv[1], "-h")|| strEquals(argv[1], "--help")){
            printf("usage: mytop [option]\n"
                   "OPTIONS:\n"
                   "  -h, --help    show this page\n"
                   "  -j, --json    encode to json object\n"
                   "  -m, --matrix  encode as a matrix (array of array) that can be decoded as json (default)\n");
            return 0;
        } else if (strEquals(argv[1], "-j")|| strEquals(argv[1], "--json")){
            makeFrames(Formatters::JSON);
        } else if (strEquals(argv[1], "-m")|| strEquals(argv[1], "--matrix")){
            makeFrames(Formatters::Matrix);
        }
    } else {
        makeFrames(Formatters::JSON);
    }

} 
