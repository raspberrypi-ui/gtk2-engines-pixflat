/* Clearlooks - a cairo based GTK+ engine
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
 */
 
 
#ifndef CLEARLOOKS_DRAW_H
#define CLEARLOOKS_DRAW_H

#include "clearlooks_types.h"
#include "clearlooks_style.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <cairo.h>

GE_INTERNAL void clearlooks_register_style_classic (ClearlooksStyleFunctions *functions, ClearlooksStyleConstants *constants);
GE_INTERNAL void clearlooks_register_style_glossy (ClearlooksStyleFunctions *functions, ClearlooksStyleConstants *constants);
GE_INTERNAL void clearlooks_register_style_gummy (ClearlooksStyleFunctions *functions, ClearlooksStyleConstants *constants);
GE_INTERNAL void clearlooks_register_style_inverted (ClearlooksStyleFunctions *functions, ClearlooksStyleConstants *constants);

/* Fallback focus function */
GE_INTERNAL void clearlooks_draw_focus (cairo_t *cr,
                                        const ClearlooksColors *colors,
                                        const WidgetParameters *widget,
                                        const FocusParameters  *focus,
                                        int x, int y, int width, int height);

GE_INTERNAL void clearlooks_set_mixed_color (cairo_t          *cr,
                                             const CairoColor *color1,
                                             const CairoColor *color2,
                                             gdouble mix_factor);

#endif /* CLEARLOOKS_DRAW_H */
