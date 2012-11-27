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

#include <glib.h>

#include "gupnp-dlna-native-restriction.h"
#include "gupnp-dlna-native-value-list.h"
#include "gupnp-dlna-native-sets-private.h"

struct _GUPnPDLNANativeRestriction {
        gchar *mime;
        GHashTable *entries; /* <gchar *, GUPnPDLNANativeValueList *> */
};

GUPnPDLNANativeRestriction *
gupnp_dlna_native_restriction_new (const gchar *mime)
{
        GUPnPDLNANativeRestriction *restriction =
                                       g_slice_new (GUPnPDLNANativeRestriction);

        restriction->mime = g_strdup (mime);
        restriction->entries = g_hash_table_new_full
                           (g_str_hash,
                            g_str_equal,
                            g_free,
                            (GDestroyNotify) gupnp_dlna_native_value_list_free);

        return restriction;
}

GUPnPDLNANativeRestriction *
gupnp_dlna_native_restriction_copy (GUPnPDLNANativeRestriction *restriction)
{
        GUPnPDLNANativeRestriction *dup;
        GHashTableIter iter;
        gpointer key;
        gpointer value;

        g_return_val_if_fail (restriction != NULL, NULL);

        dup = gupnp_dlna_native_restriction_new (restriction->mime);
        g_hash_table_iter_init (&iter, restriction->entries);
        while (g_hash_table_iter_next (&iter, &key, &value)) {
                GUPnPDLNANativeValueList * dup_entry;

                if (key == NULL || value == NULL)
                        continue;
                dup_entry = gupnp_dlna_native_value_list_copy (value);

                if (dup_entry == NULL)
                        continue;
                g_hash_table_insert (dup->entries, g_strdup (key), dup_entry);
        }

        return dup;
}

void
gupnp_dlna_native_restriction_free (GUPnPDLNANativeRestriction *restriction)
{
        if (restriction == NULL)
                return;
        g_free (restriction->mime);
        g_hash_table_unref (restriction->entries);
        g_slice_free (GUPnPDLNANativeRestriction, restriction);
}

gboolean
gupnp_dlna_native_restriction_add_value_list
                                       (GUPnPDLNANativeRestriction *restriction,
                                        const gchar                *name,
                                        GUPnPDLNANativeValueList   *list)
{
        g_return_val_if_fail (restriction != NULL, FALSE);
        g_return_val_if_fail (name != NULL, FALSE);
        g_return_val_if_fail (list != NULL, FALSE);

        if (gupnp_dlna_native_value_list_is_empty (list))
                return FALSE;
        if (g_hash_table_contains (restriction->entries, name))
                return FALSE;
        gupnp_dlna_native_value_list_sort_items (list);
        g_hash_table_insert (restriction->entries, g_strdup (name), list);

        return TRUE;
}

void
gupnp_dlna_native_restriction_merge (GUPnPDLNANativeRestriction *restriction,
                                     GUPnPDLNANativeRestriction *merged)
{
        GHashTableIter iter;
        gpointer name_ptr;
        gpointer value_list_ptr;

        g_return_if_fail (restriction != NULL);
        g_return_if_fail (merged != NULL);

        if (restriction->mime == NULL) {
                restriction->mime = merged->mime;
                merged->mime = NULL;
        }

        g_hash_table_iter_init (&iter, merged->entries);
        while (g_hash_table_iter_next (&iter,
                                       &name_ptr,
                                       &value_list_ptr)) {
                if (!g_hash_table_contains (restriction->entries, name_ptr)) {
                        g_hash_table_iter_steal (&iter);
                        g_hash_table_insert (restriction->entries,
                                             name_ptr,
                                             value_list_ptr);
                }
        }
        gupnp_dlna_native_restriction_free (merged);
}

gboolean
gupnp_dlna_native_restriction_is_empty (GUPnPDLNANativeRestriction *restriction)
{
        g_return_val_if_fail (restriction != NULL, TRUE);

        return (restriction->mime == NULL &&
                g_hash_table_size (restriction->entries) == 0);
}

gchar *
gupnp_dlna_native_restriction_to_string
                                       (GUPnPDLNANativeRestriction *restriction)
{
        GString *str;
        GHashTableIter iter;
        gpointer key;
        gpointer value;

        g_return_val_if_fail (restriction != NULL, NULL);

        if (gupnp_dlna_native_restriction_is_empty (restriction))
                return g_strdup ("EMPTY");

        str = g_string_new (restriction->mime ? restriction->mime : "(null)");
        g_hash_table_iter_init (&iter, restriction->entries);
        while (g_hash_table_iter_next (&iter, &key, &value)) {
                gchar *raw = gupnp_dlna_native_value_list_to_string (value);

                g_string_append_printf (str, ", %s=%s", (gchar *) key, raw);
                g_free (raw);
        }

        return g_string_free (str, FALSE);
}

const gchar *
gupnp_dlna_native_restriction_get_mime (GUPnPDLNANativeRestriction *restriction)
{
        g_return_val_if_fail (restriction != NULL, NULL);

        return restriction->mime;
}

GHashTable *
gupnp_dlna_native_restriction_get_entries
                                       (GUPnPDLNANativeRestriction *restriction)
{
        g_return_val_if_fail (restriction != NULL, NULL);

        return restriction->entries;
}
