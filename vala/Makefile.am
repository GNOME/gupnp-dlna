if HAVE_INTROSPECTION
if HAVE_VAPIGEN
gupnp-dlna-2.0.vapi: gupnp-dlna-2.0.stamp
	@true

gupnp-dlna-2.0.stamp: $(builddir)/../libgupnp-dlna/GUPnPDLNA-2.0.gir \
		$(srcdir)/GUPnPDLNA-2.0.metadata \
		$(srcdir)/gupnp-dlna-2.0-custom.vala
	$(AM_V_GEN)$(VAPIGEN) --metadatadir=$(srcdir) \
			      --library=gupnp-dlna-2.0 \
			      $(builddir)/../libgupnp-dlna/GUPnPDLNA-2.0.gir \
			      $(srcdir)/gupnp-dlna-2.0-custom.vala && \
	echo timestamp > $(@F)

vapidir = $(datadir)/vala/vapi
vapi_DATA = \
	gupnp-dlna-2.0.vapi \
	$(srcdir)/gupnp-dlna-2.0.deps
CLEANFILES = \
	gupnp-dlna-2.0.vapi \
	gupnp-dlna-2.0.stamp
EXTRA_DIST = \
	gupnp-dlna-2.0.deps \
	GUPnPDLNA-2.0.metadata \
	gupnp-dlna-2.0-custom.vala \
	gupnp-dlna-2.0.vapi \
	gupnp-dlna-2.0.stamp

if GSTREAMER_METADATA_BACKEND_ENABLED
gupnp-dlna-gst-2.0.vapi: gupnp-dlna-gst-2.0.stamp
	@true

gupnp-dlna-gst-2.0.stamp: $(builddir)/../libgupnp-dlna/GUPnPDLNAGst-2.0.gir \
		$(srcdir)/GUPnPDLNAGst-2.0.metadata \
		$(srcdir)/gupnp-dlna-gst-2.0-custom.vala \
		gupnp-dlna-2.0.vapi
	$(AM_V_GEN)$(VAPIGEN) --metadatadir=$(srcdir) \
			      --library=gupnp-dlna-gst-2.0 \
			      --vapidir=$(builddir) \
			      --girdir=$(builddir)/../libgupnp-dlna \
			      --pkg gupnp-dlna-2.0 \
			      --pkg gstreamer-1.0 \
			      --pkg gstreamer-pbutils-1.0 \
			      --pkg gstreamer-video-1.0 \
			      --pkg gstreamer-base-1.0 \
			      $(builddir)/../libgupnp-dlna/GUPnPDLNAGst-2.0.gir \
			      $(srcdir)/gupnp-dlna-gst-2.0-custom.vala && \
	echo timestamp > $(@F)


vapi_DATA += \
	gupnp-dlna-gst-2.0.vapi \
	$(srcdir)/gupnp-dlna-gst-2.0.deps
CLEANFILES += \
	gupnp-dlna-gst-2.0.vapi \
	gupnp-dlna-gst-2.0.stamp
EXTRA_DIST += \
	gupnp-dlna-gst-2.0.deps \
	GUPnPDLNAGst-2.0.metadata \
	gupnp-dlna-gst-2.0-custom.vala \
	gupnp-dlna-gst-2.0.vapi \
	gupnp-dlna-gst-2.0.stamp
endif

if GSTREAMER_LEGACY_METADATA_BACKEND_ENABLED
vapi_DATA += \
	gupnp-dlna-gst-legacy-2.0.vapi \
	$(srcdir)/gupnp-dlna-gst-legacy-2.0.deps
endif
endif
endif
