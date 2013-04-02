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

#ifndef __GUPNP_DLNA_INFORMATION_H__
#define __GUPNP_DLNA_INFORMATION_H__

#include <glib-object.h>
#include <libgupnp-dlna/gupnp-dlna-audio-information.h>
#include <libgupnp-dlna/gupnp-dlna-container-information.h>
#include <libgupnp-dlna/gupnp-dlna-image-information.h>
#include <libgupnp-dlna/gupnp-dlna-video-information.h>

G_BEGIN_DECLS

#define GUPNP_TYPE_DLNA_INFORMATION (gupnp_dlna_information_get_type())

#define GUPNP_DLNA_INFORMATION(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                     GUPNP_TYPE_DLNA_INFORMATION, \
                                     GUPnPDLNAInformation))

#define GUPNP_DLNA_INFORMATION_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                  GUPNP_TYPE_DLNA_INFORMATION, \
                                  GUPnPDLNAInformationClass))

#define GUPNP_IS_DLNA_INFORMATION(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                     GUPNP_TYPE_DLNA_INFORMATION))

#define GUPNP_IS_DLNA_INFORMATION_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                  GUPNP_TYPE_DLNA_INFORMATION))

#define GUPNP_DLNA_INFORMATION_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                    GUPNP_TYPE_DLNA_INFORMATION, \
                                    GUPnPDLNAInformationClass))

typedef struct _GUPnPDLNAInformationPrivate GUPnPDLNAInformationPrivate;

typedef struct {
        GObject parent;

        GUPnPDLNAInformationPrivate *priv;
} GUPnPDLNAInformation;

/**
 * GUPnPDLNAInformationClass:
 * @parent_class: Parent class.
 * @get_audio_information: This is called by #GUPnPDLNAProfileGuesser
 * to get an audio information.
 * @get_container_information: This is called by
 * #GUPnPDLNAProfileGuesser to get a container information.
 * @get_image_information: This is called by #GUPnPDLNAProfileGuesser
 * to get an image information.
 * @get_video_information: This is called by #GUPnPDLNAProfileGuesser
 * to get a video information.
 * @get_profile_name: This is called by #GUPnPDLNAProfileGuesser to
 * get the name of the DLNA profile assigned with this information.
 * @_reserved: Padding. Ignore it.
 */
typedef struct {
        GObjectClass parent_class;

        GUPnPDLNAAudioInformation *
        (* get_audio_information) (GUPnPDLNAInformation *info);

        GUPnPDLNAContainerInformation *
        (* get_container_information) (GUPnPDLNAInformation *info);

        GUPnPDLNAImageInformation *
        (* get_image_information) (GUPnPDLNAInformation *info);

        GUPnPDLNAVideoInformation *
        (* get_video_information) (GUPnPDLNAInformation *info);

        const gchar *
        (* get_profile_name) (GUPnPDLNAInformation *info);

        gpointer _reserved[11];
} GUPnPDLNAInformationClass;

GType
gupnp_dlna_information_get_type (void);

GUPnPDLNAAudioInformation*
gupnp_dlna_information_get_audio_information (GUPnPDLNAInformation *info);

GUPnPDLNAContainerInformation*
gupnp_dlna_information_get_container_information (GUPnPDLNAInformation *info);

GUPnPDLNAImageInformation*
gupnp_dlna_information_get_image_information (GUPnPDLNAInformation *info);

GUPnPDLNAVideoInformation*
gupnp_dlna_information_get_video_information (GUPnPDLNAInformation *info);

const gchar *
gupnp_dlna_information_get_profile_name (GUPnPDLNAInformation *info);

const gchar *
gupnp_dlna_information_get_uri (GUPnPDLNAInformation *info);

G_END_DECLS

#endif /* __GUPNP_DLNA_INFORMATION_H__ */
