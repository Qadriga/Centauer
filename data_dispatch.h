#ifndef DATA_DISPATCH_H
#define DATA_DISPATCH_H
#include <glib.h>
#include <gio/gio.h>

struct _data_dispatch{
    GSocketConnection* connection;

};

enum _http_type{
    GET,POST,HEAD,PUT,OPTIONS,DELETE,TRACE,CONNECT
};
enum _http_connection_type{
    keep_alive,close
};

struct _http_request{
    GList* path;
    http_type type;
    gfloat protokoll_version;
    GList* accepts;
    GList* user_agent;
    gchar* host;
    http_connection_type connection_type;

    GArray* body;

};

struct _http_response{
    gfloat protokoll_verion;
    guint http_status_code;
    GDate* date;
    gchar* srver_name;
    guint content_lenght;
    GList* content_type;

    GArray* body;
};

typedef struct _data_dispatch data_dispatch;
typedef struct _http_request http_request;
typedef struct _http_response http_response;
typedef enum _http_connection_type http_connection_type;
typedef enum _http_request http_type;

void data_dispatch_init(data_dispatch** data);
data_dispatch* data_dispatch_new(void);

void http_request_init(http_request** request);
http_request* http_request_new(void);
gboolean clear_http_request(http_request* request);
gboolean reset_http_request(http_request* request);
gboolean parse_http_request(const gchar *raw_request_string, http_request *request, GError **erroer);

void http_response_init(http_response** response);
http_response* http_response_new(void);
gboolean clear_http_response(http_response* response);
gboolean reset_http_response(http_response* response);
gboolean parse_http_response(http_response* response, gchar** response_string, GError** error);

#endif // DATA_DISPATCH_H
