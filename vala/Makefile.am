if HAVE_INTROSPECTION
if HAVE_VAPIGEN
gupnp-dlna-1.1.vapi: gupnp-dlna-1.1.stamp
	@true

gupnp-dlna-1.1.stamp: $(builddir)/../libgupnp-dlna/GUPnPDLNA-1.1.gir \
		$(srcdir)/GUPnPDLNA-1.1.metadata \
		$(srcdir)/gupnp-dlna-1.1-custom.vala
	$(AM_V_GEN)$(VAPIGEN) --metadatadir=$(srcdir) \
			      --library=gupnp-dlna-1.1 \
			      --pkg gstreamer-1.0 \
			      --pkg gstreamer-pbutils-1.0 \
			      --pkg gstreamer-base-1.0 \
			      --pkg gstreamer-video-1.0 \
			      $(builddir)/../libgupnp-dlna/GUPnPDLNA-1.1.gir \
			      $(srcdir)/gupnp-dlna-1.1-custom.vala && \
	echo timestmap > $(@F)

vapidir = $(datadir)/vala/vapi
vapi_DATA = gupnp-dlna-1.1.vapi $(srcdir)/gupnp-dlna-1.1.deps
CLEANFILES = gupnp-dlna-1.1.vapi gupnp-dlna-1.1.stamp
EXTRA_DIST = gupnp-dlna-1.1.deps GUPnPDLNA-1.1.metadata gupnp-dlna-1.1-custom.vala
endif
endif
