/*
 * locke_system.c
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 */

#include <locke_system.h>
#include <glib/gprintf.h>

LockeSystem *_instance=NULL;

LockeSystem *locke_system_get_singleton(int argc, char *argv[]){
	if (_instance == NULL) {
		_instance = locke_system_new();
		locke_system_init(_instance, argc, argv);
	}
	return _instance;
}

LockeSystem *locke_system_new() {
	return (LockeSystem *)calloc(1, sizeof(LockeSystem));
}

void locke_system_init(LockeSystem *ls, int argc, char *argv[]) {
	if ((argc==0) || (argv==NULL)) return;
	gchar *folder = getenv("LOCKE_HOME");
	if (folder==NULL)
		folder = g_get_current_dir();
	if (folder[strlen(folder)-1]!='/') {
		folder[strlen(folder)+1]='\0';
		folder[strlen(folder)]='/';
	}
	strcpy(ls->appFolder, folder);
	g_print("Application folder set to %s\n", ls->appFolder);
}


