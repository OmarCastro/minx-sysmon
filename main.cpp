#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "lib/mytop.h"


int main( int argc, char* argv[] )
{
    const char * initTemplate = "[";
    const char * midTemplate = "[%d,\"%s\",%f]";
    const char * separator = ", ";
    const char * endTemplate = "]";


    if(argc > 1){
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            printf("usage: mytop [option]\n"
                   "OPTIONS:\n"
                   "  -h, --help    show this page\n"
                   "  -j, --json    encode to json object\n"
                   "  -m, --matrix  encode as a matrix (array of array) that can be decoded as json (default)\n");
            return 0;
        } else if (strcmp(argv[1], "-j") == 0 || strcmp(argv[1], "--json") == 0){
            initTemplate = "{";
            midTemplate = "%d: {name:\"%s\", cpu_percentage: %f}";
            separator = ", ";
            endTemplate = "}";
        } else if (strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--matrix") == 0){}
    }

    while( 1 )
    {
        calc_cpu_usage_pct_all_procs(initTemplate,midTemplate,endTemplate,separator);
        sleep( INTERVAL );
    }
} 
