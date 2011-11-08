
CC=gcc
CFLAGS=-Wall -O2 $(pkg-config --cflags glib-2.0)
LDFLAGS=$(pkg-config --libs glib-2.0) 

all:
	cd src && make all

clean:
	cd src && make clean
	
