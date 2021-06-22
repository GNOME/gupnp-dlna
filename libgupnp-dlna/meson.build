headers_subdir = 'gupnp-dlna-2.0/libgupnp-dlna'

metadata_incdir = include_directories('metadata')

loader_sources = files(
    'gupnp-dlna-profile-loader.c'
)

libloader = static_library(
    'loader',
    loader_sources,
    c_args : [
        '-DG_LOG_DOMAIN="gupnp-dlna-loader"',
        '-DDATA_DIR="@0@"'.format(shareddir)
    ],
    dependencies : [glib, gio, xml],
    include_directories: [
        toplevel_incdir,
        config_h_inc
    ]
)

guesser_sources = files(
    'gupnp-dlna-profile-guesser.c',
    'gupnp-dlna-profile-guesser-impl.c'
)

libguesser = static_library(
    'guesser',
    guesser_sources,
    dependencies : [glib, gio],
    c_args : [
        '-DG_LOG_DOMAIN="gupnp-dlna-guesser"',
        '-DDATA_DIR="@0@"'.format(shareddir)
    ],
    include_directories: [
        toplevel_incdir,
        metadata_incdir,
        config_h_inc
    ]
)

metadata_sources = files(
    'gupnp-dlna-metadata-backend.c',
    'metadata/gupnp-dlna-metadata-extractor.c'
)

libmetadata = static_library(
    'metadata',
    metadata_sources,
    dependencies : [glib, gio],
    c_args : [
        '-DG_LOG_DOMAIN="gupnp-dlna-metadata"',
        '-DDATA_DIR="@0@"'.format(shareddir)
    ],
    include_directories: [
        toplevel_incdir,
        metadata_incdir,
        config_h_inc
    ]
)

headers = files(
    'gupnp-dlna-profile-guesser.h',
    'gupnp-dlna-profile.h',
    'gupnp-dlna-restriction.h',
    'gupnp-dlna-value-list.h',
    'gupnp-dlna-g-values.h',
    'gupnp-dlna-audio-information.h',
    'gupnp-dlna-container-information.h',
    'gupnp-dlna-image-information.h',
    'gupnp-dlna-video-information.h',
    'gupnp-dlna-information.h',
    'gupnp-dlna-values.h',
    'gupnp-dlna.h'
)

install_headers(
    headers,
    subdir: headers_subdir
)

metadata_headers = files('metadata/gupnp-dlna-metadata-extractor.h')

install_headers(
    metadata_headers,
    subdir: join_paths(headers_subdir, 'metadata')
)

sources = files(
    'gupnp-dlna-audio-information.c',
    'gupnp-dlna-container-information.c',
    'gupnp-dlna-image-information.c',
    'gupnp-dlna-information.c',
    'gupnp-dlna-video-information.c',
    'gupnp-dlna-field-value.c',
    'gupnp-dlna-profile.c',
    'gupnp-dlna-restriction.c',
    'gupnp-dlna-value-list.c',
    'gupnp-dlna-g-values.c',
    'gupnp-dlna-utils.c',
    'gupnp-dlna-value-type.c',
    'gupnp-dlna-info-value.c',
    'gupnp-dlna-value.c',
    'gupnp-dlna-info-set.c'
)

libgupnp_dlna = library(
    'gupnp-dlna-2.0',
    sources,
    version: library_version,
    link_whole: [
        libloader,
        libguesser,
        libmetadata
    ],
    dependencies: [glib, gio, gmodule],
    c_args : ['-DG_LOG_DOMAIN="gupnp-dlna"'],
    include_directories: [
        include_directories('..'), 
        config_h_inc
    ],
    install: true
)

gupnp_dlna = declare_dependency(
    link_with : libgupnp_dlna,
    include_directories: [include_directories('.'), include_directories('..')],
)

meson.override_dependency('gupnp-dlna-2.0', gupnp_dlna)

pkg.generate(
    libgupnp_dlna,
    version: meson.project_version(),
    subdirs : 'gupnp-dlna-2.0',
    name: 'gupnp-dlna-2.0',
    description: 'GObject-based DLNA specific UPnP library',
    libraries: [glib, gobject]
)

pkg.generate(
    version: meson.project_version(),
    subdirs: 'gupnp-dlna-2.0/metadata',
    name : 'gupnp-dlna-metadata-2.0',
    description: 'Metadata extractor plugin development files for GUPnP-DLNA',
    libraries: [glib, gobject, gupnp_dlna],
    variables: 'plugindir=@0@'.format(metadata_backend_dir)
)

if get_option('introspection')
    gupnp_dlna_gir_dir = meson.current_build_dir()
    gupnp_dlna_gir = gnome.generate_gir(
        libgupnp_dlna,
        sources : headers +
            metadata_headers +
            sources +
            guesser_sources +
            metadata_sources +
            loader_sources,
        namespace : 'GUPnPDLNA',
        header : 'libgupnp-dlna/gupnp-dlna.h',
        nsversion : '2.0',
        symbol_prefix : 'gupnp_dlna',
        identifier_prefix : 'GUPnPDLNA',
        install: true,
        includes : ['GObject-2.0']
    )
endif

subdir('metadata-backends')
