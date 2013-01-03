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

#ifndef __GUPNP_DLNA_NATIVE_INFO_VALUE_H__
#define __GUPNP_DLNA_NATIVE_INFO_VALUE_H__

#include <glib.h>

#include "gupnp-dlna-value-type.h"
#include "gupnp-dlna-value-union.h"

G_BEGIN_DECLS

typedef struct _GUPnPDLNANativeInfoValue GUPnPDLNANativeInfoValue;

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_bool (gboolean value);

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_unsupported_bool (void);

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_fraction (gint numerator,
                                           gint denominator);

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_unsupported_fraction (void);

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_int (gint value);

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_unsupported_int (void);

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_string (const gchar *value);

GUPnPDLNANativeInfoValue *
gupnp_dlna_native_info_value_new_unsupported_string (void);

void
gupnp_dlna_native_info_value_free (GUPnPDLNANativeInfoValue *info_value);

gchar *
gupnp_dlna_native_info_value_to_string (GUPnPDLNANativeInfoValue *info_value);

gboolean
gupnp_dlna_native_info_value_is_unsupported (GUPnPDLNANativeInfoValue *info);

GUPnPDLNAValueType *
gupnp_dlna_native_info_value_get_type (GUPnPDLNANativeInfoValue *info);

GUPnPDLNAValueUnion *
gupnp_dlna_native_info_value_get_value (GUPnPDLNANativeInfoValue *info);

G_END_DECLS

#endif /* __GUPNP_DLNA_NATIVE_INFO_VALUE_H__ */
