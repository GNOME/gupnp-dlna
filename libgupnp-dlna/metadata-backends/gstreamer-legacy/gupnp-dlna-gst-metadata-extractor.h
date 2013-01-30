/*
 * Copyright (C) 2010 Nokia Corporation.
 * Copyright (C) 2012,2013 Intel Corporation.
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
#include "gupnp-dlna-metadata-extractor.h"
#include "gupnp-dlna-information.h"

G_BEGIN_DECLS

#define GUPNP_TYPE_DLNA_GST_METADATA_EXTRACTOR \
        (gupnp_dlna_gst_metadata_extractor_get_type())

#define GUPNP_DLNA_GST_METADATA_EXTRACTOR(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                     GUPNP_TYPE_DLNA_GST_METADATA_EXTRACTOR, \
                                     GUPnPDLNAGstMetadataExtractor))

#define GUPNP_DLNA_GST_METADATA_EXTRACTOR_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                  GUPNP_TYPE_DLNA_GST_METADATA_EXTRACTOR, \
                                  GUPnPDLNAGstMetadataExtractorClass))

#define GUPNP_IS_DLNA_GST_METADATA_EXTRACTOR(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                     GUPNP_TYPE_DLNA_GST_METADATA_EXTRACTOR))

#define GUPNP_IS_DLNA_GST_METADATA_EXTRACTOR_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                  GUPNP_TYPE_DLNA_GST_METADATA_EXTRACTOR))

#define GUPNP_DLNA_GST_METADATA_EXTRACTOR_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                    GUPNP_TYPE_DLNA_GST_METADATA_EXTRACTOR, \
                                    GUPnPDLNAGstMetadataExtractorClass))

typedef struct _GUPnPDLNAGstMetadataExtractorPrivate
                GUPnPDLNAGstMetadataExtractorPrivate;

/**
 * GUPnPDLNAGstMetadataExtractor:
 *
 * The top-level object used to for metadata extraction.
 */
typedef struct {
        GUPnPDLNAMetadataExtractor parent;

        GUPnPDLNAGstMetadataExtractorPrivate *priv;
} GUPnPDLNAGstMetadataExtractor;

typedef struct {
        GUPnPDLNAMetadataExtractorClass parent_class;
} GUPnPDLNAGstMetadataExtractorClass;

GType
gupnp_dlna_gst_metadata_extractor_get_type (void);

GUPnPDLNAGstMetadataExtractor *
gupnp_dlna_gst_metadata_extractor_new (void);

G_END_DECLS

#endif /* _GUPNP_DLNA_GST_METADATA_EXTRACTOR */
