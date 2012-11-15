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

#ifndef __GUPNP_DLNA_NATIVE_SETS_PRIVATE_H__
#define __GUPNP_DLNA_NATIVE_SETS_PRIVATE_H__

#include <glib.h>
#include "gupnp-dlna-native-value-type.h"
#include "gupnp-dlna-native-value-list.h"
#include "gupnp-dlna-native-info-value.h"
#include "gupnp-dlna-native-restriction.h"

G_BEGIN_DECLS

/* private */
typedef union _GUPnPDLNANativeValueUnion GUPnPDLNANativeValueUnion;
typedef struct _GUPnPDLNANativeFraction GUPnPDLNANativeFraction;

struct _GUPnPDLNANativeFraction {
        gint numerator;
        gint denominator;
};

union _GUPnPDLNANativeValueUnion {
        gboolean bool_value;
        GUPnPDLNANativeFraction fraction_value;
        gint int_value;
        gchar *string_value;
};

/* ValueType */
gboolean
gupnp_dlna_native_value_type_init (GUPnPDLNANativeValueType  *type,
                                   GUPnPDLNANativeValueUnion *value,
                                   const gchar               *raw);

gboolean
gupnp_dlna_native_value_type_copy (GUPnPDLNANativeValueType  *type,
                                   GUPnPDLNANativeValueUnion *from,
                                   GUPnPDLNANativeValueUnion *to);

void
gupnp_dlna_native_value_type_clean (GUPnPDLNANativeValueType  *type,
                                    GUPnPDLNANativeValueUnion *value_union);

gboolean
gupnp_dlna_native_value_type_is_equal (GUPnPDLNANativeValueType  *type,
                                       GUPnPDLNANativeValueUnion *first,
                                       GUPnPDLNANativeValueUnion *second);

gboolean
gupnp_dlna_native_value_type_is_in_range (GUPnPDLNANativeValueType  *type,
                                          GUPnPDLNANativeValueUnion *min,
                                          GUPnPDLNANativeValueUnion *max,
                                          GUPnPDLNANativeValueUnion *value);

const gchar *
gupnp_dlna_native_value_type_name (GUPnPDLNANativeValueType *type);

gboolean
gupnp_dlna_native_value_type_verify_range (GUPnPDLNANativeValueType  *type,
                                           GUPnPDLNANativeValueUnion *min,
                                           GUPnPDLNANativeValueUnion *max);

gchar *
gupnp_dlna_native_value_type_to_string (GUPnPDLNANativeValueType  *type,
                                        GUPnPDLNANativeValueUnion *value);

gint
gupnp_dlna_native_value_type_compare (GUPnPDLNANativeValueType  *type,
                                      GUPnPDLNANativeValueUnion *a,
                                      GUPnPDLNANativeValueUnion *b);

/* ValueList */
GUPnPDLNANativeValueList *
gupnp_dlna_native_value_list_copy (GUPnPDLNANativeValueList *list);

gboolean
gupnp_dlna_native_value_list_is_superset
                                        (GUPnPDLNANativeValueList *list,
                                         GUPnPDLNANativeInfoValue *value,
                                         gboolean                 *unsupported);

void
gupnp_dlna_native_value_list_merge
                     (GUPnPDLNANativeValueList                  *value_list,
                      GUPnPDLNANativeValueList                  *mrg_value_list,
                      GUPnPDLNANativeRestrictionMergeResolution  resolution);

gboolean
gupnp_dlna_native_value_list_is_empty (GUPnPDLNANativeValueList *value_list);

GList *
gupnp_dlna_native_value_list_get_list (GUPnPDLNANativeValueList *value_list);

gboolean
gupnp_dlna_native_value_list_mergeable
                                     (GUPnPDLNANativeValueList *value_list,
                                      GUPnPDLNANativeValueList *mrg_value_list);

gchar *
gupnp_dlna_native_value_list_to_string (GUPnPDLNANativeValueList *value_list);

void
gupnp_dlna_native_value_list_sort_items (GUPnPDLNANativeValueList *value_list);

/* InfoValue */
GUPnPDLNANativeValueType *
gupnp_dlna_native_info_value_get_type (GUPnPDLNANativeInfoValue *info);

GUPnPDLNANativeValueUnion *
gupnp_dlna_native_info_value_get_value (GUPnPDLNANativeInfoValue *info);

/* Restriction */
GHashTable *
gupnp_dlna_native_restriction_get_entries
                                      (GUPnPDLNANativeRestriction *restriction);

G_END_DECLS

#endif /* __GUPNP_DLNA_NATIVE_SETS_PRIVATE_H__ */
