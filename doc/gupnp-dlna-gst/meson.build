gnome.gtkdoc('gupnp-dlna-gst',
             main_xml : 'gupnp-dlna-gst-docs.sgml',
             src_dir : ['libgupnp-dlna/metadata-backends/gstreamer'],
             ignore_headers : [
                 'gupnp-dlna-gst-container-information.h',
                 'gupnp-dlna-gst-video-information.h',
                 'gupnp-dlna-gst-metadata-extractor.h',
                 'gupnp-dlna-gst-audio-information.h',
                 'gupnp-dlna-gst-info-utils.h',
                 'gupnp-dlna-gst-information.h',
                 'gupnp-dlna-gst-image-information.h',
             ],
             fixxref_args : [
                 '--extra-dir', join_paths(meson.build_root(), 'doc/gupnp-dlna/html'),
             ],
             content_files : version_xml,
             dependencies : [
                 libgupnp_dlna,
                 libgupnp_dlna_gst
             ],
             install : true)
