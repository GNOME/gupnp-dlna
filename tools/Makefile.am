bin_PROGRAMS = gupnp-dlna-info-1.1 gupnp-dlna-ls-profiles-1.1

AM_CFLAGS = -I$(top_srcdir) $(GST_CFLAGS) $(GST_PBU_CFLAGS)
gupnp_dlna_info_1_1_SOURCES = gupnp-dlna-info.c
gupnp_dlna_ls_profiles_1_1_SOURCES = gupnp-dlna-ls-profiles.c

LIBS = $(GST_LIBS) \
       $(GST_PBU_LIBS) \
       $(top_builddir)/libgupnp-dlna/libgupnp-dlna-1.1.la
