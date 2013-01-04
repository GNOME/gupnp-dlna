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

#include <string.h> /* for memset */

#include "gupnp-dlna-native-info-value.h"

/* private */
struct _GUPnPDLNANativeInfoValue {
        GUPnPDLNANativeValueType  *type;
        GUPnPDLNAValueUnion  value;
        gboolean unsupported;
};

static GUPnPDLNANativeInfoValue *
value_new (GUPnPDLNANativeValueType *type,
           gchar *raw)
{
        GUPnPDLNANativeInfoValue *info_value =
                                        g_slice_new (GUPnPDLNANativeInfoValue);

        info_value->type = type;
        if (!gupnp_dlna_native_value_type_init (type,
                                                &info_value->value,
                                                raw)) {
                g_slice_free (GUPnPDLNANativeInfoValue, info_value);
                info_value = NULL;
        }
        else
                info_value->unsupported = FALSE;
        g_free (raw);

        return info_value;
}

static GUPnPDLNANativeInfoValue *
value_unsupported (GUPnPDLNANativeValueType *type)
{
        GUPnPDLNANativeInfoValue *info_value =
                                        g_slice_new (GUPnPDLNANativeInfoValue);

        info_value->type = type;
        memset (&info_value->value, 0, sizeof (GUPnPDLNAValueUnion));
        info_value->unsupported = TRUE;

        return info_value;
}

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_bool (gboolean value)
{
        return value_new (gupnp_dlna_native_value_type_bool (),
                          g_strdup (value ? "true" : "false"));
}

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_unsupported_bool (void)
{
        return value_unsupported (gupnp_dlna_native_value_type_bool ());
}

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_fraction (gint numerator,
                                           gint denominator)
{
        return value_new (gupnp_dlna_native_value_type_fraction (),
                          g_strdup_printf ("%d/%d",
                                           numerator,
                                           denominator));
}

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_unsupported_fraction (void)
{
        return value_unsupported (gupnp_dlna_native_value_type_fraction ());
}

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_int (gint value)
{
        return value_new (gupnp_dlna_native_value_type_int (),
                          g_strdup_printf ("%d", value));
}

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_unsupported_int (void)
{
        return value_unsupported (gupnp_dlna_native_value_type_int ());
}

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_string (const gchar *value)
{
        return value_new (gupnp_dlna_native_value_type_string (),
                          g_strdup (value));
}

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_unsupported_string (void)
{
        return value_unsupported (gupnp_dlna_native_value_type_string ());
}

void
gupnp_dlna_native_info_value_free (GUPnPDLNANativeInfoValue *info_value)
{
        if (info_value == NULL)
                return;

        if (!info_value->unsupported)
                gupnp_dlna_native_value_type_clean (info_value->type,
                                                    &info_value->value);
        g_slice_free (GUPnPDLNANativeInfoValue, info_value);
}

GUPnPDLNANativeValueType *
gupnp_dlna_native_info_value_get_type (GUPnPDLNANativeInfoValue *info)
{
        g_return_val_if_fail (info != NULL, NULL);

        return info->type;
}

GUPnPDLNAValueUnion *
gupnp_dlna_native_info_value_get_value (GUPnPDLNANativeInfoValue *info)
{
        g_return_val_if_fail (info != NULL, NULL);

        if (info->unsupported)
                return NULL;
        else
                return &info->value;
}

gchar *
gupnp_dlna_native_info_value_to_string (GUPnPDLNANativeInfoValue *info)
{
        const gchar *type;
        gchar *raw;
        gchar *str;

        g_return_val_if_fail (info != NULL, NULL);

        type = gupnp_dlna_native_value_type_name (info->type);
        if (info->unsupported)
                raw = g_strdup ("<UNSUPPORTED>");
        else
                raw = gupnp_dlna_native_value_type_to_string (info->type,
                                                              &info->value);
        str = g_strdup_printf ("(%s)%s", type, raw);
        g_free (raw);

        return str;
}

gboolean
gupnp_dlna_native_info_value_is_unsupported (GUPnPDLNANativeInfoValue *info)
{
        g_return_val_if_fail (info != NULL, FALSE);

        return info->unsupported;
}
