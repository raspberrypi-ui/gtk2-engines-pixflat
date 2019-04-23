
AC_INIT([gtk2-engines-pixflat], [2.20.2])
AC_CONFIG_SRCDIR([README])

AM_INIT_AUTOMAKE([foreign subdir-objects])

# Use AM_SILENT_RULES if present
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Initialize maintainer mode
AM_MAINTAINER_MODE

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_CC_C_O

dnl Initialize libtool
AM_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

# Don't forget to update the README when changing this.
PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.12.0,,
                  AC_MSG_ERROR([GTK+-2.12 is required to compile gtk-engines]))

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
AC_SUBST(GTK_VERSION)

BUILD_ENGINES="pixflat"
AC_SUBST(BUILD_ENGINES)

AC_SUBST([auto_find_tests], ['$(patsubst %,exported_%,$(BUILD_ENGINES)) torture_buildin $(patsubst %,torture_%,$(TORTURE_TEST_ENGINES))'])

AC_DEFINE_UNQUOTED(ENABLE_WIDGET_CHECKS, 1, [Defines whether to enable runtime widget checks as a fallback to hints from the theme.])

DEVELOPMENT_CFLAGS=
AC_SUBST(DEVELOPMENT_CFLAGS)

AM_CONFIG_HEADER([engines/support/config.h])

AC_CONFIG_FILES([
Makefile
engines/Makefile
engines/support/Makefile
engines/pixflat/Makefile
gtk-engines-pixflat.pc
]) 

AC_OUTPUT
