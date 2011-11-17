/*
 * locke_system.h
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _LockeSystem {
	gchar appFolder[1024];
} LockeSystem;

LockeSystem *locke_system_get_singleton(int argc, char *argv[]);
LockeSystem *locke_system_new();
void locke_system_init(LockeSystem *ls, int argc, char *argv[]);
