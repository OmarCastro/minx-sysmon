#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "mytop.h"
#include "fast_math.h"
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static struct pstat stats[MAX_PID_NUM + 1];
static char outputBuffer[MAX_PID_NUM * CMD_SIZE];
static unsigned int bufferPointer = 0;

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

inline void addToBuffer(const char *str, const int size){
    memcpy(&outputBuffer[bufferPointer],str,size);
    bufferPointer += size;
    outputBuffer[bufferPointer] = '\0';
}

#define NUM_DIGITS 30		/* enough for 64 bit integer and float */
static char num_buf[NUM_DIGITS + 2];  /* Room for INT_DIGITS digits, - and '\0' */
static char* const num_buf_end =  num_buf + NUM_DIGITS + 1;


char *itoa(int i, unsigned int *len){
  char *p = num_buf_end;	/* points to terminating '\0' */
  bool isNegative = (i < 0);
  do {
    *--p = '0' + (i % 10);
    i *= 0.1;
  } while (i != 0);
  if(isNegative) *--p = '-';
  *len = num_buf_end - p;
  return p;
}


char *ftoa(float f, unsigned int precision, unsigned int *len){
  char *p = num_buf_end;	/* points to terminating '\0' */
  bool isNegative = (f < 0);
  long i = f; // extracting integer part
  f -= i;      // extracting decimal part
  long d = f * quick_pow<10>(precision);
  do {
    *--p = '0' + (d % 10);
    d *= 0.1;
  } while (d != 0);
  *--p = '.';
  do {
    *--p = '0' + (i % 10);
    i *= 0.1;
  } while (i != 0);

  if(isNegative) *--p = '-';
  *len = num_buf_end - p;
  return p;
}

char *ignoreReads(char *r, unsigned int times){
	while(times-- > 0){
		while(*r++ == ' ');
		while(*r++ != ' ');
	}
	return r;
}

int mysprintf(char *str, const char *fmt, ...)
{
const char *p;
va_list argp;
int i;
unsigned int len;
float f;
char *s;
unsigned int it = 0;

va_start(argp, fmt);

for(p = fmt; *p != '\0'; p++)
	{
	if(*p != '%')
		{
		str[it++] = *p;
		continue;
	}

	switch(*++p)
		{
		case 'c': i = va_arg(argp, int); str[it++] = i; break;
		case 'd':
			i = va_arg(argp, int);
			s = itoa(i, &len);
			memcpy(&str[it],s,len);
			it += len;
			break;
		case 'f':
			f = va_arg(argp, double);
			s = ftoa(f,2,&len);
			memcpy(&str[it],s,len);
			it += len;
			break;	
		case 's':
			s = va_arg(argp, char *);
			len = strlen(s);
			memcpy(&str[it],s,len);
			it += len;
			break;
		case '%':
			str[it++] = '%';
			break;
		}
	}
va_end(argp);
str[it] = '\0';
return it;
}

inline bool isNumeric(unsigned char v){
	return (unsigned)(v-'0') <= ('9' - '0');
}

void calc_cpu_usage_pct(const char* parser, const char* separator, const struct dirent *dir){
	#define IS_BETWEEN(x, min, max) (unsigned)(x – min) <= (max – min)

    const char *pid_s = dir->d_name;
    static char file_buffer[BUFFER_SIZE];
    static char stat_filepath[30];
    const int pageSize = getpagesize();
    const int sepSize = strlen(separator);
    int i;

    if(likely(isNumeric(*pid_s))){
        int pid = atoi(pid_s);
        struct pstat* stat = &stats[pid];
        const long unsigned int pid_old_cpu_time = stat->utime_ticks + stat->stime_ticks;

        mysprintf(stat_filepath, "/proc/%s/stat", pid_s);
        if (unlikely(file2str(stat_filepath, file_buffer, BUFFER_SIZE) == -1)) return;

        char* S = strchr(file_buffer, '(') + 1;
        char* const tmp = strrchr(S, ')');
        int num = tmp - S;
        if(unlikely(num >= sizeof stat->name)) num = sizeof stat->name - 1;
        memcpy(stat->name, S, num);
        stat->name[num] = '\0';
        S = tmp + 2;   
        long int rss;
        S = ignoreReads(S,11);               //#3 to #13
        stat->utime_ticks = strtoul(S,&S,10); //#14 utime - CPU time spent in user code, measured in clock ticks
        stat->stime_ticks = strtoul(S,&S,10); //#15 stime - CPU time spent in kernel code, measured in clock ticks
        stat->cutime_ticks = strtol(S,&S,10); //#16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
        stat->cstime_ticks = strtol(S,&S,10); //#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
        S = ignoreReads(S,5);				 //#18 to #23
        stat->vsize = strtoul(S,&S,10); //#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
        stat->rss = strtoul(S,&S,10) * pageSize;

        /*if (sscanf(S, "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu"
            "%lu %ld %ld %*d %*d %*d %*d %*u %lu %ld",
            &stat->utime_ticks, &stat->stime_ticks,
            &stat->cutime_ticks, &stat->cstime_ticks, &stat->vsize,
            &rss) == EOF) { return; }*/
        //stat->name[0] = '"';
        //stat->name[strlen(result-v>name) - 1] = '"';


        //if(unlikely(get_usage_str(dir_name, stat) < 0)){ continue; }
        const long unsigned int pid_diff = stat->utime_ticks + stat->stime_ticks - pid_old_cpu_time;
        const float cpu_pct = pid_diff/F_INTERVAL;
        addToBuffer(separator, sepSize);
        int bytewritten = mysprintf(&outputBuffer[bufferPointer], parser, pid, stat->name, cpu_pct);
        bufferPointer += bytewritten;
    }
}


void calc_cpu_usage_pct_all_procs(const char* init, const char* parser, const char* end, const char* separator)
{
  DIR *d = opendir("/proc");
  struct dirent *dir;


  if (likely(d))
  {
    bufferPointer = 0;
    addToBuffer(init, strlen(init));
    while (likely(likely((dir = readdir(d)) != NULL) && likely(*dir->d_name < '0')  && likely(*dir->d_name > '9'))){}
    do { calc_cpu_usage_pct(parser,separator,dir); } while (likely((dir = readdir(d)) != NULL));

    addToBuffer(end, strlen(end));
    puts( outputBuffer );
    closedir(d);
  }
}

