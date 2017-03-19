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

#include "centauer.h"
#include "data_dispatch.h"
#include <stdlib.h>

/* private function declations */

static void dispatch(gpointer data,gpointer user_data);
static gboolean incomming_callback(GSocketService *Service, GSocketConnection *conn, GObject *src, gpointer user_data);
static gboolean read_incomming_request(data_dispatch* data, http_request* request);
static void array_cpy(gchar* dest, const gchar* src,guint len);
static guint array_append(gchar* dest,const guint dest_len,const gchar* src, const guint src_len);
static gboolean incomming_callback(GSocketService *Service, GSocketConnection *conn, GObject *src, gpointer user_data){
    GInputStream *instream;
    gchar* message;
    g_print("Recived Connection \n");
    instream = g_io_stream_get_input_stream(G_IO_STREAM(conn));

    message = (gchar*) g_try_malloc0_n(1024,sizeof(char));
    g_input_stream_read(instream,message,1024,NULL,NULL);
    g_print("Message dump: %s \n",message);
    g_free(message);
    return FALSE;
}

gboolean centauer_init(centauer **cen_struct, guint max_threads, guint16 port, GError **err){
    (*cen_struct)->listensocket = g_socket_service_new();
    g_socket_listener_add_inet_port((GSocketListener*)(*cen_struct)->listensocket,port,NULL,err);
    g_signal_connect((*cen_struct)->listensocket,"incomming",G_CALLBACK(incomming_callback),NULL);
    (*cen_struct)->threadpool = g_thread_pool_new((GFunc)dispatch,*cen_struct,max_threads,TRUE,err);
    (*cen_struct)->mainloop = g_main_loop_new(NULL,FALSE);
    return TRUE;
}
centauer* centauer_new(){
    centauer* data;
    data = g_try_malloc(sizeof(centauer)); //allocate space for struct
    data->exposes = NULL;
    data->listensocket = NULL;
    g_mutex_init(data->lock);
    g_mutex_lock(data->lock); //loks the mutex for the dispatching processes
    data->queue = g_queue_new();//generate new queue for dispatching
    return data;
}

gboolean centauer_free(centauer *cen_struct){
    gboolean succses = TRUE;
    g_list_free(cen_struct->exposes);
    if (!g_mutex_trylock(cen_struct->lock)){
        g_mutex_unlock(cen_struct->lock);
        g_mutex_clear(cen_struct->lock);

    }
    else{
        succses = FALSE;
    }
    g_socket_listener_close((GSocketListener*)cen_struct->listensocket);//free the listen socket
    g_main_loop_unref(cen_struct->mainloop); //unref the main loop to delete it
    g_queue_free(cen_struct->queue);//free the queue

    return succses;
}

static void
dispatch(gpointer data, gpointer user_data){
    centauer* obj= (centauer*) data; //cast it into a centauer struct
    data_dispatch* dispatch_data;
    http_request* request = http_request_new();
    while(obj->run){
        g_mutex_lock(obj->lock);//this will block the thread until there are work for him
        dispatch_data = (data_dispatch*) g_queue_pop_head(obj->queue);
        read_incomming_request(dispatch_data,request);
    }

}
static void
array_cpy(gchar *dest, const gchar *src, guint len){
    /* util function for copy raw array from src to dest
     * make sure that len is not grather than src array lenght */
    guint i;
    if(dest == NULL){
        dest = (gchar*) g_try_malloc0(len);
    }
    for(i=0;i<len;i++){
        dest[i] = src[i];
    }
}

static guint array_append(gchar *dest, const guint dest_len, const gchar *src, const guint src_len){
    /* util function to append src to dest
     * src array will not changed, dest will be reallocated
     * return value is the new lenght of the array (dest_len + src_len) */
    guint i;
    gchar* end;
    dest = g_try_realloc_n(dest,dest_len+src_len,sizeof(gchar));
    end = dest[dest_len-1];
    for(i=0;i<src_len;i++){
        end[i] = src[i];
    }
    return dest_len+src_len;
}

static gboolean read_incomming_request(data_dispatch *data, http_request *request){
    /* this function read the http request from the connection and wirte it into the http request struct */

    /* variables */
    GInputStream* inputstream;
    gchar* buffer,*readbuffer;
    guint32 size,readed,loops;
    GError* error = NULL;
    gboolean check;
    /* function doing */
    /* init stream and buffers */
    loops = 1;
    inputstream = g_io_stream_get_input_stream(G_IO_STREAM(data->connection));
    size = CENTAUER_DEFAULT_MSG_SIZE;
    buffer = (gchar*) g_try_malloc0_n(size,sizeof(gchar));


    check = g_input_stream_read_all(inputstream,buffer,CENTAUER_DEFAULT_MSG_SIZE,&readed,NULL,&error);
    if (error != NULL){
        g_print("%s\n",error->message);
        g_free(buffer);
        g_error_free(error);
        return FALSE;
    }
    readbuffer = (gchar*) g_try_malloc0_n(readed,sizeof(gchar));
    array_cpy(readbuffer,buffer,readed);
    /* block will read if there is any data left */
    if(readed == CENTAUER_DEFAULT_MSG_SIZE){
        /* this will happen if the request leght is larger than default msg size */
        while (size == (CENTAUER_DEFAULT_MSG_SIZE * loops)) {
            g_input_stream_read_all(inputstream,buffer,CENTAUER_DEFAULT_MSG_SIZE,&readed,NULL,&error);
            size = array_append(readbuffer,size,buffer,readed);
            loops++;
        }
    }


}
