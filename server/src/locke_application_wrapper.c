/*
 * locke_application_wrapper.c
 *
 *  Created on: 27/11/2011
 *      Author: mvalle
 */

#include <locke_application_wrapper.h>
#include <locke_application.h>
#include <locke_system.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <locke_log.h>

LockeApplicationWrapper *locke_application_wrapper_new(const gchar *baseDir,
		const gchar *filename, GError **err) {
	pid_t pid = fork();
	if (pid == 0) { /* child process */
		g_log(LAPP_DOMAIN, G_LOG_LEVEL_DEBUG, "PID(%d) I am an application child process. Will free the unused memory and start the app!", getpid());
		/* Create application */
		LockeApplication *app = locke_application_get_singleton();
		locke_application_init(app, baseDir, filename);
		/* Sets child process flag*/
		locke_system_set_child(locke_system_get_singleton(), TRUE);
		locke_system_quit_mainloop(locke_system_get_singleton());
		return NULL;
	} else if (pid < 0) { /* fork failed */
		g_set_error(err, 0, /* error domain */
		-1, /* error code */
		"Failed to fork process: %s", /* error message format string */
		g_strerror(errno));
		return NULL;
	} else { /* parent process */
		g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "PID(%d) fork done, application process created - pid=%d", getpid(), pid);
		/* In the parent process, creates the wrapper */
		LockeApplicationWrapper * result = (LockeApplicationWrapper *) calloc(1,
				sizeof(LockeApplicationWrapper));
		locke_application_wrapper_set_pid(result, pid);
		strcpy(result->appName, filename);
		return result;
	}
}
void locke_application_wrapper_destroy(LockeApplicationWrapper *law) {
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "PID(%d) Stopping application '%s' with PID(%d) ", getpid(),
			law->appName, law->pid);
	kill(law->pid, SIGTERM);
	if (law != NULL)
		free(law);
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, " Application stopped ");
}

void locke_application_wrapper_set_pid(LockeApplicationWrapper *ls, pid_t pid) {
	ls->pid = pid;
}
pid_t locke_application_wrapper_get_pid(LockeApplicationWrapper *ls) {
	return ls->pid;
}
