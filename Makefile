CC=gcc
CFLAGS=-O3 -lm -DMAX_PID_NUM=`cat /proc/sys/kernel/pid_max`
CFLPROF =-fprofile-generate
CFLUSEPROF =-fprofile-use

proc: my-lib
	$(CC) -O3 mytop.o main.o $(CFLAGS) -o main

my-lib: lib/mytop.cpp main.cpp
	$(CC) -c lib/mytop.cpp main.cpp $(CFLAGS)




use-profile: profile-lib profile-main
	$(CC) $(CFLAGS) -fprofile-use mytop.o main.o -o main

use-profile-lib: lib/mytop.c
	$(CC) -c lib/mytop.c -fprofile-use $(CFLAGS)

use-profile-main: main.c
	$(CC) -c main.c -fprofile-use $(CFLAGS)


profile: profile-lib profile-main
	$(CC) $(CFLAGS) $(CFLPROF) -fprofile-arcs mytop.o main.o -o main

profile-lib: lib/mytop.c
	$(CC) $(CFLAGS) $(CFLPROF) -c lib/mytop.c

profile-main: main.c
	$(CC) $(CFLAGS) $(CFLPROF) -c main.c


use-profile: profile-lib profile-main
	$(CC) $(CFLAGS) $(CFLUSEPROF) mytop.o main.o -o main

use-profile-lib: lib/mytop.c
	$(CC) $(CFLAGS) $(CFLUSEPROF) -c lib/mytop.c

use-profile-main: main.c
	$(CC) $(CFLAGS) $(CFLUSEPROF) -c main.c

clean-obj:
	rm *.o