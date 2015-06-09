# Redmond95 - a cairo based GTK+ engine
# Copyright (C) 2001 Red Hat, Inc.
# Copyright (C) 2006 Andrew Johnson <acjgenius@earthlink.net>  
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

engine_LTLIBRARIES = libredmond95.la

libredmond95_la_SOURCES = \
	./src/redmond_gtk2_misc.c	\
	./src/redmond_gtk2_misc.h	\
	./src/redmond_gtk2_drawing.c	\
	./src/redmond_gtk2_drawing.h	\
	./src/redmond_gtk2_engine.c	\
	./src/redmond_gtk2_engine.h

libredmond95_la_LDFLAGS = -module -avoid-version -no-undefined -export-symbols $(top_srcdir)/engines/engine.symbols
libredmond95_la_LIBADD =  $(top_builddir)/engines/support/libsupport.la $(GTK_LIBS)

-include $(top_srcdir)/git.mk

