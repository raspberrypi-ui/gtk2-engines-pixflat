
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
AC_ARG_ENABLE(clearlooks, 	[  --disable-clearlooks    skip ClearLooks engine/theme], 	[clearlooks=$enableval], 	[clearlooks="$defaultval"])
AC_ARG_ENABLE(crux, 		[  --disable-crux          skip Crux engine/theme], 		[crux=$enableval], 		[crux="$defaultval"])
AC_ARG_ENABLE(hc,		[  --disable-hc            skip HC(high contrast) engine], 	[hc=$enableval], 		[hc="$defaultval"])
AC_ARG_ENABLE(industrial, 	[  --disable-industrial    skip Industrial engine/theme], 	[industrial=$enableval], 	[industrial="$defaultval"])
AC_ARG_ENABLE(mist, 		[  --disable-mist          skip Mist engine/theme], 		[mist=$enableval], 		[mist="$defaultval"])
AC_ARG_ENABLE(redmond,		[  --disable-redmond       skip Redmond engine/theme], 		[redmond=$enableval], 		[redmond="$defaultval"])
AC_ARG_ENABLE(glide,		[  --disable-glide         skip Glide engine],			[glide=$enableval],		[glide="$defaultval"])
AC_ARG_ENABLE(thinice,		[  --disable-thinice       skip ThinIce engine/theme], 		[thinice=$enableval], 		[thinice="$defaultval"])
AC_ARG_ENABLE(lua,			[  --enable-lua            enable Lua engine], 			[lua=$enableval], 			[lua="no"])

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

if test $clearlooks = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES clearlooks"
	BUILD_THEMES="$BUILD_THEMES Clearlooks"
	if test $schemas = "yes"; then
		BUILD_SCHEMAS="$BUILD_SCHEMAS clearlooks.xml"
	fi
fi

if test $crux = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES crux"
	BUILD_THEMES="$BUILD_THEMES Crux"
	if test $schemas = "yes"; then
		BUILD_SCHEMAS="$BUILD_SCHEMAS crux-engine.xml"
	fi
fi

if test $hc = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES hc"
	if test $schemas = "yes"; then
		BUILD_SCHEMAS="$BUILD_SCHEMAS hcengine.xml"
	fi
fi

if test $industrial = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES industrial"
	BUILD_THEMES="$BUILD_THEMES Industrial"
	if test $schemas = "yes"; then
		BUILD_SCHEMAS="$BUILD_SCHEMAS industrial.xml"
	fi
fi

if test $mist = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES mist"
	BUILD_THEMES="$BUILD_THEMES Mist"
	if test $schemas = "yes"; then
		BUILD_SCHEMAS="$BUILD_SCHEMAS mist.xml"
	fi
fi

if test $redmond = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES redmond"
	BUILD_THEMES="$BUILD_THEMES Redmond"
	if test $schemas = "yes"; then
		BUILD_SCHEMAS="$BUILD_SCHEMAS redmond95.xml"
	fi
fi

if test $glide = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES glide"
	if test $schemas = "yes"; then
		BUILD_SCHEMAS="$BUILD_SCHEMAS glide.xml"
	fi
fi

if test $thinice = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES thinice"
	BUILD_THEMES="$BUILD_THEMES ThinIce"
	if test $schemas = "yes"; then
		BUILD_SCHEMAS="$BUILD_SCHEMAS thinice.xml"
	fi
fi

if test $lua = "yes"; then
	BUILD_ENGINES="$BUILD_ENGINES lua"
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
test/Makefile
engines/Makefile
engines/support/Makefile
engines/clearlooks/Makefile
engines/crux/Makefile
engines/hc/Makefile
engines/industrial/Makefile
engines/mist/Makefile
engines/redmond/Makefile
engines/glide/Makefile
engines/thinice/Makefile
engines/lua/Makefile
themes/Makefile
themes/Clearlooks/Makefile
themes/Clearlooks/gtk-2.0/Makefile
themes/Crux/Makefile
themes/Crux/gtk-2.0/Makefile
themes/Industrial/Makefile
themes/Industrial/gtk-2.0/Makefile
themes/Mist/Makefile
themes/Mist/gtk-2.0/Makefile
themes/Redmond/Makefile
themes/Redmond/gtk-2.0/Makefile
themes/ThinIce/Makefile
themes/ThinIce/gtk-2.0/Makefile
gtk-engines-2.pc
gtk-engines.spec
po/Makefile.in
schema/Makefile
schema/clearlooks.xml.in
schema/crux-engine.xml.in
schema/glide.xml.in
schema/hcengine.xml.in
schema/industrial.xml.in
schema/mist.xml.in
schema/redmond95.xml.in
schema/thinice.xml.in
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

echo -n "      Engine Schemas - "

if test "$schemas" = "yes"; then
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

echo -n "      ClearLooks - "
if test "$clearlooks" = "yes"; then
        echo -n "Including"
        if test $animation = "yes"; then
        	echo " with animation support"
        else
        	echo " without animation support"
        fi
else
        echo "Skipping"
fi

echo -n "      Crux - "
if test "$crux" = "yes"; then
	echo "Including"
else
	echo "Skipping"
fi

echo -n "      HC - "
if test "$hc" = "yes"; then
	echo "Including"
else
	echo "Skipping"
fi

echo -n "      Industrial - "
if test "$industrial" = "yes"; then
	echo "Including"
else
	echo "Skipping"
fi

echo -n "      Mist - "
if test "$mist" = "yes"; then
	echo "Including"
else
	echo "Skipping"
fi

echo -n "      Redmond - "
if test "$redmond" = "yes"; then
	echo "Including"
else
	echo "Skipping"
fi

echo -n "      Glide - "
if test "$glide" = "yes"; then
        echo "Including"
else
        echo "Skipping"
fi

echo -n "      ThinIce - "
if test "$thinice" = "yes"; then
	echo "Including"
else
	echo "Skipping"
fi

echo -n "      Lua - "
if test "$lua" = "yes"; then
	echo "Including"
else
	echo "Skipping"
fi

echo ""
