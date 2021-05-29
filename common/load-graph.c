#include <config.h>

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <glib.h>
#include <gdk/gdkx.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>

#include "colors.h"
#include "graph-data.h"
#include "load-graph.h"
#include "load.h"
#include "load-config.h"
#include "properties.h"

/* Wrapper for cairo_set_source_rgba */
static void
cairo_set_source_rgba_from_config(cairo_t *cr, GraphConfig *config, guint color_index)
{
	GdkRGBA *c = &(config->colors[color_index]);
	cairo_set_source_rgba(cr, c->red, c->green, c->blue, c->alpha);
}

void
load_graph_cairo_set_gradient(cairo_t *cr, double width, double height, int direction, GdkRGBA *a, GdkRGBA *b)
{
	cairo_pattern_t *pat;

	// sanity check
	if (direction < 0 || direction >= LOAD_GRADIENT_MAX) {
		direction = LOAD_GRADIENT_LINEAR_N_TO_S;
	}

	switch (direction) {
		case LOAD_GRADIENT_LINEAR_N_TO_S:
			pat = cairo_pattern_create_linear(0.0, 0.0, 0.0, height);
			break;
		case LOAD_GRADIENT_LINEAR_NE_TO_SW:
			pat = cairo_pattern_create_linear(width, 0.0, 0.0, height);
			break;
		case LOAD_GRADIENT_LINEAR_E_TO_W:
			pat = cairo_pattern_create_linear(width, 0.0, 0.0, 0.0);
			break;
		case LOAD_GRADIENT_LINEAR_SE_TO_NW:
			pat = cairo_pattern_create_linear(width, height, 0.0, 0.0);
			break;
		case LOAD_GRADIENT_LINEAR_S_TO_N:
			pat = cairo_pattern_create_linear(0.0, height, 0.0, 0.0);
			break;
		case LOAD_GRADIENT_LINEAR_SW_TO_NE:
			pat = cairo_pattern_create_linear(0.0, height, width, 0.0);
			break;
		case LOAD_GRADIENT_LINEAR_W_TO_E:
			pat = cairo_pattern_create_linear(0.0, 0.0, width, 0.0);
			break;
		case LOAD_GRADIENT_LINEAR_NW_TO_SE:
			pat = cairo_pattern_create_linear(0.0, 0.0, width, height);
			break;
		case LOAD_GRADIENT_RADIAL:
			pat = cairo_pattern_create_radial(width/2, height/2, 0.0, width/2, height/2, sqrt(pow(width/2,2) + pow(height/2,2)));
			break;

		default:
			g_assert_not_reached();
	}
	cairo_pattern_add_color_stop_rgb(pat, 0, a->red, a->green, a->blue);
	cairo_pattern_add_color_stop_rgb(pat, 1, b->red, b->green, b->blue);
	cairo_set_source(cr, pat);
	cairo_pattern_destroy(pat);
}

