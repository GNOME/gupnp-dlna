# Version format current:revision:age
# If the library source code has changed at all since the last update, then
# increment revision (‘c:r:a’ becomes ‘c:r+1:a’).
# If any interfaces have been added, removed, or changed since the last update,
# increment current, and set revision to 0.
# If any interfaces have been added since the last public release, then
# increment age.
# If any interfaces have been removed since the last public release, then set
# age to 0.
AUTOMAKE_OPTIONS=subdir-objects

LTVERSION = $(GUPNP_DLNA_VERSION_INFO)

include $(top_srcdir)/common.am

noinst_LTLIBRARIES =
pkglib_LTLIBRARIES =
lib_LTLIBRARIES =
dist_noinst_HEADERS =

lib_LTLIBRARIES += libgupnp-dlna-2.0.la

general_log_cflags = \
	-DG_LOG_DOMAIN='"gupnp-dlna"'

loader_log_cflags = \
	-DG_LOG_DOMAIN='"gupnp-dlna-loader"'

guesser_log_cflags = \
	-DG_LOG_DOMAIN='"gupnp-dlna-guesser"'

metadata_log_cflags = \
	-DG_LOG_DOMAIN='"gupnp-dlna-metadata"'

common_cflags = \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	-I$(top_srcdir)/libgupnp-dlna/profile-backends/native/sets

metadata_cflags = \
	-I$(top_srcdir)/libgupnp-dlna/metadata

common_libadd = \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS)

# "Profile backend" might add some stuff to variables below.
gupnp_ldflags = \
	-version-info $(LTVERSION) \
	-no-undefined

gupnp_cflags = \
	$(common_cflags)

gupnp_libadd = \
	$(common_libadd)

include $(top_srcdir)/libgupnp-dlna/profile-backends/native/native.am
include $(top_srcdir)/libgupnp-dlna/metadata/metadata.am

metadatadir=$(METADATA_DIRECTORY)
metadata_LTLIBRARIES =

if GSTREAMER_METADATA_BACKEND_ENABLED
include $(top_srcdir)/libgupnp-dlna/metadata-backends/gstreamer/gstreamer.am
endif

noinst_LTLIBRARIES += \
	libloader.la \
	libguesser.la \
	libmetadata.la

libloader_la_SOURCES = \
	gupnp-dlna-profile-loader.c
libloader_la_LIBADD = \
	$(common_libadd) \
	$(LIBXML_LIBS)
libloader_la_CFLAGS = \
	$(loader_log_cflags) \
	$(metadata_cflags) \
	$(common_cflags) \
	$(LIBXML_CFLAGS) \
	-DDATA_DIR='"$(shareddir)"'

libguesser_la_SOURCES = \
	gupnp-dlna-profile-guesser.c \
	gupnp-dlna-profile-guesser-impl.c
libguesser_la_LIBADD = \
	$(common_libadd)
libguesser_la_CFLAGS = \
	$(guesser_log_cflags) \
	$(metadata_cflags) \
	$(common_cflags)

libmetadata_la_SOURCES = \
	gupnp-dlna-metadata-backend.c \
	$(metadata_metadata_sources)
libmetadata_la_LIBADD = \
	$(common_libadd) \
	$(GMODULE_LIBS)
libmetadata_la_CFLAGS = \
	$(metadata_log_cflags) \
	$(metadata_cflags) \
	$(GUPNP_DLNA_METADATA_BACKEND_CFLAGS) \
	$(common_cflags) \
	$(GMODULE_CFLAGS)

libgupnp_dlna_2_0_la_LDFLAGS = \
	$(gupnp_ldflags)
libgupnp_dlna_2_0_la_SOURCES = \
	$(metadata_general_sources) \
	gupnp-dlna-field-value.c \
	gupnp-dlna-profile-backend.c \
	gupnp-dlna-profile.c \
	gupnp-dlna-restriction.c \
	gupnp-dlna-value-list.c \
	gupnp-dlna-g-values.c
libgupnp_dlna_2_0_la_LIBADD = \
	$(gupnp_libadd) \
	libloader.la \
	libguesser.la \
	libmetadata.la
libgupnp_dlna_2_0_la_CFLAGS = \
	$(general_log_cflags) \
	$(metadata_cflags) \
	$(gupnp_cflags)

libgupnp_dlna_incdir = \
	$(includedir)/gupnp-dlna-2.0/libgupnp-dlna
libgupnp_dlna_inc_HEADERS = \
	gupnp-dlna-profile-guesser.h \
	gupnp-dlna-profile.h \
	gupnp-dlna-restriction.h \
	gupnp-dlna-value-list.h \
	gupnp-dlna-g-values.h

libgupnp_dlna_metadata_incdir = \
	$(libgupnp_dlna_incdir)/metadata
libgupnp_dlna_metadata_inc_HEADERS = \
	metadata/gupnp-dlna-audio-information.h \
	metadata/gupnp-dlna-container-information.h \
	metadata/gupnp-dlna-image-information.h \
	metadata/gupnp-dlna-video-information.h \
	metadata/gupnp-dlna-information.h \
	metadata/gupnp-dlna-metadata-extractor.h \
	metadata/gupnp-dlna-values.h

dist_noinst_HEADERS += \
	gupnp-dlna-field-value.h \
	gupnp-dlna-metadata-backend.h \
	gupnp-dlna-profile-backend.h \
	gupnp-dlna-profile-loader.h \
	gupnp-dlna-profile-guesser-impl.h \
	gupnp-dlna-restriction-private.h \
	gupnp-dlna-value-list-private.h \
	gupnp-dlna-g-values-private.h


# INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS =
INTROSPECTION_COMPILER_ARGS = \
	--includedir=$(top_srcdir) \
	--includedir=$(top_srcdir)/libgupnp-dlna/profile-backends/native/sets

if HAVE_INTROSPECTION

girs = GUPnPDLNA-2.0.gir

introspection_sources = \
	$(libgupnp_dlna_inc_HEADERS) \
	$(libgupnp_dlna_inc_HEADERS:.h=.c)

GUPnPDLNA-2.0.gir: libgupnp-dlna-2.0.la
GUPnPDLNA_2_0_gir_FILES = $(introspection_sources)
GUPnPDLNA_2_0_gir_LIBS = libgupnp-dlna-2.0.la
GUPnPDLNA_2_0_gir_INCLUDES = GObject-2.0
GUPnPDLNA_2_0_gir_SCANNERFLAGS = \
	--warn-all \
	--symbol-prefix=gupnp_dlna \
	--identifier-prefix=GUPnPDLNA \
	-I$(top_srcdir)/libgupnp-dlna/profile-backends/native/sets

INTROSPECTION_GIRS += $(girs)

girdir = $(datadir)/gir-1.0
gir_DATA = $(girs)

typelibdir = $(libdir)/girepository-1.0/
typelib_DATA = $(gir_DATA:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelib_DATA)

endif
