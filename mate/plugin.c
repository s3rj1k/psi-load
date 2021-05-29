#include <config.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gi18n-lib.h>

#include <mate-panel-applet.h>
#include <mate-panel-applet-gsettings.h>

#include "common/load.h"

// Panel Specific Settings Implementation
#define LOAD_CONFIG_BASENAME "indicator.conf"

static void
mate_orientation_cb(MatePanelApplet *applet, guint orient, LoadPlugin *a)
{
	// ignore provided orientation as sometimes at startup has invalid values
	orient = mate_panel_applet_get_orient(applet);

	if (orient == MATE_PANEL_APPLET_ORIENT_UP || orient == MATE_PANEL_APPLET_ORIENT_DOWN) {
		a->panel_orientation = GTK_ORIENTATION_HORIZONTAL;
	} else { // if (orient == MATE_PANEL_APPLET_ORIENT_LEFT || orient == MATE_PANEL_APPLET_ORIENT_RIGHT)
		a->panel_orientation = GTK_ORIENTATION_VERTICAL;
	}

	load_refresh_orientation(a);
}

static gboolean
mate_constructor(MatePanelApplet* applet, const char* iid, gpointer data)
{
	if (g_strcmp0(iid, "LoadApplet")) {
		return FALSE;
	}

	LoadPlugin *load = load_new();

	mate_panel_applet_set_flags(applet, MATE_PANEL_APPLET_EXPAND_MINOR | MATE_PANEL_APPLET_HAS_HANDLE);
	mate_panel_applet_set_background_widget(applet, GTK_WIDGET(applet));

	gtk_container_add(GTK_CONTAINER(applet), GTK_WIDGET(load->container));
	gtk_widget_show(GTK_WIDGET(applet));

	// trigger orientation setup
	mate_orientation_cb(applet, -1, load);

	load_defaults(load);

	load_start(load);

	/* plugin signals */
	g_signal_connect(G_OBJECT(applet), "change-orient", G_CALLBACK(mate_orientation_cb), load);

	return TRUE;
}

MATE_PANEL_APPLET_OUT_PROCESS_FACTORY("LoadFactory",
	PANEL_TYPE_APPLET,
	about_data_description,
	mate_constructor,
	NULL)
