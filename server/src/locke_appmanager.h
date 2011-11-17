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
#include <locke_application.h>

typedef struct _LockeAppManager {
	gchar deployFolder[1024];
} LockeAppManager;

LockeAppManager *locke_appmanager_new();
void locke_appmanager_init(LockeAppManager *lam, gchar *folder);


#endif /* LOCKE_APPMANAGER_H_ */



