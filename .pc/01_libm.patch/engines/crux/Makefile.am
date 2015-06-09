INCLUDES = -I$(top_srcdir)/engines/support \
           $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libcrux-engine.la

EXTRA_DIST = \
	./AUTHORS

libcrux_engine_la_SOURCES = \
	./src/crux.h                  \
	./src/crux-draw.c             \
	./src/crux-draw.h             \
	./src/crux-main.c             \
	./src/crux-style.c            \
	./src/crux-style.h            \
	./src/crux-rc-style.c            \
	./src/crux-rc-style.h

libcrux_engine_la_LDFLAGS = -module -avoid-version -no-undefined -export-symbols $(top_srcdir)/engines/engine.symbols
libcrux_engine_la_LIBADD = $(top_builddir)/engines/support/libsupport.la $(GTK_LIBS)

-include $(top_srcdir)/git.mk

