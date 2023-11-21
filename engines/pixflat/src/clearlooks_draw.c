/* Clearlooks - a cairo based GTK+ engine
 * Copyright (C) 2006 Richard Stellingwerff <remenic@gmail.com>
 * Copyright (C) 2006 Daniel Borgman <daniel.borgmann@gmail.com>
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


#include "clearlooks_draw.h"
#include "clearlooks_style.h"
#include "clearlooks_types.h"

#include "support.h"
#include <ge-support.h>
#include <math.h>

#include <cairo.h>

/* Normal shadings */
#define SHADE_TOP 1.055
#define SHADE_CENTER_TOP 1.01
#define SHADE_CENTER_BOTTOM 0.98
#define SHADE_BOTTOM 0.90

#define TROUGH_SIZE 8

typedef void (*menubar_draw_proto) (cairo_t *cr,
                                    const ClearlooksColors *colors,
                                    const WidgetParameters *params,
                                    const MenuBarParameters *menubar,
                                    int x, int y, int width, int height);

static void
clearlooks_draw_inset (cairo_t          *cr,
                       const CairoColor *bg_color,
                       double x, double y, double width, double height,
                       double radius, uint8 corners)
{
	CairoColor shadow;
	CairoColor highlight;
	double line_width;
	double min = MIN (width, height);

	line_width = cairo_get_line_width (cr);

	/* not really sure of shading ratios... we will think */
	ge_shade_color (bg_color, 0.94, &shadow);
	ge_shade_color (bg_color, 1.06, &highlight);

	/* highlight */
	cairo_save (cr);

	cairo_move_to (cr, x, y + height);
	cairo_line_to (cr, x + min / 2.0, y + height - min / 2.0);
	cairo_line_to (cr, x + width - min / 2.0, y + min / 2.0);
	cairo_line_to (cr, x + width, y);
	cairo_line_to (cr, x, y);
	cairo_close_path (cr);
	
	cairo_clip (cr);

	ge_cairo_rounded_rectangle (cr, x + line_width / 2.0, y + line_width / 2.0,
	                            width - line_width, height - line_width,
	                            radius, corners);

	ge_cairo_set_color (cr, &shadow);
	cairo_stroke (cr);
	
	cairo_restore (cr);

	/* shadow */
	cairo_save (cr);

	cairo_move_to (cr, x, y + height);
	cairo_line_to (cr, x + min / 2.0, y + height - min / 2.0);
	cairo_line_to (cr, x + width - min / 2.0, y + min / 2.0);
	cairo_line_to (cr, x + width, y);
	cairo_line_to (cr, x + width, y + height);
	cairo_close_path (cr);
	
	cairo_clip (cr);

	ge_cairo_rounded_rectangle (cr, x + line_width / 2.0, y + line_width / 2.0,
	                            width - line_width, height - line_width,
	                            radius, corners);

	ge_cairo_set_color (cr, &highlight);
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void
clearlooks_draw_highlight_and_shade (cairo_t *cr, const ClearlooksColors *colors,
                                     const ShadowParameters *params,
                                     int width, int height, gdouble radius)
{
	CairoColor hilight;
	CairoColor shadow;
	uint8 corners = params->corners;
	double x = 1.0;
	double y = 1.0;

	ge_shade_color (&colors->bg[0], 1.06, &hilight);
	ge_shade_color (&colors->bg[0], 0.94, &shadow);

	width  -= 2;
	height -= 2;

	cairo_save (cr);

	/* Top/Left highlight */
	if (corners & CR_CORNER_BOTTOMLEFT)
		cairo_move_to (cr, x + 0.5, y+height-radius);
	else
		cairo_move_to (cr, x + 0.5, y+height);

	ge_cairo_rounded_corner (cr, x + 0.5, y + 0.5, radius, corners & CR_CORNER_TOPLEFT);

	if (corners & CR_CORNER_TOPRIGHT)
		cairo_line_to (cr, x+width-radius, y + 0.5);
	else
		cairo_line_to (cr, x+width, y + 0.5);

	if (params->shadow & CL_SHADOW_OUT)
		ge_cairo_set_color (cr, &hilight);
	else
		ge_cairo_set_color (cr, &shadow);

	cairo_stroke (cr);

	/* Bottom/Right highlight -- this includes the corners */
	cairo_arc (cr, x + width - 0.5 - radius, y + radius, radius, G_PI * (3/2.0+1/4.0), G_PI * 2);
	ge_cairo_rounded_corner (cr, x+width - 0.5, y+height - 0.5, radius, corners & CR_CORNER_BOTTOMRIGHT);
	cairo_arc (cr, x + radius, y + height - 0.5 - radius, radius, G_PI * 1/2, G_PI * 3/4);

	if (params->shadow & CL_SHADOW_OUT)
		ge_cairo_set_color (cr, &shadow);
	else
		ge_cairo_set_color (cr, &hilight);

	cairo_stroke (cr);

	cairo_restore (cr);
}

static void
clearlooks_set_border_gradient (cairo_t *cr, const CairoColor *color, double hilight, int width, int height)
{
	cairo_pattern_t *pattern;

	CairoColor bottom_shade;
	ge_shade_color (color, hilight, &bottom_shade);

	pattern	= cairo_pattern_create_linear (0, 0, width, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0, color->r, color->g, color->b);
	cairo_pattern_add_color_stop_rgb (pattern, 1, bottom_shade.r, bottom_shade.g, bottom_shade.b);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);
}

