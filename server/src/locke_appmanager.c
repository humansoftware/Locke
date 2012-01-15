/*
 * locke_appmanager.c
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 */

#include <locke_appmanager.h>
#include <locke_system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <locke_log.h>

/**  Callback function that monitors deploy dir */
static void
locke_appmanager_file_changed(GFileMonitor *file_monitor, GFile *child,
		GFile *other_file, GFileMonitorEvent event_type, gpointer user_data);

LockeAppManager *locke_appmanager_new() {
	LockeAppManager * result = (LockeAppManager *) calloc(1,
			sizeof(LockeAppManager));
	result->appList = g_hash_table_new(g_str_hash, g_str_equal);
	return result;
}

void locke_appmanager_destroy(LockeAppManager *lam) {
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "PID (%d) Destroying application manager", getpid());
	if (lam->deployDirMonitor != NULL)
		g_object_unref(lam->deployDirMonitor);
	g_hash_table_unref(lam->appList);
	g_object_unref(lam->deployFolder);
	free(lam);
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "PID(%d) Ok, application manager destroyed", getpid());
}
void locke_appmanager_init(LockeAppManager *lam, gchar *folder, GError **err) {
	gchar deployFolder[1024];
	strcpy(deployFolder, folder);
	if (deployFolder[strlen(deployFolder) - 1] != '/') {
		deployFolder[strlen(deployFolder) + 1] = '\0';
		deployFolder[strlen(deployFolder)] = '/';
	}

	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "Will be looking for deploys in folder %s", deployFolder);
	lam->deployFolder = g_file_new_for_path(deployFolder);

	/* Firstly, perform an initial scan on this folder, to add the applications
	 * that are already there*/
	locke_appmanager_scan_for_deploys(lam, lam->deployFolder);

	/* Monitor deploy dir, to receive events whenever a file is created,
	 * deleted, moved, etc on this folder */
	lam->deployDirMonitor = g_file_monitor_directory(lam->deployFolder,
			G_FILE_MONITOR_NONE, NULL, err);
	if (*err != NULL) {
		/* Report error to user, and free error */
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_ERROR, "Unable to monitor directory '%s': %s", deployFolder,
				(*err)->message);
		return;
	}
	/* Sets the monitor check rate */
	g_file_monitor_set_rate_limit(lam->deployDirMonitor, 3000/* 3 secs*/);
	/* Connects to the "changed" signal, to be able to receive the events */
	g_signal_connect(lam->deployDirMonitor, "changed",
			G_CALLBACK (locke_appmanager_file_changed), lam);

}
gboolean locke_appmanager_hash_removal(gpointer key, gpointer value,
		gpointer user_data) {
	LockeApplicationWrapper *law = (LockeApplicationWrapper *) value;
	locke_application_wrapper_destroy(law);
	return TRUE;
}

void locke_appmanager_stop(LockeAppManager *lam) {
	g_hash_table_foreach_remove(lam->appList, locke_appmanager_hash_removal,
			lam);
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "Number of apps: %d\n", g_hash_table_size(lam->appList));
}

void locke_appmanager_set_state(LockeAppManager *lam,
		LockeAppManagerState state) {
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO,
			"============================================================================\n");
	switch (state) {
	case SERVER_STARTING:
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "PID(%d) Server entered STARTING state. ", getpid());
		break;
	case SERVER_RUNNING:
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "PID(%d) Server entered RUNNING state. ", getpid());
		break;
	case SERVER_STOPPING:
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "PID(%d) Server entered STOPPING state. ", getpid());
		break;
	case SERVER_STOPPED:
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "PID(%d) Server entered STOPPED state. ", getpid());
		break;
	default:
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_WARNING, "PID(%d) Invalid server state '%d' - ignoring ", getpid(),
				state);
		return;
		break;
	}
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG,
			"============================================================================\n");
	lam->state = state;
}

