project("minx-sysmon")

include_directories(src)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O3")

add_executable(sysmon
    src/main.cpp
    src/lib/procs.cpp
    src/lib/inet.cpp
    src/lib/fast_math.h
    src/lib/outputbuffer.cpp
    src/lib/formatter.cpp
    src/lib/systemstat.cpp
)
