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

#ifndef __GUPNP_DLNA_NATIVE_PROFILE_H__
#define __GUPNP_DLNA_NATIVE_PROFILE_H__

#include <glib-object.h>
#include "gupnp-dlna-profile.h"
#include "gupnp-dlna-native-sets.h"

G_BEGIN_DECLS

#define GUPNP_TYPE_DLNA_NATIVE_PROFILE gupnp_dlna_native_profile_get_type()

#define GUPNP_DLNA_NATIVE_PROFILE(obj)                               \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj),                   \
                                     GUPNP_TYPE_DLNA_NATIVE_PROFILE, \
                                     GUPnPDLNANativeProfile))

#define GUPNP_DLNA_NATIVE_PROFILE_CLASS(klass)                    \
        (G_TYPE_CHECK_CLASS_CAST ((klass),                 \
                                  GUPNP_TYPE_DLNA_NATIVE_PROFILE, \
                                  GUPnPDLNANativeProfileClass))

#define GUPNP_IS_DLNA_NATIVE_PROFILE(obj)                                   \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GUPNP_TYPE_DLNA_NATIVE_PROFILE))

#define GUPNP_IS_DLNA_NATIVE_PROFILE_CLASS(klass)                           \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), GUPNP_TYPE_DLNA_NATIVE_PROFILE))

#define GUPNP_DLNA_NATIVE_PROFILE_GET_CLASS(obj)                    \
        (G_TYPE_INSTANCE_GET_CLASS ((obj),                   \
                                    GUPNP_TYPE_DLNA_NATIVE_PROFILE, \
                                    GUPnPDLNANativeProfileClass))

typedef struct _GUPnPDLNANativeProfilePrivate GUPnPDLNANativeProfilePrivate;

typedef struct {
        GUPnPDLNAProfile parent;

        GUPnPDLNANativeProfilePrivate *priv;
} GUPnPDLNANativeProfile;

typedef struct {
        GUPnPDLNAProfileClass parent_class;
} GUPnPDLNANativeProfileClass;

GType
gupnp_dlna_native_profile_get_type (void);

GUPnPDLNANativeProfile *
gupnp_dlna_native_profile_new (const gchar *name,
                               const gchar *mime,
                               GList       *audio_restrictions,
                               GList       *container_restrictions,
                               GList       *image_restrictions,
                               GList       *video_restrictions,
                               gboolean     extended);

GList *
gupnp_dlna_native_profile_get_container_restrictions
                                        (GUPnPDLNANativeProfile *self);

GList *
gupnp_dlna_native_profile_get_audio_restrictions (GUPnPDLNANativeProfile *self);

GList *
gupnp_dlna_native_profile_get_image_restrictions (GUPnPDLNANativeProfile *self);

GList *
gupnp_dlna_native_profile_get_video_restrictions (GUPnPDLNANativeProfile *self);

G_END_DECLS

#endif /* __GUPNP_DLNA_NATIVE_PROFILE_H__ */
