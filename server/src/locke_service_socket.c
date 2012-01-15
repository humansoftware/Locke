/*
 * locke_service_socket.c
 *
 *  Created on: 13/01/2012
 *      Author: mvalle
 */
#define MAX_SOCKET_THREADS 10

#include <locke_service_socket.h>
#include <locke_log.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <gio/gio.h>

gboolean locke_service_socket_new_connection(GSocketService *service,
		GSocketConnection *connection, GObject *source_object,
		gpointer user_data);

void locke_service_socket_type(void *type_name) {
	strcpy(type_name, "SOCKET");
}

int locke_service_socket_listen_port(int port,
		LockeAppOnRequest callback) {

	char *host = "127.0.0.1";
	GSocketService *service = g_threaded_socket_service_new(MAX_SOCKET_THREADS);
	GInetAddress *address = g_inet_address_new_from_string(host);
	GSocketAddress *socket_address = g_inet_socket_address_new(address, port);
	g_socket_listener_add_address(G_SOCKET_LISTENER(service), socket_address,
			G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_TCP, NULL, NULL, NULL);

	g_object_unref(socket_address);
	g_object_unref(address);
	g_socket_service_start(service);

	g_signal_connect(service, "run",
			G_CALLBACK(locke_service_socket_new_connection), callback);
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_INFO, "Accepting connections at '%s:%d'",
			host, port);

	return 1;
}

gboolean locke_service_socket_new_connection(GSocketService *service,
		GSocketConnection *connection, GObject *source_object,
		gpointer user_data) {
	LockeAppOnRequest callback = (LockeAppOnRequest) user_data;

	/* g_object_ref(connection); */ /* Tell glib not to disconnect */
	GSocketAddress *sockaddr = g_socket_connection_get_remote_address(
			connection, NULL);
	GInetAddress *addr = g_inet_socket_address_get_address(
			G_INET_SOCKET_ADDRESS(sockaddr));
	unsigned int port = g_inet_socket_address_get_port(
			G_INET_SOCKET_ADDRESS(sockaddr));

	g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "New Connection from %s:%d",
			g_inet_address_to_string(addr), port);

	LockeServiceSocketRequest request;
	request.socket = g_socket_connection_get_socket(connection);
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "Calling app callback");
	callback(&request);
	g_log(LSVR_DOMAIN, G_LOG_LEVEL_DEBUG, "App callback finished");

	return TRUE;
}

int locke_service_socket_receive(LockeServiceSocketRequest *request,
		char *buffer, int length) {
	int size = g_socket_receive((GSocket *)request->socket, buffer, length, NULL, NULL);
	buffer[size] = '\0';

	return size;
}

int locke_service_socket_send(LockeServiceSocketRequest *request, char *buffer, int length) {
	return g_socket_send((GSocket *)request->socket, buffer, length, NULL, NULL);
}

