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

#ifndef CENTAUER_H
#define CENTAUER_H
#include <glib.h>
#include <gio/gio.h>

/* main struct for an webserver */
struct _centauer{
    GMainLoop* mainloop;
    GSocketService* listensocket;
    GThreadPool* threadpool;
    GQueue* queue;
    GList* exposes;
    GMutex* lock;
    gboolean run;
};
#define CENTAUER_DEFAULT_MSG_SIZE 1<<20

typedef struct _centauer centauer;

centauer* centauer_new();
gboolean centauer_free(centauer* cen_struct);
gboolean centauer_init(centauer** cen_struct,guint max_threads,guint16 port,GError **err);



#endif // CENTAUER_H
