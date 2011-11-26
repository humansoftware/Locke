/*
 * locke_appmanager.c
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 */

/* TODO: use gmodule to dinamically load the application
 * http://developer.gnome.org/glib/stable/glib-Dynamic-Loading-of-Modules.html
 * */
#include <locke_appmanager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gprintf.h>

/**  Callback function that monitors deploy dir */
static void
locke_appmanager_file_changed(GFileMonitor *file_monitor, GFile *child,
		GFile *other_file, GFileMonitorEvent event_type, gpointer user_data);

LockeAppManager *locke_appmanager_new() {
	LockeAppManager * result = (LockeAppManager *) calloc(1,
			sizeof(LockeAppManager));
	result->appList = g_hash_table_new(NULL, &g_str_equal);
	return result;
}

void locke_appmanager_destroy(LockeAppManager *lam) {
	g_print("Destroying application manager\n");
	if (lam->deployDirMonitor != NULL)
		g_object_unref(lam->deployDirMonitor);
	g_hash_table_unref(lam->appList);
	g_object_unref(lam->deployFolder);
	free(lam);
	g_print("Ok, application manager destroyed\n");
}
void locke_appmanager_init(LockeAppManager *lam, gchar *folder, GError **err) {
	gchar deployFolder[1024];
	strcpy(deployFolder, folder);
	if (deployFolder[strlen(deployFolder) - 1] != '/') {
		deployFolder[strlen(deployFolder) + 1] = '\0';
		deployFolder[strlen(deployFolder)] = '/';
	}

	g_print("Will be looking for deploys in folder %s\n", deployFolder);
	lam->deployFolder = g_file_new_for_path(deployFolder);

	/* Firstly, perform an initial scan on this folder, to add the applications
	 * that are already there*/
	locke_appmanager_scan_for_deploys(lam, lam->deployFolder);

	/* Monitor deploy dir, to receive events whenever a file is created,
	 * deleted, moved, etc on this folder */
	lam->deployDirMonitor = g_file_monitor_directory(lam->deployFolder,
			G_FILE_MONITOR_NONE, NULL, err);
	if (*err != NULL) {
		g_print("err = %p \n", *err);
		/* Report error to user, and free error */
		fprintf(stderr, "Unable to monitor directory '%s': %s\n", deployFolder,
				(*err)->message);
		return;
	}
	/* Sets the monitor check rate */
	g_file_monitor_set_rate_limit(lam->deployDirMonitor, 3000/* 3 secs*/);
	/* Connects to the "changed" signal, to be able to receive the events */
	g_signal_connect(lam->deployDirMonitor, "changed",
			G_CALLBACK (locke_appmanager_file_changed), lam);

}

void locke_appmanager_stop(LockeAppManager *lam) {

}

void locke_appmanager_set_state(LockeAppManager *lam,
		LockeAppManagerState state) {
	switch (state) {
	case SERVER_STARTING:
		g_print("Server entered STARTING state. \n");
		break;
	case SERVER_RUNNING:
		g_print("Server entered RUNNING state. \n");
		break;
	case SERVER_STOPPING:
		g_print("Server entered STOPPING state. \n");
		break;
	case SERVER_STOPPED:
		g_print("Server entered STOPPED state. \n");
		break;
	default:
		g_print("Invalid server state '%d' - ignoring \n", state);
		return;
		break;
	}
	lam->state = state;
}

static void locke_appmanager_file_changed(GFileMonitor *file_monitor,
		GFile *child, GFile *other_file, GFileMonitorEvent event_type,
		gpointer user_data) {
	LockeAppManager *lam = (LockeAppManager *) user_data;
	g_print("Something has changed at '%s'. Checking...\n",
			g_file_get_path(lam->deployFolder));
	switch (event_type) {
	case G_FILE_MONITOR_EVENT_DELETED:
		g_print("'%s' removed\n", g_file_get_basename(child));
		if (g_file_has_parent(child, lam->deployFolder)) {
			char *path = g_file_get_path(child);
			g_print(
					" I am going to try to remove the application at path '%s' \n",
					path);
		}
		break;
	case G_FILE_MONITOR_EVENT_CREATED:
		g_print("'%s' added\n", g_file_get_basename(child));
		if (g_file_has_parent(child, lam->deployFolder)) {
			char *path = g_file_get_path(child);
			g_print(
					" I am going to try to create an application for path '%s'  \n",
					path);
		}
		break;
	case G_FILE_MONITOR_EVENT_CHANGED:
		g_print("'%s' started changing\n", g_file_get_basename(child));
		break;
	case G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT:
		g_print("'%s' finished changing\n", g_file_get_basename(child));
		break;
	case G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED:
		g_print("'%s' attributes changed\n", g_file_get_basename(child));
		break;
	default:
		g_print("'%s' received event %d \n", g_file_get_basename(child),
				event_type);
		break;

	}
}
void locke_appmanager_scan_for_deploys(LockeAppManager *lam, GFile *deployDir) {
	if (g_file_query_file_type(deployDir, G_FILE_QUERY_INFO_NONE, NULL)
			!= G_FILE_TYPE_DIRECTORY) {
		g_print(
				"Hey, autodeploy dir '%s' does not exist!! Create it to be able to make deploys. \n",
				g_file_get_path(deployDir));
		return;
	}
	g_print("Scanning folder '%s' for application deployments\n",
			g_file_get_path(deployDir));

}
