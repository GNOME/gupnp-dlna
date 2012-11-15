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

#include "gupnp-dlna-native-value-list.h"
#include "gupnp-dlna-native-value.h"
#include "gupnp-dlna-native-sets-private.h"

struct _GUPnPDLNANativeValueList {
        GUPnPDLNANativeValueType *type;
        GList                    *values; /* <GUPnPDLNANativeValue *> */
        gboolean                  sorted;
};

GUPnPDLNANativeValueList *
gupnp_dlna_native_value_list_new (GUPnPDLNANativeValueType *type)
{
        GUPnPDLNANativeValueList *list;

        g_return_val_if_fail (type != NULL, NULL);

        list = g_slice_new (GUPnPDLNANativeValueList);
        list->type = type;
        list->values = NULL;
        list->sorted = FALSE;

        return list;
}

static void
free_value (GUPnPDLNANativeValue     *value,
            GUPnPDLNANativeValueType *type)
{
        gupnp_dlna_native_value_free (value, type);
}

static void
free_value_list (GUPnPDLNANativeValueList *list)
{
        if (list->values) {
                g_list_foreach (list->values,
                                (GFunc) free_value,
                                list->type);
                g_list_free (list->values);
                list->values = NULL;
        }
}

void
gupnp_dlna_native_value_list_free (GUPnPDLNANativeValueList *list)
{
        if (!list)
                return;

        free_value_list (list);
        g_slice_free (GUPnPDLNANativeValueList, list);
}

static gint
value_compare (GUPnPDLNANativeValue     *a,
               GUPnPDLNANativeValue     *b,
               GUPnPDLNANativeValueType *type)
{
        return gupnp_dlna_native_value_compare (a, b, type);
}

static gboolean
insert_value (GUPnPDLNANativeValueList *list,
              GUPnPDLNANativeValue *value)
{
        if (value) {
                if (list->sorted)
                        list->values = g_list_insert_sorted_with_data
                                        (list->values,
                                         value,
                                         (GCompareDataFunc) value_compare,
                                         list->type);
                else
                        list->values = g_list_prepend (list->values, value);
                return TRUE;
        }

        return FALSE;
}

gboolean
gupnp_dlna_native_value_list_add_single (GUPnPDLNANativeValueList *list,
                                         const gchar *single)
{
        GUPnPDLNANativeValue *value;

        g_return_val_if_fail (list != NULL, FALSE);
        g_return_val_if_fail (single != NULL, FALSE);

        value = gupnp_dlna_native_value_new_single (list->type, single);

        return insert_value (list, value);
}

gboolean
gupnp_dlna_native_value_list_add_range (GUPnPDLNANativeValueList *list,
                                        const gchar *min,
                                        const gchar *max)
{
        GUPnPDLNANativeValue *range;

        g_return_val_if_fail (list != NULL, FALSE);
        g_return_val_if_fail (min != NULL, FALSE);
        g_return_val_if_fail (max != NULL, FALSE);

        range = gupnp_dlna_native_value_new_ranged (list->type, min, max);

        if (range) {
                list->values = g_list_prepend (list->values, range);
                return TRUE;
        }

        return FALSE;
}

/* private */
GUPnPDLNANativeValueList *
gupnp_dlna_native_value_list_copy (GUPnPDLNANativeValueList *list)
{
        GUPnPDLNANativeValueList *dup;

        if (list) {
                GList *iter;

                dup = gupnp_dlna_native_value_list_new (list->type);
                for (iter = list->values; iter != NULL; iter = iter->next) {
                        GUPnPDLNANativeValue *base =
                                        (GUPnPDLNANativeValue *) iter->data;
                        GUPnPDLNANativeValue *copy;

                        if (base == NULL)
                                continue;

                        copy = gupnp_dlna_native_value_copy (base, list->type);
                        if (copy != NULL)
                                dup->values = g_list_prepend (dup->values,
                                                              copy);
                }
                dup->values = g_list_reverse (dup->values);
        } else {
                dup = NULL;
        }

        return dup;
}

gboolean
gupnp_dlna_native_value_list_is_superset (GUPnPDLNANativeValueList *list,
                                          GUPnPDLNANativeInfoValue *value,
                                          gboolean                 *unsupported)
{
        GList *iter;

        g_return_val_if_fail (list != NULL, FALSE);
        g_return_val_if_fail (value != NULL, FALSE);
        g_return_val_if_fail (unsupported != NULL, FALSE);

        if (list->type != gupnp_dlna_native_info_value_get_type (value))
                return FALSE;

        if (gupnp_dlna_native_info_value_is_unsupported (value)) {
                *unsupported = TRUE;

                return TRUE;
        }

        for (iter = list->values; iter != NULL; iter = iter->next) {
                GUPnPDLNANativeValue *base =
                                            (GUPnPDLNANativeValue *) iter->data;

                if (gupnp_dlna_native_value_is_superset (base, value)) {
                        *unsupported = FALSE;

                        return TRUE;
                }
        }

        return FALSE;
}

