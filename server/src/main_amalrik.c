#include <stdio.h>
#include <signal.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void print_t();
void trata_sigsev(int signum);

int main() {
	//g_unix_signal_add(SIGSEV, trata_sigsev, NULL);
	signal(SIGSEGV, trata_sigsev);
	g_thread_init(NULL);
	g_thread_create(print_t, 0, FALSE, NULL);
	while(1) {
	printf("Hello Locke\n");
	g_print("Hello glib\n");
	sleep(5);
	puts("aki");
	}
	return 0;
}

void print_t()
{
	char *buffer;
	printf("hello threads\n");
	buffer[5] = '\0';
}

void trata_sigsev(int signum)
{
	printf("Process %d got signal %d\n", getpid(), signum);
	puts("pegei o sinal maldito");
	signal(signum, SIG_DFL);
	//exit(1);
}
