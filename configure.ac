# Initialization
AC_PREREQ(2.60)
AC_INIT([load], [m4_esyscmd([date +%s | tr -d '\n'])], [evasive.gyron@gmail.com])
AC_SUBST(APPICON, utilities-system-monitor)
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.11 -Wall -Werror no-dist-gzip dist-xz silent-rules foreign])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE()

# Set starting CFLAGS (instead of default "-g -O2")
: ${CFLAGS="-g -O3"}

# Checks for programs.
AM_PROG_AR()
LT_PREREQ([2.2.6])
LT_INIT([disable-static])
AC_PROG_CC()
# Compiling sources with per-target flags requires AM_PROG_CC_C_O
AM_PROG_CC_C_O()
AC_PROG_LD()
AC_PROG_INSTALL()
AC_PROG_LIBTOOL()
AC_PROG_INTLTOOL(, [no-xml])

# Check for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([time.h glib.h math.h dirent.h mntent.h ctype.h stdio.h errno.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Test CFLAGS
AC_DEFUN([TEST_CFLAGS],
	AC_MSG_CHECKING([whether compiler understands $1])
	old_CFLAGS="$CFLAGS"
	CFLAGS="$CFLAGS $1"
	AC_TRY_COMPILE([],[], AC_MSG_RESULT(yes), AC_MSG_RESULT(no); CFLAGS="$old_CFLAGS")
)

TEST_CFLAGS([-Wall])

# GCC-specific CFLAGS (uncomment when needed)
# Runtime diagnostics (use with caution, breaks some plugins)
#CFLAGS="${CFLAGS} -fdiagnostics-color=auto -fsanitize=address -fsanitize=leak -fsanitize=undefined"
# Build-time error checking
#CFLAGS="${CFLAGS} -Wextra -Wpedantic -std=gnu99 -Wno-unused-parameter"

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset rint strchr strncasecmp strtol])

# I18n
rm -f $srcdir/po/LINGUAS
for po_file in `ls $srcdir/po/*.po | sort`; do
	echo "$po_file" | sed "s|.*/po/\(.*\)\.po|\1|g" >> $srcdir/po/LINGUAS
done

# Set up gettext
GETTEXT_PACKAGE=load
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])
AM_GLIB_GNU_GETTEXT

# Default paths
AC_PREFIX_PROGRAM([ make ])
if test "${sysconfdir}" = '${prefix}/etc' ; then sysconfdir=/etc ; fi

# Check highest available GTK version
PKG_CHECK_EXISTS([gtk+-3.0], [auto_gtk=3.0])
# Select GTK version
AC_MSG_CHECKING([which gtk+ version to compile against])
AC_ARG_WITH([gtk], [AS_HELP_STRING([--with-gtk=3.0|auto], [which GTK+ version to compile against (default: auto)])],
	[case "$with_gtk" in
		3.0) ;;
		auto) with_gtk=$auto_gtk ;;
		*) AC_MSG_ERROR([invalid gtk version specified]) ;;
	esac],
	[ with_gtk=$auto_gtk ]
)
AC_MSG_RESULT([$with_gtk])

case "$with_gtk" in
	3.0)
		GTK_API=3
		GTK_REQUIRED=3.0.0
		;;
esac
AC_SUBST(GTK_API_VERSION)
AC_DEFINE_UNQUOTED(GTK_API, $GTK_API, [GTK API version to compile against.])

# Log domain
AC_DEFINE_UNQUOTED(G_LOG_DOMAIN,"load", [Log domain])

PKG_CHECK_MODULES(GTK, gtk+-$GTK_API.0 >= $GTK_REQUIRED)
AC_SUBST(GTK_FLAGS)
AC_SUBST(GTK_LIBS)

# Other dependencies
PKG_CHECK_MODULES(CAIRO, [cairo >= 1.0])
AC_SUBST(CAIRO_CFLAGS)
AC_SUBST(CAIRO_LIBS)

# Disable Glib deprecation warnings
AC_ARG_ENABLE([deprecations], AS_HELP_STRING([--disable-deprecations], [Disable GDK/GTK deprecation warnings (useful when compiling with -Werror)]),
	[if test "x$enableval" = "xno"; then
		AC_DEFINE([GDK_DISABLE_DEPRECATION_WARNINGS], [1], [Disable GDK/GTK deprecation warnings])
	fi]
)

# Enable debug CFLAGS
AC_ARG_ENABLE([debug], AS_HELP_STRING([--enable-debug], [Enable CFLAGS for debugging with GDB.]),
	[if test "x$enableval" = "xyes"; then
		AC_DEFINE([MULTILOAD_DEBUG], [1], [Debug])
		CFLAGS="${CFLAGS} -ggdb3 -Og"
	fi]
)

# Enable profiling CFLAGS
AC_ARG_ENABLE([profiling], AS_HELP_STRING([--enable-profiling], [Enable CFLAGS for profiling with gprof.]),
	[if test "x$enableval" = "xyes"; then
		CFLAGS="${CFLAGS} -pg"
	fi]
)

# Per-panel configure
m4_include([mate/configure.m4])

# Directories; for substitution into .desktop files.
# From https://mail.gnome.org/archives/commits-list/2012-March/msg14085.html
# Of course it's worth noting that the patch in that message was to replace
# this nasty hack with an awk version that runs from the Makefile...
if test "${prefix}" = "NONE"; then
	prefix=/usr/local
fi
if test "${exec_prefix}" = "NONE"; then
	exec_prefix=$prefix
fi
eval LIBDIR="$libdir"
AC_SUBST([LIBDIR])

# Output files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	common/Makefile
	po/Makefile.in
])
AC_OUTPUT

AS_ECHO([ ])
AS_ECHO([ ])
AS_ECHO([ "* -----------------------------" ])
AS_ECHO([ "*  Configured $PACKAGE_NAME version $PACKAGE_VERSION" ])
AS_ECHO([ "* -----------------------------" ])
AS_ECHO([ "*" ])
AS_ECHO([ "*  Using prefix: $prefix" ])
AS_ECHO([ "*" ])
AS_ECHO([ "*  CFLAGS: $CFLAGS" ])
AS_ECHO([ "*  Building against GTK+$GTK_API" ])
AS_ECHO([ "*" ])
if test "x$enable_debug" = "xyes"; then
	AS_ECHO([ "*  Enabled debugging CFLAGS" ])
fi
