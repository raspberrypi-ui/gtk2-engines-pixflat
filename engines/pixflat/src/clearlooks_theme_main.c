/* Clearlooks - a cairo based GTK+ engine
 * Copyright (C) 2005-2006 Andrew Johnson <acjgenius@earthlink.net>
 * Copyright (C) 2006 Benjamin Berg <benjamin@sipsolutions.net>
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
 

#include <gmodule.h>
#include <gtk/gtk.h>

#include "clearlooks_style.h"
#include "clearlooks_rc_style.h"

GE_EXPORT void
theme_init (GTypeModule *module)
{
  clearlooks_rc_style_register_types (module);
  clearlooks_style_register_types (module);
}

GE_EXPORT void
theme_exit (void)
{
}

GE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  return GTK_RC_STYLE (g_object_new (CLEARLOOKS_TYPE_RC_STYLE, NULL));  
}
