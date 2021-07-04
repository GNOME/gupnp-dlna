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

#ifndef __GUPNP_DLNA_GST_INFORMATION_H__
#define __GUPNP_DLNA_GST_INFORMATION_H__

#include <glib-object.h>
#include <gst/pbutils/pbutils.h>
#include <libgupnp-dlna/gupnp-dlna-information.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (GUPnPDLNAGstInformation,
                      gupnp_dlna_gst_information,
                      GUPNP_DLNA,
                      GST_INFORMATION,
                      GUPnPDLNAInformation)
#define GUPNP_TYPE_DLNA_GST_INFORMATION (gupnp_dlna_gst_information_get_type())

// Backwards-compatible defines
/**
 * GUPNP_IS_GST_DLNA_INFORMATION: (skip)
 */
#define GUPNP_IS_GST_DLNA_INFORMATION GUPNP_DLNA_IS_GST_IMAGE_INFORMATION
/**
 * GUPNP_IS_GST_DLNA_INFORMATION_CLASS: (skip)
 */
#define GUPNP_IS_GST_DLNA_INFORMATION_CLASS GUPNP_DLNA_IS_GST_IMAGE_INFORMATION_CLASS

struct _GUPnPDLNAGstInformationClass {
        GUPnPDLNAInformationClass parent_class;
};

GUPnPDLNAGstInformation *
gupnp_dlna_gst_information_new_from_discoverer_info (const gchar       *uri,
                                                     GstDiscovererInfo *info);

GUPnPDLNAGstInformation *
gupnp_dlna_gst_information_new_empty_with_uri (const gchar *uri);

G_END_DECLS

#endif /* __GUPNP_DLNA_GST_INFORMATION_H__ */
