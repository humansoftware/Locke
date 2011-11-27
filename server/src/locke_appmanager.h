/** \file locke_appmanager.h
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 *
 *	\brief This class is responsible for application discovering in the autodeploy dir and for any method
 *	associated with loading and unload applications. This class should maintain the list of opened
 *	applications, should be smart enough to create a process for each application and to serve as a
 *	"watchdog" for applications that crash and should be restarted if possible.
 *
 */

#ifndef LOCKE_APPMANAGER_H_
#define LOCKE_APPMANAGER_H_
#include <glib.h>
#include <gio/gio.h>
#include <locke_application.h>

typedef enum {
	SERVER_STARTING = 1, SERVER_RUNNING, SERVER_STOPPING, SERVER_STOPPED
} LockeAppManagerState;

typedef struct _LockeAppManager {
	LockeAppManagerState state;
	GHashTable * appList;
	GFileMonitor *deployDirMonitor;
	GFile *deployFolder;
} LockeAppManager;

LockeAppManager *locke_appmanager_new();
void locke_appmanager_destroy(LockeAppManager *lam);
void locke_appmanager_init(LockeAppManager *lam, gchar *folder, GError **err);
void locke_appmanager_stop(LockeAppManager *lam);
void locke_appmanager_set_state(LockeAppManager *lam,
		LockeAppManagerState state);
void locke_appmanager_scan_for_deploys(LockeAppManager *lam, GFile *deployDir);
void locke_appmanager_add_application(LockeAppManager *lam, const gchar *baseDir, const gchar *filename);
void locke_appmanager_remove_application(LockeAppManager *lam, const gchar *baseDir, const gchar *filename);
gboolean locke_appmanager_is_valid_application(LockeAppManager *lam, const gchar *baseDir, const gchar *filename);

#endif /* LOCKE_APPMANAGER_H_ */