/* Redraws the backing pixmap for the load graph and updates the window */
static void
load_graph_draw(LoadGraph *g)
{
	guint i, j;
	guint c_top, c_bottom, c_border;
	cairo_t *cr;
	GdkRGBA *colors = g->config->colors;

	guint x = 0;
	guint y = 0;
	guint W = g->draw_width;
	guint H = g->draw_height;

	double line_x, line_y, line_y_dest;

	if (!g->surface) {
		g->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, W, H);
	}

	cr = cairo_create(g->surface);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
	cairo_set_line_width(cr, 1.0);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);

	c_top = load_colors_get_extra_index(g->id, EXTRA_COLOR_BACKGROUND_TOP);
	c_bottom = load_colors_get_extra_index(g->id, EXTRA_COLOR_BACKGROUND_BOTTOM);
	c_border = load_colors_get_extra_index(g->id, EXTRA_COLOR_BORDER);

	// border
	if (DEFAULT_BORDER_WIDTH > 0) {
		cairo_set_source_rgba_from_config(cr, g->config, c_border);
		cairo_rectangle(cr, 0, 0, W, H);
		cairo_fill(cr);

		if ((guint)(2*DEFAULT_BORDER_WIDTH) < W) {
			W -= 2*DEFAULT_BORDER_WIDTH;
		} else {
			W = 0;
		}

		if ((guint)(2*DEFAULT_BORDER_WIDTH) < H) {
			H -= 2*DEFAULT_BORDER_WIDTH;
		} else {
			H = 0;
		}

		x = DEFAULT_BORDER_WIDTH;
		y = DEFAULT_BORDER_WIDTH;
	}

	if (W > 0 && H > 0) {
		// background
		load_graph_cairo_set_gradient(cr, W, H, DEFAULT_BACKGROUND_DIRECTION, &(colors[c_top]), &(colors[c_bottom]));
		cairo_rectangle(cr, x, y, W, H);
		cairo_fill(cr);

		// graph data
		for (i = 0; i < W; i++) {
			g->pos[i] = H;
		}

		for (j = 0; j < load_config_get_num_data(g->id); j++) {
			cairo_set_source_rgba_from_config(cr, g->config, j);
			for (i = 0; i < W; i++) {
				if (g->data[i][j] == 0) {
					continue;
				}

				line_x = x + W - i - 0.5;
				line_y = y + g->pos[i] - 0.5;
				line_y_dest = line_y - g->data[i][j] + 1;

				// Ensure 1px lines are drawn
				if (g->data[i][j] == 1) {
					line_y_dest -= 1;
				}

				if (line_y > y) { // don't even begin to draw out of scale values
					if (line_y_dest < y) { // makes sure that line ends to graph border
						line_y_dest = y + 0.5;
					} 

					cairo_move_to(cr, line_x, line_y);
					cairo_line_to(cr, line_x, line_y_dest);
				}

				g->pos[i] -= g->data[i][j];
			}

			cairo_stroke(cr);
		}
	}

	gtk_widget_queue_draw(g->disp);
	cairo_destroy(cr);
}

/* Rotates graph data to the right */
static void
load_graph_rotate(LoadGraph *g)
{
	guint i;
	gint* tmp;

	tmp = g->data[g->draw_width - 1];
	for(i = g->draw_width - 1; i > 0; --i) {
		g->data[i] = g->data[i-1];
	}
	g->data[0] = tmp;
}

/* Updates the load graph when the timeout expires */
static gboolean
load_graph_update(LoadGraph *g)
{
	if (g->data == NULL) {
		return TRUE;
	}

	load_graph_rotate(g);

	g_assert(g->load->extra_data != NULL);
	guint H = g->draw_height - 2 * (DEFAULT_BORDER_WIDTH);
	graph_types[g->id].get_data(H, g->data[0], g, g->load->extra_data[g->id]);

	if (g->tooltip_update) {
		load_tooltip_update(g);
	}

	load_graph_draw(g);

	if (g->update_cb) {
		g->update_cb(g, g->update_cb_user_data);
	}

	return TRUE;
}

void
load_graph_unalloc(LoadGraph *g)
{
	guint i;

	if (!g->allocated) {
		return;
	}

	for (i = 0; i < g->draw_width; i++) {
		g_free(g->data[i]);
	}

	g_free(g->data);
	g_free(g->pos);

	g->pos = NULL;
	g->data = NULL;

	if (g->surface) {
		cairo_surface_destroy(g->surface);
		g->surface = NULL;
	}

	g->allocated = FALSE;
	g_debug("[load-graph] Graph '%s' unallocated", graph_types[g->id].name);
}

static void
load_graph_alloc(LoadGraph *g)
{
	guint i;

	if (g->allocated) {
		return;
	}

	g->data = g_new0(gint *, g->draw_width);
	g->pos = g_new0(guint, g->draw_width);

	guint data_size = sizeof (guint) * load_config_get_num_data(g->id);

	for (i = 0; i < g->draw_width; i++) {
		g->data[i] = g_malloc0(data_size);
	}

	g->allocated = TRUE;
	g_debug("[load-graph] Graph '%s' allocated", graph_types[g->id].name);
}

static gboolean
load_graph_draw_cb(GtkWidget *widget, cairo_t *cr, LoadGraph *g)
{
	cairo_set_source_surface(cr, g->surface, 0, 0);
	cairo_paint(cr);

	return FALSE;
}

