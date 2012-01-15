/*
 * locke_service_socket.h
 *
 *  Created on: 13/01/2012
 *      Author: mvalle
 */

#ifndef LOCKE_SERVICE_SOCKET_H_
#define LOCKE_SERVICE_SOCKET_H_

#include <locke_api.h>

typedef struct _LockeServiceSocketRequest {
	void *socket;
} LockeServiceSocketRequest;

/** Callback called to get service type name  */
void locke_service_socket_type(void *type_name);
/** Callback called to start servicing port  */
int locke_service_socket_listen_port(int port,
		LockeAppOnRequest callback);

int locke_service_socket_receive(LockeServiceSocketRequest *request, char *buffer, int length);
int locke_service_socket_send(LockeServiceSocketRequest *request, char *buffer, int length);

#endif /* LOCKE_SERVICE_SOCKET_H_ */
