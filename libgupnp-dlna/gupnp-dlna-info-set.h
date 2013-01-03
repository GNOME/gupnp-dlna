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

#ifndef __GUPNP_DLNA_INFO_SET_H__
#define __GUPNP_DLNA_INFO_SET_H__

#include <glib.h>
#include "gupnp-dlna-restriction.h"

G_BEGIN_DECLS

typedef struct _GUPnPDLNAInfoSet GUPnPDLNAInfoSet;

GUPnPDLNAInfoSet *
gupnp_dlna_info_set_new (const gchar *mime);

void
gupnp_dlna_info_set_free (GUPnPDLNAInfoSet *info_set);

gboolean
gupnp_dlna_info_set_add_bool (GUPnPDLNAInfoSet *info_set,
                              const gchar      *name,
                              gboolean          value);

gboolean
gupnp_dlna_info_set_add_unsupported_bool (GUPnPDLNAInfoSet *info_set,
                                          const gchar      *name);

gboolean
gupnp_dlna_info_set_add_fraction (GUPnPDLNAInfoSet *info_set,
                                  const gchar      *name,
                                  gint              numerator,
                                  gint              denominator);

gboolean
gupnp_dlna_info_set_add_unsupported_fraction (GUPnPDLNAInfoSet *info_set,
                                              const gchar      *name);

gboolean
gupnp_dlna_info_set_add_int (GUPnPDLNAInfoSet *info_set,
                             const gchar      *name,
                             gint              value);

gboolean
gupnp_dlna_info_set_add_unsupported_int (GUPnPDLNAInfoSet *info_set,
                                         const gchar      *name);

gboolean
gupnp_dlna_info_set_add_string (GUPnPDLNAInfoSet *info_set,
                                const gchar      *name,
                                const gchar      *value);

gboolean
gupnp_dlna_info_set_add_unsupported_string (GUPnPDLNAInfoSet *info_set,
                                            const gchar      *name);

gboolean
gupnp_dlna_info_set_fits_restriction (GUPnPDLNAInfoSet     *info_set,
                                      GUPnPDLNARestriction *restriction);

gchar *
gupnp_dlna_info_set_to_string (GUPnPDLNAInfoSet *info_set);

const gchar *
gupnp_dlna_info_set_get_mime (GUPnPDLNAInfoSet *info_set);

G_END_DECLS

#endif /* __GUPNP_DLNA_INFO_SET_H__ */
