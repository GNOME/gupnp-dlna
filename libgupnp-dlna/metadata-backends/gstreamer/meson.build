dlna_gst_headers = files('gupnp-dlna-gst-utils.h')

install_headers(dlna_gst_headers, subdir : headers_subdir)

libgupnp_dlna_gst = library(
    'gupnp-dlna-gst-2.0',
    files(
        'gupnp-dlna-gst-audio-information.c',
        'gupnp-dlna-gst-container-information.c',
        'gupnp-dlna-gst-image-information.c',
        'gupnp-dlna-gst-information.c',
        'gupnp-dlna-gst-info-utils.c',
        'gupnp-dlna-gst-utils.c',
        'gupnp-dlna-gst-video-information.c'
    ),
    version: library_version,
    dependencies : [
        glib,
        gio,
        gstreamer_pbu,
        gstreamer,
        config_dep,
        gupnp_dlna
    ],
    install: true
)

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
        config_dep,
    ],
    include_directories : include_directories(
        '../../metadata',
        '../..'
    ),
    install: true,
    install_dir : metadata_backend_dir
)

pkg.generate(
    libraries : libgupnp_dlna_gst,
    version: meson.project_version(),
    subdirs : 'gupnp-dlna-2.0',
    name: 'gupnp-dlna-gst-2.0',
    description : 'GStreamer specific utils using GUPnP-DLNA'
)
