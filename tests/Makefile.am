AM_TESTS_ENVIRONMENT = \
	export G_SLICE=debug-blocks \
	LD_LIBRARY_PATH=$(top_builddir)/libgupnp-dlna/.libs:$(LD_LIBRARY_PATH) \
	MEDIA_DIR="$(srcdir)/media" FILE_LIST="$(srcdir)/media/media-list.txt" \
	GUPNP_DLNA_PROFILE_DIR=$(abs_top_srcdir)/data \
	GUPNP_DLNA_METADATA_BACKEND_DIR=$(top_builddir)/libgupnp-dlna/metadata-backends/gstreamer/.libs;
TESTS = $(check_PROGRAMS) test-discoverer.sh

check_SCRIPTS = test-discoverer.sh
check_PROGRAMS = sets

sets_SOURCES = \
	sets.c
sets_LDADD = \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(LIBXML_LIBS) \
	$(GMODULE_LIBS) \
	$(top_builddir)/libgupnp-dlna/libgupnp-dlna-2.0.la
sets_LDFLAGS = $(WARN_LDFLAGS)
sets_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(LIBXML_CFLAGS) \
	$(GMODULE_CFLAGS) \
	$(WARN_CFLAGS) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/libgupnp-dlna
