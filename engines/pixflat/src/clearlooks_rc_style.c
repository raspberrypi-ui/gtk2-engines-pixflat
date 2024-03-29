/* Clearlooks - a cairo based GTK+ engine
 * Copyright (C) 2005 Richard Stellingwerff <remenic@gmail.com>
 * Copyright (C) 2007 Benjamin Berg <benjamin@sipsolutions.net>
 * Copyright (C) 2007-2008 Andrea Cimitan <andrea.cimitan@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Project contact: <gnome-themes-list@gnome.org>
 *
 *
 * Originally written by Owen Taylor <otaylor@redhat.com>
 *                and by Alexander Larsson <alexl@redhat.com>
 * Modified by Richard Stellingwerff <remenic@gmail.com>
 * Modified by Kulyk Nazar <schamane@myeburg.net>
 *
 */
 

#include <string.h>
#include <widget-information.h>
#include "clearlooks_style.h"
#include "clearlooks_rc_style.h"

#ifdef HAVE_WORKING_ANIMATION
static void      clearlooks_rc_style_finalize     (GObject                *object);
#endif
static GtkStyle *clearlooks_rc_style_create_style (GtkRcStyle             *rc_style);
static guint     clearlooks_rc_style_parse        (GtkRcStyle             *rc_style,
                                                   GtkSettings            *settings,
                                                   GScanner               *scanner);
static void      clearlooks_rc_style_merge        (GtkRcStyle             *dest,
                                                   GtkRcStyle             *src);

enum
{
	TOKEN_FOCUSCOLOR = G_TOKEN_LAST + 1,
	TOKEN_SCROLLBARCOLOR,
	TOKEN_COLORIZESCROLLBAR,
	TOKEN_CONTRAST,
	TOKEN_SUNKENMENU,
	TOKEN_PROGRESSBARSTYLE,
	TOKEN_RELIEFSTYLE,
	TOKEN_MENUBARSTYLE,
	TOKEN_TOOLBARSTYLE,
	TOKEN_MENUITEMSTYLE,
	TOKEN_LISTVIEWITEMSTYLE,
	TOKEN_ANIMATION,
	TOKEN_STYLE,
	TOKEN_RADIUS,
	TOKEN_HINT,
	TOKEN_DISABLE_FOCUS,

	TOKEN_CLASSIC,
	TOKEN_GLOSSY,
	TOKEN_INVERTED,
	TOKEN_GUMMY,

	TOKEN_TRUE,
	TOKEN_FALSE,

	TOKEN_LAST
};

static gchar* clearlooks_rc_symbols =
	"focus_color\0"
	"scrollbar_color\0"
	"colorize_scrollbar\0"
	"contrast\0"
	"sunkenmenubar\0"
	"progressbarstyle\0"
	"reliefstyle\0"
	"menubarstyle\0"
	"toolbarstyle\0"
	"menuitemstyle\0"
	"listviewitemstyle\0"
	"animation\0"
	"style\0"
	"radius\0"
	"hint\0"
	"disable_focus\0"

	"CLASSIC\0"
	"GLOSSY\0"
	"INVERTED\0"
	"GUMMY\0"

	"TRUE\0"
	"FALSE\0";

G_DEFINE_DYNAMIC_TYPE (PixflatRcStyle, clearlooks_rc_style, GTK_TYPE_RC_STYLE)

void
clearlooks_rc_style_register_types (GTypeModule *module)
{
  clearlooks_rc_style_register_type (module);
}

static void
clearlooks_rc_style_init (PixflatRcStyle *clearlooks_rc)
{
	clearlooks_rc->style = CL_STYLE_CLASSIC;

	clearlooks_rc->flags = 0;

	clearlooks_rc->contrast = 1.0;
	clearlooks_rc->reliefstyle = 0;
	clearlooks_rc->menubarstyle = 0;
	clearlooks_rc->toolbarstyle = 0;
	clearlooks_rc->animation = FALSE;
	clearlooks_rc->colorize_scrollbar = FALSE;
	clearlooks_rc->radius = 3.0;
	clearlooks_rc->hint = 0;
	clearlooks_rc->disable_focus = FALSE;
}