static void pixflat_draw_button (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	int x, int y, int width, int height)
{
	double xoffset = 0, yoffset = 0;
	double radius = params->radius;
	gboolean panel = FALSE;
	const CairoColor *fill, *border;

    // an lxpanel button has a custom colour scheme in which bg[PRELIGHT] is the same as bg[SELECTED]
	CairoColor test1 = colors->bg[GTK_STATE_PRELIGHT];
	CairoColor test2 = colors->bg[GTK_STATE_SELECTED];
	if (test1.r == test2.r && test1.g == test2.g && test1.b == test2.b && test1.a == test2.a) panel = TRUE;

	if (params->xthickness >= 3 && params->ythickness >= 3)
	{
		xoffset = 1;
		yoffset = 1;
		params->style_functions->draw_inset (cr, &params->parentbg, 0, 0, width, height, radius+1, params->corners);
	}

	radius = MIN (radius, MIN ((width - 2.0 - xoffset * 2.0) / 2.0, (height - 2.0 - yoffset * 2) / 2.0));

	if (panel)
	{
		if (params->prelight)
			fill = &colors->bg[GTK_STATE_PRELIGHT];
		else if (params->active)
			fill = &colors->bg[GTK_STATE_ACTIVE];
		else 
			fill = &colors->bg[GTK_STATE_NORMAL];
		border = fill;
	}
	else if (params->active)
	{
		if (params->prelight)
		{
			fill = &colors->shade[2];
			border = &colors->shade[4];
		}
		else
		{
			fill = &colors->shade[4];
			border = &colors->shade[6];
		}
	}
	else
	{
		if (params->prelight)
		{
			fill = &colors->shade[3];
			border = &colors->shade[5];
		}
		else if (params->disabled)
		{
			fill = &colors->shade[1];
			border = &colors->shade[3];
		}
		else
		{
			fill = &colors->shade[2];
			border = &colors->shade[4];
		}
	}

	cairo_save (cr);

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1.0);

	/* fill */
	ge_cairo_rounded_rectangle (cr, xoffset+1, yoffset+1, width-(xoffset*2)-2, height-(yoffset*2)-2, 0, params->corners);
	ge_cairo_set_color (cr, fill);
	cairo_fill (cr);

	/* border */
	ge_cairo_inner_rounded_rectangle (cr, xoffset, yoffset, width-(xoffset*2), height-(yoffset*2), radius, params->corners);
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void pixflat_draw_entry (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	const FocusParameters *focus, int x, int y, int width, int height)
{
	const CairoColor *base = &colors->base[params->state_type];
	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));
	int xoffset = 0, yoffset = 0;
	const CairoColor *border;
	CairoColor shadow;

	if (params->disabled)
	{
		border = &colors->shade[3];
		ge_shade_color (&colors->shade[4], 0.925, &shadow);
	}
	else
	{
		border = &colors->shade[4];
		if (params->focus)
			ge_shade_color (&focus->color, 2.0, &shadow);
		else 
			ge_shade_color (&colors->shade[8], 0.925, &shadow);
	}

	if (params->xthickness >= 3 && params->ythickness >= 3)
	{
		xoffset = 1;
		yoffset = 1;
		params->style_functions->draw_inset (cr, &params->parentbg, 0, 0, width, height, radius+1, params->corners);
	}

	cairo_save (cr);

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1.0);

	/* background */
	ge_cairo_rounded_rectangle (cr, xoffset + 1, yoffset + 1, width - (xoffset + 1)*2, height - (yoffset + 1) * 2, MAX(0, radius-1), params->corners);
	ge_cairo_set_color (cr, base);
	cairo_fill (cr);

	/* inner shadow */
	if (params->focus)
	{
		clearlooks_set_mixed_color (cr, base, &shadow, 0.5);
		ge_cairo_inner_rounded_rectangle (cr, xoffset + 1, yoffset + 1, width - (xoffset + 1)*2, height - (yoffset + 1)*2, MAX(0, radius-1), params->corners);
		cairo_stroke (cr);
	}
	else
	{
		cairo_set_source_rgba (cr, shadow.r, shadow.g, shadow.b, params->disabled ? 0.05 : 0.1);
		cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
		cairo_move_to (cr, 2.5, height-radius);
		cairo_arc (cr, xoffset + 1.5 + MAX(0, radius-1), yoffset + 1.5 + MAX(0, radius-1), MAX(0, radius-1), G_PI, 270*(G_PI/180));
		cairo_line_to (cr, width-radius, 2.5);
		cairo_stroke (cr);
	}

	/* border */
	ge_cairo_inner_rounded_rectangle (cr, xoffset, yoffset, width-2*xoffset, height-2*yoffset, radius, params->corners);
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void
clearlooks_draw_entry_progress (cairo_t *cr,
                                const ClearlooksColors *colors,
                                const WidgetParameters *params,
                                const EntryProgressParameters *progress,
                                int x, int y, int width, int height)
{
	CairoColor border;
	CairoColor fill;
	gint entry_width, entry_height;
	double entry_radius;
	double radius;

	cairo_save (cr);

	fill = colors->bg[params->state_type];
	ge_shade_color (&fill, 0.9, &border);

	if (progress->max_size_known)
	{
		entry_width = progress->max_size.width + progress->border.left + progress->border.right;
		entry_height = progress->max_size.height + progress->border.top + progress->border.bottom;
		entry_radius = MIN (params->radius, MIN ((entry_width - 4.0) / 2.0, (entry_height - 4.0) / 2.0));
	}
	else
	{
		entry_radius = params->radius;
	}

	radius = MAX (0, entry_radius + 1.0 - MAX (MAX (progress->border.left, progress->border.right),
	                                           MAX (progress->border.top, progress->border.bottom)));

	if (progress->max_size_known)
	{
		/* Clip to the max size, and then draw a (larger) rectangle ... */
		ge_cairo_rounded_rectangle (cr, progress->max_size.x,
		                                progress->max_size.y,
		                                progress->max_size.width,
		                                progress->max_size.height,
		                                radius,
		                                CR_CORNER_ALL);
		cairo_clip (cr);

		/* We just draw wider by one pixel ... */
		ge_cairo_set_color (cr, &fill);
		cairo_rectangle (cr, x, y + 1, width, height - 2);
		cairo_fill (cr);

		cairo_set_line_width (cr, 1.0);
		ge_cairo_set_color (cr, &border);
		ge_cairo_inner_rectangle (cr, x - 1, y, width + 2, height);
		cairo_stroke (cr);
	}
	else
	{
		ge_cairo_rounded_rectangle (cr, x, y, width + 10, height + 10, radius, CR_CORNER_ALL);
		cairo_clip (cr);
		ge_cairo_rounded_rectangle (cr, x - 10, y - 10, width + 10, height + 10, radius, CR_CORNER_ALL);
		cairo_clip (cr);

		ge_cairo_set_color (cr, &fill);
		ge_cairo_rounded_rectangle (cr, x + 1, y + 1, width - 2, height - 2, radius, CR_CORNER_ALL);
		cairo_fill (cr);

		cairo_set_line_width (cr, 1.0);
		ge_cairo_set_color (cr, &border);
		ge_cairo_rounded_rectangle (cr, x + 0.5, y + 0.5, width - 1.0, height - 1.0, radius, CR_CORNER_ALL);
		cairo_stroke (cr);
	}

	cairo_restore (cr);
}

static void pixflat_draw_spinbutton (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	int x, int y, int width, int height)
{
	const CairoColor *c;

	params->style_functions->draw_button (cr, colors, params, x, y, width, height);

	if (params->active)
	{
		if (params->prelight)
			c = &colors->shade[4];
		else
			c = &colors->shade[6];
	}
	else
	{
		if (params->prelight)
			c = &colors->shade[5];
		else if (params->disabled)
			c = &colors->shade[3];
		else
			c = &colors->shade[4];
	}

	cairo_translate (cr, x, y);

	cairo_move_to (cr, params->xthickness + 0.5,       (height/2) + 0.5);
	cairo_line_to (cr, width-params->xthickness - 0.5, (height/2) + 0.5);
	ge_cairo_set_color (cr, c);
	cairo_stroke (cr);
}

static void pixflat_draw_spinbutton_down (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	int x, int y, int width, int height)
{
	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));
	const CairoColor *shadow;

	if (params->active)
	{
		if (params->prelight)
			shadow = &colors->shade[2];
		else
			shadow = &colors->shade[4];
	}
	else
	{
		if (params->prelight)
			shadow = &colors->shade[3];
		else
			shadow = &colors->shade[2];
	}

	cairo_translate (cr, x+1, y+1);

	ge_cairo_rounded_rectangle (cr, 1, 1, width-4, height-4, radius, params->corners);
	ge_cairo_set_color (cr, shadow);
	cairo_fill (cr);
}

