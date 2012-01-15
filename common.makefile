
ifeq "$(PLATFORM)" ""
PLATFORM := $(shell uname | cut -d_ -f1)
endif
 
ifneq "" "$(findstring CYGWIN, $(PLATFORM))"
# The windows platforms all define one cpp symbol or another, which CFBase.h funnels to __WIN32__.
# Simplify later checks, since we don't care about different versions of CYGWIN.
PLATFORM = CYGWIN
endif
 
ifeq "$(PLATFORM)" "CYGWIN"
DLL_SUFFIX=dll	
else
DLL_SUFFIX=so
endif

CC=colorgcc

GLIB_FLAGS=$$(pkg-config --cflags glib-2.0) 
GIO_FLAGS=$$(pkg-config --cflags gio-2.0)
GMODULE_FLAGS=$$(pkg-config --cflags gmodule-2.0)
CFLAGS=-g -Wall -O2 -I. $(GLIB_FLAGS) $(GIO_FLAGS) $(GMODULE_FLAGS)

GLIB_LIBS=$$(pkg-config --libs glib-2.0)
GIO_LIBS=$$(pkg-config --libs gio-2.0)
GMODULE_LIBS=$$(pkg-config --libs gmodule-2.0)
LDFLAGS=$(GLIB_LIBS) $(GIO_LIBS) $(GMODULE_LIBS) 

APP_DEPLOY_DIR=$(LOCKE_HOME)/server/src/autodeploy
