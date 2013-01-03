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

#ifndef __GUPNP_DLNA_VALUE_UNION_H__
#define __GUPNP_DLNA_VALUE_UNION_H__

#include <glib.h>

#include "gupnp-dlna-g-values-private.h"

G_BEGIN_DECLS

typedef union _GUPnPDLNAValueUnion GUPnPDLNAValueUnion;

union _GUPnPDLNAValueUnion {
        gboolean bool_value;
        GUPnPDLNAFraction fraction_value;
        gint int_value;
        gchar *string_value;
};

G_END_DECLS

#endif /* __GUPNP_DLNA_VALUE_UNION_H__ */