static void pixflat_draw_scale_trough (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	const SliderParameters *slider, int x, int y, int width, int height)
{
	int     trough_width, trough_height;
	double  translate_x, translate_y;

	if (slider->horizontal)
	{
		trough_width  = width;
		trough_height = TROUGH_SIZE;

		translate_x   = x;
		translate_y   = y + (height/2) - (TROUGH_SIZE/2);
	}
	else
	{
		trough_width  = TROUGH_SIZE;
		trough_height = height;

		translate_x   = x + (width/2) - (TROUGH_SIZE/2);
		translate_y  = y;
	}

	cairo_save (cr);

	cairo_translate (cr, translate_x, translate_y);
	cairo_set_line_width (cr, 1.0);

	if (!slider->lower && !slider->fill_level)
	{
		CairoColor fill;
		ge_shade_color (&colors->shade[0], 0.83, &fill);
		ge_cairo_set_color (cr, &fill);
		cairo_rectangle (cr, 1.0, 1.0, trough_width - 2, trough_height - 2);
		cairo_fill (cr);

		ge_cairo_set_color (cr, &colors->shade[4]);
		ge_cairo_inner_rectangle (cr, 1.0, 1.0, trough_width - 2, trough_height - 2);
		cairo_stroke (cr);
	}
	else
	{
		CairoColor border = colors->spot[2];
		border.a = 0.64;

		ge_cairo_set_color (cr, &colors->bg[3]);
		cairo_rectangle (cr, 1.0, 1.0, trough_width - 2, trough_height - 2);
		cairo_fill (cr);

		ge_cairo_set_color (cr, &border);
		ge_cairo_inner_rectangle (cr, 1.0, 1.0, trough_width - 2, trough_height - 2);
		cairo_stroke (cr);
	}

	cairo_restore (cr);
}

