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

#ifndef __GUPNP_DLNA_INFO_VALUE_H__
#define __GUPNP_DLNA_INFO_VALUE_H__

#include <glib.h>

#include "gupnp-dlna-value-type.h"
#include "gupnp-dlna-value-union.h"

G_BEGIN_DECLS

typedef struct _GUPnPDLNAInfoValue GUPnPDLNAInfoValue;

GUPnPDLNAInfoValue *
gupnp_dlna_info_value_new_bool (gboolean value);

GUPnPDLNAInfoValue *
gupnp_dlna_info_value_new_unsupported_bool (void);

GUPnPDLNAInfoValue *
gupnp_dlna_info_value_new_fraction (gint numerator,
                                    gint denominator);

GUPnPDLNAInfoValue *
gupnp_dlna_info_value_new_unsupported_fraction (void);

GUPnPDLNAInfoValue *
gupnp_dlna_info_value_new_int (gint value);

GUPnPDLNAInfoValue *
gupnp_dlna_info_value_new_unsupported_int (void);

GUPnPDLNAInfoValue *
gupnp_dlna_info_value_new_string (const gchar *value);

GUPnPDLNAInfoValue *
gupnp_dlna_info_value_new_unsupported_string (void);

void
gupnp_dlna_info_value_free (GUPnPDLNAInfoValue *info_value);

gchar *
gupnp_dlna_info_value_to_string (GUPnPDLNAInfoValue *info_value);

gboolean
gupnp_dlna_info_value_is_unsupported (GUPnPDLNAInfoValue *info_value);

GUPnPDLNAValueType *
gupnp_dlna_info_value_get_type (GUPnPDLNAInfoValue *info_value);

GUPnPDLNAValueUnion *
gupnp_dlna_info_value_get_value (GUPnPDLNAInfoValue *info_value);

G_END_DECLS

#endif /* __GUPNP_DLNA_INFO_VALUE_H__ */
