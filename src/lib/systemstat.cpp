#include "sys/types.h"
#include "systemstat.h"


SystemStat SysInfo;

void SystemStat::print(const Formatter &form) {
    reload();
    OutputBuffer::appendf(form.systat_format)
            % meminfo.uptime % getCurrentValue()
            % totalRam() % freeRam() % meminfo.procs;
}

void SystemStat::reload(){
    sysinfo(&meminfo);
}

char SystemStatBuffer[512];
extern int file2str(const char *filename, char *ret, int cap);


void SystemStat::init(){
    char *S = SystemStatBuffer;
    file2str("/proc/stat", S, 512);
    S = ignoreReads(S,1);
    CpuInfo &cpu = cpus[0];
    S = stoul(S,cpu.lastTotalUser);
    S = stoul(S,cpu.lastTotalUserLow);
    S = stoul(S,cpu.lastTotalSys);
    S = stoul(S,cpu.lastTotalIdle);
}

double SystemStat::getCurrentValue(){
        double percent;
        uint64_t totalUser, totalUserLow, totalSys, totalIdle, total;
        CpuInfo &cpu = cpus[0];
        char *S = SystemStatBuffer;
        file2str("/proc/stat", S, 512);
        S = ignoreReads(S,1);
        S = stoul(S,totalUser);
        S = stoul(S,totalUserLow);
        S = stoul(S,totalSys);
        S = stoul(S,totalIdle);

        if (totalUser < cpu.lastTotalUser || totalUserLow < cpu.lastTotalUserLow ||
            totalSys < cpu.lastTotalSys || totalIdle < cpu.lastTotalIdle){
            //Overflow detection. Just skip this value.
            percent = -1.0;
        }
        else{
            total = (totalUser - cpu.lastTotalUser) + (totalUserLow - cpu.lastTotalUserLow) +
                (totalSys - cpu.lastTotalSys);
            percent = total;
            total += (totalIdle - cpu.lastTotalIdle);
            percent /= total;
            percent *= 100;
        }

        cpu.lastTotalUser = totalUser;
        cpu.lastTotalUserLow = totalUserLow;
        cpu.lastTotalSys = totalSys;
        cpu.lastTotalIdle = totalIdle;


        return percent;
    }