static void pixflat_draw_slider_button (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	const SliderParameters *slider, int x, int y, int width, int height)
{
	const CairoColor *fill, *border;

	cairo_save (cr);

	if (!slider->horizontal) ge_cairo_exchange_axis (cr, &x, &y, &width, &height);
	cairo_translate (cr, x, y);

	if (params->active)
	{
		if (params->prelight)
		{
			fill = &colors->shade[2];
			border = &colors->shade[4];
		}
		else
		{
			fill = &colors->shade[4];
			border = &colors->shade[6];
		}
	}
	else
	{
		if (params->prelight)
		{
			fill = &colors->shade[3];
			border = &colors->shade[5];
		}
		else if (params->disabled)
		{
			fill = &colors->shade[1];
			border = &colors->shade[3];
		}
		else
		{
			fill = &colors->shade[2];
			border = &colors->shade[4];
		}
	}

	cairo_set_line_width (cr, 1.0);
	cairo_translate (cr, 1, 1);

	/* set clip */
	cairo_rectangle (cr, 0.0, 0.0, width - 2, height - 2);
	cairo_clip_preserve (cr);

	/* handle fill */
	ge_cairo_rounded_rectangle (cr, 1.0, 0.0, width - 2, height - 2, params->radius, params->corners);
	ge_cairo_set_color (cr, fill);
	cairo_fill (cr);

	/* handle border */
	ge_cairo_inner_rounded_rectangle (cr, 0, 0, width - 2, height - 2, params->radius, params->corners);
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void pixflat_draw_progressbar_trough (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	int x, int y, int width, int height)
{
	double radius = MIN (params->radius, MIN ((height-2.0) / 2.0, (width-2.0) / 2.0));

	cairo_save (cr);

	cairo_set_line_width (cr, 1.0);

	/* trough */
	CairoColor fill;
	ge_shade_color (&colors->shade[0], 0.83, &fill);
	ge_cairo_set_color (cr, &fill);
	ge_cairo_rounded_rectangle (cr, x+1, y+1, width-2, height-2, radius, params->corners);
	cairo_fill (cr);

	/* border */
	ge_cairo_rounded_rectangle (cr, x+0.5, y+0.5, width-1, height-1, radius, params->corners);
	ge_cairo_set_color (cr, &colors->shade[7]);
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void pixflat_draw_progressbar_fill (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	const ProgressBarParameters *progressbar, int x, int y, int width, int height, gint offset)
{
	boolean      is_horizontal = progressbar->orientation < 2;
	double       radius;
	CairoColor   border;

	radius = MAX (0, params->radius - params->xthickness);
	radius = MIN (radius, height / 2.0);

	cairo_save (cr);

	if (!is_horizontal)
		ge_cairo_exchange_axis (cr, &x, &y, &width, &height);

	if ((progressbar->orientation == CL_ORIENTATION_RIGHT_TO_LEFT) || (progressbar->orientation == CL_ORIENTATION_BOTTOM_TO_TOP))
		ge_cairo_mirror (cr, CR_MIRROR_HORIZONTAL, &x, &y, &width, &height);

	cairo_translate (cr, x, y);

	cairo_save (cr);

	/* fill */
	ge_cairo_set_color (cr, &colors->bg[3]);
	cairo_paint (cr);

	/* border */
	border = colors->spot[2];
	border.a = 0.6;
	ge_cairo_rounded_rectangle (cr, 0.5, 0.5, width-1, height-1, radius, CR_CORNER_ALL);
	ge_cairo_set_color (cr, &border);
	cairo_stroke (cr);

	cairo_restore (cr);

	cairo_restore (cr); /* rotation, mirroring */
}

static void
clearlooks_draw_optionmenu (cairo_t *cr,
                            const ClearlooksColors *colors,
                            const WidgetParameters *params,
                            const OptionMenuParameters *optionmenu,
                            int x, int y, int width, int height)
{
	SeparatorParameters separator;
	int offset = params->ythickness + 2;

	params->style_functions->draw_button (cr, colors, params, x, y, width, height);

	separator.horizontal = FALSE;
	params->style_functions->draw_separator (cr, colors, params, &separator, x+optionmenu->linepos, y + offset, 2, height - offset*2);
}

static void
clearlooks_draw_menu_item_separator (cairo_t                   *cr,
                                     const ClearlooksColors    *colors,
                                     const WidgetParameters    *widget,
                                     const SeparatorParameters *separator,
                                     int x, int y, int width, int height)
{
	cairo_save (cr);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
	ge_cairo_set_color (cr, &colors->fg[0]);

	if (separator->horizontal)
		cairo_rectangle (cr, x, y, width, 1);
	else
		cairo_rectangle (cr, x, y, 1, height);

	cairo_fill      (cr);

	cairo_restore (cr);
}

static void
clearlooks_draw_menubar0 (cairo_t *cr,
                          const ClearlooksColors *colors,
                          const WidgetParameters *params,
                          const MenuBarParameters *menubar,
                          int x, int y, int width, int height)
{
	const CairoColor *dark = &colors->shade[4];

	cairo_save (cr);

	cairo_set_line_width (cr, 1);
	cairo_translate (cr, x, y);

	cairo_move_to (cr, 0, height-0.5);
	cairo_line_to (cr, width, height-0.5);
	ge_cairo_set_color (cr, dark);
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void
clearlooks_draw_menubar2 (cairo_t *cr,
                          const ClearlooksColors *colors,
                          const WidgetParameters *params,
                          const MenuBarParameters *menubar,
                          int x, int y, int width, int height)
{
	CairoColor lower;
	cairo_pattern_t *pattern;

	cairo_save (cr);

	ge_shade_color (&colors->bg[0], 0.96, &lower);

	cairo_translate (cr, x, y);
	cairo_rectangle (cr, 0, 0, width, height);

	/* Draw the gradient */
	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.0, colors->bg[0].r,
	                                                colors->bg[0].g,
	                                                colors->bg[0].b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, lower.r,
	                                                lower.g,
	                                                lower.b);
	cairo_set_source      (cr, pattern);
	cairo_fill            (cr);
	cairo_pattern_destroy (pattern);

	/* Draw bottom line */
	cairo_set_line_width (cr, 1.0);
	cairo_move_to        (cr, 0, height-0.5);
	cairo_line_to        (cr, width, height-0.5);
	ge_cairo_set_color   (cr, &colors->shade[4]);
	cairo_stroke         (cr);

	cairo_restore (cr);
}

static void
clearlooks_draw_menubar1 (cairo_t *cr,
                          const ClearlooksColors *colors,
                          const WidgetParameters *params,
                          const MenuBarParameters *menubar,
                          int x, int y, int width, int height)
{
	const CairoColor *border = &colors->shade[4];

	clearlooks_draw_menubar2 (cr, colors, params, menubar,
	                          x, y, width, height);

	ge_cairo_set_color (cr, border);
	ge_cairo_stroke_rectangle (cr, 0.5, 0.5, width-1, height-1);
}


static menubar_draw_proto clearlooks_menubar_draw[3] =
{
	clearlooks_draw_menubar0,
	clearlooks_draw_menubar1,
	clearlooks_draw_menubar2
};

static void
clearlooks_draw_menubar (cairo_t *cr,
                         const ClearlooksColors *colors,
                         const WidgetParameters *params,
                         const MenuBarParameters *menubar,
                         int x, int y, int width, int height)
{
	if (menubar->style < 0 || menubar->style >= G_N_ELEMENTS (clearlooks_menubar_draw))
		return;

	clearlooks_menubar_draw[menubar->style](cr, colors, params, menubar,
	                             x, y, width, height);
}

static void
clearlooks_get_frame_gap_clip (int x, int y, int width, int height,
                               const FrameParameters     *frame,
                               ClearlooksRectangle *bevel,
                               ClearlooksRectangle *border)
{
	if (frame->gap_side == CL_GAP_TOP)
	{
		CLEARLOOKS_RECTANGLE_SET (*bevel,  2.0 + frame->gap_x,  0.0,
		                          frame->gap_width - 3, 2.0);
		CLEARLOOKS_RECTANGLE_SET (*border, 1.0 + frame->gap_x,  0.0,
		                         frame->gap_width - 2, 2.0);
	}
	else if (frame->gap_side == CL_GAP_BOTTOM)
	{
		CLEARLOOKS_RECTANGLE_SET (*bevel,  2.0 + frame->gap_x,  height - 2.0,
		                          frame->gap_width - 3, 2.0);
		CLEARLOOKS_RECTANGLE_SET (*border, 1.0 + frame->gap_x,  height - 1.0,
		                          frame->gap_width - 2, 2.0);
	}
	else if (frame->gap_side == CL_GAP_LEFT)
	{
		CLEARLOOKS_RECTANGLE_SET (*bevel,  0.0, 2.0 + frame->gap_x,
		                          2.0, frame->gap_width - 3);
		CLEARLOOKS_RECTANGLE_SET (*border, 0.0, 1.0 + frame->gap_x,
		                          1.0, frame->gap_width - 2);
	}
	else if (frame->gap_side == CL_GAP_RIGHT)
	{
		CLEARLOOKS_RECTANGLE_SET (*bevel,  width - 2.0, 2.0 + frame->gap_x,
		                          2.0, frame->gap_width - 3);
		CLEARLOOKS_RECTANGLE_SET (*border, width - 1.0, 1.0 + frame->gap_x,
		                          1.0, frame->gap_width - 2);
	}
}

static void pixflat_draw_frame (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	const FrameParameters *frame, int x, int y, int width, int height)
{
	double radius = MIN (params->radius, MIN ((width - 2.0) / 2.0, (height - 2.0) / 2.0));

	if (frame->shadow == CL_SHADOW_NONE) return;

	cairo_save (cr);

	cairo_set_line_width (cr, 1.0);
	cairo_translate (cr, x, y);

	if (frame->gap_x != -1)
	{
		ClearlooksRectangle bevel_clip = {0, 0, 0, 0}, frame_clip = {0, 0, 0, 0};

		/* clip for gap */
		clearlooks_get_frame_gap_clip (x, y, width, height, frame, &bevel_clip, &frame_clip);
		cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
		cairo_rectangle (cr, 0, 0, width, height);
		cairo_rectangle (cr, frame_clip.x, frame_clip.y, frame_clip.width, frame_clip.height);
		cairo_clip  (cr);
	}

	/* frame */
	ge_cairo_set_color (cr, &colors->fg[GTK_STATE_NORMAL]);
	ge_cairo_inner_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void pixflat_draw_tab (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *params,
	const TabParameters *tab, int x, int y, int width, int height)
{
	double radius = MIN (params->radius, MIN ((width - 2.0) / 2.0, (height - 2.0) / 2.0));

	cairo_save (cr);

	/* clip */
	cairo_rectangle (cr, x, y, width, height);
	cairo_clip (cr);
	cairo_new_path (cr);

	/* Translate and set line width */
	cairo_set_line_width (cr, 1.0);
	cairo_translate (cr, x, y);

	/* Make the tabs slightly bigger than they should be, to create a gap */
	switch (tab->gap_side)
	{
		case CL_GAP_TOP : 		cairo_translate (cr, 0.0, -3.0);
		case CL_GAP_BOTTOM :	height += 3.0;
								break;
		case CL_GAP_LEFT :		cairo_translate (cr, -3.0, 0.0);
		case CL_GAP_RIGHT :		width += 3.0;
								break;
	}

	/* fill */
	if (params->active) ge_cairo_set_color (cr, &colors->bg[GTK_STATE_ACTIVE]);
	else
	{
		if (params->prelight) ge_cairo_set_color (cr, &colors->bg[GTK_STATE_PRELIGHT]);
		else ge_cairo_set_color (cr, &colors->bg[GTK_STATE_NORMAL]);
	}
	ge_cairo_rounded_rectangle (cr, 0.5, 0.5, width-1, height-1, radius, params->corners);
	cairo_fill (cr);

	/* border */
	if (params->active) ge_cairo_set_color (cr, &colors->fg[GTK_STATE_ACTIVE]);
	else ge_cairo_set_color (cr, &colors->fg[GTK_STATE_NORMAL]);
	ge_cairo_inner_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void
clearlooks_draw_separator (cairo_t *cr,
                           const ClearlooksColors     *colors,
                           const WidgetParameters     *widget,
                           const SeparatorParameters  *separator,
                           int x, int y, int width, int height)
{
	CairoColor color = colors->shade[2];
	CairoColor hilight;
	ge_shade_color (&colors->bg[0], 1.065, &hilight);

	cairo_save (cr);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);

	if (separator->horizontal)
	{
		cairo_set_line_width  (cr, 1.0);
		cairo_translate       (cr, x, y+0.5);

		cairo_move_to         (cr, 0.0,   0.0);
		cairo_line_to         (cr, width, 0.0);
		ge_cairo_set_color    (cr, &color);
		cairo_stroke          (cr);

		cairo_move_to         (cr, 0.0,   1.0);
		cairo_line_to         (cr, width, 1.0);
		ge_cairo_set_color    (cr, &hilight);
		cairo_stroke          (cr);
	}
	else
	{
		cairo_set_line_width  (cr, 1.0);
		cairo_translate       (cr, x+0.5, y);

		cairo_move_to         (cr, 0.0, 0.0);
		cairo_line_to         (cr, 0.0, height);
		ge_cairo_set_color    (cr, &color);
		cairo_stroke          (cr);

		cairo_move_to         (cr, 1.0, 0.0);
		cairo_line_to         (cr, 1.0, height);
		ge_cairo_set_color    (cr, &hilight);
		cairo_stroke          (cr);
	}

	cairo_restore (cr);
}

static void
clearlooks_draw_list_view_header (cairo_t *cr,
                                  const ClearlooksColors          *colors,
                                  const WidgetParameters          *params,
                                  const ListViewHeaderParameters  *header,
                                  int x, int y, int width, int height)
{
	const CairoColor *border = &colors->shade[7];
	CairoColor hilight;

	ge_shade_color (&colors->bg[params->state_type],
	                params->style_constants->topleft_highlight_shade, &hilight);
	hilight.a = params->style_constants->topleft_highlight_alpha;

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1.0);

	/* Draw highlight */
	if (header->order & CL_ORDER_FIRST)
	{
		cairo_move_to (cr, 0.5, height-1);
		cairo_line_to (cr, 0.5, 0.5);
	}
	else
		cairo_move_to (cr, 0.0, 0.5);

	cairo_line_to (cr, width, 0.5);

	ge_cairo_set_color (cr, &hilight);
	cairo_stroke (cr);

	/* Draw bottom border */
	cairo_move_to (cr, 0.0, height-0.5);
	cairo_line_to (cr, width, height-0.5);
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);

	/* Draw resize grip */
	if ((params->ltr && !(header->order & CL_ORDER_LAST)) ||
	    (!params->ltr && !(header->order & CL_ORDER_FIRST)) || header->resizable)
	{
		SeparatorParameters separator;
		separator.horizontal = FALSE;

		if (params->ltr)
			params->style_functions->draw_separator (cr, colors, params, &separator,
			                                         width-1.5, 4.0, 2, height-8.0);
		else
			params->style_functions->draw_separator (cr, colors, params, &separator,
			                                         1.5, 4.0, 2, height-8.0);
	}
}

/* We can't draw transparent things here, since it will be called on the same
 * surface multiple times, when placed on a handlebox_bin or dockitem_bin */
static void
clearlooks_draw_toolbar (cairo_t *cr,
                         const ClearlooksColors          *colors,
                         const WidgetParameters          *widget,
                         const ToolbarParameters         *toolbar,
                         int x, int y, int width, int height)
{
	const CairoColor *fill  = &colors->bg[0];
	const CairoColor *dark  = &colors->shade[4];
	CairoColor light;
	ge_shade_color (fill, 1.065, &light);

	cairo_set_line_width (cr, 1.0);
	cairo_translate (cr, x, y);

	ge_cairo_set_color (cr, fill);
	cairo_paint (cr);

	if (!toolbar->topmost)
	{
		/* Draw highlight */
		cairo_move_to       (cr, 0, 0.5);
		cairo_line_to       (cr, width-1, 0.5);
		ge_cairo_set_color  (cr, &light);
		cairo_stroke        (cr);
	}

	/* Draw shadow */
	cairo_move_to       (cr, 0, height-0.5);
	cairo_line_to       (cr, width-1, height-0.5);
	ge_cairo_set_color  (cr, dark);
	cairo_stroke        (cr);
}

static void
clearlooks_draw_menuitem (cairo_t *cr,
                          const ClearlooksColors          *colors,
                          const WidgetParameters          *widget,
                          int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->spot[1];
	CairoColor fill_shade;
	CairoColor border = colors->spot[2];
	cairo_pattern_t *pattern;

	ge_shade_color (&border, 1.05, &border);
	ge_shade_color (fill, 0.85, &fill_shade);
	cairo_set_line_width (cr, 1.0);

	ge_cairo_rounded_rectangle (cr, x+0.5, y+0.5, width - 1, height - 1, widget->radius, widget->corners);

	pattern = cairo_pattern_create_linear (x, y, x, y + height);
	cairo_pattern_add_color_stop_rgb (pattern, 0,   fill->r, fill->g, fill->b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, fill_shade.r, fill_shade.g, fill_shade.b);

	cairo_set_source (cr, pattern);
	cairo_fill_preserve  (cr);
	cairo_pattern_destroy (pattern);

	ge_cairo_set_color (cr, &border);
	cairo_stroke (cr);
}

static void
clearlooks_draw_menubaritem (cairo_t *cr,
                          const ClearlooksColors          *colors,
                          const WidgetParameters          *widget,
                          int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->spot[1];
	CairoColor fill_shade;
	CairoColor border = colors->spot[2];
	cairo_pattern_t *pattern;

	ge_shade_color (&border, 1.05, &border);
	ge_shade_color (fill, 0.85, &fill_shade);

	cairo_set_line_width (cr, 1.0);
	ge_cairo_rounded_rectangle (cr, x + 0.5, y + 0.5, width - 1, height, widget->radius, widget->corners);

	pattern = cairo_pattern_create_linear (x, y, x, y + height);
	cairo_pattern_add_color_stop_rgb (pattern, 0,   fill->r, fill->g, fill->b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, fill_shade.r, fill_shade.g, fill_shade.b);

	cairo_set_source (cr, pattern);
	cairo_fill_preserve  (cr);
	cairo_pattern_destroy (pattern);

	ge_cairo_set_color (cr, &border);
	cairo_stroke_preserve (cr);
}

static void
clearlooks_draw_selected_cell (cairo_t                  *cr,
	                       const ClearlooksColors   *colors,
	                       const WidgetParameters   *params,
	                       int x, int y, int width, int height)
{
	CairoColor upper_color;
	CairoColor lower_color;
	cairo_pattern_t *pattern;
	cairo_save (cr);

	cairo_translate (cr, x, y);

	if (params->focus)
		upper_color = colors->base[params->state_type];
	else
		upper_color = colors->base[GTK_STATE_ACTIVE];

	ge_shade_color(&upper_color, 0.92, &lower_color);

	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.0, upper_color.r,
	                                                upper_color.g,
	                                                upper_color.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, lower_color.r,
	                                                lower_color.g,
	                                                lower_color.b);

	cairo_set_source (cr, pattern);
	cairo_rectangle  (cr, 0, 0, width, height);
	cairo_fill       (cr);

	cairo_pattern_destroy (pattern);

	cairo_restore (cr);
}

