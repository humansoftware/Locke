#!/bin/sh

#nohup 
./locke &
LASTPID=$!
echo $LASTPID > process.pid
echo "Process created, PID = ${LASTPID}"

