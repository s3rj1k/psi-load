AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	-DMATELOCALEDIR=\"$(localedir)\" \
	$(PLATFORM_CPPFLAGS)

libexec_PROGRAMS = mate-load-applet
mate_load_applet_LOCATION = \
	$(libexecdir)/mate-load-applet

mate_load_applet_SOURCES = \
	plugin.c

mate_load_applet_CFLAGS = \
	$(GTK_CFLAGS) \
	$(MATE_CFLAGS)

mate_load_applet_LDADD = \
	../common/load-core.la \
	$(GTK_LIBS) \
	$(MATE_LIBS)

plugindir = $(datadir)/mate-panel/applets
plugin_in_files = org.mate.load.Applet.mate-panel-applet.in
plugin_DATA = $(plugin_in_files:.mate-panel-applet.in=.mate-panel-applet)

$(plugin_in_files): $(plugin_in_files).in Makefile
	@$(AM_V_GEN)sed -e "s|\@LOCATION\@|$(mate_load_applet_LOCATION)|" $< > $@

org.mate.load.Applet.mate-panel-applet: org.mate.load.Applet.mate-panel-applet.in
	@$(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

servicedir = $(datadir)/dbus-1/services
service_in_files = org.mate.panel.applet.LoadFactory.service.in
service_DATA = $(service_in_files:.service.in=.service)

org.mate.panel.applet.LoadFactory.service: $(service_in_files)
	@$(AM_V_GEN)sed -e "s|\@LOCATION\@|$(mate_load_applet_LOCATION)|" $< > $@

@INTLTOOL_XML_NOMERGE_RULE@
load_gschema_in_files = org.mate.panel.applet.load.gschema.xml.in
gsettings_SCHEMAS = $(load_gschema_in_files:.xml.in=.xml)
@GSETTINGS_RULES@

EXTRA_DIST = \
	$(plugin_in_files) \
	$(service_in_files) \
	$(load_gschema_in_files)

CLEANFILES = \
	org.mate.load.Applet.mate-panel-applet \
	org.mate.panel.applet.LoadFactory.service \
	org.mate.panel.applet.load.gschema.xml \
	$(plugin_in_files)
