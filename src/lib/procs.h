#ifndef PROCS_H
#define PROCS_H

#define INTERVAL 2
#define BUFFER_SIZE 2048
#define F_INTERVAL 2.0
#define CMD_SIZE 32 
#include "fast_math.h"
#include "formatter.h"

struct pcpuHist{
    long unsigned int ttime_ticks; //total time ticks (utime_ticks + stime_ticks)
};

struct pstat {
    long unsigned int utime_ticks;
    long int cutime_ticks;
    long unsigned int stime_ticks;
    long int cstime_ticks;
    long unsigned int vsize; // virtual memory size in bytes
    long unsigned int rss; //Resident  Set  Size in bytes
    char name[CMD_SIZE];
};

void calc_cpu_usage_pct_all_procs(const Formatter &form);

constexpr int MAX_PID_NUM = t_pow<2,17>::value;


#endif // PROCS_H
