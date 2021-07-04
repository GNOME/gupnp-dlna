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

#ifndef __GUPNP_DLNA_GST_AUDIO_INFORMATION_H__
#define __GUPNP_DLNA_GST_AUDIO_INFORMATION_H__

#include <glib-object.h>
#include <gst/pbutils/pbutils.h>
#include "gupnp-dlna-audio-information.h"

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (GUPnPDLNAGstAudioInformation,
                      gupnp_dlna_gst_audio_information,
                      GUPNP_DLNA,
                      GST_AUDIO_INFORMATION,
                      GUPnPDLNAAudioInformation)

#define GUPNP_TYPE_DLNA_GST_AUDIO_INFORMATION \
        (gupnp_dlna_gst_audio_information_get_type())

// Backwards-compatible defines
/**
 * GUPNP_IS_DLNA_GST_AUDIO_INFORMATION: (skip)
 */
#define GUPNP_IS_DLNA_GST_AUDIO_INFORMATION GUPNP_DLNA_IS_GST_AUDIO_INFORMATION
/**
 * GUPNP_IS_DLNA_GST_AUDIO_INFORMATION_CLASS: (skip)
 */
#define GUPNP_IS_DLNA_GST_AUDIO_INFORMATION_CLASS                              \
        GUPNP_DLNA_IS_GST_AUDIO_INFORMATION_CLASS

struct _GUPnPDLNAGstAudioInformationClass {
        GUPnPDLNAAudioInformationClass parent_class;
};

GType
gupnp_dlna_gst_audio_information_get_type (void);

GUPnPDLNAGstAudioInformation *
gupnp_dlna_gst_audio_information_new_from_discoverer_info
                                        (GstDiscovererInfo *info);

G_END_DECLS

#endif /* __GUPNP_DLNA_GST_AUDIO_INFORMATION_H__ */
