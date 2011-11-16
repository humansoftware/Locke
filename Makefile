
CC=gcc
CFLAGS=-Wall -O2 $(pkg-config --cflags glib-2.0)
LDFLAGS=$(pkg-config --libs glib-2.0) 


all:
	cd server && make all
	cd sample_apps && make all
	
docs:
	cd documentation && make all

dist: clean all docs
	cd server && make dist	
	cd sample_apps && make dist
	cp -R documentation dist	

clean:
	cd server && make clean
	cd documentation && make clean
	
	
