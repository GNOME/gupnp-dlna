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

#ifndef _GUPNP_DLNA_GST_METADATA_EXTRACTOR
#define _GUPNP_DLNA_GST_METADATA_EXTRACTOR

#include <glib-object.h>
#include <libgupnp-dlna/metadata/gupnp-dlna-metadata-extractor.h>
#include <libgupnp-dlna/gupnp-dlna-information.h>

G_BEGIN_DECLS

#define GUPNP_TYPE_DLNA_GST_METADATA_EXTRACTOR \
        (gupnp_dlna_gst_metadata_extractor_get_type())

G_DECLARE_FINAL_TYPE (GUPnPDLNAGstMetadataExtractor,
                      gupnp_dlna_gst_metadata_extractor,
                      GUPNP_DLNA,
                      GST_METADATA_EXTRACTOR,
                      GUPnPDLNAMetadataExtractor)


struct _GUPnPDLNAGstMetadataExtractorClass {
        GUPnPDLNAMetadataExtractorClass parent_class;
};

GUPnPDLNAGstMetadataExtractor *
gupnp_dlna_gst_metadata_extractor_new (void);

G_END_DECLS

#endif /* _GUPNP_DLNA_GST_METADATA_EXTRACTOR */
