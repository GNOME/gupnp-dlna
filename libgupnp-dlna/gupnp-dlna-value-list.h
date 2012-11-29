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

#ifndef __GUPNP_DLNA_VALUE_LIST_H__
#define __GUPNP_DLNA_VALUE_LIST_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _GUPnPDLNAValueList GUPnPDLNAValueList;

#define GUPNP_DLNA_VALUE_LIST(x) \
        ((GUPnPDLNAValueList *) x)

/**
 * GUPNP_TYPE_DLNA_VALUE_LIST:
 *
 * The #GType for #GUPnPDLNAValueList.
 */
#define GUPNP_TYPE_DLNA_VALUE_LIST (gupnp_dlna_value_list_get_type ())

GType
gupnp_dlna_value_list_get_type (void) G_GNUC_CONST;

GUPnPDLNAValueList *
gupnp_dlna_value_list_copy (GUPnPDLNAValueList *list);

void
gupnp_dlna_value_list_free (GUPnPDLNAValueList *list);

gboolean
gupnp_dlna_value_list_is_empty (GUPnPDLNAValueList *list);

gchar *
gupnp_dlna_value_list_to_string (GUPnPDLNAValueList *list);

GList *
gupnp_dlna_value_list_get_g_values (GUPnPDLNAValueList *list);

G_END_DECLS

#endif /* __GUPNP_DLNA_VALUE_LIST_H__ */
