#ifndef SYSTEMSTAT_H
#define SYSTEMSTAT_H
#include <sys/sysinfo.h>
#include "outputbuffer.h"

struct CpuInfo{
   uint64_t lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;
};

struct SystemStat
{  //struct sysinfo memInfo;
    struct sysinfo meminfo;

    void reload();
    unsigned long totalRam() const  {return meminfo.totalram * meminfo.mem_unit;}
    unsigned long freeRam() const   {return meminfo.freeram  * meminfo.mem_unit;}
    unsigned long totalMem() const  {return (meminfo.totalram+meminfo.totalswap)*meminfo.mem_unit;}
    void print(const Formatter& form);

    CpuInfo cpus[20];
    void init();
    double getCurrentValue();
};

extern SystemStat SysInfo;

#endif // SYSTEMSTAT_H
