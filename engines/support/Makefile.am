noinst_LTLIBRARIES=libsupport.la

INCLUDES = $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS)

EXTRA_DIST = ./config.h.in

libsupport_la_SOURCES = \
	./cairo-support.c	\
	./cairo-support.h	\
	./general-support.h	\
	./widget-information.c	\
	./widget-information.h	\
	./ge-support.h		\
	./config.h

-include $(top_srcdir)/git.mk

