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

#ifndef __GUPNP_DLNA_PROFILE_H__
#define __GUPNP_DLNA_PROFILE_H__

#include <glib-object.h>
#include <libgupnp-dlna/gupnp-dlna-restriction.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE (GUPnPDLNAProfile,
                          gupnp_dlna_profile,
                          GUPNP_DLNA,
                          PROFILE,
                          GObject)

#define GUPNP_TYPE_DLNA_PROFILE gupnp_dlna_profile_get_type()

// Backwards-compatible defines
/**
 * GUPNP_IS_DLNA_PROFILE: (skip)
 */
#define GUPNP_IS_DLNA_PROFILE GUPNP_DLNA_IS_PROFILE
/**
 * GUPNP_IS_DLNA_PROFILE_CLASS: (skip)
 */
#define GUPNP_IS_DLNA_PROFILE_CLASS GUPNP_DLNA_IS_PROFILE_CLASS

/**
 * GUPnPDLNAProfileClass:
 * @parent_class: A #GObjectClass - parent of this class.
 */
struct _GUPnPDLNAProfileClass {
        GObjectClass parent_class;
};

const gchar *
gupnp_dlna_profile_get_name (GUPnPDLNAProfile *profile);

const gchar *
gupnp_dlna_profile_get_mime (GUPnPDLNAProfile *profile);

gboolean
gupnp_dlna_profile_get_extended (GUPnPDLNAProfile *profile);

GList *
gupnp_dlna_profile_get_container_restrictions (GUPnPDLNAProfile *profile);

GList *
gupnp_dlna_profile_get_audio_restrictions (GUPnPDLNAProfile *profile);

GList *
gupnp_dlna_profile_get_image_restrictions (GUPnPDLNAProfile *profile);

GList *
gupnp_dlna_profile_get_video_restrictions (GUPnPDLNAProfile *profile);

G_END_DECLS

#endif /* __GUPNP_DLNA_PROFILE_H__ */
