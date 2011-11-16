include ../../common.makefile

CFLAGS=-g -Wall -O2 -fPIC -I. $(GLIB_FLAGS) $(GIO_FLAGS) $(GMODULE_FLAGS)
LDFLAGS=$(GLIB_LIBS) $(GIO_LIBS) $(GMODULE_LIBS) 
PROGNAME=lockeapi
#include the default implementation
OBJS=locke_api_posix.o

all: $(OBJS)
	$(CC) -shared -Wl,-soname,lib$(PROGNAME).$(DLL_SUFFIX) -o lib$(PROGNAME).$(DLL_SUFFIX) $(OBJS) $(LDFLAGS) 

test: clean all test.o
	$(CC) test.o -o test -L. -l$(PROGNAME)
run_test: test
	export LD_LIBRARY_PATH=. && ./test

clean:
	rm -f *.o lib$(PROGNAME).$(DLL_SUFFIX)	
	rm -f test

