#include <config.h>

#include <errno.h>
#include <stdlib.h>
#include <glib/gi18n-lib.h>

#include "load-config.h"
#include "colors.h"
#include "properties.h"

guint
load_colors_get_extra_index(guint i, LoadExtraColor col)
{
	g_assert_cmpuint(col, >=, 0);
	g_assert_cmpuint(col, <, EXTRA_COLORS);
	return load_config_get_num_colors(i) - EXTRA_COLORS + col;
}

void
load_colors_default(LoadPlugin *a, guint i)
{
	memcpy(&a->graph_config[i].colors, default_color_scheme.colors[i], sizeof(default_color_scheme.colors[i]));
}

const LoadColorScheme default_color_scheme = {
	{
		{ // CPU  - hue: 196
			HEX_TO_RGBA(036F96, FF),		// Used
			HEX_TO_RGBA(005D80, FF),		// Border
			HEX_TO_RGBA(132126, FF),		// Background (top)
			HEX_TO_RGBA(000000, FF)			// Background (bottom)
		}, { // MEM  - hue: 151
			HEX_TO_RGBA(03964F, FF),		// Used
			HEX_TO_RGBA(008042, FF),		// Border
			HEX_TO_RGBA(13261D, FF),		// Background (top)
			HEX_TO_RGBA(000000, FF)			// Background (bottom)
		}, { // DISK - hue: 31
			HEX_TO_RGBA(ED7A00, FF),		// Used
			HEX_TO_RGBA(804200, FF),		// Border
			HEX_TO_RGBA(261D13, FF),		// Background (top)
			HEX_TO_RGBA(000000, FF)			// Background (bottom)
		}
	}
};
