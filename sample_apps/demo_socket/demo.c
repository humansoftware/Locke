#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <demo.h>
#include <locke_service_socket.h>

#define SOCKET_PACKET_SIZE 1024
#define HELLO_STR 	"Hello to you too!\n"
#define DUMMY_STR 	"No, YOU are dummy! Humptf!\n"
#define SMART_STR 	"Thanks! You are smart too... ;-)\n"
#define LOL_STR 	"HAHAHAHAHAHAHAHAHAHAHA\n"


void lockeapp_on_start() {
	printf("on start called!!\n ");
	return;
}
void lockeapp_on_stop() {
	printf("on stop called!!\n ");
}
void lockeapp_on_request(void *request_data) {
	LockeServiceSocketRequest *request =
			(LockeServiceSocketRequest *) request_data;
	char buffer[SOCKET_PACKET_SIZE + 1];
	printf("on request called!!\n ");

	int size;
	do {
		size = locke_service_socket_receive(request, buffer,
				SOCKET_PACKET_SIZE);
		if (size - 2 > 0)
			buffer[size - 2] = '\0';
		else
			buffer[0] = '\0';
		printf(
				"Received data [len=%d]: \n------start------\n'%s'\n-------end-------'\n",
				size, buffer);
		if (strcasecmp(buffer, "quit") == 0)
			break;

		if (strcasecmp(buffer, "hello") == 0) {
			locke_service_socket_send(request, HELLO_STR,
							strlen(HELLO_STR));
		}
		if (strcasecmp(buffer, "dummy") == 0) {
			locke_service_socket_send(request, DUMMY_STR,
							strlen(DUMMY_STR));
		}
		if (strcasecmp(buffer, "smart") == 0) {
			locke_service_socket_send(request, SMART_STR,
							strlen(SMART_STR));
		}
		if (strcasecmp(buffer, "lol") == 0) {
			locke_service_socket_send(request, LOL_STR,
							strlen(LOL_STR));
		}

	} while (size > 0);
	printf("on request finished!");
}
