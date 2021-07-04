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

G_DECLARE_DERIVABLE_TYPE (GUPnPDLNAProfileLoader,
                          gupnp_dlna_profile_loader,
                          GUPNP_DLNA,
                          PROFILE_LOADER,
                          GObject)


#define GUPNP_TYPE_DLNA_PROFILE_LOADER (gupnp_dlna_profile_loader_get_type ())
// Backwards-compatible defines
/**
 * GUPNP_IS_DLNA_PROFILE_LOADER: (skip)
 */
#define GUPNP_IS_DLNA_PROFILE_LOADER GUPNP_DLNA_IS_PROFILE_LOADER
/**
 * GUPNP_IS_DLNA_PROFILE_LOADER_CLASS: (skip)
 */
#define GUPNP_IS_DLNA_PROFILE_LOADER_CLASS GUPNP_DLNA_IS_PROFILE_LOADER_CLASS

typedef struct _GUPnPDLNAProfileLoaderPrivate GUPnPDLNAProfileLoaderPrivate;

struct _GUPnPDLNAProfileLoaderClass {
        GObjectClass parent_class;
};

GUPnPDLNAProfileLoader *
gupnp_dlna_profile_loader_new (gboolean relaxed_mode,
                               gboolean extended_mode);

GList *
gupnp_dlna_profile_loader_get_from_disk (GUPnPDLNAProfileLoader *loader);

G_END_DECLS

#endif /* __GUPNP_DLNA_PROFILE_LOADER_H__ */
