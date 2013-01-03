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

#ifndef __GUPNP_DLNA_NATIVE_SETS_PRIVATE_H__
#define __GUPNP_DLNA_NATIVE_SETS_PRIVATE_H__

#include <glib.h>
#include "gupnp-dlna-native-value-type.h"
#include "gupnp-dlna-value-list-private.h"
#include "gupnp-dlna-g-values-private.h"
#include "gupnp-dlna-native-info-value.h"
#include "gupnp-dlna-restriction-private.h"
#include "gupnp-dlna-value-union.h"

G_BEGIN_DECLS

/* ValueType */
gboolean
gupnp_dlna_native_value_type_init (GUPnPDLNANativeValueType  *type,
                                   GUPnPDLNAValueUnion *value,
                                   const gchar               *raw);

gboolean
gupnp_dlna_native_value_type_copy (GUPnPDLNANativeValueType  *type,
                                   GUPnPDLNAValueUnion *from,
                                   GUPnPDLNAValueUnion *to);

void
gupnp_dlna_native_value_type_clean (GUPnPDLNANativeValueType  *type,
                                    GUPnPDLNAValueUnion *value_union);

gboolean
gupnp_dlna_native_value_type_is_equal (GUPnPDLNANativeValueType  *type,
                                       GUPnPDLNAValueUnion *first,
                                       GUPnPDLNAValueUnion *second);

gboolean
gupnp_dlna_native_value_type_is_in_range (GUPnPDLNANativeValueType  *type,
                                          GUPnPDLNAValueUnion *min,
                                          GUPnPDLNAValueUnion *max,
                                          GUPnPDLNAValueUnion *value);

const gchar *
gupnp_dlna_native_value_type_name (GUPnPDLNANativeValueType *type);

gboolean
gupnp_dlna_native_value_type_verify_range (GUPnPDLNANativeValueType  *type,
                                           GUPnPDLNAValueUnion *min,
                                           GUPnPDLNAValueUnion *max);

gchar *
gupnp_dlna_native_value_type_to_string (GUPnPDLNANativeValueType  *type,
                                        GUPnPDLNAValueUnion *value);

gint
gupnp_dlna_native_value_type_compare (GUPnPDLNANativeValueType  *type,
                                      GUPnPDLNAValueUnion *a,
                                      GUPnPDLNAValueUnion *b);

void
gupnp_dlna_native_value_type_to_g_value (GUPnPDLNANativeValueType  *type,
                                         GUPnPDLNAValueUnion *value,
                                         GValue                    *g_value);

gboolean
gupnp_dlna_native_value_type_flatten (GUPnPDLNANativeValueType *type,
                                      GValue *target,
                                      GValue *from,
                                      GValue *to);

/* ValueList */
gboolean
gupnp_dlna_value_list_is_superset (GUPnPDLNAValueList       *list,
                                   GUPnPDLNANativeInfoValue *value,
                                   gboolean                 *unsupported);

GList *
gupnp_dlna_value_list_get_list (GUPnPDLNAValueList *value_list);

void
gupnp_dlna_value_list_sort_items (GUPnPDLNAValueList *value_list);

/* InfoValue */
GUPnPDLNANativeValueType *
gupnp_dlna_native_info_value_get_type (GUPnPDLNANativeInfoValue *info);

GUPnPDLNAValueUnion *
gupnp_dlna_native_info_value_get_value (GUPnPDLNANativeInfoValue *info);

G_END_DECLS

#endif /* __GUPNP_DLNA_NATIVE_SETS_PRIVATE_H__ */
