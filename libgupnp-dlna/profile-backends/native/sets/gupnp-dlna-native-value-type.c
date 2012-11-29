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

#include <stdlib.h>
#include "gupnp-dlna-native-value-type.h"
#include "gupnp-dlna-native-sets-private.h"

struct _GUPnPDLNANativeValueType {
        gboolean
        (* init) (GUPnPDLNANativeValueType  *type,
                  GUPnPDLNANativeValueUnion *value,
                  const gchar               *raw);

        gboolean
        (* copy) (GUPnPDLNANativeValueType  *type,
                  GUPnPDLNANativeValueUnion *from,
                  GUPnPDLNANativeValueUnion *to);

        void
        (* clean) (GUPnPDLNANativeValueType  *type,
                   GUPnPDLNANativeValueUnion *value_union);

        gboolean
        (* is_equal) (GUPnPDLNANativeValueType  *type,
                      GUPnPDLNANativeValueUnion *first,
                      GUPnPDLNANativeValueUnion *second);

        gboolean
        (* is_in_range) (GUPnPDLNANativeValueType  *type,
                         GUPnPDLNANativeValueUnion *min,
                         GUPnPDLNANativeValueUnion *max,
                         GUPnPDLNANativeValueUnion *value);

        const gchar *
        (* name) (GUPnPDLNANativeValueType *type);

        gboolean
        (* verify_range) (GUPnPDLNANativeValueType  *type,
                          GUPnPDLNANativeValueUnion *min,
                          GUPnPDLNANativeValueUnion *max);

        gchar *
        (* to_string) (GUPnPDLNANativeValueType  *type,
                       GUPnPDLNANativeValueUnion *value);

        gint
        (* compare) (GUPnPDLNANativeValueType  *type,
                     GUPnPDLNANativeValueUnion *a,
                     GUPnPDLNANativeValueUnion *b);

        GType
        (* get_g_type) (GUPnPDLNANativeValueType  *type);

        void
        (* to_g_value) (GUPnPDLNANativeValueType  *type,
                        GUPnPDLNANativeValueUnion *value,
                        GValue                    *g_value);

        gboolean
        (* flatten) (GUPnPDLNANativeValueType *type,
                     GValue                   *target,
                     GValue                   *min,
                     GValue                   *max);
};

/* utils */
static gboolean
get_int (const gchar *raw,
         gint        *store)
{
        char *end;
        long num;

        g_return_val_if_fail (store != NULL, FALSE);

        end = NULL;
        num = strtol (raw, &end, 10);
        if (*end != '\0')
                return FALSE;
        *store = (gint) num;

        return TRUE;
}

/* TODO: replace it with better implementation */
static void
equalize_denominators (GUPnPDLNAFraction *first,
                       GUPnPDLNAFraction *second)
{
        gint common;

        if (first->denominator == second->denominator)
                return;

        common = first->denominator * second->denominator;
        first->numerator *= second->denominator;
        second->numerator *= first->denominator;
        first->denominator = common;
        second->denominator = common;
}

static gboolean
fraction_equal (GUPnPDLNAFraction first,
                GUPnPDLNAFraction second)
{
        equalize_denominators (&first, &second);

        return first.numerator == second.numerator;
}

static gboolean
fraction_in_range (GUPnPDLNAFraction min,
                   GUPnPDLNAFraction max,
                   GUPnPDLNAFraction value)
{
        GUPnPDLNAFraction value_dup = value;

        equalize_denominators (&min, &value);
        if (min.numerator > value.numerator)
                return FALSE;
        equalize_denominators (&max, &value_dup);
        if (max.numerator < value_dup.numerator)
                return FALSE;

        return TRUE;
}

static gint
int_comparison (gint a,
                gint b)
{
        if (a > b)
                return 1;
        else if (a < b)
                return -1;

        return 0;
}

static gint
fraction_comparison (GUPnPDLNAFraction a,
                     GUPnPDLNAFraction b)
{
        equalize_denominators (&a, &b);

        return int_comparison (a.numerator, b.numerator);
}

static gboolean
fraction_range_valid (GUPnPDLNAFraction min,
                      GUPnPDLNAFraction max)
{
        equalize_denominators (&min, &max);

        return (min.numerator <= max.numerator);
}

/* bool */
static gboolean
bool_init (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
           GUPnPDLNANativeValueUnion *value,
           const gchar               *raw)
{
        if (!g_strcmp0 (raw, "true"))
                value->bool_value = TRUE;
        else if (!g_strcmp0 (raw, "false"))
                value->bool_value = FALSE;
        else
                return FALSE;

        return TRUE;
}

