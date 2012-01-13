/*
 * locke_system.h
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef G_OS_WIN32
#define DLL_EXT "dll"
#else

#ifdef __CYGWIN32__
#define DLL_EXT "dll"
#else
#define DLL_EXT "so"
#endif

#endif

typedef struct _LockeSystem {
	gchar appFolder[1024];
	/* This should return TRUE in case of application process */
	gboolean isChildProcess;
	/* The program main loop */
	GMainLoop *loop;
	/* server pid */
	pid_t serverPid;
} LockeSystem;

LockeSystem *locke_system_init_singleton(int argc, char *argv[]);
LockeSystem *locke_system_get_singleton();
LockeSystem *locke_system_new();
void locke_system_init(LockeSystem *ls, int argc, char *argv[]);
void locke_system_set_child(LockeSystem *ls, gboolean child);
gboolean locke_system_get_child(LockeSystem *ls);
void locke_system_quit_mainloop(LockeSystem *ls);
void locke_system_start_mainloop(LockeSystem *ls);
void locke_system_set_serverpid(LockeSystem *ls, pid_t pid);
pid_t locke_system_get_serverpid(LockeSystem *ls);

