# Clearlooks - a cairo based GTKengine
# Copyright (C) 2005-2006 Andrew Johnson <acjgenius@earthlink.net>
# Copyright (C) 2006-2007 Benjamin Berg <benjamin@sipsolutions.net>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
# Project contact: <gnome-themes-list@gnome.org>
#

INCLUDES = -I$(top_srcdir)/engines/support \
           $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libclearlooks.la

libclearlooks_la_SOURCES =		\
	./src/clearlooks_rc_style.c	\
	./src/clearlooks_rc_style.h	\
	./src/clearlooks_style.c	\
	./src/clearlooks_style.h	\
	./src/clearlooks_theme_main.c	\
	./src/support.c			\
	./src/animation.c		\
	./src/animation.h		\
	./src/support.h			\
	./src/clearlooks_draw.c		\
	./src/clearlooks_draw_glossy.c	\
	./src/clearlooks_draw_inverted.c	\
	./src/clearlooks_draw_gummy.c	\
	./src/clearlooks_draw.h		\
	./src/clearlooks_types.h

libclearlooks_la_LDFLAGS = -module -avoid-version -no-undefined -export-symbols $(top_srcdir)/engines/engine.symbols
libclearlooks_la_LIBADD =  $(top_builddir)/engines/support/libsupport.la $(GTK_LIBS)


-include $(top_srcdir)/git.mk

