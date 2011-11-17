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

LockeAppManager *locke_appmanager_new() {
	return (LockeAppManager *) calloc(1, sizeof(LockeAppManager));
}
void locke_appmanager_init(LockeAppManager *lam, gchar *folder){
	strcpy(lam->deployFolder, folder);
	if (lam->deployFolder[strlen(lam->deployFolder)-1]!='/') {
		lam->deployFolder[strlen(lam->deployFolder)+1]='\0';
		lam->deployFolder[strlen(lam->deployFolder)]='/';
	}
	g_print("Will be looking for deploys in folder %s\n", lam->deployFolder);
}