#ifdef HAVE_WORKING_ANIMATION
static void
clearlooks_rc_style_finalize (GObject *object)
{
	/* cleanup all the animation stuff */
	clearlooks_animation_cleanup ();

	if (G_OBJECT_CLASS (clearlooks_rc_style_parent_class)->finalize != NULL)
		G_OBJECT_CLASS (clearlooks_rc_style_parent_class)->finalize (object);
}
#endif


static void
clearlooks_rc_style_class_init (PixflatRcStyleClass *klass)
{
	GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);
#ifdef HAVE_WORKING_ANIMATION
	GObjectClass    *g_object_class = G_OBJECT_CLASS (klass);
#endif

	rc_style_class->parse = clearlooks_rc_style_parse;
	rc_style_class->create_style = clearlooks_rc_style_create_style;
	rc_style_class->merge = clearlooks_rc_style_merge;

#ifdef HAVE_WORKING_ANIMATION
	g_object_class->finalize = clearlooks_rc_style_finalize;
#endif
}

static void
clearlooks_rc_style_class_finalize (PixflatRcStyleClass *klass)
{
}

static guint
clearlooks_gtk2_rc_parse_boolean (GtkSettings *settings,
                                  GScanner     *scanner,
                                  gboolean *retval)
{
	guint token;
	token = g_scanner_get_next_token(scanner);

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	token = g_scanner_get_next_token(scanner);
	if (token == TOKEN_TRUE)
	   *retval = TRUE;
	else if (token == TOKEN_FALSE)
	   *retval = FALSE;
	else
	   return TOKEN_TRUE;

	return G_TOKEN_NONE;
}

static guint
clearlooks_gtk2_rc_parse_color(GtkSettings  *settings,
                               GScanner     *scanner,
                               GtkRcStyle   *style,
                               GdkColor     *color)
{
	guint token;

	/* Skip 'blah_color' */
	token = g_scanner_get_next_token(scanner);

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	return gtk_rc_parse_color_full (scanner, style, color);
}

static guint
clearlooks_gtk2_rc_parse_double (GtkSettings  *settings,
                                 GScanner     *scanner,
                                 gdouble      *val)
{
	guint token;

	/* Skip 'blah' */
	token = g_scanner_get_next_token(scanner);

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_FLOAT)
	   return G_TOKEN_FLOAT;

	*val = scanner->value.v_float;

	return G_TOKEN_NONE;
}

static guint
clearlooks_gtk2_rc_parse_int (GtkSettings  *settings,
                              GScanner     *scanner,
                              guint8       *progressbarstyle)
{
	guint token;

	/* Skip option name */
	token = g_scanner_get_next_token(scanner);

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_INT)
	   return G_TOKEN_INT;

	*progressbarstyle = scanner->value.v_int;

	return G_TOKEN_NONE;
}

