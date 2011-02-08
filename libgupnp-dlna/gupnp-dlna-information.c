/*
 * Copyright (C) 2010 Nokia Corporation.
 *
 * Authors: Arun Raghavan <arun.raghavan@collabora.co.uk>
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "gupnp-dlna-information.h"
#include <gst/gstminiobject.h>

/**
 * SECTION:gupnp-dlna-information
 * @short_description: Object containing metadata information returned by the
 * #GUPnPDLNADiscoverer API
 *
 * The GUPnPDLNAInformation object holds metadata information discovered by the
 * GUPnPDiscoverer API. The DLNA profile name and MIME type have their own
 * fields, and other metadata is held in a GstDiscovererInfo structure.
 * All fields are read-only.
 */

G_DEFINE_TYPE (GUPnPDLNAInformation, gupnp_dlna_information, G_TYPE_OBJECT)

#define GET_PRIVATE(o)                                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                              \
                                      GUPNP_TYPE_DLNA_INFORMATION,      \
                                      GUPnPDLNAInformationPrivate))

typedef struct _GUPnPDLNAInformationPrivate GUPnPDLNAInformationPrivate;

struct _GUPnPDLNAInformationPrivate {
        GstDiscovererInfo *info;
        gchar             *name;
        gchar             *mime;
};

enum {
        PROP_0,
        PROP_DLNA_NAME,
        PROP_DLNA_MIME,
        PROP_DISCOVERER_INFO,
};

static void
gupnp_dlna_information_get_property (GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
        GUPnPDLNAInformation *self = GUPNP_DLNA_INFORMATION (object);
        GUPnPDLNAInformationPrivate *priv = GET_PRIVATE (self);

        switch (property_id) {
                case PROP_DLNA_NAME:
                        g_value_set_string (value, priv->name);

                        break;

                case PROP_DLNA_MIME:
                        g_value_set_string (value, priv->mime);

                        break;

                case PROP_DISCOVERER_INFO:
                        gst_value_set_mini_object (value,
                                                   GST_MINI_OBJECT(priv->info));

                        break;

                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
                                                           property_id,
                                                           pspec);

                        break;
        }
}

static void
gupnp_dlna_information_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
        GUPnPDLNAInformation *self = GUPNP_DLNA_INFORMATION (object);
        GUPnPDLNAInformationPrivate *priv = GET_PRIVATE (self);

        switch (property_id) {
                case PROP_DLNA_NAME:
                        g_free (priv->name);
                        priv->name = g_value_dup_string (value);

                        break;

                case PROP_DLNA_MIME:
                        g_free (priv->mime);
                        priv->mime = g_value_dup_string (value);

                        break;

                case PROP_DISCOVERER_INFO:
                        if (priv->info)
                                gst_discoverer_info_unref (priv->info);
                        priv->info = GST_DISCOVERER_INFO
                                (gst_value_dup_mini_object (value));

                        break;

                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
                                                           property_id,
                                                           pspec);

                        break;
        }
}


static void
gupnp_dlna_information_finalize (GObject *object)
{
        GUPnPDLNAInformation *self = GUPNP_DLNA_INFORMATION (object);
        GUPnPDLNAInformationPrivate *priv = GET_PRIVATE (self);

        g_free (priv->name);
        g_free (priv->mime);
        if (priv->info)
                gst_discoverer_info_unref (priv->info);

        G_OBJECT_CLASS (gupnp_dlna_information_parent_class)->finalize (object);
}

static void
gupnp_dlna_information_class_init (GUPnPDLNAInformationClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
        GParamSpec *pspec;

        g_type_class_add_private (klass, sizeof (GUPnPDLNAInformationPrivate));

        object_class->get_property = gupnp_dlna_information_get_property;
        object_class->set_property = gupnp_dlna_information_set_property;
        object_class->finalize = gupnp_dlna_information_finalize;

        pspec = g_param_spec_string ("name",
                                     "DLNA profile name",
                                     "The name of the DLNA profile "
                                     "corresponding to the strream",
                                     NULL,
                                     G_PARAM_READWRITE |
                                     G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class, PROP_DLNA_NAME, pspec);

        pspec = g_param_spec_string ("mime",
                                     "DLNA profile MIME type corresponding "
                                     "to the stream",
                                     "The DLNA MIME type of the stream",
                                     NULL,
                                     G_PARAM_READWRITE |
                                     G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class, PROP_DLNA_MIME, pspec);

        pspec = gst_param_spec_mini_object ("info",
                                            "Stream metadata",
                                            "Metadata of the stream in a "
                                            "GstDiscovererInfo structure",
                                            GST_TYPE_DISCOVERER_INFO,
                                            G_PARAM_READWRITE |
                                            G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class,
                                         PROP_DISCOVERER_INFO,
                                         pspec);
}

static void
gupnp_dlna_information_init (GUPnPDLNAInformation *self)
{
        GUPnPDLNAInformationPrivate *priv = GET_PRIVATE (self);

        priv->name = NULL;
        priv->mime = NULL;
        priv->info = NULL;
}

/**
 * gupnp_dlna_information_new:
 * @name: DLNA media profile name corresponding to the media
 * @mime: DLNA MIME type for the media
 * @info: #GstDiscovererInfo type with additional metadata about the
 *        stream
 *
 * Creates a new #GUPnPDLNAInformation object with the given properties.
 *
 * Returns: A newly created #GUPnPDLNAInformation object.
 */
GUPnPDLNAInformation*
gupnp_dlna_information_new (gchar             *name,
                            gchar             *mime,
                            GstDiscovererInfo *info)
{
        return g_object_new (GUPNP_TYPE_DLNA_INFORMATION,
                             "name", name,
                             "mime", mime,
                             "info", info,
                             NULL);
}

/**
 * gupnp_dlna_information_get_name:
 * @self: The #GUPnPDLNAInformation object
 *
 * Returns: the DLNA profile name of the stream represented by @self. Do not
 *          free this string.
 */
const gchar *
gupnp_dlna_information_get_name (GUPnPDLNAInformation *self)
{
        GUPnPDLNAInformationPrivate *priv = GET_PRIVATE (self);

        return priv->name;
}

/**
 * gupnp_dlna_information_get_mime:
 * @self: The #GUPnPDLNAInformation object
 *
 * Returns: the DLNA MIME type of the stream represented by @self. Do not
 *          free this string.
 */
const gchar *
gupnp_dlna_information_get_mime (GUPnPDLNAInformation *self)
{
        GUPnPDLNAInformationPrivate *priv = GET_PRIVATE (self);

        return priv->mime;
}

/**finalize
 * gupnp_dlna_information_get_info:
 * @self: The #GUPnPDLNAInformation object
 *
 * Returns: additional stream metadata for @self in the form of a
 *          #GstDiscovererInfo structure. Do not free this structure.
 */
const GstDiscovererInfo *
gupnp_dlna_information_get_info (GUPnPDLNAInformation *self)
{
        GUPnPDLNAInformationPrivate *priv = GET_PRIVATE (self);

        return priv->info;
}
