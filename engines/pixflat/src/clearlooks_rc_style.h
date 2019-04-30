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
 */

#include <gtk/gtk.h>
#include "clearlooks_types.h"

#ifndef CLEARLOOKS_RC_STYLE_H
#define CLEARLOOKS_RC_STYLE_H

typedef struct _ClearlooksRcStyle ClearlooksPixRcStyle;
typedef struct _ClearlooksRcStyleClass ClearlooksPixRcStyleClass;

#define CLEARLOOKS_TYPE_RC_STYLE              (clearlooks_rc_style_get_type ())
#define CLEARLOOKS_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), CLEARLOOKS_TYPE_RC_STYLE, ClearlooksPixRcStyle))
#define CLEARLOOKS_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CLEARLOOKS_TYPE_RC_STYLE, ClearlooksPixRcStyleClass))
#define CLEARLOOKS_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), CLEARLOOKS_TYPE_RC_STYLE))
#define CLEARLOOKS_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CLEARLOOKS_TYPE_RC_STYLE))
#define CLEARLOOKS_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CLEARLOOKS_TYPE_RC_STYLE, ClearlooksPixRcStyleClass))

/* XXX: needs fixing! */
typedef enum {
	CL_FLAG_STYLE              = 1 <<  0,
	CL_FLAG_FOCUS_COLOR        = 1 <<  1,
	CL_FLAG_SCROLLBAR_COLOR    = 1 <<  2,
	CL_FLAG_COLORIZE_SCROLLBAR = 1 <<  3,
	CL_FLAG_CONTRAST           = 1 <<  4,
	CL_FLAG_RELIEFSTYLE        = 1 <<  5,
	CL_FLAG_MENUBARSTYLE       = 1 <<  6,
	CL_FLAG_TOOLBARSTYLE       = 1 <<  7,
	CL_FLAG_ANIMATION          = 1 <<  8,
	CL_FLAG_RADIUS             = 1 <<  9,
	CL_FLAG_HINT               = 1 <<  10,
	CL_FLAG_DISABLE_FOCUS      = 1 <<  11
} ClearlooksRcFlags;


struct _ClearlooksRcStyle
{
	GtkRcStyle parent_instance;

	ClearlooksRcFlags flags;

	ClearlooksStyles style;

	GdkColor focus_color;
	GdkColor scrollbar_color;
	gboolean colorize_scrollbar;
	double contrast;
	guint8 reliefstyle;
	guint8 menubarstyle;
	guint8 toolbarstyle;
	gboolean animation;
	double radius;
	GQuark hint;
	gboolean disable_focus;
};

struct _ClearlooksRcStyleClass
{
	GtkRcStyleClass parent_class;
};

GE_INTERNAL void  clearlooks_rc_style_register_types (GTypeModule *module);
GE_INTERNAL GType clearlooks_rc_style_get_type       (void);

#endif /* CLEARLOOKS_RC_STYLE_H */
