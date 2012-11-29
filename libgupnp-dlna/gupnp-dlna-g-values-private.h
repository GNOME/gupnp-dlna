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

#ifndef __GUPNP_DLNA_G_VALUES_PRIVATE_H__
#define __GUPNP_DLNA_G_VALUES_PRIVATE_H__

#include <glib.h>
#include <glib-object.h>

#include "gupnp-dlna-g-values.h"

G_BEGIN_DECLS

struct _GUPnPDLNAFraction
{
        gint numerator;
        gint denominator;
};

struct _GUPnPDLNAFractionRange
{
        GUPnPDLNAFraction *min;
        GUPnPDLNAFraction *max;
};

struct _GUPnPDLNAIntRange
{
        gint min;
        gint max;
};

GUPnPDLNAFractionRange *
gupnp_dlna_fraction_range_new_take (GUPnPDLNAFraction *min,
                                    GUPnPDLNAFraction *max);

GUPnPDLNAIntRange *
gupnp_dlna_int_range_new (gint min,
                          gint max);

G_END_DECLS

#endif /* __GUPNP_DLNA_G_VALUES_PRIVATE_H__ */
