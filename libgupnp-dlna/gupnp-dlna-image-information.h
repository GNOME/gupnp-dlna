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

#define GUPNP_TYPE_DLNA_IMAGE_INFORMATION \
        (gupnp_dlna_image_information_get_type())

#define GUPNP_DLNA_IMAGE_INFORMATION(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                     GUPNP_TYPE_DLNA_IMAGE_INFORMATION, \
                                     GUPnPDLNAImageInformation))

#define GUPNP_DLNA_IMAGE_INFORMATION_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                  GUPNP_TYPE_DLNA_IMAGE_INFORMATION, \
                                  GUPnPDLNAImageInformationClass))

#define GUPNP_IS_DLNA_IMAGE_INFORMATION(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                     GUPNP_TYPE_DLNA_IMAGE_INFORMATION))

#define GUPNP_IS_DLNA_IMAGE_INFORMATION_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                  GUPNP_TYPE_DLNA_IMAGE_INFORMATION))

#define GUPNP_DLNA_IMAGE_INFORMATION_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                    GUPNP_TYPE_DLNA_IMAGE_INFORMATION, \
                                    GUPnPDLNAImageInformationClass))

typedef struct _GUPnPDLNAImageInformationPrivate
                GUPnPDLNAImageInformationPrivate;

typedef struct {
        GObject parent;

        GUPnPDLNAImageInformationPrivate *priv;
} GUPnPDLNAImageInformation;

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
typedef struct {
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
} GUPnPDLNAImageInformationClass;

GType
gupnp_dlna_image_information_get_type (void);

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
