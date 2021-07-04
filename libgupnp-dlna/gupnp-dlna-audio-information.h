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

#ifndef __GUPNP_DLNA_AUDIO_INFORMATION_H__
#define __GUPNP_DLNA_AUDIO_INFORMATION_H__

#include <glib-object.h>
#include <libgupnp-dlna/gupnp-dlna-values.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE (GUPnPDLNAAudioInformation,
                          gupnp_dlna_audio_information,
                          GUPNP_DLNA,
                          AUDIO_INFORMATION,
                          GObject)

#define GUPNP_TYPE_DLNA_AUDIO_INFORMATION                                      \
        (gupnp_dlna_audio_information_get_type ())

// Backwards-compatible defines
/**
 * GUPNP_IS_DLNA_AUDIO_INFORMATION: (skip)
 */
#define GUPNP_IS_DLNA_AUDIO_INFORMATION GUPNP_DLNA_IS_AUDIO_INFORMATION
/**
 * GUPNP_IS_DLNA_AUDIO_INFORMATION_CLASS: (skip)
 */
#define GUPNP_IS_DLNA_AUDIO_INFORMATION_CLASS GUPNP_DLNA_IS_AUDIO_INFORMATION_CLASS

/**
 * GUPnPDLNAAudioInformationClass:
 * @parent_class: Parent class.
 * @get_bitrate: This is called by #GUPnPDLNAProfileGuesser to get a
 * bitrate.
 * @get_channels: This is called by #GUPnPDLNAProfileGuesser to get a
 * channels count.
 * @get_depth: This is called by #GUPnPDLNAProfileGuesser to get a
 * depth.
 * @get_layer: This is called by #GUPnPDLNAProfileGuesser to get a
 * layer.
 * @get_level: This is called by #GUPnPDLNAProfileGuesser to get a
 * level.
 * @get_mpeg_audio_version: This is called by #GUPnPDLNAProfileGuesser
 * to get an MPEG audio version.
 * @get_mpeg_version: This is called by #GUPnPDLNAProfileGuesser to
 * get an MPEG version.
 * @get_profile: This is called by #GUPnPDLNAProfileGuesser to get a
 * profile.
 * @get_rate: This is called by #GUPnPDLNAProfileGuesser to get a
 * sample rate.
 * @get_stream_format: This is called by #GUPnPDLNAProfileGuesser to
 * get a stream format.
 * @get_wma_version: This is called by #GUPnPDLNAProfileGuesser to get
 * a WMA version.
 * @get_mime: This is called by #GUPnPDLNAProfileGuesser to get a MIME
 * type.
 * @_reserved: Padding. Ignore it.
 */
struct _GUPnPDLNAAudioInformationClass {
        GObjectClass parent_class;

        GUPnPDLNAIntValue
        (* get_bitrate) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAIntValue
        (* get_channels) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAIntValue
        (* get_depth) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAIntValue
        (* get_layer) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAStringValue
        (* get_level) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAIntValue
        (* get_mpeg_audio_version) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAIntValue
        (* get_mpeg_version) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAStringValue
        (* get_profile) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAIntValue
        (* get_rate) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAStringValue
        (* get_stream_format) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAIntValue
        (* get_wma_version) (GUPnPDLNAAudioInformation *info);

        GUPnPDLNAStringValue
        (* get_mime) (GUPnPDLNAAudioInformation *info);

        gpointer _reserved[12];
};

GUPnPDLNAIntValue
gupnp_dlna_audio_information_get_bitrate (GUPnPDLNAAudioInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_audio_information_get_channels (GUPnPDLNAAudioInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_audio_information_get_depth (GUPnPDLNAAudioInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_audio_information_get_layer (GUPnPDLNAAudioInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_audio_information_get_level (GUPnPDLNAAudioInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_audio_information_get_mpeg_audio_version
                                        (GUPnPDLNAAudioInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_audio_information_get_mpeg_version (GUPnPDLNAAudioInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_audio_information_get_profile (GUPnPDLNAAudioInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_audio_information_get_rate (GUPnPDLNAAudioInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_audio_information_get_stream_format
                                        (GUPnPDLNAAudioInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_audio_information_get_wma_version (GUPnPDLNAAudioInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_audio_information_get_mime (GUPnPDLNAAudioInformation *info);

G_END_DECLS

#endif /* __GUPNP_DLNA_AUDIO_INFORMATION_H__ */
