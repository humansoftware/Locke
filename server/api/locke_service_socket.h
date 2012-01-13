/*
 * locke_service_socket.h
 *
 *  Created on: 13/01/2012
 *      Author: mvalle
 */

#ifndef LOCKE_SERVICE_SOCKET_H_
#define LOCKE_SERVICE_SOCKET_H_

#include <locke_api.h>

/** Callback called to get service type name  */
void locke_service_socket_type(void *type_name);
/** Callback called to start servicing port  */
int locke_service_socket_listen_port(void *user_data, int port,
		LockeAppOnRequest callback);

#endif /* LOCKE_SERVICE_SOCKET_H_ */
