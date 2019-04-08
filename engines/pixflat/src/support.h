/* Clearlooks - a cairo based GTK+ engine
 * Copyright (C) 2006 Andrew Johnson <acjgenius@earthlink.net>
 * Copyright (C) 2007 Benjamin Berg <benjamin@sipsolutions.net>
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
 

#ifndef SUPPORT_H
#define SUPPORT_H

#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

#include "clearlooks_types.h"

#define RADIO_SIZE 13
#define CHECK_SIZE 13

GE_INTERNAL void              clearlooks_treeview_get_header_index (GtkTreeView  *tv,
                                                 GtkWidget    *header,
                                                 gint         *column_index,
                                                 gint         *columns,
                                                 gboolean     *resizable);

#ifndef GTK_DISABLE_DEPRECATED
GE_INTERNAL void              clearlooks_clist_get_header_index    (GtkCList     *clist,
                                                 GtkWidget    *button,
                                                 gint         *column_index,
                                                 gint         *columns);
#endif

#ifdef DEVELOPMENT
#warning clearlooks_get_parent_bg is a bad hack - find out why its needed, and figure out a better way.
#endif

GE_INTERNAL void              clearlooks_get_parent_bg      (const GtkWidget *widget,
                                                 CairoColor      *color);

GE_INTERNAL ClearlooksStepper clearlooks_scrollbar_get_stepper         (GtkWidget       *widget,
                                                 GdkRectangle    *stepper);
GE_INTERNAL ClearlooksStepper clearlooks_scrollbar_visible_steppers    (GtkWidget       *widget);
GE_INTERNAL ClearlooksJunction clearlooks_scrollbar_get_junction       (GtkWidget    *widget);

GE_INTERNAL void clearlooks_set_toolbar_parameters (ToolbarParameters *toolbar, GtkWidget *widget, GdkWindow *window, gint x, gint y);
GE_INTERNAL void clearlooks_get_notebook_tab_position (GtkWidget *widget, gboolean *start, gboolean *end);

#endif /* SUPPORT_H */
