#include <config.h>

#include "colors.h"
#include "graph-data.h"
#include "load-config.h"

GraphType graph_types[GRAPH_MAX];

guint load_config_get_num_colors(guint id)
{
	g_assert_cmpuint(graph_types[id].num_colors, <=, MAX_COLORS);
	return graph_types[id].num_colors;
}

guint load_config_get_num_data(guint id)
{
	// Subtract colors that do not map 1:1 with graph data
	return load_config_get_num_colors(id) - EXTRA_COLORS;
}

void load_config_init()
{
	GraphType temp[] = {
		{	"cpu", _("Processor"), 4,
			(GraphGetDataFunc) graph_cpu_get_data,
			(GraphTooltipUpdateFunc) graph_tooltip_update
		},
		{	"mem", _("Memory"), 4,
			(GraphGetDataFunc) graph_mem_get_data,
			(GraphTooltipUpdateFunc) graph_tooltip_update
		},
		{	"disk", _("Disk"), 4,
			(GraphGetDataFunc) graph_disk_get_data,
			(GraphTooltipUpdateFunc) graph_tooltip_update
		}
	};

	memcpy(&graph_types, &temp, sizeof(graph_types));
}
