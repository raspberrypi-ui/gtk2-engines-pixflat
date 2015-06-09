/* Helper functions for gtk-engines
 *
 * Copyright (C) 2006 Andrew Johnson <acjgenius@earthlink.net>
 * Copyright (C) 2006-2007 Benjamin Berg <benjamin@sipsolutions.net>
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
 * Written by Andrew Johnson <acjgenius@earthlink.net>
 * Written by Benjamin Berg <benjamin@sipsolutions.net>
 *
 */

#ifndef __GENERAL_SUPPORT_H
#define __GENERAL_SUPPORT_H

#include <gmodule.h>
#include <glib.h>

/* macros to make sure that things are sane ... */

#define CHECK_DETAIL(detail, value) ((detail) && (!strcmp(value, detail)))

#define CHECK_ARGS					\
  g_return_if_fail (window != NULL);			\
  g_return_if_fail (style != NULL);

#define SANITIZE_SIZE					\
  g_return_if_fail (width  >= -1);			\
  g_return_if_fail (height >= -1);			\
                                                        \
  if ((width == -1) && (height == -1))			\
    gdk_drawable_get_size (window, &width, &height);	\
  else if (width == -1)					\
    gdk_drawable_get_size (window, &width, NULL);	\
  else if (height == -1)				\
    gdk_drawable_get_size (window, NULL, &height);

#define GE_EXPORT	G_MODULE_EXPORT
#define GE_INTERNAL	G_GNUC_INTERNAL

/* explicitly export with ggc, G_MODULE_EXPORT does not do this, this should
 * make it possible to compile with -fvisibility=hidden */
#ifdef G_HAVE_GNUC_VISIBILITY
# undef GE_EXPORT
# define GE_EXPORT	__attribute__((__visibility__("default")))
#endif

#if defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550)
# undef GE_EXPORT
# undef GE_INTERNAL
# define GE_EXPORT      __global
# define GE_INTERNAL    __hidden
#endif 

#endif /* __GENERAL_SUPPORT_H */
