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

#ifndef __GUPNP_DLNA_IMAGE_INFORMATION_H__
#define __GUPNP_DLNA_IMAGE_INFORMATION_H__

#include <glib-object.h>
#include <libgupnp-dlna/gupnp-dlna-values.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(GUPnPDLNAImageInformation,
                          gupnp_dlna_image_information,
                          GUPNP_DLNA,
                          IMAGE_INFORMATION,
                          GObject)


#define GUPNP_TYPE_DLNA_IMAGE_INFORMATION \
        (gupnp_dlna_image_information_get_type())

// Backwards-compatible defines
/**
 * GUPNP_IS_DLNA_IMAGE_INFORMATION: (skip)
 */
#define GUPNP_IS_DLNA_IMAGE_INFORMATION GUPNP_DLNA_IS_IMAGE_INFORMATION
/**
 * GUPNP_IS_DLNA_IMAGE_INFORMATION_CLASS: (skip)
 */
#define GUPNP_IS_DLNA_IMAGE_INFORMATION_CLASS GUPNP_DLNA_IS_IMAGE_INFORMATION_CLASS

/**
 * GUPnPDLNAImageInformationClass:
 * @parent_class: Parent class.
 * @get_depth: This is called by #GUPnPDLNAProfileGuesser to get a
 * depth.
 * @get_height: This is called by #GUPnPDLNAProfileGuesser to get a
 * height.
 * @get_width: This is called by #GUPnPDLNAProfileGuesser to get a
 * width.
 * @get_mime: This is called by #GUPnPDLNAProfileGuesser to get a MIME
 * type.
 * @_reserved: Padding. Ignore it.
 */
struct _GUPnPDLNAImageInformationClass {
        GObjectClass parent_class;

        GUPnPDLNAIntValue
        (* get_depth) (GUPnPDLNAImageInformation *info);

        GUPnPDLNAIntValue
        (* get_height) (GUPnPDLNAImageInformation *info);

        GUPnPDLNAIntValue
        (* get_width) (GUPnPDLNAImageInformation *info);

        GUPnPDLNAStringValue
        (* get_mime) (GUPnPDLNAImageInformation *info);

        gpointer _reserved[12];
};

GUPnPDLNAIntValue
gupnp_dlna_image_information_get_depth (GUPnPDLNAImageInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_image_information_get_height (GUPnPDLNAImageInformation *info);

GUPnPDLNAIntValue
gupnp_dlna_image_information_get_width (GUPnPDLNAImageInformation *info);

GUPnPDLNAStringValue
gupnp_dlna_image_information_get_mime (GUPnPDLNAImageInformation *info);

G_END_DECLS

#endif /* __GUPNP_DLNA_IMAGE_INFORMATION_H__ */
