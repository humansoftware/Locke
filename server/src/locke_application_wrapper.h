/** \file locke_application_wrapper.h
 * locke_application_wrapper.h
 *
 *  Created on: 27/11/2011
 *      Author: mvalle
 *
 *	\brief This class is the wrapper for the application in the server process.
 *	Each application is loaded in a different process, which should communicate
 *	to server process by using some kind of IPC.
 *	In the server process, this class will contain things like the pid of the
 *	corresponding application process, the general app info, methods to communicate
 *	to the app process, etc.
 *
 */

#ifndef LOCKE_APPLICATION_WRAPPER_H_
#define LOCKE_APPLICATION_WRAPPER_H_


typedef struct _LockeApplicationWrapper {
	int b;
} LockeApplicationWrapper;

#endif /* LOCKE_APPLICATION_WRAPPER_H_ */
