#ifndef SESSION_H
#define SESSION_H
#include <glib.h>

struct _Session{
    GDate* Expiredate;

    GList* user_data;
};


typedef struct _Session Session;
Session* session_new();
gboolean session_set_expiredate(GDate* Expiredate);
gboolean session_is_expired(Session* session);
#endif // SESSION_H
