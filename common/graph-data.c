#include <config.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>

#include "psi.h"
#include "graph-data.h"
#include "properties.h"

#define PATH_CPU "/proc/pressure/cpu"
#define PATH_MEM "/proc/pressure/memory"
#define PATH_IO "/proc/pressure/io"

void
graph_cpu_get_data(int Maximum, int data[1], LoadGraph *g, GraphData *xd)
{ 
	xd->used = read_psi_avg10_kind_some_value(PATH_CPU);
	data[0] = rint((float) Maximum * xd->used / 100);
}

void
graph_mem_get_data(int Maximum, int data[1], LoadGraph *g, GraphData *xd)
{
	xd->used = read_psi_avg10_kind_some_value(PATH_MEM);
	data[0] = rint((float) Maximum * xd->used / 100);
}

void
graph_disk_get_data(int Maximum, int data[1], LoadGraph *g, GraphData *xd)
{
	xd->used = read_psi_avg10_kind_some_value(PATH_IO);
	data[0] = rint((float) Maximum * xd->used / 100);
}

void
graph_tooltip_update(char *buf_title, size_t len_title, char *buf_text, size_t len_text, LoadGraph *g, GraphData *xd)
{
	g_snprintf(buf_text, len_text, _("%ld%%"), xd->used);
}
