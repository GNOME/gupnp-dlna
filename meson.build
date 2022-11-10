project(
    'gupnp-dlna',
    'c',
    version: '0.13.0'
)

pkg = import('pkgconfig')
gnome = import('gnome')

shareddir = join_paths(get_option('prefix'), get_option('datadir'), 'gupnp-dlna-2.0')
metadata_backend_dir = join_paths(get_option('prefix'), get_option('libdir'), 'gupnp-dlna')
library_version = '4.0.0'

config = configuration_data()
config.set_quoted('GUPNP_DLNA_DEFAULT_METADATA_BACKEND', get_option('default_backend'))
config.set_quoted('GUPNP_DLNA_DEFAULT_METADATA_BACKEND_DIR', metadata_backend_dir)


# Generate config.h
subdir('internal')
add_project_arguments('-DHAVE_CONFIG_H=1', language : 'c')
toplevel_incdir = include_directories('.')

glib = dependency('glib-2.0')
gobject = dependency('gobject-2.0')
gio = dependency('gio-2.0')
gmodule = dependency('gmodule-2.0')
xml = dependency('libxml-2.0')

gstreamer = dependency('gstreamer-1.0', required: get_option('gstreamer_backend'), disabler : true)
gstreamer_pbu = dependency('gstreamer-pbutils-1.0', required: get_option('gstreamer_backend'), disabler : true)

# default is relative to the source dir; if test_media is absolute, join_paths() will return that instead
media_dir = join_paths(meson.current_source_dir(), get_option('test_media'))
dlna_profile_dir = join_paths(meson.current_source_dir(), 'data')

install_data(
    files(
        'data/dlna-profiles.rng',
        'data/aac.xml',
        'data/ac3.xml',
        'data/amr.xml',
        'data/avc.xml',
        'data/common.xml',
        'data/jpeg.xml',
        'data/lpcm.xml',
        'data/mp3.xml',
        'data/mpeg1.xml',
        'data/mpeg4.xml',
        'data/mpeg-common.xml',
        'data/mpeg-ps.xml',
        'data/mpeg-ts.xml',
        'data/png.xml',
        'data/wma.xml'
    ),
    install_dir : join_paths(shareddir, 'dlna-profiles')
)

subdir('libgupnp-dlna')
subdir('tools')
if get_option('introspection')
    if get_option('vapi')
        subdir('vala')
    endif
endif
subdir('doc')
subdir('tests')