gboolean
gupnp_dlna_native_value_list_mergeable
                                      (GUPnPDLNANativeValueList *value_list,
                                       GUPnPDLNANativeValueList *mrg_value_list)
{
        if (value_list->type != mrg_value_list->type) {
                return FALSE;
        }

        return TRUE;
}

void
gupnp_dlna_native_value_list_merge
                     (GUPnPDLNANativeValueList                  *value_list,
                      GUPnPDLNANativeValueList                  *mrg_value_list,
                      GUPnPDLNANativeRestrictionMergeResolution  resolution)
{
        g_return_if_fail (value_list != NULL);
        g_return_if_fail (mrg_value_list != NULL);

        switch (resolution) {
        case GUPNP_DLNA_NATIVE_RESTRICTION_MERGE_RESOLUTION_FROM_TARGET:
                /* do nothing */
                break;
        case GUPNP_DLNA_NATIVE_RESTRICTION_MERGE_RESOLUTION_FROM_SOURCE:
                free_value_list (value_list);
                value_list->values = mrg_value_list->values;
                value_list->type = mrg_value_list->type;
                mrg_value_list->values = NULL;
        case GUPNP_DLNA_NATIVE_RESTRICTION_MERGE_RESOLUTION_NONE:
                if (!gupnp_dlna_native_value_list_mergeable (value_list,
                                                             mrg_value_list)) {
                        g_critical ("Tried to merge values of type '%s' into "
                                    "values of type '%s'.",
                                    gupnp_dlna_native_value_type_name
                                        (mrg_value_list->type),
                                    gupnp_dlna_native_value_type_name
                                        (value_list->type));

                        return;
                }
                value_list->values = g_list_concat (value_list->values,
                                                    mrg_value_list->values);
                mrg_value_list->values = NULL;

                break;
        default:
                g_critical ("Unknown conflict resolution: %d", resolution);
        }
}

gboolean
gupnp_dlna_native_value_list_is_empty (GUPnPDLNANativeValueList *value_list)
{
        g_return_val_if_fail (value_list != NULL, TRUE);

        return (value_list->values == NULL);
}

GList *
gupnp_dlna_native_value_list_get_list (GUPnPDLNANativeValueList *value_list)
{
        g_return_val_if_fail (value_list != NULL, NULL);

        return value_list->values;
}

static gchar *
list_to_string (GUPnPDLNANativeValueList *value_list)
{
        GList *iter;
        GPtrArray* strings = g_ptr_array_new_with_free_func (g_free);
        gchar *str;

        for (iter = value_list->values; iter != NULL; iter = iter->next) {
                GUPnPDLNANativeValue *value =
                                            (GUPnPDLNANativeValue *) iter->data;

                g_ptr_array_add (strings,
                                 gupnp_dlna_native_value_to_string
                                        (value,
                                         value_list->type));
        }
        g_ptr_array_add (strings, NULL);

        str = g_strjoinv (", ", (gchar **) strings->pdata);
        g_ptr_array_unref (strings);

        return str;
}

gchar *
gupnp_dlna_native_value_list_to_string (GUPnPDLNANativeValueList *value_list)
{
        GString *str;
        gchar *val_str;

        g_return_val_if_fail (value_list != NULL, NULL);

        if (value_list->values == NULL)
                return g_strdup ("");

        str = g_string_new (NULL);
        g_string_append_printf (str,
                                "(%s)",
                                gupnp_dlna_native_value_type_name
                                        (value_list->type));
        if (value_list->values->next != NULL) {
                g_string_append (str, "{ ");
                val_str = list_to_string (value_list);
                g_string_append (str, val_str);
                g_string_append (str, " }");
        } else {
                val_str = list_to_string (value_list);
                g_string_append (str, val_str);
        }
        g_free (val_str);

        return g_string_free (str, FALSE);
}

void
gupnp_dlna_native_value_list_sort_items (GUPnPDLNANativeValueList *value_list)
{
        g_return_if_fail (value_list != NULL);

        if (!value_list->sorted) {
                value_list->values = g_list_sort_with_data
                                        (value_list->values,
                                         (GCompareDataFunc) value_compare,
                                         value_list->type);
                value_list->sorted = TRUE;
        }
}
