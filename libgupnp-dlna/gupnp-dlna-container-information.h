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

#ifndef __GUPNP_DLNA_CONTAINER_INFORMATION_H__
#define __GUPNP_DLNA_CONTAINER_INFORMATION_H__

#include <glib-object.h>
#include <libgupnp-dlna/gupnp-dlna-values.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE (GUPnPDLNAContainerInformation,
                          gupnp_dlna_container_information,
                          GUPNP_DLNA,
                          CONTAINER_INFORMATION,
                          GObject)

#define GUPNP_TYPE_DLNA_CONTAINER_INFORMATION \
        (gupnp_dlna_container_information_get_type())

// Backwards-compatible defines
/**
 * GUPNP_IS_DLNA_CONTAINER_INFORMATION: (skip)
 */
#define GUPNP_IS_DLNA_CONTAINER_INFORMATION GUPNP_DLNA_IS_CONTAINER_INFORMATION
/**
 * GUPNP_IS_DLNA_CONTAINER_INFORMATION_CLASS: (skip)
 */
#define GUPNP_IS_DLNA_CONTAINER_INFORMATION_CLASS GUPNP_DLNA_IS_CONTAINER_INFORMATION_CLASS

/**
 * GUPnPDLNAContainerInformationClass:
 * @parent_class: Parent class.
 * @get_mpeg_version: This is called by #GUPnPDLNAProfileGuesser to
 * get an MPEG version.
 * @get_packet_size: This is called by #GUPnPDLNAProfileGuesser to get
 * a packet size.
 * @get_profile: This is called by #GUPnPDLNAProfileGuesser to get a
 * profile.
 * @is_system_stream: This is called by #GUPnPDLNAProfileGuesser to
 * get whether it is a system stream
 * @get_variant: This is called by #GUPnPDLNAProfileGuesser to get a
 * variant.
 * @get_mime: This is called by #GUPnPDLNAProfileGuesser to get a MIME
 * type.
 * @_reserved: Padding. Ignore it.
 */
struct _GUPnPDLNAContainerInformationClass {
        GObjectClass parent_class;

        GUPnPDLNAIntValue
        (* get_mpeg_version) (GUPnPDLNAContainerInformation *info);

        GUPnPDLNAIntValue
        (* get_packet_size) (GUPnPDLNAContainerInformation *info);

        GUPnPDLNAStringValue
        (* get_profile) (GUPnPDLNAContainerInformation *info);

        GUPnPDLNABoolValue
        (* is_system_stream) (GUPnPDLNAContainerInformation *info);

        GUPnPDLNAStringValue
        (* get_variant) (GUPnPDLNAContainerInformation *info);

        GUPnPDLNAStringValue
        (* get_mime) (GUPnPDLNAContainerInformation *info);

        gpointer _reserved[12];
};

GUPnPDLNAIntValue
gupnp_dlna_container_information_get_mpeg_version
                                        (GUPnPDLNAContainerInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_container_information_get_packet_size
                                        (GUPnPDLNAContainerInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_container_information_get_profile
                                        (GUPnPDLNAContainerInformation *info);

GUPnPDLNABoolValue
gupnp_dlna_container_information_is_system_stream
                                        (GUPnPDLNAContainerInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_container_information_get_variant
                                        (GUPnPDLNAContainerInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_container_information_get_mime (GUPnPDLNAContainerInformation *info);

G_END_DECLS

#endif /* __GUPNP_DLNA_CONTAINER_INFORMATION_H__ */
