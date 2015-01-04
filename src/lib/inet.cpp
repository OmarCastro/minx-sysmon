#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include "outputbuffer.h"

void walkIPv4(struct ifaddrs * ifAddrStruct,const Formatter &form){
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    int len = 0;
    static char addressBuffer[INET_ADDRSTRLEN];


    OutputBuffer::appends(form.ipv4_start);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            OutputBuffer::appends(form.separator.mStr, len);
            OutputBuffer::appendf(form.ipv4_format) % ifa->ifa_name % addressBuffer;
            len = form.separator.mLength;
        }
    }

    OutputBuffer::appends(form.ipv4_end);

}

void walkIPv6(struct ifaddrs * ifAddrStruct,const Formatter &form){
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    int len = 0;
    static char addressBuffer[INET_ADDRSTRLEN];
    OutputBuffer::appends(form.ipv6_start);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            OutputBuffer::appends(form.separator.mStr, len);
            OutputBuffer::appendf(form.ipv6_format) % ifa->ifa_name % addressBuffer;
            len = form.separator.mLength;
        }
    }

    OutputBuffer::appends(form.ipv6_end);

}


void printIP(const Formatter &form){
    struct ifaddrs * ifAddrStruct=NULL;
    getifaddrs(&ifAddrStruct);
    walkIPv4(ifAddrStruct,form);
    OutputBuffer::appends(form.separator);
    walkIPv6(ifAddrStruct,form);
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
}
