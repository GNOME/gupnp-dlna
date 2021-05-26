entities = configuration_data()
entities.set('VERSION', meson.project_version())
version_xml = configure_file(
    input: 'version.xml.in',
    output: 'version.xml',
    configuration: entities
)

if get_option('gtk_doc')
    subdir('gupnp-dlna')
    subdir('gupnp-dlna-gst')
    subdir('gupnp-dlna-metadata')
endif
