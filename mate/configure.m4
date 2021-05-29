GLIB_GSETTINGS
PKG_CHECK_MODULES(MATE, [libmatepanelapplet-4.0 >= 1.7.0], [
	AC_DEFINE([HAVE_MATE], [1], [Support for mate-panel])
],[
	AC_MSG_FAILURE([libmatepanelapplet-4.0 test failed])
])

AC_SUBST(MATE_CFLAGS)
AC_SUBST(MATE_LIBS)

AC_CONFIG_FILES([
	mate/Makefile
	mate/org.mate.load.Applet.mate-panel-applet.in.in
	mate/org.mate.panel.applet.load.gschema.xml.in
])
