#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "procs.h"
#include "inet.h"
#include "systemstat.h"



static pcpuHist stats[MAX_PID_NUM + 1];

static int file2str(const char *filename, char *ret, int cap) {
    int fd, num_read;
    fd = open(filename, O_RDONLY, 0);
    if(unlikely(fd==-1)) return -1;
    num_read = read(fd, ret, cap - 1);
    close(fd);
    if(unlikely(num_read<=0)) return -1;
    ret[num_read] = '\0';
    return num_read;
}


void calc_cpu_usage_pct(const FormatString &parser, const struct dirent* const &dir){
    const char *pid_s = dir->d_name;
    static char file_buffer[BUFFER_SIZE];
    static char stat_filepath[30];
    static Literal strfs[] = {"/proc/","/stat"};
    const int pageSize = getpagesize();
    static pstat stat;
    int pid = atoi(pid_s);
    pcpuHist &pcpu = stats[pid];
    Format::format(strfs, stat_filepath) % pid_s;
    if (unlikely(file2str(stat_filepath, file_buffer, BUFFER_SIZE) == -1)) return;

    char* S = strchr(file_buffer, '(') + 1;
    char* const tmp = strrchr(S, ')');
    unsigned int num = tmp - S;
    if(unlikely(num >= sizeof(stat.name))) num = sizeof(stat.name) - 1;
    memcpy(stat.name, S, num);
    stat.name[num] = '\0';
    S = tmp + 2;
    S = ignoreReads(S,11);               //#3 to #13
    stat.utime_ticks = strtoul(S,&S,10); //#14 utime - CPU time spent in user code, measured in clock ticks
    stat.stime_ticks = strtoul(S,&S,10); //#15 stime - CPU time spent in kernel code, measured in clock ticks
    stat.cutime_ticks = strtol(S,&S,10); //#16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
    stat.cstime_ticks = strtol(S,&S,10); //#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
    S = ignoreReads(S,5);				 //#18 to #23
    stat.vsize = strtoul(S,&S,10);
    stat.rss = strtoul(S,&S,10) * pageSize;

    //if(unlikely(get_usage_str(dir_name, stat) < 0)){ continue; }
    const decltype(pcpu.ttime_ticks) ttime_ticks = stat.utime_ticks + stat.stime_ticks;
    const long unsigned int pid_diff = ttime_ticks - pcpu.ttime_ticks;
    pcpu.ttime_ticks = ttime_ticks;
    const float cpu_pct = pid_diff/F_INTERVAL;
    double mem_pct = (stat.rss*100)/(float)SysInfo.totalRam();

    OutputBuffer::appendf(parser) % pid % stat.name % cpu_pct % mem_pct;
}


void calc_cpu_usage_pct_all_procs(const Formatter &form)
{
  DIR *d = opendir("/proc");
  struct dirent *dir;


  if (likely(d))
  {
    OutputBuffer::appends(form.process_start);
    while (likely(likely((dir = readdir(d)) != NULL) && likely(!isDigit(*dir->d_name)))){}
    calc_cpu_usage_pct(form.process_format,dir);

    while (likely((dir = readdir(d)) != NULL) && likely(isDigit(*dir->d_name))){
        OutputBuffer::appends(form.separator);
        calc_cpu_usage_pct(form.process_format,dir);
    }
    OutputBuffer::appends(form.process_end);
    closedir(d);
  }
}