static guint
clearlooks_gtk2_rc_parse_style (GtkSettings      *settings,
                                GScanner         *scanner,
                                ClearlooksStyles *style)
{
	guint token;

	g_assert (CL_NUM_STYLES == CL_STYLE_CLASSIC + 1); /* so that people don't forget ;-) */

	/* Skip 'style' */
	token = g_scanner_get_next_token (scanner);

	token = g_scanner_get_next_token (scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	token = g_scanner_get_next_token (scanner);

	switch (token)
	{
		case TOKEN_CLASSIC:
		   *style = CL_STYLE_CLASSIC;
		   break;
		default:
		   return TOKEN_CLASSIC;
	}

	return G_TOKEN_NONE;
}

static guint
clearlooks_gtk2_rc_parse_dummy (GtkSettings      *settings,
                                GScanner         *scanner,
                                gchar            *name)
{
	guint token;

	/* Skip option */
	token = g_scanner_get_next_token (scanner);

	/* print a warning. Isn't there a way to get the string from the scanner? */
	g_scanner_warn (scanner, "Clearlooks configuration option \"%s\" is not supported and will be ignored.", name);

	/* equal sign */
	token = g_scanner_get_next_token (scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	/* eat whatever comes next */
	token = g_scanner_get_next_token (scanner);

	return G_TOKEN_NONE;
}

static guint
clearlooks_rc_style_parse (GtkRcStyle *rc_style,
                           GtkSettings  *settings,
                           GScanner   *scanner)
{
	static GQuark scope_id = 0;
	PixflatRcStyle *clearlooks_style = CLEARLOOKS_RC_STYLE (rc_style);

	guint old_scope;
	guint token;

	/* Set up a new scope in this scanner. */

	if (!scope_id)
	   scope_id = g_quark_from_string("clearlooks_theme_engine");

	/* If we bail out due to errors, we *don't* reset the scope, so the
	* error messaging code can make sense of our tokens.
	*/
	old_scope = g_scanner_set_scope(scanner, scope_id);

	/* Now check if we already added our symbols to this scope
	* (in some previous call to clearlooks_rc_style_parse for the
	* same scanner.
	*/
	if (!g_scanner_lookup_symbol(scanner, clearlooks_rc_symbols)) {
		gchar *current_symbol = clearlooks_rc_symbols;
		gint i = G_TOKEN_LAST + 1;

		/* Add our symbols */
		while ((current_symbol[0] != '\0') && (i < TOKEN_LAST)) {
			g_scanner_scope_add_symbol(scanner, scope_id, current_symbol, GINT_TO_POINTER (i));

			current_symbol += strlen(current_symbol) + 1;
			i++;
		}
		g_assert (i == TOKEN_LAST && current_symbol[0] == '\0');
	}

	/* We're ready to go, now parse the top level */

	token = g_scanner_peek_next_token(scanner);
	while (token != G_TOKEN_RIGHT_CURLY)
	{
		switch (token)
		{
			case TOKEN_FOCUSCOLOR:
				token = clearlooks_gtk2_rc_parse_color (settings, scanner, rc_style, &clearlooks_style->focus_color);
				clearlooks_style->flags |= CL_FLAG_FOCUS_COLOR;
				break;
			case TOKEN_SCROLLBARCOLOR:
				token = clearlooks_gtk2_rc_parse_color (settings, scanner, rc_style, &clearlooks_style->scrollbar_color);
				clearlooks_style->flags |= CL_FLAG_SCROLLBAR_COLOR;
				break;
			case TOKEN_COLORIZESCROLLBAR:
				token = clearlooks_gtk2_rc_parse_boolean (settings, scanner, &clearlooks_style->colorize_scrollbar);
				clearlooks_style->flags |= CL_FLAG_COLORIZE_SCROLLBAR;
				break;
			case TOKEN_CONTRAST:
				token = clearlooks_gtk2_rc_parse_double (settings, scanner, &clearlooks_style->contrast);
				clearlooks_style->flags |= CL_FLAG_CONTRAST;
				break;
			case TOKEN_RELIEFSTYLE:
				token = clearlooks_gtk2_rc_parse_int (settings, scanner, &clearlooks_style->reliefstyle);
				clearlooks_style->flags |= CL_FLAG_RELIEFSTYLE;
				break;
			case TOKEN_MENUBARSTYLE:
				token = clearlooks_gtk2_rc_parse_int (settings, scanner, &clearlooks_style->menubarstyle);
				clearlooks_style->flags |= CL_FLAG_MENUBARSTYLE;
				break;
			case TOKEN_TOOLBARSTYLE:
				token = clearlooks_gtk2_rc_parse_int (settings, scanner, &clearlooks_style->toolbarstyle);
				clearlooks_style->flags |= CL_FLAG_TOOLBARSTYLE;
				break;
			case TOKEN_ANIMATION:
				token = clearlooks_gtk2_rc_parse_boolean (settings, scanner, &clearlooks_style->animation);
				clearlooks_style->flags |= CL_FLAG_ANIMATION;
				break;
			case TOKEN_STYLE:
				token = clearlooks_gtk2_rc_parse_style (settings, scanner, &clearlooks_style->style);
				clearlooks_style->flags |= CL_FLAG_STYLE;
				break;
			case TOKEN_RADIUS:
				token = clearlooks_gtk2_rc_parse_double (settings, scanner, &clearlooks_style->radius);
				clearlooks_style->flags |= CL_FLAG_RADIUS;
				break;
			case TOKEN_HINT:
				token = ge_rc_parse_hint (scanner, &clearlooks_style->hint);
				clearlooks_style->flags |= CL_FLAG_HINT;
				break;
			case TOKEN_DISABLE_FOCUS:
				token = clearlooks_gtk2_rc_parse_boolean (settings, scanner, &clearlooks_style->disable_focus);
				clearlooks_style->flags |= CL_FLAG_DISABLE_FOCUS;
				break;

			/* stuff to ignore */
			case TOKEN_SUNKENMENU:
				token = clearlooks_gtk2_rc_parse_dummy (settings, scanner, "sunkenmenu");
				break;
			case TOKEN_PROGRESSBARSTYLE:
				token = clearlooks_gtk2_rc_parse_dummy (settings, scanner, "progressbarstyle");
				break;
			case TOKEN_MENUITEMSTYLE:
				token = clearlooks_gtk2_rc_parse_dummy (settings, scanner, "menuitemstyle");
				break;
			case TOKEN_LISTVIEWITEMSTYLE:
				token = clearlooks_gtk2_rc_parse_dummy (settings, scanner, "listviewitemstyle");
				break;

			default:
				g_scanner_get_next_token(scanner);
				token = G_TOKEN_RIGHT_CURLY;
				break;
		}

		if (token != G_TOKEN_NONE)
			return token;

		token = g_scanner_peek_next_token(scanner);
	}

	g_scanner_get_next_token(scanner);

	g_scanner_set_scope(scanner, old_scope);

	return G_TOKEN_NONE;
}

static void
clearlooks_rc_style_merge (GtkRcStyle *dest,
                           GtkRcStyle *src)
{
	PixflatRcStyle *dest_w, *src_w;
	ClearlooksRcFlags flags;

	GTK_RC_STYLE_CLASS (clearlooks_rc_style_parent_class)->merge (dest, src);

	if (!CLEARLOOKS_IS_RC_STYLE (src))
		return;

	src_w = CLEARLOOKS_RC_STYLE (src);
	dest_w = CLEARLOOKS_RC_STYLE (dest);

	flags = (~dest_w->flags) & src_w->flags;

	if (flags & CL_FLAG_STYLE)
		dest_w->style = src_w->style;
	if (flags & CL_FLAG_CONTRAST)
		dest_w->contrast = src_w->contrast;
	if (flags & CL_FLAG_RELIEFSTYLE)
		dest_w->reliefstyle = src_w->reliefstyle;
	if (flags & CL_FLAG_MENUBARSTYLE)
		dest_w->menubarstyle = src_w->menubarstyle;
	if (flags & CL_FLAG_TOOLBARSTYLE)
		dest_w->toolbarstyle = src_w->toolbarstyle;
	if (flags & CL_FLAG_FOCUS_COLOR)
		dest_w->focus_color = src_w->focus_color;
	if (flags & CL_FLAG_SCROLLBAR_COLOR)
		dest_w->scrollbar_color = src_w->scrollbar_color;
	if (flags & CL_FLAG_COLORIZE_SCROLLBAR)
		dest_w->colorize_scrollbar = src_w->colorize_scrollbar;
	if (flags & CL_FLAG_ANIMATION)
		dest_w->animation = src_w->animation;
	if (flags & CL_FLAG_RADIUS)
		dest_w->radius = src_w->radius;
	if (flags & CL_FLAG_HINT)
		dest_w->hint = src_w->hint;
	if (flags & CL_FLAG_DISABLE_FOCUS)
		dest_w->disable_focus = src_w->disable_focus;

	dest_w->flags |= src_w->flags;
}


/* Create an empty style suitable to this RC style
 */
static GtkStyle *
clearlooks_rc_style_create_style (GtkRcStyle *rc_style)
{
	return GTK_STYLE (g_object_new (CLEARLOOKS_TYPE_STYLE, NULL));
}