static void locke_appmanager_file_changed(GFileMonitor *file_monitor,
		GFile *child, GFile *other_file, GFileMonitorEvent event_type,
		gpointer user_data) {
	LockeAppManager *lam = (LockeAppManager *) user_data;
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "\n-----------------------------------------------------\n");
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "PID=%d Something has changed at '%s'. \nChecking...", getpid(),
			g_file_get_path(lam->deployFolder));
	switch (event_type) {
	case G_FILE_MONITOR_EVENT_DELETED:
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "'%s' removed", g_file_get_basename(child));
		if (g_file_has_parent(child, lam->deployFolder)) {
			char *path = g_file_get_path(child);
			g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO,
					" =========> I am going to try to remove the application at path '%s' ",
					path);
			locke_appmanager_remove_application(lam,
					g_file_get_basename(child));
		}
		break;
	case G_FILE_MONITOR_EVENT_CREATED:
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "'%s' added\n", g_file_get_basename(child));
		if (g_file_has_parent(child, lam->deployFolder)) {
			char *path = g_file_get_path(child);
			g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO,
					" =========> I am going to try to create an application for path '%s'  ",
					path);
			locke_appmanager_add_application(lam,
					g_file_get_path(lam->deployFolder),
					g_file_get_basename(child));
		}
		break;
	case G_FILE_MONITOR_EVENT_CHANGED:
		/* g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "'%s' started changing\n", g_file_get_basename(child)); */
		break;
	case G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT:
		/* g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "'%s' finished changing\n", g_file_get_basename(child)); */
		break;
	case G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED:
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "'%s' attributes changed\n", g_file_get_basename(child));
		/* Process only directories. Files are just ignored. */
		if (g_file_query_file_type(child, G_FILE_QUERY_INFO_NONE, NULL)
				!= G_FILE_TYPE_DIRECTORY) {
			break;
		}
		locke_appmanager_file_changed(file_monitor, child, other_file,
				G_FILE_MONITOR_EVENT_DELETED, user_data);
		locke_appmanager_file_changed(file_monitor, child, other_file,
				G_FILE_MONITOR_EVENT_CREATED, user_data);
		break;
	default:
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "'%s' received event %d ", g_file_get_basename(child),
		 event_type);
		break;
	}
}
void locke_appmanager_scan_for_deploys(LockeAppManager *lam, GFile *deployDir) {
	if (g_file_query_file_type(deployDir, G_FILE_QUERY_INFO_NONE, NULL)
			!= G_FILE_TYPE_DIRECTORY) {
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_WARNING,
				"Hey, autodeploy dir '%s' does not exist!! Create it to be able to make deploys. ",
				g_file_get_path(deployDir));
		return;
	}
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "Scanning folder '%s' for application deployments",
			g_file_get_path(deployDir));

	/* Get file enumerator */
	GError *err = NULL;
	GFileEnumerator *files = g_file_enumerate_children(deployDir, "*",
			G_FILE_QUERY_INFO_NONE, NULL, &err);
	if (err != NULL) {
		/* Report error to user, and free error */
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_ERROR, "Unable to get file list for directory '%s': %s",
				g_file_get_path(deployDir), err->message);
		g_error_free(err);
		goto scan_for_deploys_finally;
	}

	/* process each file individually */
	GFileInfo *fileInfo = NULL;
	do {
		fileInfo = g_file_enumerator_next_file(files, NULL, &err);
		if (err != NULL) {
			/* Report error to user, and free error */
			g_log(LSVR_DOMAIN, G_LOG_LEVEL_ERROR,  "Unable to get file for directory '%s': %s",
					g_file_get_path(deployDir), err->message);
			g_error_free(err);
			goto scan_for_deploys_finally;
		}
		/* stop condition */
		if (fileInfo == NULL)
			break;
		/* finally, process the file */
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, " =========> Processing file '%s'",
				g_file_info_get_display_name(fileInfo));
		locke_appmanager_add_application(lam, g_file_get_path(deployDir),
				g_file_info_get_name(fileInfo));
	} while (TRUE);

	/* Close open things */
	g_file_enumerator_close(files, NULL, &err);
	if (err != NULL) {
		/* Report error to user, and free error */
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_ERROR,
				"Error closing file enumerator for directory '%s': %s",
				g_file_get_path(deployDir), err->message);
		g_error_free(err);
		goto scan_for_deploys_finally;
	}
	/* Free allocated memory  */
	scan_for_deploys_finally: g_object_unref(files);
}

gboolean locke_appmanager_is_valid_application(LockeAppManager *lam,
		const gchar *baseDir, const gchar *filename) {
	gchar fullpath[2048];
	gchar appfolder[2048];
	/* calculates app folder*/
	strcpy(appfolder, baseDir);
	if (appfolder[strlen(baseDir) - 1] != G_DIR_SEPARATOR)
		strcat(appfolder, G_DIR_SEPARATOR_S);
	strcat(appfolder, filename);
	strcat(appfolder, G_DIR_SEPARATOR_S);
	/* check for .so existence */
	strcpy(fullpath, appfolder);
	strcat(fullpath, "lib");
	strcat(fullpath, filename);
	strcat(fullpath, ".");
	strcat(fullpath, DLL_EXT);

	GFile *file = g_file_new_for_path(fullpath);
	if (g_file_query_file_type(file, G_FILE_QUERY_INFO_NONE, NULL)
			!= G_FILE_TYPE_REGULAR) {
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_WARNING,
				"Application path '%s' doesn't exist or is not a regular file. \nWon't try to deploy it. ",
				fullpath);
		return FALSE;
	}
	g_object_unref(file);

	/* check for .ini existence */
	strcpy(fullpath, appfolder);
	strcat(fullpath, filename);
	strcat(fullpath, ".ini");
	file = g_file_new_for_path(fullpath);
	if (g_file_query_file_type(file, G_FILE_QUERY_INFO_NONE, NULL)
			!= G_FILE_TYPE_REGULAR) {
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_WARNING,
				"App config file path '%s' doesn't exist or is not a regular file. \nWon't try to deploy it. ",
				fullpath);
		return FALSE;
	}
	g_object_unref(file);
	/* All checks completed, app is valid */
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "Application path '%s' is valid. \nWill try to deploy it. ",
			fullpath);
	return TRUE;
}

void locke_appmanager_add_application(LockeAppManager *lam,
		const gchar *baseDir, const gchar *filename) {
	if (!locke_appmanager_is_valid_application(lam, baseDir, filename)) {
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_WARNING,  "Application '%s' is not valid! Ignoring it! ",
				filename);
		return;
	}

	GError *err = NULL;
	LockeApplicationWrapper *law = locke_application_wrapper_new(baseDir,
			filename, &err);
	if (err != NULL) {
		/* Report error to user, and free error */
		fprintf(stderr, "Error creating application '%s': %s\n", filename,
				err->message);
		g_error_free(err);
	}
	if (law != NULL) { /* Inserts application at the application list */
		g_hash_table_insert(lam->appList, (gpointer) filename, law);
	}
}
void locke_appmanager_remove_application(LockeAppManager *lam,
		const gchar *filename) {
	LockeApplicationWrapper *law = g_hash_table_lookup(lam->appList, filename);
	if (law == NULL) {
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_WARNING, "Couldn't find app named '%s' to remove!!! ", filename);
		return;
	}
	g_hash_table_remove(lam->appList, filename);
	locke_application_wrapper_destroy(law);

}
