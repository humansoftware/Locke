include ../../common.makefile

#The include dir reffers to the folder with header files that will be included by applications too
API_DIR=../api

CFLAGS=-g -Wall -O2 --ansi -I. -I$(API_DIR) $(GLIB_FLAGS) $(GIO_FLAGS) $(GMODULE_FLAGS)
LDFLAGS=$(GLIB_LIBS) $(GIO_LIBS) $(GMODULE_LIBS)

PROGNAME=locke
OBJS=locke.o locke_appmanager.o \
	locke_application.o locke_system.o locke_application_wrapper.o

all: $(OBJS)	
	echo $(GLIB_LIBS)
	$(CC) $(OBJS) -o $(PROGNAME) $(LDFLAGS)

clean:
	rm -f *.o $(PROGNAME)	

