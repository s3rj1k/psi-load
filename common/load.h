#ifndef __LOAD_H__
#define __LOAD_H__

#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

#define MAX_COLORS 4

enum GraphType {
	GRAPH_CPULOAD,
	GRAPH_MEMLOAD,
	GRAPH_DISKLOAD,

	GRAPH_MAX
};

enum LoadOrientationPolicy {
	LOAD_ORIENTATION_AUTO = 0,
	LOAD_ORIENTATION_HORIZONTAL = 1,
	LOAD_ORIENTATION_VERTICAL = 2,

	LOAD_ORIENTATION_N_VALUES = 2
};

typedef struct _LoadGraph LoadGraph;
typedef void (*GraphUpdateFunc)	(LoadGraph *g, gpointer user_data);

typedef struct _GraphConfig {
	GdkRGBA colors[MAX_COLORS];
} GraphConfig;

typedef struct _LoadPlugin {
	gpointer panel_data;

	GtkContainer *container;
	GtkWidget *box;
	GtkOrientation panel_orientation;

	LoadGraph *graphs[GRAPH_MAX];
	GraphConfig graph_config[GRAPH_MAX];
	gpointer extra_data[GRAPH_MAX]; // data depend on graph type

	gint orientation_policy;
} LoadPlugin;

struct _LoadGraph {
	LoadPlugin *load;

	guint id;
	guint draw_width, draw_height;

	gint **data;
	guint *pos;

	GtkWidget *main_widget;
	GtkWidget *box, *disp;
	cairo_surface_t *surface;
	int timer_index;

	gboolean allocated;
	gboolean tooltip_update;

	GraphUpdateFunc update_cb;
	gpointer update_cb_user_data;

	GraphConfig *config;
};

typedef enum {
	LOAD_GRADIENT_LINEAR_N_TO_S,
	LOAD_GRADIENT_LINEAR_NE_TO_SW,
	LOAD_GRADIENT_LINEAR_E_TO_W,
	LOAD_GRADIENT_LINEAR_SE_TO_NW,
	LOAD_GRADIENT_LINEAR_S_TO_N,
	LOAD_GRADIENT_LINEAR_SW_TO_NE,
	LOAD_GRADIENT_LINEAR_W_TO_E,
	LOAD_GRADIENT_LINEAR_NW_TO_SE,
	LOAD_GRADIENT_RADIAL,

	LOAD_GRADIENT_MAX
} LoadGradientDirection;

G_BEGIN_DECLS

#ifndef GDK_EVENT_PROPAGATE
#define GDK_EVENT_PROPAGATE (FALSE)
#endif

G_GNUC_INTERNAL void
load_start(LoadPlugin *a);
G_GNUC_INTERNAL GtkOrientation
load_get_orientation(LoadPlugin *a);
G_GNUC_INTERNAL void
load_tooltip_update(LoadGraph *g);
G_GNUC_INTERNAL void
load_refresh_orientation(LoadPlugin *a);
G_GNUC_INTERNAL void
load_defaults(LoadPlugin *a);
G_GNUC_INTERNAL LoadPlugin*
load_new();
G_GNUC_INTERNAL void
load_free(LoadPlugin *a);

G_END_DECLS

#endif /* __LOAD_H__ */
