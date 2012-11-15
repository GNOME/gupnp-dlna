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

#include "gupnp-dlna-profile.h"

/**
 * SECTION:gupnp-dlna-profile
 * @short_description: Object representing a DLNA profile
 *
 * The #GUPnPDLNAProfileGuesser object provides a few APIs that return
 * #GUPnPDLNAProfile objects. These represent a single DLNA
 * profile. Each #GUPnPDLNAProfile has a name (the name of the DLNA
 * profile) and the corresponding MIME type.
 *
 * Public representation of the various audio/container/image/video
 * restrictions specified for that DLNA profile is to be added.
 */
G_DEFINE_TYPE (GUPnPDLNAProfile, gupnp_dlna_profile, G_TYPE_OBJECT)

struct _GUPnPDLNAProfilePrivate {
        gchar    *name;
        gchar    *mime;
        gboolean  extended;
};

enum {
        PROP_0,
        PROP_DLNA_NAME,
        PROP_DLNA_MIME,
        PROP_DLNA_EXTENDED,
};

static void
gupnp_dlna_profile_get_property (GObject    *object,
                                 guint       property_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
        GUPnPDLNAProfile *self = GUPNP_DLNA_PROFILE (object);
        GUPnPDLNAProfilePrivate *priv = self->priv;

        switch (property_id) {
                case PROP_DLNA_NAME:
                        g_value_set_string (value, priv->name);
                        break;

                case PROP_DLNA_MIME:
                        g_value_set_string (value, priv->mime);
                        break;

                case PROP_DLNA_EXTENDED:
                        g_value_set_boolean (value, priv->extended);
                        break;

                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
                                                           property_id,
                                                           pspec);
                        break;
        }
}

static void
gupnp_dlna_profile_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
        GUPnPDLNAProfile *self = GUPNP_DLNA_PROFILE (object);
        GUPnPDLNAProfilePrivate *priv = self->priv;

        switch (property_id) {
        case PROP_DLNA_NAME:
                g_free (priv->name);
                priv->name = g_value_dup_string (value);
                break;

        case PROP_DLNA_MIME:
                g_free (priv->mime);
                priv->mime = g_value_dup_string (value);
                break;

        case PROP_DLNA_EXTENDED:
                priv->extended = g_value_get_boolean (value);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_dlna_profile_finalize (GObject *object)
{
        GUPnPDLNAProfile *self = GUPNP_DLNA_PROFILE (object);
        GUPnPDLNAProfilePrivate *priv = self->priv;

        g_free (priv->name);
        g_free (priv->mime);

        G_OBJECT_CLASS (gupnp_dlna_profile_parent_class)->finalize (object);
}

static void
gupnp_dlna_profile_class_init (GUPnPDLNAProfileClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
        GParamSpec *pspec;

        object_class->get_property = gupnp_dlna_profile_get_property;
        object_class->set_property = gupnp_dlna_profile_set_property;
        object_class->finalize = gupnp_dlna_profile_finalize;

        /**
         * GUPnPDLNAProfile:name:
         *
         * Name of the DLNA profile.
         */
        pspec = g_param_spec_string ("name",
                                     "DLNA profile name",
                                     "The name of the DLNA profile",
                                     NULL,
                                     G_PARAM_READWRITE |
                                     G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class, PROP_DLNA_NAME, pspec);

        /**
         * GUPnPDLNAProfile:mime:
         *
         * MIME type of the DLNA profile.
         */
        pspec = g_param_spec_string ("mime",
                                     "DLNA profile MIME type",
                                     "The MIME type of the DLNA profile",
                                     NULL,
                                     G_PARAM_READWRITE |
                                     G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class, PROP_DLNA_MIME, pspec);

        /**
         * GUPnPDLNAProfile:extended:
         *
         * Whether the DLNA profile is not a part of DLNA specification.
         */

        pspec = g_param_spec_boolean ("extended",
                                      "Extended mode property",
                                      "Indicates that this profile is not "
                                      "part of the DLNA specification",
                                      FALSE,
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class,
                                         PROP_DLNA_EXTENDED,
                                         pspec);

        g_type_class_add_private (klass, sizeof (GUPnPDLNAProfilePrivate));
}

static void
gupnp_dlna_profile_init (GUPnPDLNAProfile *self)
{
        GUPnPDLNAProfilePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                                  GUPNP_TYPE_DLNA_PROFILE,
                                                  GUPnPDLNAProfilePrivate);

        priv->name = NULL;
        priv->mime = NULL;
        priv->extended = FALSE;
        self->priv = priv;
}

/**
 * gupnp_dlna_profile_get_name:
 * @profile: The #GUPnPDLNAProfile object.
 *
 * Returns: The name of the DLNA profile represented by @profile.
 */
const gchar *
gupnp_dlna_profile_get_name (GUPnPDLNAProfile *profile)
{
        g_return_val_if_fail (GUPNP_IS_DLNA_PROFILE (profile), NULL);

        return profile->priv->name;
}

/**
 * gupnp_dlna_profile_get_mime:
 * @profile: The #GUPnPDLNAProfile object.
 *
 * Returns: The DLNA MIME type of the DLNA profile represented by @profile.
 */
const gchar *
gupnp_dlna_profile_get_mime (GUPnPDLNAProfile *profile)
{
        g_return_val_if_fail (GUPNP_IS_DLNA_PROFILE (profile), NULL);

        return profile->priv->mime;
}

/**
 * gupnp_dlna_profile_get_extended:
 * @profile: The #GUPnPDLNAProfile object.
 *
 * Returns: %TRUE if @profile is extended one and %FALSE otherwise.
 */
gboolean
gupnp_dlna_profile_get_extended (GUPnPDLNAProfile *profile)
{
        g_return_val_if_fail (GUPNP_IS_DLNA_PROFILE (profile), FALSE);

        return profile->priv->extended;
}
