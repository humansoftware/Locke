/*
 * locke_service_socket.c
 *
 *  Created on: 13/01/2012
 *      Author: mvalle
 */

#include <locke_service_socket.h>
#include <string.h>

void locke_service_socket_type(void *type_name) {
	strcpy(type_name, "SOCKET");
}


int locke_service_socket_listen_port(void *user_data, int port,
		LockeAppOnRequest callback) {

	return 1;
}
