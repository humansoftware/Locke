/*
 * locke_service.h
 *
 *  Created on: 13/01/2012
 *      Author: mvalle
 */

#ifndef LOCKE_SERVICE_H_
#define LOCKE_SERVICE_H_

#include <locke_api.h>
#include <glib.h>
#include <gio/gio.h>

typedef struct _LockeServiceInstance {
	int port;
	LockeAppOnRequest callback;
	void *user_data;
} LockeServiceInstance;

/** Callback called to get service type name  */
typedef void (*LockeServiceGetTypeName)(void *type_name);
/** Callback called to start servicing port  */
typedef int(*LockeServiceListenPort)(int port,
		LockeAppOnRequest callback);

/* Service callback events */
typedef struct _LockeServiceEvents {
	LockeServiceListenPort 	locke_service_listen_port;
	LockeServiceGetTypeName locke_service_get_type_name;
} LockeServiceEvents;

/* Error type definitions */
#define LSERVICE_MODULE_ERROR locke_service_error_quark ()
GQuark locke_service_error_quark(void);
typedef enum {
	LSERVICE_MODULE_LOAD_ERROR, LSERVICE_MODULE_OPEN_ERROR, LSERVICE_MODULE_NULL
} LockeServiceErrors;

typedef struct _LockeService {
	char name[1024];
	LockeServiceEvents events;
	GModule *loaded_module;
	GFile *service_dll_file;
	GSList *instances;
} LockeService;

LockeService * locke_service_new_builtin_socket_service();
LockeService * locke_service_new_builtin_http_service();

LockeService * locke_service_new();
void locke_service_destroy(LockeService *service);
LockeServiceInstance * locke_service_instance_new();
void locke_service_instance_destroy(LockeServiceInstance *instance);
void locke_service_init(LockeService *service, char *dll_filename);
void locke_service_load(LockeService *service, GError **error);
void locke_service_load_callback(LockeService *service, const char* name,
		gpointer *method_pointer, GError **error);
int locke_service_listen_port(LockeService *service, void *user_data, int port,
		LockeAppOnRequest callback);
void locke_service_on_request(void *user_data, void *request_data);

#endif /* LOCKE_SERVICE_H_ */
