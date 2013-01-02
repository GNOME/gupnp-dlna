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
vapi_DATA = gupnp-dlna-2.0.vapi $(srcdir)/gupnp-dlna-2.0.deps
CLEANFILES = gupnp-dlna-2.0.vapi gupnp-dlna-2.0.stamp
EXTRA_DIST = gupnp-dlna-2.0.deps GUPnPDLNA-2.0.metadata gupnp-dlna-2.0-custom.vala
endif
endif
