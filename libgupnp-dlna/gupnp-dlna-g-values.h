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

#ifndef __GUPNP_DLNA_G_VALUES_H__
#define __GUPNP_DLNA_G_VALUES_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _GUPnPDLNAFraction GUPnPDLNAFraction;
typedef struct _GUPnPDLNAFractionRange GUPnPDLNAFractionRange;
typedef struct _GUPnPDLNAIntRange GUPnPDLNAIntRange;

#define GUPNP_DLNA_FRACTION(x) \
        ((GUPnPDLNAFraction *) x)

#define GUPNP_DLNA_FRACTION_RANGE(x) \
        ((GUPnPDLNAFractionRange *) x)

#define GUPNP_DLNA_INT_RANGE(x) \
        ((GUPnPDLNAIntRange *) x)

/**
 * GUPNP_TYPE_DLNA_FRACTION:
 *
 * The #GType for #GUPnPDLNAFraction.
 */
#define GUPNP_TYPE_DLNA_FRACTION (gupnp_dlna_fraction_get_type ())

/**
 * GUPNP_TYPE_DLNA_FRACTION_RANGE:
 *
 * The #GType for #GUPnPDLNAFractionRange.
 */
#define GUPNP_TYPE_DLNA_FRACTION_RANGE (gupnp_dlna_fraction_range_get_type ())

/**
 * GUPNP_TYPE_DLNA_INT_RANGE:
 *
 * The #GType for #GUPnPDLNAIntRange.
 */
#define GUPNP_TYPE_DLNA_INT_RANGE (gupnp_dlna_int_range_get_type ())

GType
gupnp_dlna_fraction_get_type (void) G_GNUC_CONST;

GType
gupnp_dlna_fraction_range_get_type (void) G_GNUC_CONST;

GType
gupnp_dlna_int_range_get_type (void) G_GNUC_CONST;

GUPnPDLNAFraction *
gupnp_dlna_fraction_copy (GUPnPDLNAFraction *fraction);

void
gupnp_dlna_fraction_free (GUPnPDLNAFraction *fraction);

gint
gupnp_dlna_fraction_get_numerator (GUPnPDLNAFraction *fraction);

gint
gupnp_dlna_fraction_get_denominator (GUPnPDLNAFraction *fraction);

GUPnPDLNAFractionRange *
gupnp_dlna_fraction_range_copy (GUPnPDLNAFractionRange *range);

void
gupnp_dlna_fraction_range_free (GUPnPDLNAFractionRange *range);

GUPnPDLNAFraction *
gupnp_dlna_fraction_range_get_min (GUPnPDLNAFractionRange *range);

GUPnPDLNAFraction *
gupnp_dlna_fraction_range_get_max (GUPnPDLNAFractionRange *range);

GUPnPDLNAIntRange *
gupnp_dlna_int_range_copy (GUPnPDLNAIntRange *range);

void
gupnp_dlna_int_range_free (GUPnPDLNAIntRange *range);

gint
gupnp_dlna_int_range_get_min (GUPnPDLNAIntRange *range);

gint
gupnp_dlna_int_range_get_max (GUPnPDLNAIntRange *range);

G_END_DECLS

#endif /* __GUPNP_DLNA_G_VALUES_H__ */
