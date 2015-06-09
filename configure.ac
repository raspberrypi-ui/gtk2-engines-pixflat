
AC_INIT([gtk-engines], [2.20.2])
AC_CONFIG_SRCDIR([README])

AM_INIT_AUTOMAKE

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

dnl Initialize intltool
AC_PROG_INTLTOOL([0.31.0])
GETTEXT_PACKAGE=gtk-engines
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],["$GETTEXT_PACKAGE"],[Gettext package])
AM_GLIB_GNU_GETTEXT

AC_ARG_ENABLE(all, 		[  --disable-all           skip all engines/themes by default], [defaultval=$enableval], 	[defaultval="yes"])
AC_ARG_ENABLE(clearlookspix, 	[  --disable-clearlookspix    skip ClearLooks engine/theme], 	[clearlookspix=$enableval], 	[clearlookspix="$defaultval"])

AC_ARG_ENABLE(animation, 	[  --enable-animation      compile clearlooks with animation support], 	[animation=$enableval],	[animation="no"])
AC_ARG_ENABLE(widget_checks, 	[  --disable-widget-checks disable superfluous widget checks (needs updated themes)], 	[widget_checks=$enableval],	[widget_checks="yes"])

AC_ARG_ENABLE(development, 	[  --enable-development    Enable any development warnings/incomplete code],	[development=$enableval],	[development="no"])

AC_ARG_ENABLE(paranoia, 	[  --enable-paranoia       use wall, werror, ansi, pedantic compiler flags],	[paranoia=$enableval],	[paranoia="no"])

AC_ARG_ENABLE(deprecated,	[  --disable-deprecated    disable deprecated functions in gtk et al],	[deprecated=$enableval],		[deprecated="no"])

AC_ARG_ENABLE(schemas,		[  --disable-schema        disable engine schemas],	[schemas=$enableval],	[schemas="yes"])
AC_ARG_WITH(system-lua,	[  --with-system-lua        link with system Lua library], 		[system_lua=$withval], 	[system_lua="no"])

AM_CONDITIONAL([SYSTEM_LUA], [test x$system_lua = xyes])

BUILD_ENGINES=""
BUILD_THEMES=""
BUILD_SCHEMAS=""

if test $clearlookspix = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES clearlookspix"
fi

# Don't forget to update the README when changing this.
PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.12.0,,
                  AC_MSG_ERROR([GTK+-2.12 is required to compile gtk-engines]))

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
AC_SUBST(GTK_VERSION)

if test $system_lua = "yes"; then
	PKG_CHECK_MODULES(LUA, lua,,
                  AC_MSG_ERROR([--with-system-lua specified but no system liblua found]))
	AC_SUBST(LUA_CFLAGS)
	AC_SUBST(LUA_LIBS)
fi

AC_SUBST(BUILD_ENGINES)
AC_SUBST(BUILD_THEMES)
AC_SUBST(BUILD_SCHEMAS)
AC_SUBST([auto_find_tests], ['$(patsubst %,exported_%,$(BUILD_ENGINES)) torture_buildin $(patsubst %,torture_%,$(TORTURE_TEST_ENGINES))'])


if test $animation = "yes"; then
	AC_DEFINE_UNQUOTED(HAVE_ANIMATION, 1, [Defines whether to compile with animation support])
fi

if test $widget_checks = "yes"; then
	AC_DEFINE_UNQUOTED(ENABLE_WIDGET_CHECKS, 1, [Defines whether to enable runtime widget checks as a fallback to hints from the theme.])
fi

DEVELOPMENT_CFLAGS=
if test $development = "yes"; then
	DEVELOPMENT_CFLAGS+=" -DDEVELOPMENT"
fi

if test $paranoia = "yes"; then
	DEVELOPMENT_CFLAGS+=" -Wall -Werror -ansi -pedantic"
fi

if test $deprecated = "yes"; then
	DEVELOPMENT_CFLAGS+=" -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED"
fi

AC_SUBST(DEVELOPMENT_CFLAGS)

AM_CONFIG_HEADER([engines/support/config.h])

AC_CONFIG_FILES([
Makefile
engines/Makefile
engines/support/Makefile
engines/clearlookspix/Makefile
gtk-engines-2.pc
gtk-engines.spec
po/Makefile.in
]) 

AC_OUTPUT

echo
echo "Special Flags:"

echo -n "      Development Code - "

if test "$development" = "yes"; then
	echo "Enabled"
else
	echo "Disabled"
fi

echo -n "      Paranoia Flags - "

if test "$paranoia" = "yes"; then
        echo "Enabled"
else
        echo "Disabled"
fi

echo -n "      Disable Deprecated - "

if test "$deprecated" = "yes"; then
	echo "Enabled"
else
	echo "Disabled"
fi

echo -n "      Fallback Widget Checks - "

if test "$widget_checks" = "yes"; then
	echo "Enabled"
else
	echo "Disabled -- Warning this WILL break themes that are not updated."
fi

echo
echo "Engines and Themes:"

echo -n "      ClearLooksPix - "
if test "$clearlookspix" = "yes"; then
        echo -n "Including"
        if test $animation = "yes"; then
        	echo " with animation support"
        else
        	echo " without animation support"
        fi
else
        echo "Skipping"
fi

echo ""
