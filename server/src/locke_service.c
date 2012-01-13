/*
 * locke_service.c
 *
 *  Created on: 13/01/2012
 *      Author: mvalle
 */

#include <locke_service.h>
#include <locke_service_socket.h>
#include <locke_service_http.h>
#include <stdlib.h>
#include <string.h>

LockeService * locke_service_new_builtin_socket_service() {
	LockeService *service = locke_service_new();
	locke_service_init(service, NULL);
	service->events.locke_service_get_type_name = locke_service_socket_type;
	service->events.locke_service_listen_port =
			locke_service_socket_listen_port;
	return service;
}

LockeService * locke_service_new_builtin_http_service() {
	LockeService *service = locke_service_new();
	locke_service_init(service, NULL);
	service->events.locke_service_get_type_name = locke_service_http_type;
	service->events.locke_service_listen_port = locke_service_http_listen_port;
	return service;
}

GQuark locke_service_error_quark(void) {
	return g_quark_from_static_string("locke_service_error_quark");
}

LockeService * locke_service_new() {
	return (LockeService *) calloc(1, sizeof(LockeService));
}

void locke_service_destroy(LockeService *service) {
	if (service->instances != NULL)
		g_slist_free_full(service->instances, g_free);
	g_free(service);
}

LockeServiceInstance * locke_service_instance_new() {
	return (LockeServiceInstance *) calloc(1, sizeof(LockeServiceInstance));
}

void locke_service_instance_destroy(LockeServiceInstance *instance) {
	g_free(instance);
}

void locke_service_load(LockeService *service, GError **error) {
	service->instances = NULL;
	service->loaded_module = g_module_open(
			g_file_get_path(service->service_dll_file), G_MODULE_BIND_LAZY);
	if (!service->loaded_module) {
		g_set_error(error, LSERVICE_MODULE_ERROR, LSERVICE_MODULE_LOAD_ERROR,
				"%s", g_module_error());
		return;
	}

	locke_service_load_callback(service, "locke_service_listen_port",
			(gpointer *) &(service->events.locke_service_listen_port), error);
	if (*error != NULL)
		return;

	locke_service_load_callback(service, "locke_service_get_type_name",
			(gpointer *) &(service->events.locke_service_get_type_name), error);
	if (*error != NULL)
		return;

	strcpy(service->name, "");
	service->events.locke_service_get_type_name(service->name);
}

void locke_service_load_callback(LockeService *service, const char* name,
		gpointer *method_pointer, GError **error) {
	if (!service->loaded_module) {
		g_set_error(error, LSERVICE_MODULE_ERROR, LSERVICE_MODULE_LOAD_ERROR,
				"Can't load callback '%s' from module, as it was not loaded!",
				name);
		return;
	}
	if (!g_module_symbol(service->loaded_module, name,
			(gpointer *) method_pointer)) {
		g_set_error(error, LSERVICE_MODULE_ERROR, LSERVICE_MODULE_NULL,
				"Can't load symbol %s from module '%s': Error: '%s'", name,
				g_file_get_path(service->service_dll_file), g_module_error());
		return;
	}
	if (*method_pointer == NULL) {
		g_set_error(error, LSERVICE_MODULE_ERROR, LSERVICE_MODULE_OPEN_ERROR,
				"symbol %s is NULL", name);
		return;
	}
}

int locke_service_listen_port(LockeService *service, void *user_data, int port,
		LockeAppOnRequest callback) {
	LockeServiceInstance * instance = locke_service_instance_new();
	instance->user_data = user_data;
	instance->port = port;
	instance->callback = callback;

	service->instances = g_slist_prepend(service->instances, instance);

	return service->events.locke_service_listen_port(instance, port,
			locke_service_on_request);
}

void locke_service_on_request(void *user_data, void *request_data) {
	LockeServiceInstance * instance = (LockeServiceInstance *) user_data;
	if (instance == NULL)
		return;
	if (instance->callback == NULL)
		return;
	instance->callback(instance->user_data, request_data);
}

void locke_service_init(LockeService *service, char *dll_filename) {
	service->instances = NULL;
	service->loaded_module = NULL;
	service->service_dll_file =
			(dll_filename != NULL) ? g_file_new_for_path(dll_filename) : NULL;

}
