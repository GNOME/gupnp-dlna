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

#ifndef __GUPNP_DLNA_VIDEO_INFORMATION_H__
#define __GUPNP_DLNA_VIDEO_INFORMATION_H__

#include <glib-object.h>
#include <libgupnp-dlna/gupnp-dlna-values.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE (GUPnPDLNAVideoInformation,
                          gupnp_dlna_video_information,
                          GUPNP_DLNA,
                          VIDEO_INFORMATION,
                          GObject)


#define GUPNP_TYPE_DLNA_VIDEO_INFORMATION \
        (gupnp_dlna_video_information_get_type())

// Backwards-compatible defines
/**
 * GUPNP_IS_DLNA_VIDEO_INFORMATION: (skip)
 */
#define GUPNP_IS_DLNA_VIDEO_INFORMATION GUPNP_DLNA_IS_VIDEO_INFORMATION
/**
 * GUPNP_IS_DLNA_VIDEO_INFORMATION_CLASS: (skip)
 */
#define GUPNP_IS_DLNA_VIDEO_INFORMATION_CLASS GUPNP_DLNA_IS_VIDEO_INFORMATION_CLASS

/**
 * GUPnPDLNAVideoInformationClass:
 * @parent_class: Parent class.
 * @get_bitrate: This is called by #GUPnPDLNAProfileGuesser to get a
 * bitrate.
 * @get_framerate: This is called by #GUPnPDLNAProfileGuesser to get a
 * framerate.
 * @get_height: This is called by #GUPnPDLNAProfileGuesser to get a
 * height.
 * @is_interlaced: This is called by #GUPnPDLNAProfileGuesser to get
 * whether it is interlaced.
 * @get_level: This is called by #GUPnPDLNAProfileGuesser to get a
 * level.
 * @get_mpeg_version: This is called by #GUPnPDLNAProfileGuesser to
 * get an MPEG version.
 * @get_pixel_aspect_ratio: This is called by #GUPnPDLNAProfileGuesser
 * to get a pixel-aspect-ratio.
 * @get_profile: This is called by #GUPnPDLNAProfileGuesser to get a
 * profile.
 * @is_system_stream: This is called by #GUPnPDLNAProfileGuesser to
 * get whether it is a system stream.
 * @get_width: This is called by #GUPnPDLNAProfileGuesser to get a
 * width.
 * @get_mime: This is called by #GUPnPDLNAProfileGuesser to get a MIME
 * type.
 * @_reserved: Padding. Ignore it.
 */
struct _GUPnPDLNAVideoInformationClass {
        GObjectClass parent_class;

        GUPnPDLNAIntValue
        (* get_bitrate) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNAFractionValue
        (* get_framerate) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNAIntValue
        (* get_height) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNABoolValue
        (* is_interlaced) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNAStringValue
        (* get_level) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNAIntValue
        (* get_mpeg_version) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNAFractionValue
        (* get_pixel_aspect_ratio) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNAStringValue
        (* get_profile) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNABoolValue
        (* is_system_stream) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNAIntValue
        (* get_width) (GUPnPDLNAVideoInformation *info);

        GUPnPDLNAStringValue
        (* get_mime) (GUPnPDLNAVideoInformation *info);

        gpointer _reserved[12];
};

GUPnPDLNAIntValue
gupnp_dlna_video_information_get_bitrate (GUPnPDLNAVideoInformation *info);

GUPnPDLNAFractionValue
gupnp_dlna_video_information_get_framerate (GUPnPDLNAVideoInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_video_information_get_height (GUPnPDLNAVideoInformation *info);

GUPnPDLNABoolValue
gupnp_dlna_video_information_is_interlaced (GUPnPDLNAVideoInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_video_information_get_level (GUPnPDLNAVideoInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_video_information_get_mpeg_version (GUPnPDLNAVideoInformation *info);

GUPnPDLNAFractionValue
gupnp_dlna_video_information_get_pixel_aspect_ratio
                                        (GUPnPDLNAVideoInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_video_information_get_profile (GUPnPDLNAVideoInformation *info);

GUPnPDLNABoolValue
gupnp_dlna_video_information_is_system_stream (GUPnPDLNAVideoInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_video_information_get_width (GUPnPDLNAVideoInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_video_information_get_mime (GUPnPDLNAVideoInformation *info);

G_END_DECLS

#endif /* __GUPNP_DLNA_VIDEO_INFORMATION_H__ */
