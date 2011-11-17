#!/bin/sh

#export LOCKE_HOME=set_to_base_path
export LD_LIBRARY_PATH=./api/
#nohup 
./locke &
LASTPID=$!
echo $LASTPID > process.pid
echo "Process created, PID = ${LASTPID}"

