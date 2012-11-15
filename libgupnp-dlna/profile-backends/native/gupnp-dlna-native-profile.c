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

#include "gupnp-dlna-native-profile.h"
#include "gupnp-dlna-native-utils.h"

G_DEFINE_TYPE (GUPnPDLNANativeProfile,
               gupnp_dlna_native_profile,
               GUPNP_TYPE_DLNA_PROFILE)

struct _GUPnPDLNANativeProfilePrivate {
        GList *audio_restrictions;
        GList *container_restrictions;
        GList *image_restrictions;
        GList *video_restrictions;
};

enum {
        PROP_0,
        PROP_AUDIO_RESTRICTIONS,
        PROP_CONTAINER_RESTRICTIONS,
        PROP_IMAGE_RESTRICTIONS,
        PROP_VIDEO_RESTRICTIONS
};

static void
gupnp_dlna_native_profile_get_property (GObject    *object,
                                        guint       property_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
        GUPnPDLNANativeProfile *self = GUPNP_DLNA_NATIVE_PROFILE (object);
        GUPnPDLNANativeProfilePrivate *priv = self->priv;

        switch (property_id) {
                case PROP_AUDIO_RESTRICTIONS:
                        g_value_set_pointer (value, priv->audio_restrictions);

                        break;
                case PROP_CONTAINER_RESTRICTIONS:
                        g_value_set_pointer
                                        (value, priv->container_restrictions);

                        break;
                case PROP_IMAGE_RESTRICTIONS:
                        g_value_set_pointer (value, priv->image_restrictions);

                        break;
                case PROP_VIDEO_RESTRICTIONS:
                        g_value_set_pointer (value, priv->video_restrictions);

                        break;
                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
                                                           property_id,
                                                           pspec);

                        break;
        }
}

static void
gupnp_dlna_native_profile_set_property (GObject      *object,
                                        guint         property_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
        GUPnPDLNANativeProfile *self = GUPNP_DLNA_NATIVE_PROFILE (object);
        GUPnPDLNANativeProfilePrivate *priv = self->priv;

        switch (property_id) {
                case PROP_AUDIO_RESTRICTIONS:
                        gupnp_dlna_native_utils_free_restrictions
                                        (priv->audio_restrictions);
                        priv->audio_restrictions = g_value_get_pointer (value);

                        break;
                case PROP_CONTAINER_RESTRICTIONS:
                        gupnp_dlna_native_utils_free_restrictions
                                        (priv->container_restrictions);
                        priv->container_restrictions =
                                        g_value_get_pointer (value);

                        break;
                case PROP_IMAGE_RESTRICTIONS:
                        gupnp_dlna_native_utils_free_restrictions
                                        (priv->image_restrictions);
                        priv->image_restrictions = g_value_get_pointer (value);

                        break;
                case PROP_VIDEO_RESTRICTIONS:
                        gupnp_dlna_native_utils_free_restrictions
                                        (priv->video_restrictions);
                        priv->video_restrictions = g_value_get_pointer (value);

                        break;
                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
                                                           property_id,
                                                           pspec);

                        break;
        }
}

static void
gupnp_dlna_native_profile_finalize (GObject *object)
{
        GUPnPDLNANativeProfile *self = GUPNP_DLNA_NATIVE_PROFILE (object);
        GUPnPDLNANativeProfilePrivate *priv = self->priv;
        GObjectClass *parent_class =
                        G_OBJECT_CLASS (gupnp_dlna_native_profile_parent_class);

        gupnp_dlna_native_utils_free_restrictions (priv->audio_restrictions);
        gupnp_dlna_native_utils_free_restrictions
                                        (priv->container_restrictions);
        gupnp_dlna_native_utils_free_restrictions (priv->image_restrictions);
        gupnp_dlna_native_utils_free_restrictions (priv->video_restrictions);

        parent_class->finalize (object);
}

