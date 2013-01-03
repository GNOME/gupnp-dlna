/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2012, 2013 Intel Corporation.
 *
 * Authors: Zeeshan Ali <zeeshanak@gnome.org>
 *                      <zeeshan.ali@nokia.com>
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

#ifndef __GUPNP_DLNA_PROFILE_LOADER_H__
#define __GUPNP_DLNA_PROFILE_LOADER_H__

#include <glib-object.h>
#include "gupnp-dlna-profile.h"
#include "gupnp-dlna-field-value.h"

G_BEGIN_DECLS

#define GUPNP_TYPE_DLNA_PROFILE_LOADER (gupnp_dlna_profile_loader_get_type ())

#define GUPNP_DLNA_PROFILE_LOADER(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                     GUPNP_TYPE_DLNA_PROFILE_LOADER, \
                                     GUPnPDLNAProfileLoader))

#define GUPNP_DLNA_PROFILE_LOADER_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                  GUPNP_TYPE_DLNA_PROFILE_LOADER, \
                                  GUPnPDLNAProfileLoaderClass))

#define GUPNP_IS_DLNA_PROFILE_LOADER(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                     GUPNP_TYPE_DLNA_PROFILE_LOADER))

#define GUPNP_IS_DLNA_PROFILE_LOADER_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                  GUPNP_TYPE_DLNA_PROFILE_LOADER))

#define GUPNP_DLNA_PROFILE_LOADER_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                    GUPNP_TYPE_DLNA_PROFILE_LOADER, \
                                    GUPnPDLNAProfileLoaderClass))

typedef struct _GUPnPDLNAProfileLoaderPrivate GUPnPDLNAProfileLoaderPrivate;

typedef struct {
        GObject parent;

        GUPnPDLNAProfileLoaderPrivate *priv;
} GUPnPDLNAProfileLoader;

typedef struct {
        GObjectClass parent_class;
} GUPnPDLNAProfileLoaderClass;

GType
gupnp_dlna_profile_loader_get_type (void);

GUPnPDLNAProfileLoader *
gupnp_dlna_profile_loader_new (gboolean relaxed_mode,
                               gboolean extended_mode);

GList *
gupnp_dlna_profile_loader_get_from_disk (GUPnPDLNAProfileLoader *loader);

G_END_DECLS

#endif /* __GUPNP_DLNA_PROFILE_LOADER_H__ */