static void pixflat_draw_scrollbar_trough (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *widget,
	const ScrollBarParameters *scrollbar, int x, int y, int width, int height)
{
	const CairoColor *bg     = &colors->shade[2];
	const CairoColor *border = &colors->shade[8];
	CairoColor        bg_shade;
	cairo_pattern_t *pattern;
	double radius = MIN (widget->radius, MIN ((width - 2.0) / 2.0, (height - 2.0) / 2.0));

	ge_shade_color (bg, 0.95, &bg_shade);

	cairo_set_line_width (cr, 1);

	if (scrollbar->horizontal)
		ge_cairo_exchange_axis (cr, &x, &y, &width, &height);

	cairo_translate (cr, x, y);

	cairo_rectangle (cr, 0, 0, width, height);
	ge_cairo_set_color (cr, bg);
	cairo_fill (cr);
}

static void pixflat_draw_scrollbar_stepper (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *widget,
	const ScrollBarParameters *scrollbar, const ScrollBarStepperParameters *stepper, int x, int y, int width, int height)
{
	CairoColor fill;

	if (scrollbar->horizontal)
	{
		if (stepper->stepper == CL_STEPPER_B)
		{
			x -= 1;
			width += 1;
		}
		else if (stepper->stepper == CL_STEPPER_C)
		{
			width += 1;
		}
	}
	else
	{
		if (stepper->stepper == CL_STEPPER_B)
		{
			y -= 1;
			height += 1;
		}
		else if (stepper->stepper == CL_STEPPER_C)
		{
			height += 1;
		}
	}

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1);

	cairo_rectangle (cr, 1, 1, width-2, height-2);
	if (widget->state_type == CL_STATE_SELECTED)
		ge_shade_color (&colors->shade[0], 1.0, &fill);
	else
		ge_shade_color (&colors->shade[2], 1.0, &fill);
	cairo_set_source_rgb (cr, fill.r, fill.g, fill.b);
	cairo_fill (cr);
}