static gboolean
bool_copy (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
           GUPnPDLNANativeValueUnion *from,
           GUPnPDLNANativeValueUnion *to)
{
        to->bool_value = from->bool_value;

        return TRUE;
}

static void
bool_clean (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
            GUPnPDLNANativeValueUnion *value_union G_GNUC_UNUSED)
{

}

static gboolean
bool_is_equal (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
               GUPnPDLNANativeValueUnion *first,
               GUPnPDLNANativeValueUnion *second)
{
        return !!first->bool_value == !!second->bool_value;
}

static gboolean
bool_is_in_range (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                  GUPnPDLNANativeValueUnion *min G_GNUC_UNUSED,
                  GUPnPDLNANativeValueUnion *max G_GNUC_UNUSED,
                  GUPnPDLNANativeValueUnion *value G_GNUC_UNUSED)
{
        /* boolean range? */
        return FALSE;
}

static const gchar *
bool_name (GUPnPDLNANativeValueType *type G_GNUC_UNUSED)
{
        return "boolean";
}

static gboolean
bool_verify_range (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                   GUPnPDLNANativeValueUnion *min G_GNUC_UNUSED,
                   GUPnPDLNANativeValueUnion *max G_GNUC_UNUSED)
{
        return FALSE;
}

static gchar *
bool_to_string (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                GUPnPDLNANativeValueUnion *value)
{
        return g_strdup (value->bool_value ? "true" : "false");
}

static gint
bool_compare (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
              GUPnPDLNANativeValueUnion *a,
              GUPnPDLNANativeValueUnion *b)
{
        if ((a->bool_value && b->bool_value) ||
            (!a->bool_value && !b->bool_value))
                return 0;
        else if (a->bool_value)
                return 1;
        else
                return -1;
}

static GType
bool_get_g_type (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED)
{
        return G_TYPE_BOOLEAN;
}

static void
bool_to_g_value (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                 GUPnPDLNANativeValueUnion *value,
                 GValue                    *g_value)
{
        g_value_set_boolean (g_value, value->bool_value);
}

static gboolean
bool_flatten (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
              GValue                    *target G_GNUC_UNUSED,
              GValue                    *from G_GNUC_UNUSED,
              GValue                    *to G_GNUC_UNUSED)
{
        return FALSE;
}

static GUPnPDLNANativeValueType bool_type_impl = {
        bool_init,
        bool_copy,
        bool_clean,
        bool_is_equal,
        bool_is_in_range,
        bool_name,
        bool_verify_range,
        bool_to_string,
        bool_compare,
        bool_get_g_type,
        bool_to_g_value,
        bool_flatten
};

/* fraction */
static gboolean
fraction_init (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
               GUPnPDLNANativeValueUnion *value,
               const gchar               *raw)
{
        gchar **tokens = g_strsplit (raw, "/", 2);
        gboolean result = FALSE;
        gint numerator;
        gint denominator;

        if (g_strv_length (tokens) != 2)
                goto out;

        if (!get_int (tokens[0], &numerator) ||
            !get_int (tokens[1], &denominator))
                goto out;

        if (!denominator)
                goto out;

        value->fraction_value.numerator = numerator;
        value->fraction_value.denominator = denominator;
        result = TRUE;
 out:
        g_strfreev (tokens);

        return result;
}

static gboolean
fraction_copy (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
               GUPnPDLNANativeValueUnion *from,
               GUPnPDLNANativeValueUnion *to)
{
        to->fraction_value = from->fraction_value;

        return TRUE;
}

static void
fraction_clean (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                GUPnPDLNANativeValueUnion *value_union G_GNUC_UNUSED)
{

}

static gboolean
fraction_is_equal (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                   GUPnPDLNANativeValueUnion *first,
                   GUPnPDLNANativeValueUnion *second)
{
        return fraction_equal (first->fraction_value,
                               second->fraction_value);
}

static gboolean
fraction_is_in_range (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                      GUPnPDLNANativeValueUnion *min,
                      GUPnPDLNANativeValueUnion *max,
                      GUPnPDLNANativeValueUnion *value)
{
        /* fraction range? */
        return fraction_in_range (min->fraction_value,
                                  max->fraction_value,
                                  value->fraction_value);
}

static const gchar *
fraction_name (GUPnPDLNANativeValueType *type G_GNUC_UNUSED)
{
        return "fraction";
}

static gboolean
fraction_verify_range (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                       GUPnPDLNANativeValueUnion *min,
                       GUPnPDLNANativeValueUnion *max)
{
        return fraction_range_valid (min->fraction_value, max->fraction_value);
}