static gint
load_graph_configure(GtkWidget *widget, GdkEventConfigure *event, LoadGraph *g)
{
	GtkAllocation allocation;

	load_graph_unalloc(g);

	gtk_widget_get_allocation(g->disp, &allocation);

	g->draw_width = allocation.width;
	g->draw_height = allocation.height;
	g->draw_width = MAX(g->draw_width, 1);
	g->draw_height = MAX(g->draw_height, 1);

	g_debug("[load-graph] widget allocation for graph '%s': [%d,%d] resulting draw size: [%d,%d]", graph_types[g->id].name, allocation.width, allocation.height, g->draw_width, g->draw_height);

	load_graph_alloc(g);

	if (!g->surface) {
		g->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			g->draw_width, g->draw_height);
	}

	gtk_widget_queue_draw(widget);

	return TRUE;
}

static void
load_graph_destroy(GtkWidget *widget, LoadGraph *g)
{
	load_graph_stop(g);
	gtk_widget_destroy(widget);
}

static gboolean
load_graph_mouse_move_cb(GtkWidget *widget, GdkEventCrossing *event, LoadGraph *g)
{
	g->tooltip_update = (event->type == GDK_ENTER_NOTIFY);
	g_debug("[load-graph] Mouse %s event on graph '%s'", (event->type == GDK_ENTER_NOTIFY) ? "enter":"leave", graph_types[g->id].name);
	return GDK_EVENT_PROPAGATE;
}

LoadGraph *
load_graph_new(LoadPlugin *a, guint id)
{
	LoadGraph *g;

	g = g_new0(LoadGraph, 1);
	g->id = id;

	g->tooltip_update = FALSE;
	g->load = a;
	g->config = &a->graph_config[id];

	g->main_widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	g->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(g->main_widget), g->box, TRUE, TRUE, 0);

	g->timer_index = -1;

	load_graph_resize(g);

	g->disp = gtk_drawing_area_new();
	gtk_widget_set_events(g->disp,
		GDK_EXPOSURE_MASK |
		GDK_ENTER_NOTIFY_MASK |
		GDK_LEAVE_NOTIFY_MASK);

	g_signal_connect(G_OBJECT(g->disp), "draw", G_CALLBACK(load_graph_draw_cb), g);
	g_signal_connect(G_OBJECT(g->disp), "configure_event", G_CALLBACK(load_graph_configure), g);
	g_signal_connect(G_OBJECT(g->main_widget), "destroy", G_CALLBACK(load_graph_destroy), g);
	g_signal_connect(G_OBJECT(g->disp), "enter-notify-event", G_CALLBACK(load_graph_mouse_move_cb), g);
	g_signal_connect(G_OBJECT(g->disp), "leave-notify-event", G_CALLBACK(load_graph_mouse_move_cb), g);

	gtk_box_pack_start(GTK_BOX(g->box), g->disp, TRUE, TRUE, 0);
	gtk_widget_show_all(g->box);

	return g;
}

void
load_graph_resize(LoadGraph *g)
{
	gint w, h;

	if (load_get_orientation(g->load) == GTK_ORIENTATION_VERTICAL) {
		w = -1;
		h = DEFAULT_SIZE;
	} else { /* GTK_ORIENTATION_HORIZONTAL */ 
		w = DEFAULT_SIZE;
		h = -1;
	}

	gtk_widget_set_size_request(g->main_widget, w, h);

	g_debug("[load-graph] Set size request of graph '%s' to [%d, %d]", graph_types[g->id].name, w, h);
}

void
load_graph_start(LoadGraph *g)
{
	load_graph_stop(g);
	g->timer_index = g_timeout_add(DEFAULT_INTERVAL, (GSourceFunc) load_graph_update, g);
	g_debug("[load-graph] Timer started for graph '%s' (interval: %d ms)", graph_types[g->id].name, DEFAULT_INTERVAL);
}

void
load_graph_stop(LoadGraph *g)
{
	if (g->timer_index != -1) {
		g_source_remove(g->timer_index);
	}

	g->timer_index = -1;
	g_debug("[load-graph] Time stopped for graph '%s'", graph_types[g->id].name);
}
