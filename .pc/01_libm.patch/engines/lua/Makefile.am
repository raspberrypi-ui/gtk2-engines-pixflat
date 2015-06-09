# LUA - a LUA scriptable GTK+ engine
# Copyright (C) 2006 Daniel Borgmann <daniel.borgmann@gmail.com>
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


NULL =

LIBLUA_FILES = \
	./src/liblua/lapi.h \
	./src/liblua/lauxlib.h \
	./src/liblua/lcode.h \
	./src/liblua/ldebug.h \
	./src/liblua/ldo.h \
	./src/liblua/lfunc.h \
	./src/liblua/lgc.h \
	./src/liblua/llex.h \
	./src/liblua/llimits.h \
	./src/liblua/lmem.h \
	./src/liblua/lobject.h \
	./src/liblua/lopcodes.h \
	./src/liblua/lparser.h \
	./src/liblua/lstate.h \
	./src/liblua/lstring.h \
	./src/liblua/ltable.h \
	./src/liblua/ltm.h \
	./src/liblua/lua.h \
	./src/liblua/luaconf.h \
	./src/liblua/lualib.h \
	./src/liblua/lundump.h \
	./src/liblua/lvm.h \
	./src/liblua/lzio.h \
	./src/liblua/lapi.c \
	./src/liblua/lcode.c \
	./src/liblua/ldebug.c \
	./src/liblua/ldo.c \
	./src/liblua/ldump.c \
	./src/liblua/lfunc.c \
	./src/liblua/lgc.c \
	./src/liblua/liolib.c \
	./src/liblua/llex.c \
	./src/liblua/lmem.c \
	./src/liblua/lobject.c \
	./src/liblua/lopcodes.c \
	./src/liblua/loslib.c \
	./src/liblua/lparser.c \
	./src/liblua/lstate.c \
	./src/liblua/lstring.c \
	./src/liblua/lstrlib.c \
	./src/liblua/ltable.c \
	./src/liblua/ltablib.c \
	./src/liblua/ltm.c \
	./src/liblua/lundump.c \
	./src/liblua/lvm.c \
	./src/liblua/lzio.c \
	./src/liblua/lauxlib.c \
	./src/liblua/lbaselib.c \
	./src/liblua/ldblib.c \
	./src/liblua/lmathlib.c \
	./src/liblua/loadlib.c \
	./src/liblua/linit.c

if SYSTEM_LUA
LIBLUA_SOURCES =
LIBLUA_CFLAGS = $(LUA_CFLAGS)
EXTRA_DIST = $(LIBLUA_FILES)
else
LIBLUA_CFLAGS = -I$(top_srcdir)/engines/lua/src/liblua
LIBLUA_SOURCES = $(LIBLUA_FILES)
EXTRA_DIST = 
endif

INCLUDES = \
	-I$(top_srcdir)/engines/support \
	$(LIBLUA_CFLAGS) $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS) \
	$(NULL)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libluaengine.la

libluaengine_la_SOURCES = \
	./src/draw_lib.c \
	./src/draw_lib.h \
	./src/draw_utils.c \
	./src/draw_utils.h \
	./src/lua_rc_style.c \
	./src/lua_rc_style.h \
	./src/lua_style.c \
	./src/lua_style.h \
	./src/lua_utils.c \
	./src/lua_utils.h \
	./src/main.c \
	./src/misc_utils.c \
	./src/misc_utils.h \
	$(LIBLUA_SOURCES) \
	$(NULL)

libluaengine_la_LDFLAGS = -module -avoid-version -no-undefined -export-symbols $(top_srcdir)/engines/engine.symbols
libluaengine_la_LIBADD = $(top_builddir)/engines/support/libsupport.la $(GTK_LIBS) $(LUA_LIBS)

-include $(top_srcdir)/git.mk

