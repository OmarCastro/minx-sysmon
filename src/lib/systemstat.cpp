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
    S = stoul(S,lastTotalUser);
    S = stoul(S,lastTotalUserLow);
    S = stoul(S,lastTotalSys);
    S = stoul(S,lastTotalIdle);
}

double SystemStat::getCurrentValue(){
        double percent;
        uint64_t totalUser, totalUserLow, totalSys, totalIdle, total;

        char *S = SystemStatBuffer;
        file2str("/proc/stat", S, 512);
        S = ignoreReads(S,1);
        S = stoul(S,totalUser);
        S = stoul(S,totalUserLow);
        S = stoul(S,totalSys);
        S = stoul(S,totalIdle);

        if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
            totalSys < lastTotalSys || totalIdle < lastTotalIdle){
            //Overflow detection. Just skip this value.
            percent = -1.0;
        }
        else{
            total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
                (totalSys - lastTotalSys);
            percent = total;
            total += (totalIdle - lastTotalIdle);
            percent /= total;
            percent *= 100;
        }


        lastTotalUser = totalUser;
        lastTotalUserLow = totalUserLow;
        lastTotalSys = totalSys;
        lastTotalIdle = totalIdle;


        return percent;
    }
