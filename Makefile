
CC=gcc
CFLAGS=-Wall -O2 $(pkg-config --cflags glib-2.0)
LDFLAGS=$(pkg-config --libs glib-2.0) 

all:
	cd server && make all

clean:
	cd server && make clean
	
