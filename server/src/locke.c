/**
 * \file locke.c
 *
 *  Created on: 10/11/2011
 *      Author: Marcelo Elias Del Valle
 *
 *  \brief Application Server main file.
 *  This file includes the main function and corresponds to the app server itself.
 *
 */

#include <stdio.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib-object.h>
#include <signal.h>

#include <locke.h>
#include <locke_system.h>
#include <locke_appmanager.h>


gboolean timeout_callback(gpointer data)
{
    static int i = 0;

    i++;
    g_print("timeout_callback called %d/10 times\n", i);
    if (10 == i)
    {
        g_main_loop_quit( (GMainLoop*)data );
        return FALSE;
    }

    return TRUE;
}



/* The program main loop */
GMainLoop *loop;

static void signals_handler(int signum) {
	printf(" Signal %d detected!! \n ", signum);
	if (loop && g_main_loop_is_running(loop)) {
		g_main_loop_quit(loop);
	}
}

int main(int argc, char *argv[]) {
	/* Setup program signals */
	signal(SIGINT, signals_handler);
	signal(SIGHUP, signals_handler);
	signal(SIGTERM, signals_handler);
	signal(SIGSEGV, signals_handler);

	g_print("Locke server v%d.%d\n", LOCKE_MAJOR_VERSION, LOCKE_MINOR_VERSION);
	g_print("Starting glib backend\n");
	/* Initialize GLib type system */
	g_type_init();

	g_print("Creating main loop\n");
	/* Create the main loop */
	loop = g_main_loop_new(NULL, FALSE);

	// add source to default context
	// g_timeout_add (1000, timeout_callback , loop);

	/* Init system */
	LockeSystem *system = locke_system_get_singleton(argc, argv);

	/* Create application manager */
	gchar deployFolder[1024];
	strcpy(deployFolder, system->appFolder);
	strcat(deployFolder, "autodeploy");
	LockeAppManager *appmanager = locke_appmanager_new();
	locke_appmanager_init(appmanager, deployFolder);

	/* Run the mais loop */
	g_main_loop_run(loop);
	g_main_loop_unref (loop);

	g_print("Main loop is over. Goodbye! \n");
	return 0;
}



