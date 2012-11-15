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

#include "gupnp-dlna-profile-guesser-impl.h"

G_DEFINE_ABSTRACT_TYPE (GUPnPDLNAProfileGuesserImpl,
                        gupnp_dlna_profile_guesser_impl,
                        G_TYPE_OBJECT)

struct _GUPnPDLNAProfileGuesserImplPrivate {
        gpointer placeholder;
};

static void
gupnp_dlna_profile_guesser_impl_class_init
                          (GUPnPDLNAProfileGuesserImplClass *guesser_impl_class)
{
        g_type_class_add_private (guesser_impl_class,
                                  sizeof (GUPnPDLNAProfileGuesserImplPrivate));
}

static void
gupnp_dlna_profile_guesser_impl_init (GUPnPDLNAProfileGuesserImpl *self)
{
        GUPnPDLNAProfileGuesserImplPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE
                                        (self,
                                         GUPNP_TYPE_DLNA_PROFILE_GUESSER_IMPL,
                                         GUPnPDLNAProfileGuesserImplPrivate);

        self->priv = priv;
}

GUPnPDLNAProfile *
gupnp_dlna_profile_guesser_impl_guess_profile
                                     (GUPnPDLNAProfileGuesserImpl *guesser_impl,
                                      GUPnPDLNAInformation        *info,
                                      GList                       *profiles)
{
        GUPnPDLNAVideoInformation *video_info =
                            gupnp_dlna_information_get_video_information (info);
        GUPnPDLNAAudioInformation *audio_info =
                            gupnp_dlna_information_get_audio_information (info);
        GUPnPDLNAImageInformation *image_info =
                            gupnp_dlna_information_get_image_information (info);
        GUPnPDLNAProfile *profile = NULL;
        GUPnPDLNAProfileGuesserImplClass *guesser_impl_class =
                       GUPNP_DLNA_PROFILE_GUESSER_IMPL_GET_CLASS (guesser_impl);

        g_return_val_if_fail
                 (GUPNP_IS_DLNA_PROFILE_GUESSER_IMPL_CLASS (guesser_impl_class),
                  NULL);

        if (image_info) {
                g_return_val_if_fail
                               (guesser_impl_class->guess_image_profile != NULL,
                                NULL);

                profile = guesser_impl_class->guess_image_profile (guesser_impl,
                                                                   info,
                                                                   profiles);
        } else if (video_info) {
                g_return_val_if_fail
                               (guesser_impl_class->guess_video_profile != NULL,
                                NULL);

                profile = guesser_impl_class->guess_video_profile (guesser_impl,
                                                                   info,
                                                                   profiles);
        } else if (audio_info) {
                g_return_val_if_fail
                               (guesser_impl_class->guess_audio_profile != NULL,
                                NULL);

                profile = guesser_impl_class->guess_audio_profile (guesser_impl,
                                                                   info,
                                                                   profiles);
        }

        return profile;
}
