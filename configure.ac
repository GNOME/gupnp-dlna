AC_PREREQ([2.63])
AC_INIT([gupnp-dlna],
        [0.10.2],
        [https://bugzilla.gnome.org/enter_bug.cgi?product=gupnp&component=gupnp-dlna],
        [gupnp-dlna],
        [http://www.gupnp.org/])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR(libgupnp-dlna/gupnp-dlna-profile.h)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 tar-ustar no-dist-gzip dist-xz])
AM_MAINTAINER_MODE([enable])

AM_SILENT_RULES([yes])

dnl library versioning
dnl Increase when changing the API
GUPNP_DLNA_CURRENT=3

dnl Update when changing implementation of current API,
dnl reset to 0 when changing CURRENT.  This is the revision of
dnl current API version
GUPNP_DLNA_REVISION=0

dnl Increase if API change is ABI compatible, otherwise reset to 0
GUPNP_DLNA_AGE=0

GUPNP_DLNA_VERSION_INFO="$GUPNP_DLNA_CURRENT:$GUPNP_DLNA_REVISION:$GUPNP_DLNA_AGE"
AC_SUBST(GUPNP_DLNA_VERSION_INFO)

# Check for programs
AC_PROG_CC
AC_HEADER_STDC

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT([win32-dll])

PKG_CHECK_MODULES([LIBXML], [libxml-2.0 >= 2.5.0])

AM_PROG_CC_C_O

AC_ARG_WITH([default-metadata-backend],
            [AS_HELP_STRING([--with-default-metadata-backend=your_backend],
                            [set a default metadata backend @<:@default=gstreamer@:>@])])

if test "x$with_default_metadata_backend" = 'x'
then
        with_default_metadata_backend='gstreamer'
fi

if test "x$with_default_metadata_backend" = 'xyes' || test "x$with_default_metadata_backend" = 'xno'
then
        AC_MSG_WARN([Dubious default metadata backend name: $with_default_metadata_backend])
fi

GUPNP_DLNA_METADATA_BACKEND_CFLAGS="$GUPNP_DLNA_METADATA_BACKEND_CFLAGS -DGUPNP_DLNA_DEFAULT_METADATA_BACKEND=\\\"$with_default_metadata_backend\\\""

AC_ARG_ENABLE([gstreamer-metadata-backend],
              [AS_HELP_STRING([--enable-gstreamer-metadata-backend],
                              [build a GStreamer metadata backend @<:@default=auto@:>@])],
                              ,
                              [enable_gstreamer_metadata_backend=auto])

if test "x$enable_gstreamer_metadata_backend" != 'xno'
then
        gstreamer_found='yes'
        GST_MAJORMINOR=1.0
        GST_REQ=1.0
        GSTPBU_REQ=1.0

        PKG_CHECK_MODULES([GST],
                          [gstreamer-$GST_MAJORMINOR >= $GST_REQ],
                          ,
                          [gstreamer_found='no'])
        PKG_CHECK_MODULES([GST_PBU],
                          [gstreamer-pbutils-$GST_MAJORMINOR >= $GSTPBU_REQ],
                          ,
                          [gstreamer_found='no'])

        if test "x$gstreamer_found" = 'xyes'
        then
                AC_SUBST(GST_MAJORMINOR)

                GST_CFLAGS="$GST_CFLAGS -DGST_USE_UNSTABLE_API"
                AC_SUBST(GST_CFLAGS)
                enable_gstreamer_metadata_backend='yes'
        elif test "x$enable_gstreamer_metadata_backend" = 'xyes'
        then
                AC_MSG_ERROR([GStreamer $GST_MAJORMINOR not found.])
        else
                enable_gstreamer_metadata_backend='no'
        fi
fi

if test "x$enable_gstreamer_metadata_backend" = 'xyes'
then
        AM_CONDITIONAL(GSTREAMER_METADATA_BACKEND_ENABLED, true)
else
        AM_CONDITIONAL(GSTREAMER_METADATA_BACKEND_ENABLED, false)
fi

AC_ARG_ENABLE([legacy-gstreamer-metadata-backend],
              [AS_HELP_STRING([--enable-legacy-gstreamer-metadata-backend],
                              [build a GStreamer 0.10 metadata backend @<:@default=no@:>@])],
                              ,
                              [enable_legacy_gstreamer_metadata_backend=no])

if test "x$enable_legacy_gstreamer_metadata_backend" != 'xno'
then
        gstreamer_found='yes'
        GST_LEGACY_MAJORMINOR=0.10
        GST_LEGACY_REQ=0.10.36
        GSTPBU_LEGACY_REQ=0.10.36

        PKG_CHECK_MODULES([GST_LEGACY],
                          [gstreamer-$GST_LEGACY_MAJORMINOR >= $GST_LEGACY_REQ],
                          ,
                          [gstreamer_found='no'])
        PKG_CHECK_MODULES([GST_PBU_LEGACY],
                          [gstreamer-pbutils-$GST_LEGACY_MAJORMINOR >= $GSTPBU_LEGACY_REQ],
                          ,
                          [gstreamer_found='no'])

        if test "x$gstreamer_found" = 'xyes'
        then
                AC_SUBST(GST_LEGACY_MAJORMINOR)
                enable_legacy_gstreamer_metadata_backend='yes'
        elif test "x$enable_legacy_gstreamer_metadata_backend" = 'xyes'
        then
                AC_MSG_ERROR([GStreamer $GST_LEGACY_MAJORMINOR not found.])
        else
                enable_legacy_gstreamer_metadata_backend='no'
        fi
fi

if test "x$enable_legacy_gstreamer_metadata_backend" = 'xyes'
then
        AM_CONDITIONAL(GSTREAMER_LEGACY_METADATA_BACKEND_ENABLED, true)
else
        AM_CONDITIONAL(GSTREAMER_LEGACY_METADATA_BACKEND_ENABLED, false)
fi

AC_ARG_WITH([default-metadata-backend-dir],
            [AS_HELP_STRING([--with-default-metadata-backend-dir=your_directory],
                            [set a default metadata backend directory @<:@default=$libdir/$PACKAGE_NAME@:>@])])

if test "x$with_default_metadata_backend_dir" = 'x'
then
        with_default_metadata_backend_dir="$libdir/$PACKAGE_NAME"
fi

if test "x$with_default_metadata_backend_dir" = 'xyes' || test "x$with_default_metadata_backend_dir" = 'xno'
then
        AC_MSG_WARN([Dubious default metadata backend directory: $with_default_metadata_backend_dir])
fi

AC_SUBST(METADATA_DIRECTORY, $with_default_metadata_backend_dir)

GUPNP_DLNA_METADATA_BACKEND_CFLAGS="$GUPNP_DLNA_METADATA_BACKEND_CFLAGS -DGUPNP_DLNA_DEFAULT_METADATA_BACKEND_DIR=\\\"$with_default_metadata_backend_dir\\\""

AC_SUBST(GUPNP_DLNA_METADATA_BACKEND_CFLAGS)

PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.32])
PKG_CHECK_MODULES([GOBJECT], [gobject-2.0])
PKG_CHECK_MODULES([GMODULE], [gmodule-2.0])

# Check for g_clear_pointer
have_g_clear_pointer='no'
PKG_CHECK_EXISTS([glib-2.0 >= 2.34],
                 [have_g_clear_pointer='yes'])
AS_VAR_IF([have_g_clear_pointer], [no],
          [AC_MSG_CHECKING([for g_clear_pointer])
           GUPNP_DLNA_TEMP_CFLAGS="$CFLAGS"
           CFLAGS="$CFLAGS $GLIB_CFLAGS"
           GUPNP_DLNA_TEMP_LIBS="$LIBS"
           LIBS="$LIBS $GLIB_LIBS"
           AC_LANG([C])
           AC_LINK_IFELSE([AC_LANG_PROGRAM([[
                                             #include <glib.h>
                                           ]],
                                           [[
                                             gchar *test = g_malloc (2);
                                             g_clear_pointer (&test, g_free);
                                             return (test == NULL)
                                           ]])],
                          [have_g_clear_pointer='yes'])
          LIBS="$GUPNP_DLNA_TEMP_LIBS"
          CFLAGS="$GUPNP_DLNA_TEMP_CFLAGS"
          AC_MSG_RESULT([$have_g_clear_pointer])])

AS_VAR_IF([have_g_clear_pointer], [no],
          [AC_DEFINE([g_clear_pointer(ptr, func)],
                     [\
                      G_STMT_START { \
                        if (ptr != NULL && func != NULL) { \
                          gpointer g_clear_pointer_tmp_var = *ptr; \
                          *ptr = NULL; \
                          func (g_clear_pointer_tmp_var); \
                        } \
                      } G_STMT_END],
                     [Naive g_clear_pointer implementation.])])

# Debugging
AC_ARG_ENABLE([debug],
              AS_HELP_STRING([--enable-debug], [enable debugging @<:@default=no@:>@]),
              ,
              [enable_debug=no])
if test "x$enable_debug" = "xyes"; then
        CFLAGS="$CFLAGS -g -Wall"
fi

GOBJECT_INTROSPECTION_CHECK([0.6.4])

# vapigen
have_vapigen=no
AS_IF([test "x$found_introspection" = "xyes"],
      [
       dnl output of g-i >= 1.36.0 needs vapigen >= 0.20 to process
       AC_MSG_CHECKING([whether G-I is 1.36 or newer])
       VAPIGEN_MIN_VERSION=0.18
       AS_IF([pkg-config --atleast-version=1.36.0 gobject-introspection-1.0],
             [
              AC_MSG_RESULT([yes])
              VAPIGEN_MIN_VERSION=0.20
             ],[AC_MSG_RESULT([no])]
       )
       GUPNP_PROG_VAPIGEN([$VAPIGEN_MIN_VERSION])
      ]
)
AS_IF([test "x$VAPIGEN" != "x"], [have_vapigen=yes])
AM_CONDITIONAL([HAVE_VAPIGEN], [test "x$VAPIGEN" != "x"])

GTK_DOC_CHECK([1.11],[--flavour no-tmpl])
# for fixxref
GLIB_PREFIX="`$PKG_CONFIG --variable=prefix glib-2.0`"
AC_SUBST(GLIB_PREFIX)

dnl xmllint for schema validation
AC_PATH_PROG([XMLLINT],[xmllint])
AM_CONDITIONAL([HAVE_XMLLINT], [test "x$XMLLINT" != "x"])

AC_CONFIG_FILES([
Makefile
libgupnp-dlna/Makefile
tools/Makefile
tests/Makefile
data/Makefile
gupnp-dlna-2.0.pc
gupnp-dlna-metadata-2.0.pc
gupnp-dlna-gst-2.0.pc
gupnp-dlna-gst-legacy-2.0.pc
doc/Makefile
doc/version.xml
doc/gupnp-dlna/Makefile
doc/gupnp-dlna-metadata/Makefile
doc/gupnp-dlna-gst/Makefile
vala/Makefile
])
AC_CONFIG_FILES([tests/test-discoverer.sh],
                [chmod a+x tests/test-discoverer.sh])
AC_OUTPUT

echo
echo 'Configuration status:'
echo
echo "GObject-Introspection:                   $found_introspection"
echo "VALA bindings:                           $have_vapigen"
echo "Default metadata backend:                $with_default_metadata_backend"
echo "Default metadata backend directory:      $with_default_metadata_backend_dir"
echo "Build GStreamer metadata backend:        $enable_gstreamer_metadata_backend"
echo "Build legacy GStreamer metadata backend: $enable_legacy_gstreamer_metadata_backend"
echo
