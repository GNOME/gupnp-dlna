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

#ifndef __GUPNP_DLNA_VALUE_H__
#define __GUPNP_DLNA_VALUE_H__

#include <glib.h>
#include <glib-object.h>

#include "gupnp-dlna-info-value.h"
#include "gupnp-dlna-value-type.h"

G_BEGIN_DECLS

typedef struct _GUPnPDLNAValue GUPnPDLNAValue;
typedef struct _GUPnPDLNAValueVTable GUPnPDLNAValueVTable;

struct _GUPnPDLNAValue {
        GUPnPDLNAValueVTable *vtable;
};

GUPnPDLNAValue *
gupnp_dlna_value_new_single (GUPnPDLNAValueType *type,
                             const gchar        *single);

GUPnPDLNAValue *
gupnp_dlna_value_new_ranged (GUPnPDLNAValueType *type,
                             const gchar        *min,
                             const gchar        *max);

gboolean
gupnp_dlna_value_is_superset (GUPnPDLNAValue     *base,
                              GUPnPDLNAInfoValue *single);

GUPnPDLNAValue *
gupnp_dlna_value_copy (GUPnPDLNAValue     *base,
                       GUPnPDLNAValueType *type);

void
gupnp_dlna_value_free (GUPnPDLNAValue     *base,
                       GUPnPDLNAValueType *type);

gchar *
gupnp_dlna_value_to_string (GUPnPDLNAValue     *base,
                            GUPnPDLNAValueType *type);

gint
gupnp_dlna_value_compare (GUPnPDLNAValue     *base,
                          GUPnPDLNAValue     *other,
                          GUPnPDLNAValueType *type);

GValue *
gupnp_dlna_value_to_g_value (GUPnPDLNAValue     *base,
                             GUPnPDLNAValueType *type);

G_END_DECLS

#endif /* __GUPNP_DLNA_VALUE_H__ */
