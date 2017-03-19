/**
 ** This file is part of the Centauer project.
 ** Copyright 2017 Qadriga <copyright_email>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/


#include <stdio.h>
#include "centauer.h"
static GMainLoop* main_loop;
int main(int argc, char *argv[])
{
    GSocketService *service;
    GError* err = NULL;
    g_type_init();/* init glib */
    service = g_socket_service_new();

    g_socket_listener_add_inet_port((GSocketListener*)service,80,NULL,&err);
    if (err != NULL){
        g_error(err->message);
    }

    g_socket_service_start(service);
    main_loop = g_main_loop_new(NULL,FALSE);
    g_main_loop_run(main_loop);
    return 0;
}
