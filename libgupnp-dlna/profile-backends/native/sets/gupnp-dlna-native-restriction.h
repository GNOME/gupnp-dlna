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

#ifndef __GUPNP_DLNA_NATIVE_RESTRICTION_H__
#define __GUPNP_DLNA_NATIVE_RESTRICTION_H__

#include <glib.h>
#include <glib-object.h>
#include "gupnp-dlna-native-value-list.h"

G_BEGIN_DECLS

typedef struct _GUPnPDLNANativeRestriction GUPnPDLNANativeRestriction;

#define GUPNP_DLNA_NATIVE_RESTRICTION(x) \
        ((GUPnPDLNANativeRestriction *) x)

GUPnPDLNANativeRestriction *
gupnp_dlna_native_restriction_new (const gchar *mime);

GUPnPDLNANativeRestriction *
gupnp_dlna_native_restriction_copy (GUPnPDLNANativeRestriction *restriction);

void
gupnp_dlna_native_restriction_free (GUPnPDLNANativeRestriction *restriction);

gboolean
gupnp_dlna_native_restriction_add_value_list
                                       (GUPnPDLNANativeRestriction *restriction,
                                        const gchar                *name,
                                        GUPnPDLNANativeValueList   *list);

void
gupnp_dlna_native_restriction_merge (GUPnPDLNANativeRestriction *restriction,
                                     GUPnPDLNANativeRestriction *merged);

gboolean
gupnp_dlna_native_restriction_is_empty
                                      (GUPnPDLNANativeRestriction *restriction);

gchar *
gupnp_dlna_native_restriction_to_string
                                      (GUPnPDLNANativeRestriction *restriction);

const gchar *
gupnp_dlna_native_restriction_get_mime
                                      (GUPnPDLNANativeRestriction *restriction);

G_END_DECLS

#endif /* __GUPNP_DLNA_NATIVE_RESTRICTION_H__ */
