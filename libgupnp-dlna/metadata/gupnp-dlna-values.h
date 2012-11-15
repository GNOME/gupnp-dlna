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

#ifndef __GUPNP_DLNA_VALUES_H__
#define __GUPNP_DLNA_VALUES_H__

#include <glib.h>

G_BEGIN_DECLS

/**
 * SECTION:gupnp-dlna-values
 * @short_description: State values for metadata attributes.
 * @title: GUPnP DLNA Values
 *
 * The #GUPnPDLNABoolValue, #GUPnPDLNAFractionValue,
 * #GUPnPDLNAIntValue and #GUPnPDLNAStringValue hold respectively
 * boolean, fraction, integer and string values.
 *
 * The value can be in one of three possible states - set, unset or
 * unsupported. When value is in set state it is possible to use the
 * underlying value it holds. In other states using such value is
 * undefined.
 *
 * Main difference between unset state and unsupported state is that
 * the former is set when metadata extractor is capable of extracting
 * some attribute in general, but current media file does not have
 * such attribute, while the latter means that metadata extractor has
 * no idea how to extract such attribute at all.
 *
 * One note on #GUPnPDLNAStringValue - it holds a string that has to
 * be allocated with g_malloc() (that is - g_strdup() and others are
 * fine as well as they use g_malloc() internally). The string is
 * freed by consumer.
 */

/**
 * GUPnPDLNAValueState:
 * @GUPNP_DLNA_VALUE_STATE_SET: Value is set.
 * @GUPNP_DLNA_VALUE_STATE_UNSET: Value is unset.
 * @GUPNP_DLNA_VALUE_STATE_UNSUPPORTED: Value is unsupported.
 *
 * Flags describing a state of GUPnP DLNA Value.
 */
typedef enum {
        GUPNP_DLNA_VALUE_STATE_SET,
        GUPNP_DLNA_VALUE_STATE_UNSET,
        GUPNP_DLNA_VALUE_STATE_UNSUPPORTED
} GUPnPDLNAValueState;

/**
 * GUPnPDLNABoolValue:
 * @value: The boolean value.
 * @state: The state of #GUPnPDLNABoolValue.
 *
 * GUPnP DLNA Value representing a boolean value of some metadata attribute.
 */
typedef struct {
        gboolean            value;
        GUPnPDLNAValueState state;
} GUPnPDLNABoolValue;

/**
 * GUPNP_DLNA_BOOL_VALUE_UNSET:
 *
 * Static initializer for unset #GUPnPDLNABoolValue. Can be used in
 * two ways:
 *
 * |[
 *   GUPnPDLNABoolValue value = GUPNP_DLNA_BOOL_VALUE_UNSET;
 * ]|
 *
 * or
 *
 * |[
 *   return GUPNP_DLNA_BOOL_VALUE_UNSET;
 * ]|
 */
#define GUPNP_DLNA_BOOL_VALUE_UNSET \
        ((GUPnPDLNABoolValue) {FALSE, GUPNP_DLNA_VALUE_STATE_UNSET})
/**
 * GUPNP_DLNA_BOOL_VALUE_UNSUPPORTED:
 *
 * Static initializer for unsupported #GUPnPDLNABoolValue. Can be used
 * in two ways:
 *
 * |[
 *   GUPnPDLNABoolValue value = GUPNP_DLNA_BOOL_VALUE_UNSUPPORTED;
 * ]|
 *
 * or
 *
 * |[
 *   return GUPNP_DLNA_BOOL_VALUE_UNSUPPORTED;
 * ]|
 */
#define GUPNP_DLNA_BOOL_VALUE_UNSUPPORTED \
        ((GUPnPDLNABoolValue) {FALSE, GUPNP_DLNA_VALUE_STATE_UNSUPPORTED})

/**
 * GUPnPDLNAFractionValue:
 * @numerator: The numerator of fraction.
 * @denominator: The denominator of fraction.
 * @state: The state of #GUPnPDLNAFractionValue.
 *
 * GUPnP DLNA Value representing a fraction value of some metadata attribute.
 */
typedef struct {
        gint                numerator;
        gint                denominator;
        GUPnPDLNAValueState state;
} GUPnPDLNAFractionValue;

/**
 * GUPNP_DLNA_FRACTION_VALUE_UNSET:
 *
 * Static initializer for unset #GUPnPDLNAFractionValue. Can be used
 * in two ways:
 *
 * |[
 *   GUPnPDLNAFractionValue value = GUPNP_DLNA_FRACTION_VALUE_UNSET;
 * ]|
 *
 * or
 *
 * |[
 *   return GUPNP_DLNA_FRACTION_VALUE_UNSET;
 * ]|
 */
