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


void SystemStat::init(){
    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %Ld %Ld %Ld %Ld", &lastTotalUser, &lastTotalUserLow,
        &lastTotalSys, &lastTotalIdle);
    fclose(file);
}

double SystemStat::getCurrentValue(){
        double percent;
        FILE* file;
        unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;


        file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %Ld %Ld %Ld %Ld", &totalUser, &totalUserLow,
            &totalSys, &totalIdle);
        fclose(file);


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