static void
gupnp_dlna_native_profile_class_init
                             (GUPnPDLNANativeProfileClass *native_profile_class)
{
        GObjectClass *object_class = G_OBJECT_CLASS (native_profile_class);
        GParamSpec *pspec;

        object_class->get_property = gupnp_dlna_native_profile_get_property;
        object_class->set_property = gupnp_dlna_native_profile_set_property;
        object_class->finalize = gupnp_dlna_native_profile_finalize;

        pspec = g_param_spec_pointer ("audio-restrictions",
                                      "Audio restrictions",
                                      "Audio restrictions for the DLNA Profile",
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class,
                                         PROP_AUDIO_RESTRICTIONS,
                                         pspec);
        pspec = g_param_spec_pointer ("container-restrictions",
                                      "Container restrictions",
                                      "Container restrictions for the DLNA "
                                      "Profile",
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class,
                                         PROP_CONTAINER_RESTRICTIONS,
                                         pspec);
        pspec = g_param_spec_pointer ("image-restrictions",
                                      "Image restrictions",
                                      "Image restrictions for the DLNA Profile",
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class,
                                         PROP_IMAGE_RESTRICTIONS,
                                         pspec);
        pspec = g_param_spec_pointer ("video-restrictions",
                                      "Video restrictions",
                                      "Video restrictions for the DLNA Profile",
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class,
                                         PROP_VIDEO_RESTRICTIONS,
                                         pspec);

        g_type_class_add_private (native_profile_class,
                                  sizeof (GUPnPDLNANativeProfilePrivate));
}

static void
gupnp_dlna_native_profile_init (GUPnPDLNANativeProfile *self)
{
        GUPnPDLNANativeProfilePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE
                                        (self,
                                         GUPNP_TYPE_DLNA_NATIVE_PROFILE,
                                         GUPnPDLNANativeProfilePrivate);

        priv->audio_restrictions = NULL;
        priv->container_restrictions = NULL;
        priv->image_restrictions = NULL;
        priv->video_restrictions = NULL;
        self->priv = priv;
}

GList *
gupnp_dlna_native_profile_get_audio_restrictions (GUPnPDLNANativeProfile *self)
{
        g_return_val_if_fail (GUPNP_IS_DLNA_NATIVE_PROFILE (self), NULL);

        return self->priv->audio_restrictions;
}

GList *
gupnp_dlna_native_profile_get_container_restrictions
                                        (GUPnPDLNANativeProfile *self)
{
        g_return_val_if_fail (GUPNP_IS_DLNA_NATIVE_PROFILE (self), NULL);

        return self->priv->container_restrictions;
}

GList *
gupnp_dlna_native_profile_get_image_restrictions (GUPnPDLNANativeProfile *self)
{
        g_return_val_if_fail (GUPNP_IS_DLNA_NATIVE_PROFILE (self), NULL);

        return self->priv->image_restrictions;
}

GList *
gupnp_dlna_native_profile_get_video_restrictions (GUPnPDLNANativeProfile *self)
{
        g_return_val_if_fail (GUPNP_IS_DLNA_NATIVE_PROFILE (self), NULL);

        return self->priv->video_restrictions;
}

GUPnPDLNANativeProfile *
gupnp_dlna_native_profile_new (const gchar *name,
                               const gchar *mime,
                               GList       *audio_restrictions,
                               GList       *container_restrictions,
                               GList       *image_restrictions,
                               GList       *video_restrictions,
                               gboolean     extended)
{
        return GUPNP_DLNA_NATIVE_PROFILE
                             (g_object_new
                              (GUPNP_TYPE_DLNA_NATIVE_PROFILE,
                               "name", name,
                               "mime", mime,
                               "audio-restrictions", audio_restrictions,
                               "container-restrictions", container_restrictions,
                               "image-restrictions", image_restrictions,
                               "video-restrictions", video_restrictions,
                               "extended", extended,
                               NULL));
}
