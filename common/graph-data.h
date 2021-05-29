#ifndef __GRAPH_DATA_H__
#define __GRAPH_DATA_H__

#include "load.h"

G_BEGIN_DECLS

typedef struct _GraphData {
	guint64 used;
} GraphData;

G_GNUC_INTERNAL void
graph_cpu_get_data(int Maximum, int data[1], LoadGraph *g, GraphData *xd);

G_GNUC_INTERNAL void
graph_mem_get_data(int Maximum, int data[1], LoadGraph *g, GraphData *xd);

G_GNUC_INTERNAL void
graph_disk_get_data(int Maximum, int data[1], LoadGraph *g, GraphData *xd);

G_GNUC_INTERNAL void
graph_tooltip_update(char *buf_title, size_t len_title, char *buf_text, size_t len_text, LoadGraph *g, GraphData *xd);

G_END_DECLS

#endif /* __GRAPH_DATA_H__ */
