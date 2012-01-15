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

#include <locke_service.h>
#include <locke_service_manager.h>
#include <locke_api.h>
#include <glib.h>
#include <gio/gio.h>

/* Error type definitions */
#define LAPP_MODULE_ERROR locke_application_error_quark ()
GQuark locke_application_error_quark (void);

typedef enum {
	LAPP_MODULE_LOAD_ERROR,
	LAPP_MODULE_OPEN_ERROR,
	LAPP_MODULE_NULL
} LockeApplicationErrors;

/* Application callback events */
typedef struct _LockeApplicationEvents {
	LockeAppOnStart 			lockeapp_on_start;
	LockeAppOnStop  			lockeapp_on_stop;
	LockeAppOnRequest 			lockeapp_on_request;
} LockeApplicationEvents;

/* Application itself */
typedef struct _LockeApplication {
	LockeApplicationEvents events;
	GModule *loaded_module;
	GFile *appFolder;
	GFile *deployFolder;
	gchar basename[1024];
	GFile *appDllFile;
	GFile *configFile;
	GKeyFile *config;

	LockeService *service;
	int port;
} LockeApplication;

LockeApplication *locke_application_get_singleton();
void locke_application_destroy_singleton();
LockeApplication *locke_application_new();
void locke_application_destroy(LockeApplication *app);
void locke_application_run(LockeApplication *app);
void locke_application_load(LockeApplication *app, GError **error);
void locke_application_load_callback(LockeApplication *app, const char* name, gpointer *method_pointer, GError **error);
void locke_application_init(LockeApplication *app, const gchar *baseDir, const gchar *filename);

#endif /* LOCKE_APPLICATION_H_ */
