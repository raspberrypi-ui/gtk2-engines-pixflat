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
 *
 */
 
#include <gtk/gtk.h>

#ifndef CLEARLOOKS_STYLE_H
#define CLEARLOOKS_STYLE_H

#include "animation.h"
#include "clearlooks_types.h"

typedef struct _ClearlooksStyle ClearlooksStyle;
typedef struct _ClearlooksStyleClass ClearlooksStyleClass;

#define CLEARLOOKS_TYPE_STYLE              (clearlooks_style_get_type ())
#define CLEARLOOKS_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), CLEARLOOKS_TYPE_STYLE, ClearlooksStyle))
#define CLEARLOOKS_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CLEARLOOKS_TYPE_STYLE, ClearlooksStyleClass))
#define CLEARLOOKS_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), CLEARLOOKS_TYPE_STYLE))
#define CLEARLOOKS_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CLEARLOOKS_TYPE_STYLE))
#define CLEARLOOKS_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CLEARLOOKS_TYPE_STYLE, ClearlooksStyleClass))

struct _ClearlooksStyle
{
	GtkStyle parent_instance;

	ClearlooksColors colors;

	ClearlooksStyles style;

	guint8   reliefstyle;
	guint8   menubarstyle;
	guint8   toolbarstyle;
	GdkColor focus_color;
	gboolean has_focus_color;
	GdkColor scrollbar_color;
	gboolean colorize_scrollbar;
	gboolean has_scrollbar_color;
	gboolean animation;
	gfloat   radius;
	gboolean disable_focus;
};

struct _ClearlooksStyleClass
{
	GtkStyleClass parent_class;

	ClearlooksStyleFunctions style_functions[CL_NUM_STYLES];
	ClearlooksStyleConstants style_constants[CL_NUM_STYLES];
};

GE_INTERNAL void  clearlooks_style_register_types (GTypeModule *module);
GE_INTERNAL GType clearlooks_style_get_type       (void);

#endif /* CLEARLOOKS_STYLE_H */
