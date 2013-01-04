/*
 * Copyright (C) 2012, 2013 Intel Corporation.
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

#ifndef __GUPNP_DLNA_NATIVE_VALUE_H__
#define __GUPNP_DLNA_NATIVE_VALUE_H__

#include <glib.h>
#include <glib-object.h>

#include "gupnp-dlna-native-info-value.h"
#include "gupnp-dlna-native-value-type.h"

G_BEGIN_DECLS

typedef struct _GUPnPDLNANativeValue GUPnPDLNANativeValue;
typedef struct _GUPnPDLNANativeValueVTable GUPnPDLNANativeValueVTable;

struct _GUPnPDLNANativeValue {
        GUPnPDLNANativeValueVTable *vtable;
};

GUPnPDLNANativeValue *
gupnp_dlna_native_value_new_single (GUPnPDLNANativeValueType *type,
                                    const gchar              *single);

GUPnPDLNANativeValue *
gupnp_dlna_native_value_new_ranged (GUPnPDLNANativeValueType *type,
                                    const gchar              *min,
                                    const gchar              *max);

gboolean
gupnp_dlna_native_value_is_superset (GUPnPDLNANativeValue     *base,
                                     GUPnPDLNANativeInfoValue *single);

GUPnPDLNANativeValue *
gupnp_dlna_native_value_copy (GUPnPDLNANativeValue     *base,
                              GUPnPDLNANativeValueType *type);

void
gupnp_dlna_native_value_free (GUPnPDLNANativeValue     *base,
                              GUPnPDLNANativeValueType *type);

gchar *
gupnp_dlna_native_value_to_string (GUPnPDLNANativeValue     *base,
                                   GUPnPDLNANativeValueType *type);

gint
gupnp_dlna_native_value_compare (GUPnPDLNANativeValue     *base,
                                 GUPnPDLNANativeValue     *other,
                                 GUPnPDLNANativeValueType *type);

GValue *
gupnp_dlna_native_value_to_g_value (GUPnPDLNANativeValue     *base,
                                    GUPnPDLNANativeValueType *type);

G_END_DECLS

#endif /* __GUPNP_DLNA_NATIVE_VALUE_H__ */
