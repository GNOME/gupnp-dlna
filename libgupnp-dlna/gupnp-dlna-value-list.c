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

#include "gupnp-dlna-value-list-private.h"
#include "gupnp-dlna-native-value.h"
#include "gupnp-dlna-info-value.h"

struct _GUPnPDLNAValueList {
        GUPnPDLNAValueType *type;
        GList              *values; /* <GUPnPDLNANativeValue *> */
        gboolean            sorted;
};

G_DEFINE_BOXED_TYPE (GUPnPDLNAValueList,
                     gupnp_dlna_value_list,
                     gupnp_dlna_value_list_copy,
                     gupnp_dlna_value_list_free)

GUPnPDLNAValueList *
gupnp_dlna_value_list_new (GUPnPDLNAValueType *type)
{
        GUPnPDLNAValueList *list;

        g_return_val_if_fail (type != NULL, NULL);

        list = g_slice_new (GUPnPDLNAValueList);
        list->type = type;
        list->values = NULL;
        list->sorted = FALSE;

        return list;
}

static void
free_value (GUPnPDLNANativeValue     *value,
            GUPnPDLNAValueType *type)
{
        gupnp_dlna_native_value_free (value, type);
}

static void
free_value_list (GUPnPDLNAValueList *list)
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
gupnp_dlna_value_list_free (GUPnPDLNAValueList *list)
{
        if (!list)
                return;

        free_value_list (list);
        g_slice_free (GUPnPDLNAValueList, list);
}

static gint
value_compare (GUPnPDLNANativeValue     *a,
               GUPnPDLNANativeValue     *b,
               GUPnPDLNAValueType *type)
{
        return gupnp_dlna_native_value_compare (a, b, type);
}

static gboolean
insert_value (GUPnPDLNAValueList *list,
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
gupnp_dlna_value_list_add_single (GUPnPDLNAValueList *list,
                                         const gchar *single)
{
        GUPnPDLNANativeValue *value;

        g_return_val_if_fail (list != NULL, FALSE);
        g_return_val_if_fail (single != NULL, FALSE);

        value = gupnp_dlna_native_value_new_single (list->type, single);

        return insert_value (list, value);
}

gboolean
gupnp_dlna_value_list_add_range (GUPnPDLNAValueList *list,
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
GUPnPDLNAValueList *
gupnp_dlna_value_list_copy (GUPnPDLNAValueList *list)
{
        GUPnPDLNAValueList *dup;

        if (list) {
                GList *iter;

                dup = gupnp_dlna_value_list_new (list->type);
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
gupnp_dlna_value_list_is_superset (GUPnPDLNAValueList *list,
                                   GUPnPDLNAInfoValue *value,
                                   gboolean           *unsupported)
{
        GList *iter;

        g_return_val_if_fail (list != NULL, FALSE);
        g_return_val_if_fail (value != NULL, FALSE);
        g_return_val_if_fail (unsupported != NULL, FALSE);

        if (list->type != gupnp_dlna_info_value_get_type (value))
                return FALSE;

        if (gupnp_dlna_info_value_is_unsupported (value)) {
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
gupnp_dlna_value_list_is_empty (GUPnPDLNAValueList *value_list)
{
        g_return_val_if_fail (value_list != NULL, TRUE);

        return (value_list->values == NULL);
}

GList *
gupnp_dlna_value_list_get_list (GUPnPDLNAValueList *value_list)
{
        g_return_val_if_fail (value_list != NULL, NULL);

        return value_list->values;
}

static gchar *
list_to_string (GUPnPDLNAValueList *value_list)
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
gupnp_dlna_value_list_to_string (GUPnPDLNAValueList *value_list)
{
        GString *str;
        gchar *val_str;

        g_return_val_if_fail (value_list != NULL, NULL);

        if (value_list->values == NULL)
                return g_strdup ("");

        str = g_string_new (NULL);
        g_string_append_printf (str,
                                "(%s)",
                                gupnp_dlna_value_type_name (value_list->type));
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
gupnp_dlna_value_list_sort_items (GUPnPDLNAValueList *value_list)
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

GList *
gupnp_dlna_value_list_get_g_values (GUPnPDLNAValueList *list)
{
        GList *g_values;
        GList *iter;

        g_return_val_if_fail (list != NULL, NULL);

        g_values = NULL;
        for (iter = list->values; iter != NULL; iter = iter->next) {
                GValue *g_value =
                                gupnp_dlna_native_value_to_g_value (iter->data,
                                                                    list->type);

                if (g_value)
                        g_values = g_list_prepend (g_values, g_value);
        }

        return g_list_reverse (g_values);
}
