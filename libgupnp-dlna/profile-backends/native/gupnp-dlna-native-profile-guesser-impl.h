/*
 * Copyright (C) 2012 Intel Corporation
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

#ifndef __GUPNP_DLNA_NATIVE_PROFILE_GUESSER_IMPL_H__
#define __GUPNP_DLNA_NATIVE_PROFILE_GUESSER_IMPL_H__

#include <glib-object.h>
#include "gupnp-dlna-profile-guesser-impl.h"

G_BEGIN_DECLS

#define GUPNP_TYPE_DLNA_NATIVE_PROFILE_GUESSER_IMPL \
        (gupnp_dlna_native_profile_guesser_impl_get_type())

#define GUPNP_DLNA_NATIVE_PROFILE_GUESSER_IMPL(obj) \
     (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                  GUPNP_TYPE_DLNA_NATIVE_PROFILE_GUESSER_IMPL, \
                                  GUPnPDLNANativeProfileGuesserImpl))

#define GUPNP_DLNA_NATIVE_PROFILE_GUESSER_IMPL_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                  GUPNP_TYPE_DLNA_NATIVE_PROFILE_GUESSER_IMPL, \
                                  GUPnPDLNANativeProfileGuesserImplClass))

#define GUPNP_IS_DLNA_NATIVE_PROFILE_GUESSER_IMPL(obj) \
      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                   GUPNP_TYPE_DLNA_NATIVE_PROFILE_GUESSER_IMPL))

#define GUPNP_IS_DLNA_NATIVE_PROFILE_GUESSER_IMPL_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                  GUPNP_TYPE_DLNA_NATIVE_PROFILE_GUESSER_IMPL))

#define GUPNP_DLNA_NATIVE_PROFILE_GUESSER_IMPL_GET_CLASS(obj) \
      (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                  GUPNP_TYPE_DLNA_NATIVE_PROFILE_GUESSER_IMPL, \
                                  GUPnPDLNANativeProfileGuesserImplClass))

typedef struct _GUPnPDLNANativeProfileGuesserImplPrivate
                GUPnPDLNANativeProfileGuesserImplPrivate;

typedef struct {
        GUPnPDLNAProfileGuesserImpl parent;

        GUPnPDLNANativeProfileGuesserImplPrivate *priv;
} GUPnPDLNANativeProfileGuesserImpl;

typedef struct {
        GUPnPDLNAProfileGuesserImplClass parent_class;
} GUPnPDLNANativeProfileGuesserImplClass;

GType
gupnp_dlna_native_profile_guesser_impl_get_type (void) G_GNUC_CONST;

GUPnPDLNANativeProfileGuesserImpl *
gupnp_dlna_native_profile_guesser_impl_new (void);

G_END_DECLS

#endif /* __GUPNP_DLNA_NATIVE_PROFILE_GUESSER_IMPL_H__ */
