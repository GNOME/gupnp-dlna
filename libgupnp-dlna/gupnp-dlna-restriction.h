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

#ifndef __GUPNP_DLNA_RESTRICTION_H__
#define __GUPNP_DLNA_RESTRICTION_H__

#include <glib.h>
#include <glib-object.h>

#include <libgupnp-dlna/gupnp-dlna-value-list.h>

G_BEGIN_DECLS

typedef struct _GUPnPDLNARestriction GUPnPDLNARestriction;

#define GUPNP_DLNA_RESTRICTION(x) \
        ((GUPnPDLNARestriction *) x)

/**
 * GUPNP_TYPE_DLNA_RESTRICTION:
 *
 * The #GType for #GUPnPDLNARestriction.
 */
#define GUPNP_TYPE_DLNA_RESTRICTION (gupnp_dlna_restriction_get_type ())

GType
gupnp_dlna_restriction_get_type (void) G_GNUC_CONST;

GUPnPDLNARestriction *
gupnp_dlna_restriction_copy (GUPnPDLNARestriction *restriction);

void
gupnp_dlna_restriction_free (GUPnPDLNARestriction *restriction);

gboolean
gupnp_dlna_restriction_is_empty (GUPnPDLNARestriction *restriction);

gchar *
gupnp_dlna_restriction_to_string (GUPnPDLNARestriction *restriction);

const gchar *
gupnp_dlna_restriction_get_mime (GUPnPDLNARestriction *restriction);

GHashTable *
gupnp_dlna_restriction_get_entries (GUPnPDLNARestriction *restriction);

G_END_DECLS

#endif /* __GUPNP_DLNA_RESTRICTION_H__ */
