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

#ifndef __GUPNP_DLNA_NATIVE_VALUE_LIST_PRIVATE_H__
#define __GUPNP_DLNA_NATIVE_VALUE_LIST_PRIVATE_H__

#include <glib.h>

#include "gupnp-dlna-native-value-list.h"
#include "gupnp-dlna-native-value-type.h"

G_BEGIN_DECLS

GUPnPDLNANativeValueList *
gupnp_dlna_native_value_list_new (GUPnPDLNANativeValueType *type);

void
gupnp_dlna_native_value_list_free (GUPnPDLNANativeValueList *list);

gboolean
gupnp_dlna_native_value_list_add_range (GUPnPDLNANativeValueList *list,
                                        const gchar *min,
                                        const gchar *max);

gboolean
gupnp_dlna_native_value_list_add_single (GUPnPDLNANativeValueList *list,
                                         const gchar *single);

G_END_DECLS

#endif /* __GUPNP_DLNA_NATIVE_VALUE_LIST_PRIVATE_H__ */
