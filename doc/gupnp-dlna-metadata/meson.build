gnome.gtkdoc('gupnp-dlna-metadata',
             main_xml : 'gupnp-dlna-metadata-docs.sgml',
             src_dir : [join_paths(meson.source_root(), 'libgupnp-dlna/metadata')],
             fixxref_args : [
                 '--extra-dir', join_paths(meson.build_root(), 'doc/gupnp-dlna/html'),
             ],
             content_files : version_xml,
             dependencies : libgupnp_dlna,
             install : true)