static gchar *
fraction_to_string (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                    GUPnPDLNANativeValueUnion *value)
{
        return g_strdup_printf ("%d/%d",
                                value->fraction_value.numerator,
                                value->fraction_value.denominator);
}

static gint
fraction_compare (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                  GUPnPDLNANativeValueUnion *a,
                  GUPnPDLNANativeValueUnion *b)
{
        return fraction_comparison (a->fraction_value, b->fraction_value);
}

static GType
fraction_get_g_type (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED)
{
        return GUPNP_TYPE_DLNA_FRACTION;
}

static void
fraction_to_g_value (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                     GUPnPDLNANativeValueUnion *value,
                     GValue                    *g_value)
{
        g_value_set_boxed (g_value, &value->fraction_value);
}

static gboolean
fraction_flatten (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                  GValue                    *target,
                  GValue                    *from,
                  GValue                    *to)
{
        GUPnPDLNAFraction *fraction_min =
                                 GUPNP_DLNA_FRACTION (g_value_get_boxed (from));
        GUPnPDLNAFraction *fraction_max =
                                   GUPNP_DLNA_FRACTION (g_value_get_boxed (to));

        g_value_init (target, GUPNP_TYPE_DLNA_FRACTION_RANGE);
        g_value_take_boxed (target,
                            gupnp_dlna_fraction_range_new_take (fraction_min,
                                                                fraction_max));

        return TRUE;
}

static GUPnPDLNANativeValueType fraction_type_impl = {
        fraction_init,
        fraction_copy,
        fraction_clean,
        fraction_is_equal,
        fraction_is_in_range,
        fraction_name,
        fraction_verify_range,
        fraction_to_string,
        fraction_compare,
        fraction_get_g_type,
        fraction_to_g_value,
        fraction_flatten
};

/* int */
static gboolean
int_init (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
          GUPnPDLNANativeValueUnion *value,
          const gchar               *raw)
{
        gint num;

        if (get_int (raw, &num)) {
                value->int_value = num;
                return TRUE;
        }

        return FALSE;
}

static gboolean
int_copy (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
          GUPnPDLNANativeValueUnion *from,
          GUPnPDLNANativeValueUnion *to)
{
        to->int_value = from->int_value;

        return TRUE;
}

static void
int_clean (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
           GUPnPDLNANativeValueUnion *value_union G_GNUC_UNUSED)
{

}

static gboolean
int_is_equal (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
              GUPnPDLNANativeValueUnion *first,
              GUPnPDLNANativeValueUnion *second)
{
        return first->int_value == second->int_value;
}

static gboolean
int_is_in_range (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                 GUPnPDLNANativeValueUnion *min,
                 GUPnPDLNANativeValueUnion *max,
                 GUPnPDLNANativeValueUnion *value)
{
        return (min->int_value <= value->int_value &&
                max->int_value >= value->int_value);
}

static const gchar *
int_name (GUPnPDLNANativeValueType *type G_GNUC_UNUSED)
{
        return "int";
}

static gboolean
int_verify_range (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                  GUPnPDLNANativeValueUnion *min,
                  GUPnPDLNANativeValueUnion *max)
{
        return (min->int_value <= max->int_value);
}

static gchar *
int_to_string (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
               GUPnPDLNANativeValueUnion *value)
{
        return g_strdup_printf ("%d", value->int_value);
}

static gint
int_compare (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
             GUPnPDLNANativeValueUnion *a,
             GUPnPDLNANativeValueUnion *b)
{
        return int_comparison (a->int_value, b->int_value);
}

static GType
int_get_g_type (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED)
{
        return G_TYPE_INT;
}

static void
int_to_g_value (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                GUPnPDLNANativeValueUnion *value,
                GValue                    *g_value)
{
        g_value_set_int (g_value, value->int_value);
}

static gboolean
int_flatten (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
             GValue                    *target,
             GValue                    *from,
             GValue                    *to)
{
        g_value_init (target, GUPNP_TYPE_DLNA_INT_RANGE);
        g_value_take_boxed (target,
                            gupnp_dlna_int_range_new (g_value_get_int (from),
                                                      g_value_get_int (to)));

        return TRUE;
}

static GUPnPDLNANativeValueType int_type_impl = {
        int_init,
        int_copy,
        int_clean,
        int_is_equal,
        int_is_in_range,
        int_name,
        int_verify_range,
        int_to_string,
        int_compare,
        int_get_g_type,
        int_to_g_value,
        int_flatten
};

