include ../../common.makefile

CFLAGS=-g -Wall -O2 -fPIC -I. -I../../server/api $(GLIB_FLAGS) $(GIO_FLAGS) $(GMODULE_FLAGS)
LDFLAGS=$(GLIB_LIBS) $(GIO_LIBS) $(GMODULE_LIBS) -L../../server/api -llockeapi
PROGNAME=demo_http
OBJS=demo.o

all: $(OBJS)
	$(CC) -shared -Wl,-soname,lib$(PROGNAME).$(DLL_SUFFIX) -o lib$(PROGNAME).$(DLL_SUFFIX) $(OBJS) $(LDFLAGS) 

test: clean all test.o
	$(CC) test.o -o test -L. -l$(PROGNAME)
run_test: test
	export LD_LIBRARY_PATH=. && ./test

clean:
	rm -f *.o lib$(PROGNAME).$(DLL_SUFFIX)	
	rm -f test

dist: all
	rm -fr $(APP_DEPLOY_DIR)/$(PROGNAME)
	mkdir $(APP_DEPLOY_DIR)/$(PROGNAME)
	cp lib$(PROGNAME).$(DLL_SUFFIX) $(APP_DEPLOY_DIR)/$(PROGNAME)
	cp $(PROGNAME).ini $(APP_DEPLOY_DIR)/$(PROGNAME)
