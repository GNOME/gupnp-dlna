/*
 * Copyright (C) 2010 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Arun Raghavan <arun.raghavan@collabora.co.uk>
 *          Krzesimir Nowak <krnowak@openismus.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GUPNP_DLNA_METADATA_EXTRACTOR_H__
#define __GUPNP_DLNA_METADATA_EXTRACTOR_H__

#include <glib-object.h>
#include <libgupnp-dlna/gupnp-dlna-information.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE (GUPnPDLNAMetadataExtractor,
                          gupnp_dlna_metadata_extractor,
                          GUPNP_DLNA,
                          METADATA_EXTRACTOR,
                          GObject)

#define GUPNP_TYPE_DLNA_METADATA_EXTRACTOR \
        (gupnp_dlna_metadata_extractor_get_type())

// Backwards-compatible defines
/**
 * GUPNP_IS_DLNA_METADATA_EXTRACTOR: (skip)
 */
#define GUPNP_IS_DLNA_METADATA_EXTRACTOR GUPNP_DLNA_IS_METADATA_EXTRACTOR

/**
 * GUPNP_IS_DLNA_METADATA_EXTRACTOR_CLASS: (skip)
 */
#define GUPNP_IS_DLNA_METADATA_EXTRACTOR_CLASS GUPNP_DLNA_IS_METADATA_EXTRACTOR_CLASS


/**
 * GUPnPDLNAMetadataExtractorClass:
 * @parent_class: Parent class.
 * @extract_async: This is called by #GUPnPDLNAProfileGuesser to get a
 * information about media file asynchronously.
 * @extract_sync: This is called by #GUPnPDLNAProfileGuesser to get a
 * information about media file synchronously.
 * @_reserved: Padding. Ignore it.
 */
struct _GUPnPDLNAMetadataExtractorClass {
        GObjectClass parent_class;

        /* virtuals */
        gboolean
        (* extract_async) (GUPnPDLNAMetadataExtractor  *extractor,
                           const gchar                 *uri,
                           guint                        timeout_in_ms,
                           GError                     **error);

        GUPnPDLNAInformation *
        (* extract_sync) (GUPnPDLNAMetadataExtractor  *extractor,
                          const gchar                 *uri,
                          guint                        timeout_in_ms,
                          GError                     **error);

        gpointer _reserved[12];
};

gboolean
gupnp_dlna_metadata_extractor_extract_async
                                    (GUPnPDLNAMetadataExtractor  *extractor,
                                     const gchar                 *uri,
                                     guint                        timeout_in_ms,
                                     GError                     **error);

GUPnPDLNAInformation *
gupnp_dlna_metadata_extractor_extract_sync
                                    (GUPnPDLNAMetadataExtractor  *extractor,
                                     const gchar                 *uri,
                                     guint                        timeout_in_ms,
                                     GError                     **error);

void
gupnp_dlna_metadata_extractor_emit_done (GUPnPDLNAMetadataExtractor *extractor,
                                         GUPnPDLNAInformation       *info,
                                         GError                     *error);

G_END_DECLS

#endif /* __GUPNP_DLNA_METADATA_EXTRACTOR_H__ */