/* string */
static gboolean
string_init (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
             GUPnPDLNANativeValueUnion *value,
             const gchar               *raw)
{
        value->string_value = g_strdup (raw);

        return TRUE;
}

static gboolean
string_copy (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
             GUPnPDLNANativeValueUnion *from,
             GUPnPDLNANativeValueUnion *to)
{
        to->string_value = g_strdup (from->string_value);

        return TRUE;
}

static void
string_clean (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
              GUPnPDLNANativeValueUnion *value_union)
{
        g_free (value_union->string_value);
}

static gboolean
string_is_equal (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                 GUPnPDLNANativeValueUnion *first,
                 GUPnPDLNANativeValueUnion *second)
{
        return !g_strcmp0 (first->string_value, second->string_value);
}

static gboolean
string_is_in_range (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                    GUPnPDLNANativeValueUnion *min,
                    GUPnPDLNANativeValueUnion *max,
                    GUPnPDLNANativeValueUnion *value)
{
        /* string range? */
        return (!g_strcmp0 (min->string_value,
                            value->string_value) ||
                !g_strcmp0 (max->string_value,
                            value->string_value));
}

static const gchar *
string_name (GUPnPDLNANativeValueType *type G_GNUC_UNUSED)
{
        return "string";
}

static gboolean
string_verify_range (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                     GUPnPDLNANativeValueUnion *min G_GNUC_UNUSED,
                     GUPnPDLNANativeValueUnion *max G_GNUC_UNUSED)
{
        return FALSE;
}

static gchar *
string_to_string (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                  GUPnPDLNANativeValueUnion *value)
{
        return g_strdup (value->string_value);
}

static gint
string_compare (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                GUPnPDLNANativeValueUnion *a,
                GUPnPDLNANativeValueUnion *b)
{
        return g_strcmp0 (a->string_value, b->string_value);
}

static GType
string_get_g_type (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED)
{
        return G_TYPE_STRING;
}

static void
string_to_g_value (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                   GUPnPDLNANativeValueUnion *value,
                   GValue                    *g_value)
{
        g_value_set_string (g_value, value->string_value);
}

static gboolean
string_flatten (GUPnPDLNANativeValueType  *type G_GNUC_UNUSED,
                GValue                    *target G_GNUC_UNUSED,
                GValue                    *from G_GNUC_UNUSED,
                GValue                    *to G_GNUC_UNUSED)
{
        return FALSE;
}

static GUPnPDLNANativeValueType string_type_impl = {
        string_init,
        string_copy,
        string_clean,
        string_is_equal,
        string_is_in_range,
        string_name,
        string_verify_range,
        string_to_string,
        string_compare,
        string_get_g_type,
        string_to_g_value,
        string_flatten
};

GUPnPDLNANativeValueType *
gupnp_dlna_native_value_type_bool (void)
{
        return &bool_type_impl;
}

GUPnPDLNANativeValueType *
gupnp_dlna_native_value_type_fraction (void)
{
        return &fraction_type_impl;
}

GUPnPDLNANativeValueType *
gupnp_dlna_native_value_type_int (void)
{
        return &int_type_impl;
}

GUPnPDLNANativeValueType *
gupnp_dlna_native_value_type_string (void)
{
        return &string_type_impl;
}

gboolean
gupnp_dlna_native_value_type_init (GUPnPDLNANativeValueType  *type,
                                   GUPnPDLNANativeValueUnion *value,
                                   const gchar               *raw)
{
        g_return_val_if_fail (type != NULL, FALSE);
        g_return_val_if_fail (value != NULL, FALSE);
        g_return_val_if_fail (raw != NULL, FALSE);
        g_return_val_if_fail (type->init != NULL, FALSE);

        return type->init (type, value, raw);
}

gboolean
gupnp_dlna_native_value_type_copy (GUPnPDLNANativeValueType  *type,
                                   GUPnPDLNANativeValueUnion *from,
                                   GUPnPDLNANativeValueUnion *to)
{
        g_return_val_if_fail (type != NULL, FALSE);
        g_return_val_if_fail (from != NULL, FALSE);
        g_return_val_if_fail (to != NULL, FALSE);
        g_return_val_if_fail (type->copy != NULL, FALSE);

        return type->copy (type, from, to);
}

void
gupnp_dlna_native_value_type_clean (GUPnPDLNANativeValueType  *type,
                                    GUPnPDLNANativeValueUnion *value)
{
        g_return_if_fail (type != NULL);
        g_return_if_fail (value != NULL);
        g_return_if_fail (type->clean != NULL);

        type->clean (type, value);
}

