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

#ifndef __GUPNP_DLNA_GST_INFORMATION_H__
#define __GUPNP_DLNA_GST_INFORMATION_H__

#include <glib-object.h>
#include <gst/pbutils/pbutils.h>
#include "gupnp-dlna-information.h"

G_BEGIN_DECLS

#define GUPNP_TYPE_DLNA_GST_INFORMATION (gupnp_dlna_gst_information_get_type())

#define GUPNP_DLNA_GST_INFORMATION(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                     GUPNP_TYPE_DLNA_GST_INFORMATION, \
                                     GUPnPDLNAGstInformation))

#define GUPNP_DLNA_GST_INFORMATION_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                  GUPNP_TYPE_DLNA_GST_INFORMATION, \
                                  GUPnPDLNAGstInformationClass))

#define GUPNP_IS_DLNA_GST_INFORMATION(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                     GUPNP_TYPE_DLNA_GST_INFORMATION))

#define GUPNP_IS_DLNA_GST_INFORMATION_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                  GUPNP_TYPE_DLNA_GST_INFORMATION))

#define GUPNP_DLNA_GST_INFORMATION_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                    GUPNP_TYPE_DLNA_GST_INFORMATION, \
                                    GUPnPDLNAGstInformationClass))

typedef struct _GUPnPDLNAGstInformationPrivate GUPnPDLNAGstInformationPrivate;

typedef struct {
        GUPnPDLNAInformation parent;

        GUPnPDLNAGstInformationPrivate *priv;
} GUPnPDLNAGstInformation;

typedef struct {
        GUPnPDLNAInformationClass parent_class;
} GUPnPDLNAGstInformationClass;

GType
gupnp_dlna_gst_information_get_type (void);

GUPnPDLNAGstInformation *
gupnp_dlna_gst_information_new_from_discoverer_info (const gchar       *uri,
                                                     GstDiscovererInfo *info);

GUPnPDLNAGstInformation *
gupnp_dlna_gst_information_new_empty_with_uri (const gchar *uri);

G_END_DECLS

#endif /* __GUPNP_DLNA_GST_INFORMATION_H__ */
