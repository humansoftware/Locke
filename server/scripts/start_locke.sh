#!/bin/sh

export LD_LIBRARY_PATH=./api/
#nohup 
./locke &
LASTPID=$!
echo $LASTPID > process.pid
echo "Process created, PID = ${LASTPID}"

