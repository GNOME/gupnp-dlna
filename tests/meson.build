test_file_config = configuration_data()
test_file_config.set('abs_top_builddir', meson.project_build_root())
discoverer_test = configure_file(
    input: 'test-discoverer.sh.in',
    output: 'test-discoverer.sh',
    configuration: test_file_config
)

test(
    'discoverer',
    discoverer_test,
    env : [
        'MEDIA_DIR=' + media_dir,
        'FILE_LIST=' + join_paths(media_dir, 'media-list.txt'),
        'GUPNP_DLNA_PROFILE_DIR=' + dlna_profile_dir,
        'GUPNP_DLNA_METADATA_BACKEND_DIR=' + dlna_backend_dir
    ]
)

test(
    'test-sets',
    executable(
        'sets',
        'sets.c',
        dependencies : [glib, gio, gobject, gupnp_dlna],
    )
)

