/**
 * \file locke.c
 *
 *  Created on: 10/11/2011
 *      Author: Marcelo Elias Del Valle
 *
 *  \brief Application Server main file.
 *  This file includes the main function and corresponds to the app server itself.
 *
 */
#include <locke.h>

gboolean timeout_callback(gpointer data)
{
    static int i = 0;

    i++;
    g_print("timeout_callback called %d/10 times\n", i);
    if (10 == i)
    {
        g_main_loop_quit( (GMainLoop*)data );
        return FALSE;
    }

    return TRUE;
}
