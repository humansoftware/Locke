/*
 * locke_service_manager.h
 *
 *  Created on: 13/01/2012
 *      Author: mvalle
 *
 *	\brief This class is responsible for auto loading service plugins. Each loaded plugin is registered in the manager
 *	and become accessible to any application.
 *
 */

#ifndef LOCKE_SERVICE_MANAGER_H_
#define LOCKE_SERVICE_MANAGER_H_

#include <locke_service.h>
#include <glib.h>
#include <gio/gio.h>
#include <locke_log.h>

typedef struct _LockeServiceManager {
	GHashTable * services;
	GFile *pluginsFolder;
} LockeServiceManager;

LockeServiceManager *locke_service_manager_get_singleton();
void locke_service_manager_destroy_singleton();
LockeServiceManager *locke_service_manager_new();
void locke_service_manager_destroy(LockeServiceManager *lsm);
void locke_service_manager_init(LockeServiceManager *lsm);
void locke_service_manager_stop(LockeServiceManager *lsm);
void locke_service_manager_scan_for_plugins(LockeServiceManager *lsm);
void locke_service_manager_register_service(LockeServiceManager *lsm, const gchar *filename);
LockeService *locke_service_manager_lookup_service(LockeServiceManager *lsm, const gchar *type_name);


#endif /* LOCKE_SERVICE_MANAGER_H_ */
