headers_subdir = 'gupnp-dlna-2.0/libgupnp-dlna'

libloader = static_library(
    'loader',
    files(
        'gupnp-dlna-profile-loader.c'
    ),
    c_args : [
        '-DG_LOG_DOMAIN="gupnp-dlna-loader"',
        '-DDATA_DIR="@0@"'.format(shareddir)
    ],
    dependencies : [glib, gio, xml],
    include_directories: include_directories('..')
)

libguesser = static_library(
    'guesser',
    files(
        'gupnp-dlna-profile-guesser.c',
        'gupnp-dlna-profile-guesser-impl.c'
    ),
    dependencies : [glib, gio],
    c_args : [
        '-DG_LOG_DOMAIN="gupnp-dlna-guesser"',
        '-DDATA_DIR="@0@"'.format(shareddir)
    ],
    include_directories: include_directories(
        '..',
        'metadata'
    )
)

libmetadata = static_library(
    'metadata',
    files(
        'gupnp-dlna-metadata-backend.c',
        'metadata/gupnp-dlna-metadata-extractor.c'
    ),
    dependencies : [glib, gio],
    c_args : [
        '-DG_LOG_DOMAIN="gupnp-dlna-metadata"',
        '-DDATA_DIR="@0@"'.format(shareddir)
    ],
    include_directories: include_directories(
        '..',
        'metadata'
    )
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

install_headers(
    'metadata/gupnp-dlna-metadata-extractor.h',
    subdir: join_paths(headers_subdir, 'metadata')
)

libgupnp_dlna = library(
    'gupnp-dlna-2.0',
    files(
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
    ),
    version: library_version,
    link_whole: [
        libloader,
        libguesser,
        libmetadata
    ],
    dependencies: [glib, gio, gmodule],
    include_directories: include_directories('..'),
    install: true
)

gupnp_dlna = declare_dependency(
    link_with : libgupnp_dlna,
    include_directories: include_directories('.')
)

pkg.generate(
    libgupnp_dlna,
    version: meson.project_version(),
    subdirs : 'gupnp-dlna-2.0',
    name: 'gupnp-dlna-2.0',
    description: 'GObject-based DLNA specific UPnP library'
)

subdir('metadata-backends')