static void pixflat_draw_scrollbar_slider (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *widget,
	const ScrollBarParameters *scrollbar, int x, int y, int width, int height)
{
    CairoColor fill;
	double radius = MIN (widget->radius, (height - 4) / 2);

	cairo_save (cr);

	if (scrollbar->junction & CL_JUNCTION_BEGIN)
	{
		if (scrollbar->horizontal)
		{
			x -= 1;
			width += 1;
		}
		else
		{
			y -= 1;
			height += 1;
		}
	}
	if (scrollbar->junction & CL_JUNCTION_END)
	{
		if (scrollbar->horizontal)
			width += 1;
		else
			height += 1;
	}

	if (!scrollbar->horizontal)
		ge_cairo_exchange_axis (cr, &x, &y, &width, &height);

	cairo_translate (cr, x, y);

    if (widget->prelight)
		ge_shade_color (&colors->shade[0], 0.44, &fill);
    else if (widget->active)
		ge_shade_color (&colors->base[1], 1.0, &fill);
    else
		ge_shade_color (&colors->shade[0], 0.62, &fill);

	ge_cairo_rounded_rectangle (cr, 3, 3, width-6, height-6, radius, CR_CORNER_ALL);
	cairo_set_source_rgb (cr, fill.r, fill.g, fill.b);
    cairo_fill (cr);
    cairo_restore (cr);
}

static void
clearlooks_draw_statusbar (cairo_t *cr,
                           const ClearlooksColors          *colors,
                           const WidgetParameters          *widget,
                           int x, int y, int width, int height)
{
	const CairoColor *dark = &colors->shade[4];
	CairoColor hilight;

	ge_shade_color (dark, 1.4, &hilight);

	cairo_set_line_width  (cr, 1);
	cairo_translate       (cr, x, y+0.5);
	cairo_move_to         (cr, 0, 0);
	cairo_line_to         (cr, width, 0);
	ge_cairo_set_color    (cr, dark);
	cairo_stroke          (cr);

	cairo_translate       (cr, 0, 1);
	cairo_move_to         (cr, 0, 0);
	cairo_line_to         (cr, width, 0);
	ge_cairo_set_color    (cr, &hilight);
	cairo_stroke          (cr);
}

static void
clearlooks_draw_menu_frame (cairo_t *cr,
                            const ClearlooksColors          *colors,
                            const WidgetParameters          *widget,
                            int x, int y, int width, int height)
{
	const CairoColor *border = &colors->shade[2];
	cairo_translate      (cr, x, y);
	cairo_set_line_width (cr, 1);

	ge_cairo_set_color (cr, border);
	ge_cairo_stroke_rectangle (cr, 0.5, 0.5, width-1, height-1);
}

static void
clearlooks_draw_tooltip (cairo_t *cr,
                         const ClearlooksColors          *colors,
                         const WidgetParameters          *widget,
                         int x, int y, int width, int height)
{
	CairoColor border;

	ge_shade_color (&colors->bg[widget->state_type], 0.6, &border);

	cairo_save (cr);

	cairo_translate      (cr, x, y);
	cairo_set_line_width (cr, 1);

	ge_cairo_set_color (cr, &colors->bg[widget->state_type]);
	cairo_rectangle (cr, 0, 0, width, height);
	cairo_fill (cr);

	ge_cairo_set_color (cr, &border);
	ge_cairo_stroke_rectangle (cr, 0.5, 0.5, width-1, height-1);

	cairo_restore (cr);
}

static void
clearlooks_draw_icon_view_item (cairo_t                  *cr,
	                        const ClearlooksColors   *colors,
	                        const WidgetParameters   *params,
	                        int x, int y, int width, int height)
{
	CairoColor upper_color;
	CairoColor lower_color;
	cairo_pattern_t *pattern;
	cairo_save (cr);

	cairo_translate (cr, x, y);

	if (params->focus)
		upper_color = colors->base[params->state_type];
	else
		upper_color = colors->base[GTK_STATE_ACTIVE];

	ge_shade_color(&upper_color, 0.92, &lower_color);

	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.0, upper_color.r,
	                                                upper_color.g,
	                                                upper_color.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, lower_color.r,
	                                                lower_color.g,
	                                                lower_color.b);

	cairo_set_source (cr, pattern);
	ge_cairo_rounded_rectangle  (cr, 0, 0, width, height, params->radius, CR_CORNER_ALL);
	cairo_fill       (cr);

	cairo_pattern_destroy (pattern);

	cairo_restore (cr);
}

