/** \file locke_application.h
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 *
 *	\brief This class is responsible for all methods associated with a loaded application.
 *	All events generated to the application should be sent by using the methods of this class.
 *  Also, the management of resources for a single application should also be maintained by this
 *  class, so thread pools, datasources, caching, etc. should all be part of this.
 *  One instance of this class should correspond to a running process.
 */

#ifndef LOCKE_APPLICATION_H_
#define LOCKE_APPLICATION_H_
#include <glib.h>
#include <gio/gio.h>

typedef struct _LockeApplication {
	GFile *appFolder;
	GFile *deployFolder;
	gchar basename[1024];
	GFile *appDllFile;
	GFile *configFile;
} LockeApplication;

LockeApplication *locke_application_get_singleton();
LockeApplication *locke_application_new();
void locke_application_destroy(LockeApplication *app);
void locke_application_destroy_singleton();
void locke_application_run(LockeApplication *app);
void locke_application_load(LockeApplication *app);
void locke_application_init(LockeApplication *app, const gchar *baseDir, const gchar *filename);

#endif /* LOCKE_APPLICATION_H_ */
