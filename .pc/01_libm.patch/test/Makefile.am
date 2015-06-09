# gtk-engines - a collection of drawing engines for GTK+ 
# Copyright (C) 2007 Benjamin Berg <benjamin@sipsolutions.net>
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


# Initilize TESTS and XFAIL_TESTS
TESTS =
XFAIL_TESTS =

GTKRCS =				\
	gtkrcs/buildin			\
	gtkrcs/clearlooks		\
	gtkrcs/clearlooks-glossy	\
	gtkrcs/clearlooks-inverted	\
	gtkrcs/clearlooks-gummy		\
	gtkrcs/crux			\
	gtkrcs/glide			\
	gtkrcs/hc			\
	gtkrcs/industrial		\
	gtkrcs/mist			\
	gtkrcs/redmond			\
	gtkrcs/thinice

EXTRA_DIST = exported torture runinx valgrind-suppressions $(GTKRCS)


EXTRA_PROGRAMS = torturetest
torturetest_CFLAGS = $(GTK_CFLAGS)
torturetest_LDFLAGS = $(GTK_LIBS)



# Setup enviroment variables for GTK+
# ie. add $(top_builddir)/tests to the module searchpath, so that GTK+ finds
# the engines in tests/engines
# Also set the debug-blocks debugging of g_slice, will be overridden during the valgrind test.
# Unset GTK_MODULES to not load a11y stuff or similar.
TESTS_ENVIRONMENT=GTK_PATH="." G_SLICE=debug-blocks GTK_MODULES=""

VALGRIND_COMMAND=valgrind --leak-resolution=high --leak-check=full --suppressions=$(srcdir)/valgrind-suppressions --num-callers=20 --log-file=valgrind-logs/THEME
# set some flags to make glib more valgrind friendly
VALGRIND_ENV=G_SLICE=always-malloc G_DEBUG=gc-friendly,resident-modules

#################################################################
#################################################################

#############################################################
# Exported Symbols Test - Tests all engines unconditionally
#############################################################
# Prefix with exported_
EXPORTED_SYMBOLS_TESTS = $(patsubst %,exported_%,$(BUILD_ENGINES))

TESTS += $(EXPORTED_SYMBOLS_TESTS)


#############################################################
# Torture Test
#############################################################
SUPPORTED_TORTURE_TEST_ENGINES = \
	clearlooks	\
	crux		\
	glide		\
	hc		\
	industrial	\
	mist		\
	redmond		\
	thinice

# Filter engines that are not build from the set of engines
TORTURE_TEST_ENGINES = $(filter $(SUPPORTED_TORTURE_TEST_ENGINES),$(BUILD_ENGINES))
# Prefix with torture_
TORTURE_TEST_TESTS = torture_buildin $(patsubst %,torture_%,$(TORTURE_TEST_ENGINES))

# Add TORTURE_TEST_ENGINES to list of tests
TESTS += $(TORTURE_TEST_TESTS)

# Possible other tests:
#  - An extensive theme switch tests that loads/unloads the engine
#    multiple times. A requirement here is that multiple rc styles need
#    to be merged.
#  - RC style merging? Don't think there is a way to do that.
#  - Maybe a visual test that looks for visible changes? That way one can
#    check that a modification does not break anything. (eg. when doing
#    code cleanups)
#  - I would love to have some coverage statistics for the torture test
#    anyone knows how to do that?

#################################################################
#################################################################

engines:
	-rm -rf engines
	mkdir engines
	cd engines; \
	for engine in $(BUILD_ENGINES); do \
		# Fail if the engine does not exist. \
		# Maybe we should force a build or something? \
		if [ ! -f ../$(top_builddir)/engines/$$engine/.libs/*.so ]; then \
			exit 1; \
		fi; \
		ln -s ../$(top_builddir)/engines/$$engine/.libs/*.so lib$$engine.so ; \
	done;

CLEANFILES = \
	torture_buildin-failed \
	$(EXTRA_PROGRAMS)

clean-local:
	-$(RM) torture_*
	-$(RM) exported_*
	-$(RM) -r engines
	-$(RM) -r valgrind-logs
	-$(RM) -r symbols

valgrind-logs:
	-$(RM) -r valgrind-logs
	mkdir valgrind-logs

symbols:
	mkdir symbols

torture_%: torture torturetest engines
	cp $(srcdir)/torture $@

exported_%: exported engines symbols
	cp $(srcdir)/exported $@

check-valgrind: valgrind-logs
	$(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(TESTS_ENVIRONMENT) $(VALGRIND_ENV) EXEC_WRAPPER=\"$(VALGRIND_COMMAND)\""

test:
	@$(MAKE) $(AM_MAKEFLAGS) check

.PHONY: engines valgrind-logs

-include $(top_srcdir)/git.mk

