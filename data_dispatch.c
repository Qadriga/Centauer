#include "data_dispatch.h"

void data_dispatch_init(data_dispatch **data){
    (*data)->connection = NULL;
}

data_dispatch* data_dispatch_new(){
    data_dispatch* data;
    data = (data_dispatch*) g_try_malloc(sizeof(data_dispatch));
    data_dispatch_init(&data);
    return data;
}

void http_request_init(http_request **request){
    (*request)->accepts = NULL;
    (*request)->body = g_array_new(FALSE,TRUE,sizeof(gchar));
    (*request)->path = NULL;
    (*request)->user_agent = NULL;
    (*request)->protokoll_version = 1.0;

}
http_request* http_request_new(){
    http_request* data;
    data = (http_request*) g_try_malloc0(sizeof(http_request));
    http_request_init(&data);
    return data;
}

void http_response_init(http_response **response){
    (*response)->content_lenght = 0;
    (*response)->content_type = NULL;
    (*response)->date = g_date_new();
    (*response)->http_status_code = 200;
    (*response)->protokoll_verion = 1.0;
    (*response)->srver_name = NULL;
}

http_response* http_response_new(){
    http_response* data;
    data = (http_response*) g_try_malloc0(sizeof(http_response));
    http_response_init(&data);
    return data;
}

gboolean parse_http_request(const gchar *raw_request_string, http_request *request, GError **erroer){

}
gboolean parse_http_response(http_response *response, gchar **response_string, GError **error){

}

gboolean clear_http_request(http_request *request){
    g_array_unref(request->body);
    g_list_free(request->accepts);
    g_list_free(request->path);
    g_list_free(request->user_agent);
    return TRUE;

}

gboolean clear_http_response(http_response *response){
    g_array_unref(response->body);
    g_date_free(response->date);
    g_free(response->srver_name);
    g_list_free(response->content_type);
    return TRUE;
}

gboolean reset_http_request(http_request *request){
    clear_http_request(request);
    http_request_init(&request);
    return TRUE;

}
gboolean reset_http_response(http_response *response){
    clear_http_response(response);
    http_response_init(&response);
    return TRUE;

}
