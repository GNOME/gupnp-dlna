bin_PROGRAMS = \
	gupnp-dlna-info-2.0 \
	gupnp-dlna-ls-profiles-2.0

AM_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(LIBXML_CFLAGS) \
	$(GMODULE_CFLAGS)
LIBS = \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(LIBXML_LIBS) \
	$(GMODULE_LIBS) \
	$(top_builddir)/libgupnp-dlna/libgupnp-dlna-2.0.la

gupnp_dlna_info_2_0_SOURCES = \
	gupnp-dlna-info.c

gupnp_dlna_ls_profiles_2_0_SOURCES = \
	gupnp-dlna-ls-profiles.c
