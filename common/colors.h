#ifndef __COLORS_H__
#define __COLORS_H__

#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "load.h"

G_BEGIN_DECLS

#define HEX_TO_RGBA(rgb, a) {(gdouble)((0x##rgb >> 16) & 0xFF)/255.0, (gdouble)((0x##rgb >> 8) & 0xFF)/255.0, (gdouble)(0x##rgb & 0xFF)/255.0, (gdouble)(0x##a)/255.0}

// Color scheme contents
typedef struct {
	GdkRGBA colors[GRAPH_MAX][MAX_COLORS];
} LoadColorScheme;

extern const LoadColorScheme default_color_scheme;

typedef enum {
	EXTRA_COLOR_BORDER = 0,
	EXTRA_COLOR_BACKGROUND_TOP,
	EXTRA_COLOR_BACKGROUND_BOTTOM,

	EXTRA_COLORS
} LoadExtraColor;

G_GNUC_INTERNAL guint
load_colors_get_extra_index(guint i, LoadExtraColor col);

G_GNUC_INTERNAL void
load_colors_default(LoadPlugin *a, guint graph_index);

G_END_DECLS

#endif /* __COLORS_H__ */
