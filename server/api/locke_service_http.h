/*
 * locke_service_http.h
 *
 *  Created on: 13/01/2012
 *      Author: mvalle
 */

#ifndef LOCKE_SERVICE_HTTP_H_
#define LOCKE_SERVICE_HTTP_H_

#include <locke_api.h>

/** Callback called to get service type name  */
void locke_service_http_type(void *type_name);
/** Callback called to start servicing port  */
int locke_service_http_listen_port(int port,
		LockeAppOnRequest callback);


#endif /* LOCKE_SERVICE_HTTP_H_ */
