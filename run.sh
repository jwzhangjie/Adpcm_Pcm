#!/bin/bash
scons -c
rm foo.pprof
scons 
CPUPROFILE='foo.pprof' PRE_LOAD=/usr/local/lib/libprofiler.so ./tdec encode.pcm decode.pcm
pprof tdec foo.pprof
