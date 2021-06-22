dlna_gst_headers = files('gupnp-dlna-gst-utils.h')

dlna_backend_dir = meson.current_build_dir()

install_headers(dlna_gst_headers, subdir : headers_subdir)

dlna_gst_sources = files(
    'gupnp-dlna-gst-audio-information.c',
    'gupnp-dlna-gst-container-information.c',
    'gupnp-dlna-gst-image-information.c',
    'gupnp-dlna-gst-information.c',
    'gupnp-dlna-gst-info-utils.c',
    'gupnp-dlna-gst-utils.c',
    'gupnp-dlna-gst-video-information.c'
)

libgupnp_dlna_gst = library(
    'gupnp-dlna-gst-2.0',
    dlna_gst_sources,
    version: library_version,
    dependencies : [
        glib,
        gio,
        gstreamer_pbu,
        gstreamer,
        gupnp_dlna
    ],
    include_directories : [
        toplevel_incdir,
        metadata_incdir,
        config_h_inc],
    c_args : ['-DG_LOG_DOMAIN="gupnp-dlna-metadata"'],
    install: true
)

gupnp_dlna_gst = declare_dependency(
    link_with: libgupnp_dlna_gst,
    include_directories : [include_directories('.')]
)

meson.override_dependency('gupnp-dlna-gst-2.0', gupnp_dlna_gst)

shared_module(
    'gstreamer',
    files(
        'gupnp-dlna-gst-metadata-backend.c',
        'gupnp-dlna-gst-metadata-extractor.c',
    ),
    link_with: libgupnp_dlna_gst,
    dependencies : [
        glib,
        gstreamer_pbu,
    ],
    include_directories : [
        toplevel_incdir,
        metadata_incdir,
        config_h_inc],
    install: true,
    install_dir : metadata_backend_dir
)

pkg.generate(
    libgupnp_dlna_gst,
    version: meson.project_version(),
    subdirs : 'gupnp-dlna-2.0',
    name: 'gupnp-dlna-gst-2.0',
    description : 'GStreamer specific utils using GUPnP-DLNA',
    libraries: gupnp_dlna,
    requires: gstreamer_pbu
)

if get_option('introspection')
    gupnp_dlna_gst_gir = gnome.generate_gir(
        libgupnp_dlna_gst,
        sources : dlna_gst_headers +
            dlna_gst_sources,
        namespace : 'GUPnPDLNAGst',
        nsversion : '2.0',
        symbol_prefix : 'gupnp_dlna_gst',
        identifier_prefix : 'GUPnPDLNAGst',
        install: true,
        includes : ['GObject-2.0', gupnp_dlna_gir.get(0), 'Gst-1.0', 'GstPbutils-1.0']
    )
endif
