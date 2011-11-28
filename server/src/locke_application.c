/*
 * locke_application.c
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 */

#include <locke_application.h>
#include <locke_system.h>
#include <glib/gprintf.h>
#include <stdlib.h>

LockeApplication *locke_application_instance = NULL;
LockeApplication *locke_application_get_singleton() {
	if (locke_application_instance == NULL) {
		locke_application_instance = locke_application_new();
	}
	return locke_application_instance;
}
LockeApplication *locke_application_new() {
	return (LockeApplication *) calloc(1, sizeof(LockeApplication));
}

/*
 * This is the 'main' function for the application process
 * the instance variables should have been already populated by the server
 * process
 *
 */
void locke_application_run(LockeApplication *app) {
	locke_application_load(app);
	g_print(
				"============================================================================\n");
	g_print("PID(%d) locke_application_run called on the child process!!! \n",
			getpid());
	g_print(
				"============================================================================\n");

	/* TODO call dll method to set up the app, start servicing requests on a port and be able to call
	 * and application method on each request */
	locke_system_start_mainloop(locke_system_get_singleton(0, NULL));
}

void locke_application_load(LockeApplication *app) {
	/* TODO Load the application itself, using the dll module loader */
}

void locke_application_destroy_singleton() {
	if (locke_application_instance == NULL)
		return;
	locke_application_destroy(locke_application_instance);
	locke_application_instance = NULL;
}
void locke_application_destroy(LockeApplication *app) {
	g_print(
			"============================================================================\n");
	g_print("PID(%d) Destroying application \n ", getpid());
	g_object_unref(app->deployFolder);
	g_object_unref(app->appFolder);
	g_object_unref(app->appDllFile);
	g_object_unref(app->configFile);
	g_print(
			"============================================================================\n");
}

void locke_application_init(LockeApplication *app, const gchar *baseDir,
		const gchar *filename) {
	gchar fullpath[2048];
	gchar appfolder[2048];
	/* file name is the basename of the app */
	strcpy(app->basename, filename);

	app->deployFolder = g_file_new_for_path(baseDir);

	/* calculates app folder*/
	strcpy(appfolder, baseDir);
	if (appfolder[strlen(baseDir) - 1] != G_DIR_SEPARATOR)
		strcat(appfolder, G_DIR_SEPARATOR_S);
	strcat(appfolder, filename);
	strcat(appfolder, G_DIR_SEPARATOR_S);
	app->appFolder = g_file_new_for_path(appfolder);

	/* Calculates .so  path */
	strcpy(fullpath, appfolder);
	strcat(fullpath, "lib");
	strcat(fullpath, filename);
	strcat(fullpath, ".so");
	app->appDllFile = g_file_new_for_path(fullpath);

	/* Calculates .ini path */
	strcpy(fullpath, appfolder);
	strcat(fullpath, filename);
	strcat(fullpath, ".ini");
	app->configFile = g_file_new_for_path(fullpath);
}
