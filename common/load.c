#include <config.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib/gi18n-lib.h>

#include "colors.h"
#include "graph-data.h"
#include "load-graph.h"
#include "load.h"
#include "load-config.h"
#include "properties.h"

void
load_tooltip_update(LoadGraph *g)
{
	gchar buf_title[100];
	gchar buf_text[800];
	gchar tooltip_markup[1024];

	g_assert(g != NULL);
	g_assert(g->load != NULL);
	g_assert(g->load->extra_data != NULL);
	g_assert_cmpuint(g->id, >=, 0);
	g_assert_cmpuint(g->id, <, GRAPH_MAX);

	buf_title[0] = '\0';
	buf_text[0] = '\0';

	graph_types[g->id].tooltip_update(buf_title, sizeof(buf_title), buf_text, sizeof(buf_text), g, g->load->extra_data[g->id]);

	if (buf_text[0] == '\0') {
		g_warning("[load] Empty text for tooltip #%d", g->id);
	}

	const gchar *title = (*buf_title)? buf_title : graph_types[g->id].label;
	g_snprintf(tooltip_markup, sizeof(tooltip_markup), "<span weight='bold' size='larger'>%s</span>\n%s", title, buf_text);

	gtk_widget_set_tooltip_markup(g->disp, tooltip_markup);
	gtk_widget_trigger_tooltip_query(g->disp);
}

/* get current orientation */
GtkOrientation
load_get_orientation(LoadPlugin *a) {
	if (a->orientation_policy == LOAD_ORIENTATION_HORIZONTAL) {
		return GTK_ORIENTATION_HORIZONTAL;
	} else if (a->orientation_policy == LOAD_ORIENTATION_VERTICAL) {
		return GTK_ORIENTATION_VERTICAL;
	} else { // LOAD_ORIENTATION_AUTO
		return a->panel_orientation;
	}
}

void
load_start(LoadPlugin *a)
{
	guint i;

	a->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	gtk_box_set_spacing(GTK_BOX(a->box), DEFAULT_SPACING);
	gtk_container_set_border_width(GTK_CONTAINER(a->box), DEFAULT_PADDING);

	load_refresh_orientation(a);
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(a->container), FALSE);

	gtk_widget_show(a->box);
	gtk_container_add(a->container, a->box);

	// Children (graphs) are individually shown/hidden to control visibility
	gtk_widget_set_no_show_all(a->box, TRUE);

	for (i = 0; i < GRAPH_MAX; i++) {
		gtk_box_pack_start(GTK_BOX(a->box), a->graphs[i]->main_widget, TRUE, TRUE, 0);
		gtk_widget_set_visible(a->graphs[i]->main_widget, TRUE);
		load_graph_start(a->graphs[i]);
	}

	return;
}

void
load_refresh_orientation(LoadPlugin *a)
{
	GtkAllocation alloc;
	guint i;
	LoadGraph *g;
	gint opposite_size = 120;

	gtk_orientable_set_orientation(GTK_ORIENTABLE(a->box), load_get_orientation(a));

	if (a->panel_orientation == GTK_ORIENTATION_HORIZONTAL && a->orientation_policy == LOAD_ORIENTATION_VERTICAL) {
		gtk_widget_set_size_request(GTK_WIDGET(a->container), opposite_size, -1);
	} else if (a->panel_orientation == GTK_ORIENTATION_VERTICAL && a->orientation_policy == LOAD_ORIENTATION_HORIZONTAL) {
		gtk_widget_set_size_request(GTK_WIDGET(a->container), -1, opposite_size);
	} else {
		gtk_widget_set_size_request(GTK_WIDGET(a->container), -1, -1);
	}

	for (i = 0; i < GRAPH_MAX; i++) {
		g = a->graphs[i];
		if (g != NULL) {
			load_graph_resize(g);
		}
	}

	gtk_widget_get_allocation(GTK_WIDGET(a->container), &alloc);
	g_debug("[load] New allocation for applet container: %d,%d", alloc.width, alloc.height);
}

void load_defaults(LoadPlugin *a)
{
	guint i;

	for (i = 0; i < GRAPH_MAX; i++) {
		load_colors_default(a, i);
	}
}

LoadPlugin*
load_new()
{
	guint i;
	LoadPlugin *a = g_slice_new0(LoadPlugin);
	load_config_init();

	a->container = GTK_CONTAINER(gtk_event_box_new());
	gtk_widget_show(GTK_WIDGET(a->container));

	a->extra_data[GRAPH_CPULOAD] = (gpointer)g_new0(GraphData, 1);
	a->extra_data[GRAPH_MEMLOAD] = (gpointer)g_new0(GraphData, 1);
	a->extra_data[GRAPH_DISKLOAD] = (gpointer)g_new0(GraphData, 1);

	for (i = 0; i < GRAPH_MAX; i++) {
		a->graphs[i] = load_graph_new(a, i);
	}

	return a;
}

void
load_free(LoadPlugin *a)
{
	gint i;

	for (i = 0; i < GRAPH_MAX; i++) {
		load_graph_stop(a->graphs[i]);
		gtk_widget_destroy(a->graphs[i]->main_widget);

		load_graph_unalloc(a->graphs[i]);
		g_free(a->graphs[i]);
		g_free(a->extra_data[i]);
	}

	g_free(a);

	g_debug("[load] Destroyed");
}
