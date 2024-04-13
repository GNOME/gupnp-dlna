
gupnp_dlna_vapi = gnome.generate_vapi(
    'gupnp-dlna-2.0',
    sources : [
        gupnp_dlna_gir.get(0),
        'gupnp-dlna-2.0-custom.vala'
    ],
    packages : ['glib-2.0', 'gio-2.0', 'gmodule-2.0'],
    install : true
)

gupnp_dlna_gst_vapi = gnome.generate_vapi(
    'gupnp-dlna-gst-2.0',
    sources : [
        gupnp_dlna_gst_gir.get(0),
        'gupnp-dlna-gst-2.0-custom.vala'
    ],
    packages : [
        'glib-2.0',
        'gobject-2.0',
        'gstreamer-1.0',
        'gstreamer-pbutils-1.0',
        gupnp_dlna_vapi
    ],
    gir_dirs : gupnp_dlna_gir_dir,
    install : true
)
