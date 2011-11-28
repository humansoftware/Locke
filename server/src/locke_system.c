/*
 * locke_system.c
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 */

#include <locke_system.h>
#include <glib/gprintf.h>

LockeSystem *locke_system_instance = NULL;

LockeSystem *locke_system_get_singleton(int argc, char *argv[]) {
	if (locke_system_instance == NULL) {
		locke_system_instance = locke_system_new();
		locke_system_init(locke_system_instance, argc, argv);
	}
	return locke_system_instance;
}

LockeSystem *locke_system_new() {
	return (LockeSystem *) calloc(1, sizeof(LockeSystem));
}

void locke_system_init(LockeSystem *ls, int argc, char *argv[]) {
	if ((argc == 0) || (argv == NULL))
		return;
	gchar *folder = getenv("LOCKE_HOME");
	if (folder == NULL)
		folder = g_get_current_dir();
	if (folder[strlen(folder) - 1] != '/') {
		folder[strlen(folder) + 1] = '\0';
		folder[strlen(folder)] = '/';
	}
	strcpy(ls->appFolder, folder);
	g_print("Application folder set to %s\n", ls->appFolder);

	ls->isChildProcess = FALSE;
}

void locke_system_set_child(LockeSystem *ls, gboolean child) {
	ls->isChildProcess = child;
}
gboolean locke_system_get_child(LockeSystem *ls) {
	return ls->isChildProcess;
}

void locke_system_start_mainloop(LockeSystem *ls) {
	/* Create the main loop */
	ls->loop = g_main_loop_new(NULL, FALSE);

	/* add source to default context */
	/* g_timeout_add (1000, timeout_callback , loop); */
	g_main_loop_run(ls->loop);
}

void locke_system_quit_mainloop(LockeSystem *ls) {
	if (ls->loop && g_main_loop_is_running(ls->loop)) {
		g_main_loop_quit(ls->loop);
		g_main_loop_unref(ls->loop);
		ls->loop = NULL;
	}
}

void locke_system_set_serverpid(LockeSystem *ls, pid_t pid) {
	ls->serverPid = pid;
}
pid_t locke_system_get_serverpid(LockeSystem *ls) {
	return ls->serverPid;
}

