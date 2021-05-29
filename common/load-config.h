#ifndef __LOAD_CONFIG_H__
#define __LOAD_CONFIG_H__

#include <glib.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>

#include "load.h"

G_BEGIN_DECLS

typedef void (*GraphGetDataFunc) (int Maximum, int data[], LoadGraph *g, gpointer xd);
typedef void (*GraphTooltipUpdateFunc) (char *buf_title, size_t len_title, char *buf_text, size_t len_text, LoadGraph *g, gpointer xd);

typedef struct _GraphType {
	const char *name;
	const char *label;
	const guint num_colors;
	const GraphGetDataFunc get_data;
	const GraphTooltipUpdateFunc tooltip_update;
} GraphType;

// global variable
extern GraphType graph_types[GRAPH_MAX];

G_GNUC_INTERNAL guint
load_config_get_num_colors(guint id);
G_GNUC_INTERNAL guint
load_config_get_num_data(guint id);
G_GNUC_INTERNAL void
load_config_init();

G_END_DECLS

#endif /* __LOAD_CONFIG_H__ */
