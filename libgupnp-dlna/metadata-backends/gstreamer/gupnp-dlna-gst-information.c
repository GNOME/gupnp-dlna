/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Krzesimir Nowak <krnowak@openismus.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H_ */
#include "gupnp-dlna-gst-information.h"
#include "gupnp-dlna-gst-info-utils.h"
#include "gupnp-dlna-gst-audio-information.h"
#include "gupnp-dlna-gst-container-information.h"
#include "gupnp-dlna-gst-image-information.h"
#include "gupnp-dlna-gst-video-information.h"

struct _GUPnPDLNAGstInformationPrivate {
        GstDiscovererInfo* info;
};

typedef struct _GUPnPDLNAGstInformationPrivate GUPnPDLNAGstInformationPrivate;

struct _GUPnPDLNAGstInformation {
        GUPnPDLNAInformation parent;
};

G_DEFINE_TYPE_WITH_PRIVATE (GUPnPDLNAGstInformation,
                            gupnp_dlna_gst_information,
                            GUPNP_TYPE_DLNA_INFORMATION)

enum {
        PROP_0,

        PROP_INFO
};

static void
gupnp_dlna_gst_information_dispose (GObject *object)
{
        GUPnPDLNAGstInformation *info = GUPNP_DLNA_GST_INFORMATION (object);
        GUPnPDLNAGstInformationPrivate *priv =
                gupnp_dlna_gst_information_get_instance_private (info);

        g_clear_pointer (&priv->info, gupnp_dlna_gst_discoverer_info_unref);
        G_OBJECT_CLASS
                    (gupnp_dlna_gst_information_parent_class)->dispose (object);
}

static void
gupnp_dlna_gst_information_set_property (GObject      *object,
                                         guint         property_id,
                                         const GValue *value,
                                         GParamSpec   *pspec)
{
        GUPnPDLNAGstInformation *info = GUPNP_DLNA_GST_INFORMATION (object);
        GUPnPDLNAGstInformationPrivate *priv =
                gupnp_dlna_gst_information_get_instance_private (info);

        switch (property_id) {
        case PROP_INFO:
                g_clear_pointer (&priv->info,
                                 gupnp_dlna_gst_discoverer_info_unref);
                priv->info =
                        GST_DISCOVERER_INFO (g_value_dup_object (value));

                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);

                break;
        }
}

static void
gupnp_dlna_gst_information_get_property (GObject    *object,
                                         guint       property_id,
                                         GValue     *value,
                                         GParamSpec *pspec)
{
        GUPnPDLNAGstInformation *info = GUPNP_DLNA_GST_INFORMATION (object);
        GUPnPDLNAGstInformationPrivate *priv =
                gupnp_dlna_gst_information_get_instance_private (info);

        switch (property_id) {
        case PROP_INFO:
                g_value_set_object (value, G_OBJECT (priv->info));

                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);

                break;
        }
}

GUPnPDLNAAudioInformation *
backend_get_audio_information (GUPnPDLNAInformation *self)
{
        if (!GUPNP_DLNA_IS_GST_INFORMATION (self))
                return NULL;

        GUPnPDLNAGstInformation *info = GUPNP_DLNA_GST_INFORMATION (self);

        GUPnPDLNAGstInformationPrivate *priv =
                gupnp_dlna_gst_information_get_instance_private (info);

        return GUPNP_DLNA_AUDIO_INFORMATION (
                gupnp_dlna_gst_audio_information_new_from_discoverer_info (
                        priv->info));
}

GUPnPDLNAContainerInformation *
backend_get_container_information (GUPnPDLNAInformation *self)
{
        if (!GUPNP_DLNA_IS_GST_INFORMATION (self))
                return NULL;

        GUPnPDLNAGstInformation *info = GUPNP_DLNA_GST_INFORMATION (self);
        GUPnPDLNAGstInformationPrivate *priv =
                gupnp_dlna_gst_information_get_instance_private (info);

        return GUPNP_DLNA_CONTAINER_INFORMATION
                (gupnp_dlna_gst_container_information_new_from_discoverer_info
                                        (priv->info));
}

GUPnPDLNAImageInformation *
backend_get_image_information (GUPnPDLNAInformation *self)
{
        GUPnPDLNAGstInformation *info = GUPNP_DLNA_GST_INFORMATION (self);
        GUPnPDLNAGstInformationPrivate *priv =
                gupnp_dlna_gst_information_get_instance_private (info);

        return GUPNP_DLNA_IMAGE_INFORMATION (
                gupnp_dlna_gst_image_information_new_from_discoverer_info (
                        priv->info));
}

GUPnPDLNAVideoInformation *
backend_get_video_information (GUPnPDLNAInformation *self)
{
        GUPnPDLNAGstInformation *info = GUPNP_DLNA_GST_INFORMATION (self);
        GUPnPDLNAGstInformationPrivate *priv =
                gupnp_dlna_gst_information_get_instance_private (info);

        return GUPNP_DLNA_VIDEO_INFORMATION (
                gupnp_dlna_gst_video_information_new_from_discoverer_info (
                        priv->info));
}

static void
gupnp_dlna_gst_information_class_init
                                  (GUPnPDLNAGstInformationClass *gst_info_class)
{
        GObjectClass *object_class = G_OBJECT_CLASS (gst_info_class);
        GUPnPDLNAInformationClass *info_class =
                                  GUPNP_DLNA_INFORMATION_CLASS (gst_info_class);
        GParamSpec *pspec;

        object_class->dispose = gupnp_dlna_gst_information_dispose;
        object_class->set_property = gupnp_dlna_gst_information_set_property;
        object_class->get_property = gupnp_dlna_gst_information_get_property;
        info_class->get_audio_information = backend_get_audio_information;
        info_class->get_container_information =
                                        backend_get_container_information;
        info_class->get_image_information = backend_get_image_information;
        info_class->get_video_information = backend_get_video_information;

        pspec = g_param_spec_object ("info",
                                     "info",
                                     "discoverer info",
                                     GST_TYPE_DISCOVERER_INFO,
                                     G_PARAM_READWRITE |
                                     G_PARAM_CONSTRUCT);
        g_object_class_install_property (object_class,
                                         PROP_INFO,
                                         pspec);
}

static void
gupnp_dlna_gst_information_init (GUPnPDLNAGstInformation *self)
{
}

GUPnPDLNAGstInformation *
gupnp_dlna_gst_information_new_from_discoverer_info (const gchar       *uri,
                                                     GstDiscovererInfo *info)
{
        return GUPNP_DLNA_GST_INFORMATION
                                 (g_object_new (GUPNP_TYPE_DLNA_GST_INFORMATION,
                                                "uri", uri,
                                                "info", info,
                                                NULL));
}

GUPnPDLNAGstInformation *
gupnp_dlna_gst_information_new_empty_with_uri (const gchar *uri)
{
        return GUPNP_DLNA_GST_INFORMATION
                                 (g_object_new (GUPNP_TYPE_DLNA_GST_INFORMATION,
                                                "uri", uri,
                                                NULL));
}
