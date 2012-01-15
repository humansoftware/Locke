/**
 * \file locke_api.h
 *
 *  Created on: 16/11/2011
 *      Author: mvalle
 *
 *	\brief This file should contain an api definition to be used by Locke applications.
 *	Every application should make use of this api to send messages to the Locke process and
 *	problably Locke itself should know this interface, as it should provide an implementation
 *	for it, even if it is not a direct implementation.
 *
 *	For each method of the API, there should be a corresponding method provided in Locke main
 *	process. To call it, the application should like to an API Library, which could have
 *	several kinds of implementations to this API, and would call Locke main process corresponding
 *	method.
 *
 *	For instance, a default implementation of this API would make use of POSIX IPC to call
 *	the Locke corresponding method.
 *
 */

#ifndef LOCKE_API_H_
#define LOCKE_API_H_


/* ---------------Callbacks that should be implemented by the application ----*/
/** Callback called when application is first started */
typedef void(*LockeAppOnStart)      (void);
/** Callback called when application is stopped */
typedef void(*LockeAppOnStop)      (void);
/** Callback called when application receives socket data */
typedef void(*LockeAppOnRequest)      (void * /* request_data */);




#endif /* LOCKE_API_H_ */
