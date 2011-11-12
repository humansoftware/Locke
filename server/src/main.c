#include <stdio.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib-object.h>
#include <signal.h>

#include <locke.h>

/* The program main loop */
GMainLoop *loop;

static void signals_handler(int signum) {
	printf(" Signal %d detected!! \n ", signum);
	if (loop && g_main_loop_is_running(loop)) {
		g_main_loop_quit(loop);
	}
}

int main() {
	/* Setup program signals */
	signal(SIGINT, signals_handler);
	signal(SIGHUP, signals_handler);
	signal(SIGTERM, signals_handler);

	g_print("Locke server v%d.%d\n", LOCKE_MAJOR_VERSION, LOCKE_MINOR_VERSION);
	g_print("Starting glib backend\n");
	/* Initialize GLib type system */
	g_type_init();

	g_print("Creating main loop\n");
	/* Create the main loop */
	loop = g_main_loop_new(NULL, FALSE);

	// add source to default context
	g_timeout_add (1000, timeout_callback , loop);

	/* Run the mais loop */
	g_main_loop_run(loop);
	g_main_loop_unref (loop);

	g_print("Main loop is over. Goodbye! \n");
	return 0;
}


