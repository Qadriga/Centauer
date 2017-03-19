TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0 gio-2.0

SOURCES += main.c \
    centauer.c \
    data_dispatch.c \
    session.c

HEADERS += \
    global.h \
    centauer.h \
    data_dispatch.h \
    session.h