gboolean
gupnp_dlna_native_value_type_is_equal (GUPnPDLNANativeValueType  *type,
                                       GUPnPDLNANativeValueUnion *first,
                                       GUPnPDLNANativeValueUnion *second)
{
        g_return_val_if_fail (type != NULL, FALSE);
        g_return_val_if_fail (first != NULL, FALSE);
        g_return_val_if_fail (second != NULL, FALSE);
        g_return_val_if_fail (type->is_equal != NULL, FALSE);

        return type->is_equal (type, first, second);
}

gboolean
gupnp_dlna_native_value_type_is_in_range (GUPnPDLNANativeValueType  *type,
                                          GUPnPDLNANativeValueUnion *min,
                                          GUPnPDLNANativeValueUnion *max,
                                          GUPnPDLNANativeValueUnion *value)
{
        g_return_val_if_fail (type != NULL, FALSE);
        g_return_val_if_fail (min != NULL, FALSE);
        g_return_val_if_fail (max != NULL, FALSE);
        g_return_val_if_fail (value != NULL, FALSE);
        g_return_val_if_fail (type->is_in_range != NULL, FALSE);

        return type->is_in_range (type, min, max, value);
}

const gchar *
gupnp_dlna_native_value_type_name (GUPnPDLNANativeValueType *type)
{
        g_return_val_if_fail (type != NULL, NULL);
        g_return_val_if_fail (type->name != NULL, NULL);

        return type->name (type);
}

gboolean
gupnp_dlna_native_value_type_verify_range (GUPnPDLNANativeValueType  *type,
                                           GUPnPDLNANativeValueUnion *min,
                                           GUPnPDLNANativeValueUnion *max)
{
        g_return_val_if_fail (type != NULL, FALSE);
        g_return_val_if_fail (min != NULL, FALSE);
        g_return_val_if_fail (max != NULL, FALSE);
        g_return_val_if_fail (type->verify_range != NULL, FALSE);

        return type->verify_range (type, min, max);
}

gchar *
gupnp_dlna_native_value_type_to_string (GUPnPDLNANativeValueType  *type,
                                        GUPnPDLNANativeValueUnion *value)
{
        g_return_val_if_fail (type != NULL, NULL);
        g_return_val_if_fail (value != NULL, NULL);
        g_return_val_if_fail (type->to_string != NULL, NULL);

        return type->to_string (type, value);
}

gint
gupnp_dlna_native_value_type_compare (GUPnPDLNANativeValueType  *type,
                                      GUPnPDLNANativeValueUnion *a,
                                      GUPnPDLNANativeValueUnion *b)
{
        g_return_val_if_fail (type != NULL, 0);
        g_return_val_if_fail (a != NULL, 0);
        g_return_val_if_fail (b != NULL, 0);
        g_return_val_if_fail (type->compare != NULL, 0);

        return type->compare (type, a, b);
}

void
gupnp_dlna_native_value_type_to_g_value (GUPnPDLNANativeValueType  *type,
                                         GUPnPDLNANativeValueUnion *value,
                                         GValue                    *g_value)
{
        g_return_if_fail (type != NULL);
        g_return_if_fail (value != NULL);
        g_return_if_fail (g_value != NULL);
        g_return_if_fail (type->get_g_type != NULL);
        g_return_if_fail (type->to_g_value != NULL);
        g_return_if_fail (G_IS_VALUE (g_value) == FALSE);

        g_value_init (g_value, type->get_g_type (type));
        type->to_g_value (type, value, g_value);
}

gboolean
gupnp_dlna_native_value_type_flatten (GUPnPDLNANativeValueType *type,
                                      GValue *target,
                                      GValue *from,
                                      GValue *to)
{
        GType native_value_type_g_type;

        g_return_val_if_fail (type != NULL, FALSE);
        g_return_val_if_fail (target != NULL, FALSE);
        g_return_val_if_fail (from != NULL, FALSE);
        g_return_val_if_fail (to != NULL, FALSE);
        g_return_val_if_fail (type->flatten != NULL, FALSE);
        g_return_val_if_fail (type->get_g_type != NULL, FALSE);
        g_return_val_if_fail (G_IS_VALUE (target) == FALSE, FALSE);

        native_value_type_g_type = type->get_g_type (type);

        g_return_val_if_fail (G_VALUE_HOLDS (from, native_value_type_g_type),
                              FALSE);
        g_return_val_if_fail (G_VALUE_HOLDS (to, native_value_type_g_type),
                              FALSE);

        return type->flatten (type, target, from, to);
}
