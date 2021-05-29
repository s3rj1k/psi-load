AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	$(PLATFORM_CPPFLAGS)

noinst_LTLIBRARIES = \
	load-core.la

load_core_la_SOURCES = \
	colors.c colors.h \
	psi.c psi.h\
	graph-data.c graph-data.h \
	load-graph.c load-graph.h \
	load.c load.h \
	load-config.c load-config.h \
	properties.h

load_core_la_CFLAGS = \
	$(GTK_CFLAGS) \
	$(CAIRO_CFLAGS)

load_core_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined \
	-lm \
	$(PLATFORM_LDFLAGS)

load_core_la_LIBADD = \
	$(GTK_LIBS) \
	$(CAIRO_LIBS)
