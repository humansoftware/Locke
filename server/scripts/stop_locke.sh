#!/bin/sh

echo Trying to kill process $(cat process.pid) 
kill $(cat process.pid)
echo Kill Status: $?