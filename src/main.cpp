#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "lib/procs.h"
#include "lib/inet.h"
#include "lib/formatter.h"


void makeJSONFrames(){
    while( 1 ){
        calc_cpu_usage_pct_all_procs(Formatters::JSON);
        sleep( INTERVAL );
    }
}

void makeMatrixFrames(){
    while( 1 ){
        calc_cpu_usage_pct_all_procs(Formatters::Matrix);
        sleep( INTERVAL );
    }
}

int main( int argc, char* argv[] )
{
    if(argc > 1){
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            printf("usage: mytop [option]\n"
                   "OPTIONS:\n"
                   "  -h, --help    show this page\n"
                   "  -j, --json    encode to json object\n"
                   "  -m, --matrix  encode as a matrix (array of array) that can be decoded as json (default)\n");
            return 0;
        } else if (strcmp(argv[1], "-j") == 0 || strcmp(argv[1], "--json") == 0){
            makeJSONFrames();
        } else if (strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--matrix") == 0){
            makeMatrixFrames();
        }
    } else {
        makeMatrixFrames();
    }
} 
