
GLIB_FLAGS=$$(pkg-config --cflags glib-2.0) 
GIO_FLAGS=$$(pkg-config --cflags gio-2.0)
CFLAGS=-g -Wall -O2 -I. $(GLIB_FLAGS) $(GIO_FLAGS)

GLIB_LIBS=$$(pkg-config --libs glib-2.0) 
GIO_LIBS=$$(pkg-config --libs gio-2.0)
LDFLAGS=$(GLIB_LIBS) $(GIO_LIBS)

CC=gcc
PROGNAME=locke
OBJS=main.o locke.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(PROGNAME) $(LDFLAGS)

clean:
	rm -f *.o $(PROGNAME)

