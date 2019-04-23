
AM_CPPFLAGS = -I$(top_srcdir)/engines/support \
           $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libpixflat.la

libpixflat_la_SOURCES =		\
	./src/clearlooks_rc_style.c	\
	./src/clearlooks_rc_style.h	\
	./src/clearlooks_style.c	\
	./src/clearlooks_style.h	\
	./src/clearlooks_theme_main.c	\
	./src/support.c			\
	./src/support.h			\
	./src/clearlooks_draw.c		\
	./src/clearlooks_draw.h		\
	./src/clearlooks_types.h

libpixflat_la_LDFLAGS = -module -avoid-version -no-undefined -export-symbols $(top_srcdir)/engines/engine.symbols
libpixflat_la_LIBADD =  $(top_builddir)/engines/support/libsupport.la $(GTK_LIBS) -lm


-include $(top_srcdir)/git.mk

