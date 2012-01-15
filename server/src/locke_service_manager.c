/*
 * locke_service_manager.c
 *
 *  Created on: 13/01/2012
 *      Author: mvalle
 */

#include <locke_service_manager.h>
#include <locke_system.h>
#include <stdlib.h>
#include <string.h>
#include <locke_log.h>

LockeServiceManager *locke_service_manager_instance = NULL;
LockeServiceManager *locke_service_manager_get_singleton() {
	if (locke_service_manager_instance == NULL) {
		locke_service_manager_instance = locke_service_manager_new();
		locke_service_manager_init(locke_service_manager_instance);
	}
	return locke_service_manager_instance;
}
void locke_service_manager_destroy_singleton() {
	locke_service_manager_destroy(locke_service_manager_instance);
}

LockeServiceManager *locke_service_manager_new() {
	return (LockeServiceManager *) calloc(1, sizeof(LockeServiceManager));
}

void locke_service_manager_stop(LockeServiceManager *lsm) {
}

void locke_service_manager_destroy(LockeServiceManager *lsm) {
	if (lsm->services != NULL)
		g_hash_table_destroy(lsm->services);
	g_object_unref(lsm->pluginsFolder);
	g_free(lsm);
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "Service Manager destroyed");
}

LockeService *locke_service_manager_lookup_service(LockeServiceManager *lsm,
		const gchar *type_name) {
	return g_hash_table_lookup(lsm->services, type_name);
}

void locke_service_manager_init(LockeServiceManager *lsm) {
	char pluginDir[1024];
	LockeSystem *system = locke_system_get_singleton();
	strcpy(pluginDir, system->appFolder);
	if (pluginDir[0] == '\0')
		strcpy(pluginDir, ".");
	if (pluginDir[strlen(pluginDir) - 1] != G_DIR_SEPARATOR)
		strcat(pluginDir, G_DIR_SEPARATOR_S);
	strcat(pluginDir, "plugins");
	strcat(pluginDir, G_DIR_SEPARATOR_S);
	lsm->pluginsFolder = g_file_new_for_path(pluginDir);

	lsm->services = g_hash_table_new_full(g_str_hash, g_str_equal, NULL,
			(GDestroyNotify) locke_service_destroy);

	/* First, add built in services */
	LockeService *service;
	service = locke_service_new_builtin_socket_service();
	g_hash_table_insert(lsm->services, service->name, service);
	service = locke_service_new_builtin_http_service();
	g_hash_table_insert(lsm->services, service->name, service);
	/* Scan for service plugins on plugins folder */
	locke_service_manager_scan_for_plugins(lsm);
}

void locke_service_manager_scan_for_plugins(LockeServiceManager *lsm) {
	if (g_file_query_file_type(lsm->pluginsFolder, G_FILE_QUERY_INFO_NONE, NULL)
			!= G_FILE_TYPE_DIRECTORY) {
		g_log(
				LSVR_DOMAIN,
				G_LOG_LEVEL_WARNING,
				"ERROR: services dir '%s' does not exist!! Create it to be able to service requests! ",
				g_file_get_path(lsm->pluginsFolder));
		return;
	}
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO,
			"Scanning folder '%s' for service plugins\n",
			g_file_get_path(lsm->pluginsFolder));

	/* Get file enumerator */
	GError *err = NULL;
	GFileEnumerator *files = g_file_enumerate_children(lsm->pluginsFolder, "*",
			G_FILE_QUERY_INFO_NONE, NULL, &err);
	if (err != NULL) {
		/* Report error to user, and free error */
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_ERROR,
				"Unable to get file list for directory '%s': %s",
				g_file_get_path(lsm->pluginsFolder), err->message);
		g_error_free(err);
		goto scan_for_plugins_finally;
	}

	/* process each file individually */
	GFileInfo *fileInfo = NULL;
	do {
		fileInfo = g_file_enumerator_next_file(files, NULL, &err);
		if (err != NULL) {
			/* Report error to user, and free error */
			g_log(LSVR_DOMAIN, G_LOG_LEVEL_ERROR,
					"Unable to get file for directory '%s': %s",
					g_file_get_path(lsm->pluginsFolder), err->message);
			g_error_free(err);
			goto scan_for_plugins_finally;
		}
		/* stop condition */
		if (fileInfo == NULL)
			break;
		/* process the file */
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO,
				" =========> Registering service plugin '%s'",
				g_file_info_get_display_name(fileInfo));
		locke_service_manager_register_service(lsm,
				g_file_info_get_name(fileInfo));
	} while (TRUE);

	/* Close open things */
	g_file_enumerator_close(files, NULL, &err);
	if (err != NULL) {
		/* Report error to user, and free error */
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_ERROR,
				"Error closing file enumerator for directory '%s': %s",
				g_file_get_path(lsm->pluginsFolder), err->message);
		g_error_free(err);
		goto scan_for_plugins_finally;
	}
	/* Free allocated memory  */
	scan_for_plugins_finally: g_object_unref(files);
}

void locke_service_manager_register_service(LockeServiceManager *lsm,
		const gchar *filename) {
	gchar fullpath[2048];
	strcpy(fullpath, g_file_get_path(lsm->pluginsFolder));
	if (fullpath[strlen(fullpath) - 1] != G_DIR_SEPARATOR)
		strcat(fullpath, G_DIR_SEPARATOR_S);
	strcat(fullpath, filename);

	LockeService *service = locke_service_new();
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO,
			"Loading service from plugin file '%s'", fullpath);
	locke_service_init(service, fullpath);
	GError *err = NULL;
	locke_service_load(service, &err);
	if (err != NULL) {
		/* Report error to user, and free error */
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_ERROR,
				"Unable to load service plugin. Details: '%s'", err->message);
		g_error_free(err);
		locke_service_destroy(service);
		return;
	}
	LockeService *old_service = g_hash_table_lookup(lsm->services,
			service->name);
	if (old_service != NULL) {
		g_log(
				LSVR_DOMAIN,
				G_LOG_LEVEL_ERROR,
				"ERROR: There was already a plugin registered with name '%s'. I will replace it with the plugin from file '%s'",
				service->name, fullpath);
		locke_service_destroy(old_service);
	}

	g_hash_table_insert(lsm->services, service->name, service);

}