static void
clearlooks_draw_handle (cairo_t *cr,
                        const ClearlooksColors          *colors,
                        const WidgetParameters          *params,
                        const HandleParameters          *handle,
                        int x, int y, int width, int height)
{
	const CairoColor *fill  = &colors->bg[params->state_type];
	int num_bars = 6; /* shut up gcc warnings */

	cairo_save (cr);

	switch (handle->type)
	{
		case CL_HANDLE_TOOLBAR:
			num_bars    = 6;
		break;
		case CL_HANDLE_SPLITTER:
			num_bars    = 16;
		break;
	}

	if (params->prelight)
	{
		cairo_rectangle (cr, x, y, width, height);
		ge_cairo_set_color (cr, fill);
		cairo_fill (cr);
	}

	cairo_translate (cr, x, y);

	cairo_set_line_width (cr, 1);
#if 0
	/* removed for pixflat */
	if (handle->horizontal)
	{
		params->style_functions->draw_gripdots (cr, colors, 0, 0, width, height, num_bars, 2, 0.1);
	}
	else
	{
		params->style_functions->draw_gripdots (cr, colors, 0, 0, width, height, 2, num_bars, 0.1);
	}
#endif
	cairo_restore (cr);
}

static void
clearlooks_draw_resize_grip (cairo_t *cr,
                             const ClearlooksColors          *colors,
                             const WidgetParameters          *widget,
                             const ResizeGripParameters      *grip,
                             int x, int y, int width, int height)
{
	const CairoColor *dark   = &colors->shade[7];
	CairoColor hilight;
	int lx, ly;
	int x_down;
	int y_down;
	int dots;

	ge_shade_color (dark, 1.5, &hilight);

	/* The number of dots fitting into the area. Just hardcoded to 4 right now. */
	/* dots = MIN (width - 2, height - 2) / 3; */
	dots = 4;

	cairo_save (cr);

	switch (grip->edge)
	{
		case CL_WINDOW_EDGE_NORTH_EAST:
			x_down = 0;
			y_down = 0;
			cairo_translate (cr, x + width - 3*dots + 2, y + 1);
		break;
		case CL_WINDOW_EDGE_SOUTH_EAST:
			x_down = 0;
			y_down = 1;
			cairo_translate (cr, x + width - 3*dots + 2, y + height - 3*dots + 2);
		break;
		case CL_WINDOW_EDGE_SOUTH_WEST:
			x_down = 1;
			y_down = 1;
			cairo_translate (cr, x + 1, y + height - 3*dots + 2);
		break;
		case CL_WINDOW_EDGE_NORTH_WEST:
			x_down = 1;
			y_down = 0;
			cairo_translate (cr, x + 1, y + 1);
		break;
		default:
			/* Not implemented. */
			return;
	}

	for (lx = 0; lx < dots; lx++) /* horizontally */
	{
		for (ly = 0; ly <= lx; ly++) /* vertically */
		{
			int mx, my;
			mx = x_down * dots + (1 - x_down * 2) * lx - x_down;
			my = y_down * dots + (1 - y_down * 2) * ly - y_down;

			ge_cairo_set_color (cr, &hilight);
			cairo_rectangle (cr, mx*3-1, my*3-1, 2, 2);
			cairo_fill (cr);

			ge_cairo_set_color (cr, dark);
			cairo_rectangle (cr, mx*3-1, my*3-1, 1, 1);
			cairo_fill (cr);
		}
	}

	cairo_restore (cr);
}

static void pixflat_draw_radiobutton (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *widget,
	const CheckboxParameters *checkbox, int x, int y, int width, int height)
{
	const CairoColor *border, *dot;
	gboolean inconsistent;
	gboolean draw_bullet = (checkbox->shadow_type == GTK_SHADOW_IN);
	gdouble w, h, cx, cy, radius;

	w = (gdouble) width;
	h = (gdouble) height;
	cx = width / 2.0;
	cy = height / 2.0;
	radius = MIN (width, height) / 2.0;

	inconsistent = (checkbox->shadow_type == GTK_SHADOW_ETCHED_IN);
	draw_bullet |= inconsistent;

	if (widget->disabled)
	{
		border = &colors->shade[3];
		dot    = &colors->shade[8];
	}
	else
	{
		border = &colors->shade[4];
		dot    = &colors->text[0];
	}

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1.0);
	cairo_arc (cr, ceil (cx), ceil (cy), floor (radius - 0.1), 0, G_PI*2);

	if (!widget->disabled)
	{
		ge_cairo_set_color (cr, &colors->base[0]);
		cairo_fill_preserve (cr);
	}

	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);

	if (draw_bullet)
	{
		if (inconsistent)
		{
			gdouble line_width = floor (radius * 2 / 3);

			cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
			cairo_set_line_width (cr, line_width);

			cairo_move_to (cr, ceil (cx - radius/3.0 - line_width) + line_width, ceil (cy - line_width) + line_width);
			cairo_line_to (cr, floor (cx + radius/3.0 + line_width) - line_width, ceil (cy - line_width) + line_width);

			ge_cairo_set_color (cr, dot);
			cairo_stroke (cr);
		}
		else
		{
			cairo_arc (cr, ceil (cx), ceil (cy), floor (radius/2.5), 0, G_PI*2);
			ge_cairo_set_color (cr, dot);
			cairo_fill (cr);
		}
	}
}

static void pixflat_draw_checkbox (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *widget,
	const CheckboxParameters *checkbox, int x, int y, int width, int height)
{
	const CairoColor *border, *dot;
	gboolean inconsistent = FALSE;
	gboolean draw_bullet = (checkbox->shadow_type == GTK_SHADOW_IN);

	inconsistent = (checkbox->shadow_type == GTK_SHADOW_ETCHED_IN);
	draw_bullet |= inconsistent;

	if (widget->disabled)
	{
		border = &colors->shade[3];
		dot    = &colors->shade[8];
	}
	else
	{
		border = &colors->shade[5];
		dot    = &colors->text[GTK_STATE_NORMAL];
	}

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1);

	if (widget->xthickness >= 3 && widget->ythickness >= 3)
	{
		widget->style_functions->draw_inset (cr, &widget->parentbg, 0, 0, width, height, 1, CR_CORNER_ALL);

		/* Draw the rectangle for the checkbox itself */
		ge_cairo_rounded_rectangle (cr, 1.5, 1.5, width-3, height-3, (widget->radius > 0)? 1 : 0, CR_CORNER_ALL);
	}
	else
	{
		/* Draw the rectangle for the checkbox itself */
		ge_cairo_rounded_rectangle (cr, 0.5, 0.5, width-1, height-1, (widget->radius > 0)? 1 : 0, CR_CORNER_ALL);
	}

	if (!widget->disabled)
	{
		ge_cairo_set_color (cr, &colors->base[0]);
		cairo_fill_preserve (cr);
	}

	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);

	if (draw_bullet)
	{
		if (inconsistent)
		{
			cairo_set_line_width (cr, 2.0);
			cairo_move_to (cr, 3, height*0.5);
			cairo_line_to (cr, width-3, height*0.5);
		}
		else
		{
			cairo_set_line_width (cr, 3);
			cairo_move_to (cr, 0.5 + (width*0.2), (height*0.5));
			cairo_line_to (cr, 0.5 + (width*0.4), (height*0.7));
			cairo_line_to (cr, 0.5 + (width*0.8), (height*0.3));
		}

		ge_cairo_set_color (cr, dot);
		cairo_stroke (cr);
	}
}

