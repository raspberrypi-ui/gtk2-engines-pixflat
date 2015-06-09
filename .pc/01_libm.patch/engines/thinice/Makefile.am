INCLUDES = -I$(top_srcdir)/engines/support \
           $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libthinice.la

EXTRA_DIST = \
	./AUTHORS

libthinice_la_SOURCES = \
	./src/thinice_rc_style.c	\
	./src/thinice_rc_style.h	\
	./src/thinice_style.h		\
	./src/thinice_misc.h		\
	./src/thinice_misc.c		\
	./src/thinice_theme_draw.c	\
	./src/thinice_main.c

libthinice_la_LDFLAGS = -module -avoid-version -no-undefined -export-symbols $(top_srcdir)/engines/engine.symbols
libthinice_la_LIBADD = $(top_builddir)/engines/support/libsupport.la $(GTK_LIBS)

-include $(top_srcdir)/git.mk