#define GUPNP_DLNA_FRACTION_VALUE_UNSET \
        ((GUPnPDLNAFractionValue) {0, 0, GUPNP_DLNA_VALUE_STATE_UNSET})
/**
 * GUPNP_DLNA_FRACTION_VALUE_UNSUPPORTED:
 *
 * Static initializer for unsupported #GUPnPDLNAFractionValue. Can be
 * used in two ways:
 *
 * |[
 *   GUPnPDLNAFractionValue value = GUPNP_DLNA_FRACTION_VALUE_UNSUPPORTED;
 * ]|
 *
 * or
 *
 * |[
 *   return GUPNP_DLNA_FRACTION_VALUE_UNSUPPORTED;
 * ]|
 */
#define GUPNP_DLNA_FRACTION_VALUE_UNSUPPORTED \
        ((GUPnPDLNAFractionValue) {0, 0, GUPNP_DLNA_VALUE_STATE_UNSUPPORTED})

/**
 * GUPnPDLNAIntValue:
 * @value: The integer value.
 * @state: The state of #GUPnPDLNAIntValue.
 *
 * GUPnP DLNA Value representing an integer value of some metadata attribute.
 */
typedef struct {
        gint                value;
        GUPnPDLNAValueState state;
} GUPnPDLNAIntValue;

/**
 * GUPNP_DLNA_INT_VALUE_UNSET:
 *
 * Static initializer for unset #GUPnPDLNAIntValue. Can be used in
 * two ways:
 *
 * |[
 *   GUPnPDLNAIntValue value = GUPNP_DLNA_INT_VALUE_UNSET;
 * ]|
 *
 * or
 *
 * |[
 *   return GUPNP_DLNA_INT_VALUE_UNSET;
 * ]|
 */
#define GUPNP_DLNA_INT_VALUE_UNSET \
        ((GUPnPDLNAIntValue) {0, GUPNP_DLNA_VALUE_STATE_UNSET})
/**
 * GUPNP_DLNA_INT_VALUE_UNSUPPORTED:
 *
 * Static initializer for unsupported #GUPnPDLNAIntValue. Can be used in
 * two ways:
 *
 * |[
 *   GUPnPDLNAIntValue value = GUPNP_DLNA_INT_VALUE_UNSUPPORTED;
 * ]|
 *
 * or
 *
 * |[
 *   return GUPNP_DLNA_INT_VALUE_UNSUPPORTED;
 * ]|
 */
#define GUPNP_DLNA_INT_VALUE_UNSUPPORTED \
        ((GUPnPDLNAIntValue) {0, GUPNP_DLNA_VALUE_STATE_UNSUPPORTED})

/**
 * GUPnPDLNAStringValue:
 * @value: The string value.
 * @state: The state of #GUPnPDLNAStringValue.
 *
 * GUPnP DLNA Value representing a string value of some metadata
 * attribute.
 */
typedef struct {
        gchar               *value;
        GUPnPDLNAValueState  state;
} GUPnPDLNAStringValue;

/**
 * GUPNP_DLNA_STRING_VALUE_UNSET:
 *
 * Static initializer for unset #GUPnPDLNAStringValue. Can be used in
 * two ways:
 *
 * |[
 *   GUPnPDLNAStringValue value = GUPNP_DLNA_STRING_VALUE_UNSET;
 * ]|
 *
 * or
 *
 * |[
 *   return GUPNP_DLNA_STRING_VALUE_UNSET;
 * ]|
 */
#define GUPNP_DLNA_STRING_VALUE_UNSET \
        ((GUPnPDLNAStringValue) {NULL, GUPNP_DLNA_VALUE_STATE_UNSET})
/**
 * GUPNP_DLNA_STRING_VALUE_UNSUPPORTED:
 *
 * Static initializer for unsupported #GUPnPDLNAStringValue. Can be
 * used in two ways:
 *
 * |[
 *   GUPnPDLNAStringValue value = GUPNP_DLNA_STRING_VALUE_UNSUPPORTED;
 * ]|
 *
 * or
 *
 * |[
 *   return GUPNP_DLNA_STRING_VALUE_UNSUPPORTED;
 * ]|
 */
#define GUPNP_DLNA_STRING_VALUE_UNSUPPORTED \
        ((GUPnPDLNAStringValue) {NULL, GUPNP_DLNA_VALUE_STATE_UNSUPPORTED})

G_END_DECLS

#endif /* __GUPNP_DLNA_VALUES_H__ */