static void pixflat_draw_arrow (cairo_t *cr, const ClearlooksColors *colors, const WidgetParameters *widget,
	const ArrowParameters *arrow, int x, int y, int width, int height)
{
	const CairoColor *color = &colors->fg[widget->state_type];
	gdouble tx, ty, rotate = 0, size;

	tx = x + width/2.0;
	ty = y + height/2.0;
	if (arrow->type == CL_ARROW_NORMAL || arrow->type == CL_ARROW_SCROLLBAR)
	{
		if (arrow->direction == CL_DIRECTION_LEFT)
			rotate = G_PI*1.5;
		else if (arrow->direction == CL_DIRECTION_RIGHT)
			rotate = G_PI*0.5;
		else if (arrow->direction == CL_DIRECTION_UP)
			rotate = G_PI;
		else rotate = 0;
	}

	size = (arrow->type == CL_ARROW_COMBO) ? MIN (height * 2.0 / 3.0, width) : width;

	cairo_save (cr);

	cairo_translate (cr, tx, ty);
	if (rotate) cairo_rotate (cr, -rotate);

	cairo_translate (cr, 0, -size / 4.0);
	cairo_move_to (cr, -size / 2.0, 0);
	cairo_line_to (cr, 0, size / 2.0);
	cairo_line_to (cr, size / 2.0, 0);
	cairo_close_path (cr);

	ge_cairo_set_color (cr, color);
	cairo_fill (cr);

	cairo_restore (cr);
}

void
clearlooks_draw_focus (cairo_t *cr,
                       const ClearlooksColors *colors,
                       const WidgetParameters *widget,
                       const FocusParameters  *focus,
                       int x, int y, int width, int height)
{
	if (focus->has_color)
		ge_cairo_set_color (cr, &focus->color);
	else if (focus->type == CL_FOCUS_COLOR_WHEEL_LIGHT)
		cairo_set_source_rgb (cr, 0., 0., 0.);
	else if (focus->type == CL_FOCUS_COLOR_WHEEL_DARK)
		cairo_set_source_rgb (cr, 1., 1., 1.);
	else
		cairo_set_source_rgba (cr,
		                       colors->fg[widget->state_type].r,
		                       colors->fg[widget->state_type].g,
		                       colors->fg[widget->state_type].b,
		                       0.7);

	cairo_set_line_width (cr, focus->line_width);

	if (focus->dash_list[0])
	{
		gint n_dashes = strlen ((gchar *)focus->dash_list);
		gdouble *dashes = g_new (gdouble, n_dashes);
		gdouble total_length = 0;
		gdouble dash_offset;
		gint i;

		for (i = 0; i < n_dashes; i++)
		{
			dashes[i] = focus->dash_list[i];
			total_length += focus->dash_list[i];
		}

		dash_offset = -focus->line_width / 2.0;
		while (dash_offset < 0)
			dash_offset += total_length;

		cairo_set_dash (cr, dashes, n_dashes, dash_offset);
		g_free (dashes);
	}

	cairo_rectangle (cr,
	                 x + focus->line_width / 2.0,
	                 y + focus->line_width / 2.0,
	                 width - focus->line_width, height - focus->line_width);
	cairo_stroke (cr);
}

void
clearlooks_set_mixed_color (cairo_t          *cr,
                            const CairoColor *color1,
                            const CairoColor *color2,
                            gdouble mix_factor)
{
	CairoColor composite;

	ge_mix_color (color1, color2, mix_factor, &composite);
	ge_cairo_set_color (cr, &composite);
}

void
clearlooks_register_style_classic (ClearlooksStyleFunctions *functions, ClearlooksStyleConstants *constants)
{
	g_assert (functions);

	functions->draw_button              = pixflat_draw_button;
	functions->draw_scale_trough        = pixflat_draw_scale_trough;
	functions->draw_progressbar_trough  = pixflat_draw_progressbar_trough;
	functions->draw_progressbar_fill    = pixflat_draw_progressbar_fill;
	functions->draw_slider_button       = pixflat_draw_slider_button;
	functions->draw_entry               = pixflat_draw_entry;
	functions->draw_entry_progress      = clearlooks_draw_entry_progress;
	functions->draw_spinbutton          = pixflat_draw_spinbutton;
	functions->draw_spinbutton_down     = pixflat_draw_spinbutton_down;
	functions->draw_optionmenu          = clearlooks_draw_optionmenu;
	functions->draw_inset               = clearlooks_draw_inset;
	functions->draw_menubar	            = clearlooks_draw_menubar;
	functions->draw_tab                 = pixflat_draw_tab;
	functions->draw_frame               = pixflat_draw_frame;
	functions->draw_separator           = clearlooks_draw_separator;
	functions->draw_menu_item_separator = clearlooks_draw_menu_item_separator;
	functions->draw_list_view_header    = clearlooks_draw_list_view_header;
	functions->draw_toolbar             = clearlooks_draw_toolbar;
	functions->draw_menuitem            = clearlooks_draw_menuitem;
	functions->draw_menubaritem         = clearlooks_draw_menubaritem;
	functions->draw_selected_cell       = clearlooks_draw_selected_cell;
	functions->draw_scrollbar_stepper   = pixflat_draw_scrollbar_stepper;
	functions->draw_scrollbar_slider    = pixflat_draw_scrollbar_slider;
	functions->draw_scrollbar_trough    = pixflat_draw_scrollbar_trough;
	functions->draw_statusbar           = clearlooks_draw_statusbar;
	functions->draw_menu_frame          = clearlooks_draw_menu_frame;
	functions->draw_tooltip             = clearlooks_draw_tooltip;
	functions->draw_icon_view_item      = clearlooks_draw_icon_view_item;
	functions->draw_handle              = clearlooks_draw_handle;
	functions->draw_resize_grip         = clearlooks_draw_resize_grip;
	functions->draw_arrow               = pixflat_draw_arrow;
	functions->draw_focus               = clearlooks_draw_focus;
	functions->draw_checkbox            = pixflat_draw_checkbox;
	functions->draw_radiobutton         = pixflat_draw_radiobutton;

	constants->topleft_highlight_shade  = 1.3;
	constants->topleft_highlight_alpha  = 0.6;
}
